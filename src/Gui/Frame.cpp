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
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Frame::Frame ()
	: AbstractFrame(),
	  focused_widget_(0),
	  hovered_widget_(0),
	  layout_(0)
	{
		set_size(500, 400);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);
	}

	Frame::~Frame()
	{
		if(focused_widget_) {
			set_widget_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Frame::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Frame::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	void Frame::SetLayout(AbstractLayout* layout)
	{
		if(!layout) return;

		if(layout == layout_) {
			DBG_PRINT_MSG("layout %s is already in this frame, skip this function", layout->name().c_str());
			return;
		}

		layout_->destroyed().disconnectOne(this, &Frame::OnLayoutDestroyed);

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			layout->Add(dynamic_cast<Widget*>(p));
		}

		if(PushBackSubWidget(layout)) {
			layout_ = layout;
			events()->connect(layout_->destroyed(), this, &Frame::OnLayoutDestroyed);
			SetSubWidgetPosition(layout_, 0, 0);
			ResizeSubWidget(layout_, size());
		} else {
			DBG_PRINT_MSG("Warning: %s", "Fail to set layout");
		}
	}

	void Frame::AddWidget(Widget* widget)
	{
		if(layout_) {
			layout_->Add(widget);
		} else {
			PushBackSubWidget(widget);
		}
	}

	bool Frame::IsExpandX() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool Frame::IsExpandY() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size Frame::GetPreferredSize() const
	{
		Size prefer;

		if(subs_count()) {
			int minx = 0;
			int miny = 0;
			int maxx = 0;
			int maxy = 0;

			for(AbstractWidget* p = first_child(); p; p = p->next()) {
				minx = std::min(p->position().x(), minx);
				miny = std::min(p->position().y(), miny);
				maxx = std::max(p->position().x() + p->size().width(), maxx);
				maxy = std::max(p->position().y() + p->size().height(), maxy);

				prefer.reset(maxx - minx, maxy - miny);
			}

		} else {
			prefer.reset(500, 400);
		}

		return prefer;
	}

	void Frame::DispatchHoverEvent(const MouseEvent& event)
	{
		Widget* original_hovered_widget = hovered_widget_;

		hovered_widget_ = DispatchHoverEventsInSubWidgets(original_hovered_widget, event);

		if(original_hovered_widget != hovered_widget_) {

			if(original_hovered_widget) {
				original_hovered_widget->destroyed().disconnectOne(this,
						&Frame::OnHoverWidgetDestroyed);
			}

			if(hovered_widget_) {
				events()->connect(hovered_widget_->destroyed(), this,
						&Frame::OnHoverWidgetDestroyed);
			}

		}
	}

	bool Frame::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		if(request.source() == layout_) {
			return false;
		} else {
			return true;
		}
	}

	bool Frame::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		if(request.source() == layout_) {
			return false;
		} else {
			return true;
		}
	}

	void Frame::PerformPositionUpdate(const PositionUpdateRequest& request)
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

	void Frame::PerformSizeUpdate(const SizeUpdateRequest& request)
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

			if(layout_) {
				ResizeSubWidget(layout_, size());
			}

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Frame::PreDraw(Profile& profile)
	{
		assign_profile_frame(profile);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);
	}

	ResponseType Frame::Draw (Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return subs_count() ? Ignore : Accept;
	}

	void Frame::PostDraw(Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());
	}

	void Frame::FocusEvent(bool focus)
	{
	}

	void Frame::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Frame::MouseHoverOutEvent(const MouseEvent& event)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &Frame::OnHoverWidgetDestroyed);
			ClearHoverWidgets(hovered_widget_);
		}
	}

	ResponseType Frame::KeyPressEvent(const KeyEvent& event)
	{
		set_event_frame(event);

		ResponseType response = Ignore;

		if(focused_widget_) {
			call_key_press_event(focused_widget_, event);
		}

		return response;
	}

	ResponseType Frame::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event);

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

	ResponseType Frame::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(focused_widget_) {
			set_event_frame(event);
			retval = call_mouse_release_event(focused_widget_, event);
			// TODO: reset pressed flag
		}

		return retval;
	}

	ResponseType Frame::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(pressed_ext() && focused_widget_) {
			set_event_frame(event);
			retval = call_mouse_move_event(focused_widget_, event);
		}

		return retval;
	}

	void Frame::SetFocusedWidget(Widget* widget)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Frame::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, true);
			events()->connect(focused_widget_->destroyed(), this, &Frame::OnFocusedWidgetDestroyed);
		}
	}

	void Frame::OnFocusedWidgetDestroyed(Widget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Frame::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void Frame::OnHoverWidgetDestroyed(Widget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Frame::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

	void Frame::OnLayoutDestroyed(Widget* layout)
	{
		assert(layout == layout_);

		DBG_PRINT_MSG("layout %s is destroyed", layout->name().c_str());
		layout->destroyed().disconnectOne(this, &Frame::OnLayoutDestroyed);
		layout_ = 0;
	}

}
