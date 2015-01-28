/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Workspace.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	EdgeButton::EdgeButton(int round_type)
	: AbstractButton()
	{
		set_size(14, 14);
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		buffer_.generate();

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

	EdgeButton::~EdgeButton()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void EdgeButton::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			GenerateRoundedVertices(&inner_verts, &outer_verts);

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			buffer_.reset();

			set_size(*request.size());

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType EdgeButton::Draw(const AbstractWindow* context)
	{
		AbstractWindow::shaders->widget_inner_program()->use();

		if (hover()) {

			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 15);
			if (is_checked()) {
				glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        AbstractWindow::theme->radio_button().inner_sel.data());
			} else {
				glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        AbstractWindow::theme->radio_button().inner.data());
			}

		} else {
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
			if (is_checked()) {
				glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        AbstractWindow::theme->radio_button().inner_sel.data());
			} else {
				glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				        AbstractWindow::theme->radio_button().inner.data());
			}
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1,
		        AbstractWindow::theme->radio_button().outline.data());
		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
		        0.f, 0.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		return Finish;
	}

	// -------------------------------

	Workspace::Workspace()
	: Frame(),
	  left_sidebar_(0),
	  right_sidebar_(0),
	  header_(0),
	  viewport_(0),
	  splitter_(0),
	  hover_frame_(0),
	  focused_frame_(0)
	{
		set_size(800, 600);

		InitializeWorkspace();
	}

	Workspace::~Workspace()
	{
	}

	void Workspace::SetViewport (Frame* viewport)
	{
		if((viewport == nullptr) || (viewport == viewport_)) return;

		/*
		if(viewport_)
			splitter_->Remove(viewport_);
		*/

		splitter_->AddFrame(viewport);
		viewport_ = viewport;

		DBG_PRINT_MSG("viewport size: %d %d", viewport_->size().width(), viewport_->size().height());
		RequestRedraw();
	}

	void Workspace::SetLeftSideBar (ToolBox* leftbar)
	{
		if((leftbar == nullptr) || (leftbar == left_sidebar_)) return;

		/*
		if(left_sidebar_)
			splitter_->Remove(left_sidebar_);
		*/

		splitter_->AddFrame(leftbar);
		left_sidebar_ = leftbar;
		RequestRedraw();
	}

	void Workspace::SetRightSideBar (ToolBox* rightbar)
	{
		if((rightbar == nullptr) || (rightbar == right_sidebar_)) return;

		/*
		if(right_sidebar_)
			splitter_->Remove(right_sidebar_);
		*/

		splitter_->AddFrame(rightbar);
		right_sidebar_ = rightbar;

		RequestRedraw();
	}

	void Workspace::SetHeader (ToolBox* header, bool append)
	{
		if((header == nullptr) || (header_ == header)) return;

		header_ = header;

		if(append) {

			if(PushBackSubView(header_)) {
				Size prefer = header_->GetPreferredSize();
				ResizeSubView(header_, size().width(), prefer.height());
				ResizeSubView(splitter_, size().width(), size().height() - prefer.height());

				MoveSubViewTo(header_, position());
				MoveSubViewTo(splitter_, position().x(), position().y() + header_->size().height());
			} else {
				DBG_PRINT_MSG("Error: %s", "cannot add header frame");
			}

		} else {

			if(PushFrontSubView(header_)) {
				Size prefer = header_->GetPreferredSize();
				ResizeSubView(header_, size().width(), prefer.height());
				ResizeSubView(splitter_, size().width(), size().height() - prefer.height());

				MoveSubViewTo(splitter_, position());
				MoveSubViewTo(header_, position().x(), position().y() + splitter_->size().height());
			} else {
				DBG_PRINT_MSG("Error: %s", "cannot add header frame");
			}

		}

		RequestRedraw();
	}

	void Workspace::SwitchHeaderPosition ()
	{
		if(header_) {

			if(first_subview() == header_) {

				MoveToLast(header_);

				MoveSubViewTo(header_, position());
				MoveSubViewTo(splitter_, position().x(), position().y() + header_->size().height());


			} else {

				MoveToFirst(header_);

				MoveSubViewTo(splitter_, position());
				MoveSubViewTo(header_, position().x(), position().y() + splitter_->size().height());

			}

			RequestRedraw();
		}
	}

	bool Workspace::IsExpandX () const
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandX()) return true;
		}

		return false;
	}

	bool Workspace::IsExpandY () const
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandY()) return true;
		}

		return false;
	}

	Size Workspace::GetPreferredSize () const
	{
		Size prefer;

		if(subs_count() == 0) {
			prefer.reset(500, 400);
		} else {
			Size tmp;
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				tmp = p->GetPreferredSize();
				prefer.set_width(std::max(prefer.width(), tmp.width()));
				prefer.set_height(prefer.height() + tmp.height());
			}
		}

		return prefer;
	}

	void Workspace::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int ox = request.position()->x() - position().x();
			int oy = request.position()->y() - position().y();

			set_position(*request.position());

			for (AbstractView* p = first_subview(); p; p = p->next_view()) {
				MoveSubViewTo(p, p->position().x() + ox,
				        p->position().y() + oy);
			}

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void Workspace::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size(*request.size());

			if(header_) {

				Size prefer = header_->GetPreferredSize();

				ResizeSubView(header_, size().width(), prefer.height());
				ResizeSubView(splitter_, size().width(), size().height() - header_->size().height());

				if(first_subview() == header_) {
					MoveSubViewTo(splitter_, position());
					MoveSubViewTo(header_, position().x(), position().y() + splitter_->size().height());
				} else {
					MoveSubViewTo(header_, position());
					MoveSubViewTo(splitter_, position().x(), position().y() + header_->size().height());
				}

			} else {
				ResizeSubView(splitter_, size());
			}

			RequestRedraw();
		}
		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool Workspace::PreDraw(const AbstractWindow* context)
	{
		return visiable();
	}

	ResponseType Workspace::Draw (const AbstractWindow* context)
	{
		DrawSubViewsOnce(context);

		return subs_count() ? Ignore : Finish;
	}

	void Workspace::PostDraw(const AbstractWindow* context)
	{
	}

	void Workspace::PerformFocusOn (const AbstractWindow* context)
	{
	}

	void Workspace::PerformFocusOff (const AbstractWindow* context)
	{
	}

	void Workspace::PerformHoverIn(const AbstractWindow* context)
	{
	}

	void Workspace::PerformHoverOut(const AbstractWindow* context)
	{
		if(hover_frame_) {
			delegate_mouse_hover_out_event(hover_frame_, context);
			hover_frame_ = nullptr;
		}
	}

	ResponseType Workspace::PerformKeyPress(const AbstractWindow* context)
	{
		if(focused_frame_) {
			return delegate_key_press_event(focused_frame_, context);
		}
		return Ignore;
	}

	ResponseType Workspace::PerformMousePress(const AbstractWindow* context)
	{
		ResponseType response = Ignore;
		set_pressed(true);

		if(hover_frame_ != nullptr) {
			response = delegate_mouse_press_event(hover_frame_, context);

			if(response == Finish) {
				SetFocusedFrame(hover_frame_, context);
			}
		} else {
			SetFocusedFrame(0, context);
		}

		return Finish;
	}

	ResponseType Workspace::PerformMouseRelease(const AbstractWindow* context)
	{
		ResponseType response = Ignore;
		set_pressed(false);

		if(focused_frame_ != nullptr) {
			response = delegate_mouse_release_event(focused_frame_, context);
		}

		return response;
	}

	ResponseType Workspace::PerformMouseMove(const AbstractWindow* context)
	{
		ResponseType response = Ignore;

		if(pressed_ext() && focused_frame_) {
			response = delegate_mouse_move_event(focused_frame_, context);
		}

		return response;
	}

	ResponseType Workspace::DispatchHoverEvent(const AbstractWindow* context)
	{
		if(Contain(context->GetCursorPosition())) {

			ResponseType response = Finish;
			SetHoveredFrame(context);
			if(hover_frame_ != nullptr) {
				response = delegate_dispatch_hover_event(hover_frame_, context);
			}

			return response;

		} else {
			return Ignore;
		}
	}

	void Workspace::InitializeWorkspace ()
	{
		splitter_ = Manage(new FrameSplitter);
		DBG_SET_NAME(splitter_, "Splitter");

		PushBackSubView(splitter_);
		ResizeSubView(splitter_, size());
		MoveSubViewTo(splitter_, position());
	}

	AbstractView* Workspace::GetFocusedView() const
	{
		return focused_frame_;
	}

	void Workspace::SetFocusedFrame(AbstractFrame* frame, const AbstractWindow* context)
	{
    	if(focused_frame_ == frame) return;

    	if(focused_frame_ != nullptr) {
    		delegate_focus_off(focused_frame_, context);
    	}

    	focused_frame_ = frame;
    	if(focused_frame_ != nullptr) {
    		delegate_focus_on(focused_frame_, context);
    	}
	}

	bool Workspace::RemoveSubView (AbstractView* view)
	{
		if(view == left_sidebar_) {
			left_sidebar_ = nullptr;
		} else if (view == right_sidebar_) {
			right_sidebar_ = nullptr;
		} else if (view == header_) {
			header_ = nullptr;
		} else if (view == viewport_) {
			viewport_ = nullptr;
		}

		if(view == hover_frame_) {
			hover_frame_ = nullptr;
		}

		if(view == focused_frame_) {
			focused_frame_ = nullptr;
		}

		return Frame::RemoveSubView(view);
	}

	void Workspace::SetHoveredFrame (const AbstractWindow* context)
	{
		AbstractFrame* original = hover_frame_;

		if(hover_frame_ != nullptr) {
			if(!hover_frame_->Contain(context->GetCursorPosition())) {

				hover_frame_ = nullptr;
				for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
					if(p->Contain(context->GetCursorPosition())) {
						hover_frame_ = dynamic_cast<AbstractFrame*>(p);
						break;
					}
				}

			}
		} else {

			for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
				if(p->Contain(context->GetCursorPosition())) {
					hover_frame_ = dynamic_cast<AbstractFrame*>(p);
					break;
				}
			}

		}

		if (original != hover_frame_) {

			if (original != nullptr) {
				delegate_mouse_hover_out_event(original, context);
			}

			if (hover_frame_ != nullptr) {
				delegate_mouse_hover_in_event(hover_frame_, context);
			}

		}
	}

}
