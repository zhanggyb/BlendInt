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
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Icons.hpp>

#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

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
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_OUTER_COORD), 2,
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

	ResponseType EdgeButton::Draw(const Context* context)
	{
		Shaders::instance->widget_inner_program()->use();

		if (hover()) {

			glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 15);
			if (is_checked()) {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				        Theme::instance->radio_button().inner_sel.data());
			} else {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				        Theme::instance->radio_button().inner.data());
			}

		} else {
			glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
			if (is_checked()) {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				        Theme::instance->radio_button().inner_sel.data());
			} else {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				        Theme::instance->radio_button().inner.data());
			}
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		Shaders::instance->widget_outer_program()->use();

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
		        Theme::instance->radio_button().outline.data());
		glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
		        0.f, 0.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		return Finish;
	}

	// -------------------------------

	EdgeButtonLayer::EdgeButtonLayer()
	: Widget()
	{
		InitializeSideButtonLayer();
	}

	EdgeButtonLayer::~EdgeButtonLayer()
	{

	}

	bool EdgeButtonLayer::Contain(const Point& point) const
	{
		bool retval = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {

			if(p->visiable()) {
				retval = p->Contain(point);
			}

			if(retval) break;
		}

		return retval;
	}

	void EdgeButtonLayer::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			AlighButtons(position(), *request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void EdgeButtonLayer::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AlighButtons(*request.position(), size());
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void EdgeButtonLayer::InitializeSideButtonLayer()
	{
		EdgeButton* left = Manage(new EdgeButton(RoundTopRight | RoundBottomRight));
		DBG_SET_NAME(left, "LeftButton");
		EdgeButton* right = Manage(new EdgeButton(RoundTopLeft | RoundBottomLeft));
		DBG_SET_NAME(right, "RightButton");
		EdgeButton* head = Manage(new EdgeButton(RoundTopLeft | RoundTopRight));
		DBG_SET_NAME(head, "HeadButton");

		PushBackSubView(left);
		PushBackSubView(right);
		PushBackSubView(head);

		AlighButtons(position(), size());
	}

	void EdgeButtonLayer::AlighButtons(const Point& out_pos,
			const Size& out_size)
	{
		int x = out_pos.x();
		int y = out_pos.y();
		int w = out_size.width();
		int h = out_size.height();

		AlignButtons(x, y, w, h);
	}

	void EdgeButtonLayer::AlignButtons(int x, int y, int w, int h)
	{
		AbstractView* p = first_subview();

		MoveSubViewTo(p, x, y + h * 9 / 10);
		p = p->next_view();
		MoveSubViewTo(p, x + w - last_subview()->size().width(), y + h * 9 / 10);
		p = p->next_view();
		MoveSubViewTo(p, x + w * 9 / 10, y);
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

	void Workspace::SetHeader (ToolBox* header)
	{
		if((header == nullptr) || (header_ == header)) return;

		// TODO: check null and resize
		header_ = header;

		if(PushBackSubView(header_)) {
			Size prefer = header_->GetPreferredSize();
			MoveSubViewTo(header_, position());
			ResizeSubView(header_, size().width(), prefer.height());

			MoveSubViewTo(splitter_, position().x(), position().y() + prefer.height());
			ResizeSubView(splitter_, size().width(), size().height() - prefer.height());
		} else {
			DBG_PRINT_MSG("Error: %s", "cannot add header frame");
		}

		RequestRedraw();
	}

	bool Workspace::IsExpandX () const
	{
		return true;
	}

	bool Workspace::IsExpandY () const
	{
		return true;
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
				prefer.set_height(std::max(prefer.height(), tmp.height()));
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
			MoveSubWidgets(ox, oy);

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
				MoveSubViewTo(header_, position());
				ResizeSubView(header_, size().width(), prefer.height());

				MoveSubViewTo(splitter_, position().x(), position().y() + prefer.height());
				ResizeSubView(splitter_, size().width(), size().height() - prefer.height());
			} else {
				ResizeSubView(splitter_, size());
			}

			RequestRedraw();
		}
		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool Workspace::PreDraw(const Context* context)
	{
		return visiable();
	}

	ResponseType Workspace::Draw (const Context* context)
	{
		DrawSubViewsOnce(context);

		return subs_count() ? Ignore : Finish;
	}

	void Workspace::PostDraw(const Context* context)
	{
	}

	void Workspace::PerformFocusOn (const Context* context)
	{
	}

	void Workspace::PerformFocusOff (const Context* context)
	{
	}

	void Workspace::PerformHoverIn(const Context* context)
	{
	}

	void Workspace::PerformHoverOut(const Context* context)
	{
		if(hover_frame_) {
			delegate_mouse_hover_out_event(hover_frame_, context);
			hover_frame_->destroyed().disconnectOne(this, &Workspace::OnHoverFrameDestroyed);
			hover_frame_ = 0;
		}
	}

	ResponseType Workspace::PerformKeyPress(const Context* context)
	{
		if(focused_frame_) {
			return delegate_key_press_event(focused_frame_, context);
		}
		return Ignore;
	}

	ResponseType Workspace::PerformMousePress(const Context* context)
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

	ResponseType Workspace::PerformMouseRelease(const Context* context)
	{
		ResponseType response = Ignore;
		set_pressed(false);

		if(focused_frame_ != nullptr) {
			response = delegate_mouse_release_event(focused_frame_, context);
		}

		return response;
	}

	ResponseType Workspace::PerformMouseMove(const Context* context)
	{
		ResponseType response = Ignore;

		if(pressed_ext() && focused_frame_) {
			response = delegate_mouse_move_event(focused_frame_, context);
		}

		return response;
	}

	ResponseType Workspace::DispatchHoverEvent(const Context* context)
	{
		if(Contain(context->cursor_position())) {

			AbstractFrame* new_hovered = CheckHoveredFrame(hover_frame_, context);

			if(new_hovered != hover_frame_) {

				if(hover_frame_) {
					delegate_mouse_hover_out_event(hover_frame_, context);
					hover_frame_->destroyed().disconnectOne(this, &Workspace::OnHoverFrameDestroyed);
				}

				hover_frame_ = new_hovered;
				if(hover_frame_) {
					delegate_mouse_hover_in_event(hover_frame_, context);
					events()->connect(hover_frame_->destroyed(), this, &Workspace::OnHoverFrameDestroyed);
				}

			}

			if(hover_frame_) {
				delegate_dispatch_hover_event(hover_frame_, context);
			}

			return Finish;

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

	void Workspace::OnHoverFrameDestroyed(AbstractFrame* frame)
	{
		assert(frame->hover());
		assert(hover_frame_ == frame);

		DBG_PRINT_MSG("unset hover status of widget %s", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Workspace::OnHoverFrameDestroyed);

		hover_frame_ = 0;
	}

	void Workspace::SetFocusedFrame(AbstractFrame* frame, const Context* context)
	{
    	if(focused_frame_ == frame) return;

    	if(focused_frame_ != nullptr) {
    		delegate_focus_off(focused_frame_, context);
    		focused_frame_->destroyed().disconnectOne(this, &Workspace::OnFocusedFrameDestroyed);
    	}

    	focused_frame_ = frame;
    	if(focused_frame_ != nullptr) {
    		delegate_focus_on(focused_frame_, context);
    		events()->connect(focused_frame_->destroyed(), this, &Workspace::OnFocusedFrameDestroyed);
    	}
	}

	void Workspace::OnFocusedFrameDestroyed(AbstractFrame* frame)
	{
		assert(focused_frame_ == frame);
		assert(frame->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused frame %s destroyed", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Workspace::OnFocusedFrameDestroyed);

		focused_frame_ = 0;
	}

}
