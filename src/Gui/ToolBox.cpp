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
 * License along with BlendInt.  If not, see
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

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ToolBox::ToolBox (Orientation orientation)
	: Frame(),
	focused_widget_(0),
	hovered_widget_(0),
	space_(1),
	vao_(0),
	orientation_(orientation)
	{
		if(orientation_ == Horizontal) {
			set_size(400, 240);
		} else {
			set_size(240, 400);
		}

		InitializeToolBoxOnce();
	}

	ToolBox::ToolBox (int width, int height, Orientation orientation)
	: Frame(),
	focused_widget_(0),
	hovered_widget_(0),
	space_(1),
	vao_(0),
	orientation_(orientation)
	{
		set_size(width, height);

		InitializeToolBoxOnce();
	}

	ToolBox::~ToolBox()
	{
		glDeleteVertexArrays(1, &vao_);

		if(focused_widget_) {
			set_widget_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	void ToolBox::Add (Widget* widget, bool append)
	{
		if(orientation_ == Horizontal) {

			int x = GetLastPosition();
			int y = margin_.bottom();
			int h = size().height() - margin_.vsum();

			if(PushBackSubWidget(widget)) {

				Size prefer = widget->GetPreferredSize();
				SetSubWidgetPosition(widget, x, y);
				if(widget->IsExpandY()) {
					ResizeSubWidget(widget, prefer.width(), h);
				} else {
					if(widget->size().height() > h) {
						ResizeSubWidget(widget, prefer.width(), h);
					} else {
						ResizeSubWidget(widget, prefer.width(), widget->size().height());
						SetSubWidgetPosition(widget, x,
										y + (h - widget->size().height()) / 2);
					}
				}

				Refresh();
			}

		} else {

			int x = margin_.left();
			int y = GetLastPosition();
			int w = size().width() - margin_.hsum();

			if(PushBackSubWidget(widget)) {
				Size prefer = widget->GetPreferredSize();
				y = y - prefer.height();
				SetSubWidgetPosition(widget, x, y);
				if(widget->IsExpandX()) {
					ResizeSubWidget(widget, w, prefer.height());
				} else {
					if(widget->size().width() > w) {
						ResizeSubWidget(widget, w, prefer.height());
					} else {
						ResizeSubWidget(widget, widget->size().width(), prefer.height());
					}
				}

				Refresh();
			}

		}
	}

	bool ToolBox::IsExpandX () const
	{
		return orientation_ == Horizontal ? true : false;
	}

	bool ToolBox::IsExpandY () const
	{
		return orientation_ == Vertical ? true : false;
	}

	Size ToolBox::GetPreferredSize () const
	{
		Size preferred_size;

		if(subs_count() == 0) {

			if(orientation_ == Horizontal) {
				preferred_size.reset(400, 240);
			} else {
				preferred_size.reset(240, 400);
			}

		} else {

			Size tmp;

			if(orientation_ == Horizontal) {

				preferred_size.set_width(-space_);

				for(AbstractWidget* p = first_child(); p; p = p->next())
				{
					if(p->visiable()) {
						tmp = p->GetPreferredSize();

						preferred_size.add_width(tmp.width() + space_);
						preferred_size.set_height(std::max(preferred_size.height(), tmp.height()));
					}
				}

				preferred_size.add_width(margin_.hsum());
				preferred_size.add_height(margin_.vsum());

			} else {

				preferred_size.set_height(-space_);

				for(AbstractWidget* p = first_child(); p; p = p->next())
				{
					if(p->visiable()) {
						tmp = p->GetPreferredSize();

						preferred_size.add_height(tmp.height() + space_);
						preferred_size.set_width(std::max(preferred_size.width(), tmp.width()));
					}
				}

				preferred_size.add_width(margin_.hsum());
				preferred_size.add_height(margin_.vsum());

			}

		}

		return preferred_size;
	}

	bool ToolBox::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	void ToolBox::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(request.position()->x()  + offset().x());
			float y = static_cast<float>(request.position()->y()  + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)size().width(),
				y,
				y + (float)size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

			set_position(*request.position());
		}

		if(request.source() == this) {
			ReportPositionUpdate (request);
		}
	}

	void ToolBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			float x = static_cast<float>(position().x() + offset().x());
			float y = static_cast<float>(position().y() + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0, RoundNone, 0.f, &inner_verts, 0);

			inner_.bind();
			inner_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_.reset();

			FillSubWidgets();

		} else if (request.target()->parent() == this) {
			FillSubWidgets();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool ToolBox::PreDraw (Profile& profile)
	{
		if(!visiable()) return false;

		assign_profile_frame(profile);

		Shaders::instance->frame_inner_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(Shaders::instance->location(Stock::FRAME_INNER_GAMMA), 0);
		glUniform4f(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);

		return true;
	}

	ResponseType ToolBox::Draw (Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return subs_count() ? Ignore : Accept;
	}

	void ToolBox::PostDraw (Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());
	}

	void ToolBox::FocusEvent (bool focus)
	{
	}

	void ToolBox::MouseHoverInEvent (const MouseEvent& event)
	{
	}

	void ToolBox::MouseHoverOutEvent (const MouseEvent& event)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	ResponseType ToolBox::KeyPressEvent (const KeyEvent& event)
	{
		set_event_frame(event, this);

		ResponseType response = Ignore;

		if(focused_widget_) {
			call_key_press_event(focused_widget_, event);
		}

		return response;
	}

	ResponseType ToolBox::MousePressEvent (const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event, this);

		if(hovered_widget_) {

			AbstractWidget* widget = 0;	// widget may be focused

			widget = DispatchMousePressEvent(hovered_widget_, event);

			if(widget == 0) {
				DBG_PRINT_MSG("%s", "widget 0");
			}

			// TODO: set pressed flag
			SetFocusedWidget(dynamic_cast<Widget*>(widget));
		} else {
			SetFocusedWidget(0);
		}

		return retval;
	}

	ResponseType ToolBox::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(focused_widget_) {
			set_event_frame(event, this);
			retval = call_mouse_release_event(focused_widget_, event);
			// TODO: reset pressed flag
		}

		return retval;
	}

	ResponseType ToolBox::MouseMoveEvent (const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(pressed_ext() && focused_widget_) {
			set_event_frame(event, this);
			retval = call_mouse_move_event(focused_widget_, event);
		}

		return retval;
	}

	ResponseType ToolBox::DispatchHoverEvent (const MouseEvent& event)
	{
		if(Contain(event.position())) {

			Widget* new_hovered_widget = DispatchHoverEventsInSubWidgets(hovered_widget_, event);

			if(new_hovered_widget != hovered_widget_) {

				if(hovered_widget_) {
					hovered_widget_->destroyed().disconnectOne(this,
							&ToolBox::OnHoverWidgetDestroyed);
				}

				hovered_widget_ = new_hovered_widget;
				if(hovered_widget_) {

					events()->connect(hovered_widget_->destroyed(), this,
							&ToolBox::OnHoverWidgetDestroyed);
				}

			}

			set_event_frame(event, this);
			return Accept;
		} else {
			set_event_frame(event, 0);
			return Ignore;
		}
	}

	void ToolBox::InitializeToolBoxOnce()
	{
		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		inner_.generate();
		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_.reset();
	}

	void ToolBox::FillSubWidgets ()
	{
		if(orientation_ == Horizontal) {
			FillSubWidgetsHorizontally();
		} else {
			FillSubWidgetsVertically();
		}
	}

	void ToolBox::FillSubWidgetsHorizontally()
	{
		int x = margin_.left();
		int y = margin_.bottom();
		//int width = size().width() - margin_.hsum();
		int height = size().height() - margin_.vsum();

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			SetSubWidgetPosition(p, x, y);

			if (p->IsExpandY()) {
				ResizeSubWidget(p, p->size().width(), height);
			} else {

				if (p->size().height() > height) {
					ResizeSubWidget(p, p->size().width(), height);
				} else {
					SetSubWidgetPosition(p, x,
							y + (height - p->size().height()) / 2);
				}

			}

			x += p->size().width() + space_;
		}
	}

	void ToolBox::FillSubWidgetsVertically()
	{
		int x = margin_.left();
		int y = size().height() - margin_.top();
		int width = size().width() - margin_.hsum();
		//int height = size().height() - margin_.vsum();

		y = y + space_;

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			y = y - p->size().height() - space_;

			SetSubWidgetPosition(p, x, y);

			if(p->IsExpandX()) {
				ResizeSubWidget(p, width, p->size().height());
			} else {

				if(p->size().width() > width) {
					ResizeSubWidget(p, width, p->size().height());
				} else {
					SetSubWidgetPosition(p, x + (width - p->size().width()) / 2,
									y);
				}

			}
		}
	}

	int ToolBox::GetLastPosition () const
	{
		int retval = 0;

		if(orientation_ == Horizontal) {

			retval = margin_.left();
			if (last_child()) {
				retval = last_child()->position().x() + last_child()->size().width() + space_;
			}

		} else {

			retval = size().height() - margin_.top();
			if(last_child()) {
				retval = last_child()->position().y() - space_;
			}

		}

		return retval;
	}

	void ToolBox::SetFocusedWidget (Widget* widget)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, true);
			events()->connect(focused_widget_->destroyed(), this, &ToolBox::OnFocusedWidgetDestroyed);
		}
	}

	void ToolBox::OnFocusedWidgetDestroyed (Widget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &ToolBox::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void ToolBox::OnHoverWidgetDestroyed (Widget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &ToolBox::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

}
