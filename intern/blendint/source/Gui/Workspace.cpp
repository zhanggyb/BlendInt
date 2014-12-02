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

	ResponseType EdgeButton::Draw(Profile& profile)
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

		return Accept;
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

		for(AbstractInteractiveForm* p = first_child(); p; p = p->next()) {

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

		PushBackSubForm(left);
		PushBackSubForm(right);
		PushBackSubForm(head);

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
		AbstractInteractiveForm* p = first_child();

		MoveSubFormTo(p, x, y + h * 9 / 10);
		p = p->next();
		MoveSubFormTo(p, x + w - last_child()->size().width(), y + h * 9 / 10);
		p = p->next();
		MoveSubFormTo(p, x + w * 9 / 10, y);
	}

	// -------------------------------

	ViewportLayer::ViewportLayer()
	{
	}

	ViewportLayer::~ViewportLayer()
	{
	}

	bool ViewportLayer::Contain(const Point& point) const
	{
		if(next()) {
			if(next()->Contain(point)) {
				return false;
			} else {
				return VLayout::Contain(point);
			}
		}

		return VLayout::Contain(point);
	}

	// -------------------------------

	Workspace::Workspace()
	: Frame(),
	  left_sidebar_(0),
	  right_sidebar_(0),
	  header_(0),
	  viewport_(0),
	  splitter_(0),
	  hover_(0)
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
		if(header_ == header) return;

		// TODO: check null and resize
		header_ = header;
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
			for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
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

			} else {
				ResizeSubForm(splitter_, size());
			}

			RequestRedraw();
		}
		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool Workspace::PreDraw(Profile& profile)
	{
		return visiable();
	}

	ResponseType Workspace::Draw (Profile& profile)
	{
		for(AbstractInteractiveForm* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile, false);
		}

		return subs_count() ? Ignore : Accept;
	}

	void Workspace::PostDraw(Profile& profile)
	{
	}

	void Workspace::FocusEvent(bool focus)
	{
	}

	void Workspace::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Workspace::MouseHoverOutEvent(const MouseEvent& event)
	{
		if(hover_) {
			set_widget_mouse_hover_out_event(hover_, event);
			hover_->destroyed().disconnectOne(this, &Workspace::OnHoverFrameDestroyed);
			hover_ = 0;
		}
	}

	ResponseType Workspace::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MousePressEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		AbstractInteractiveForm* p = 0;
		for(p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = call_mouse_press_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType Workspace::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		/*
		for(AbstractInteractiveForm* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = assign_mouse_release_event(p, event);
				if(response == Accept) break;
			}

		}
		*/

		return response;
	}

	ResponseType Workspace::MouseMoveEvent(const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Workspace::DispatchHoverEvent(const MouseEvent& event)
	{
		if(Contain(event.position())) {

			AbstractFrame* new_hovered = CheckHoveredFrame(hover_, event);

			if(new_hovered != hover_) {

				if(hover_) {
					set_widget_mouse_hover_out_event(hover_, event);
					hover_->destroyed().disconnectOne(this, &Workspace::OnHoverFrameDestroyed);
				}

				hover_ = new_hovered;
				if(hover_) {
					set_widget_mouse_hover_in_event(hover_, event);
					events()->connect(hover_->destroyed(), this, &Workspace::OnHoverFrameDestroyed);
				}

			}

			if(hover_) {
				delegate_dispatch_hover_event(hover_, event);
			}

			// make sure to set event frame in this function, to tell parent frame or context to set this hover flag
			set_event_frame(event, this);

			return Accept;

		} else {
			set_event_frame(event, 0);
			return Ignore;
		}
	}

	void Workspace::InitializeWorkspace ()
	{
		splitter_ = Manage(new FrameSplitter);
		DBG_SET_NAME(splitter_, "Splitter");

		PushBackSubForm(splitter_);
		ResizeSubForm(splitter_, size());
		MoveSubFormTo(splitter_, position());
	}

	void Workspace::OnHoverFrameDestroyed(AbstractFrame* frame)
	{
		assert(frame->hover());
		assert(hover_ == frame);

		DBG_PRINT_MSG("unset hover status of widget %s", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Workspace::OnHoverFrameDestroyed);

		hover_ = 0;
	}

}
