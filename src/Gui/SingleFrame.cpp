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

#include <BlendInt/Gui/SingleFrame.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	SingleFrame::SingleFrame()
	: Frame(),
	  focused_widget_(0),
	  hovered_widget_(0)
	{
		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);
	}

	SingleFrame::~SingleFrame()
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &SingleFrame::OnHoverWidgetDestroyed);
		}

		ClearHoverWidgets(hovered_widget_);
	}

	void SingleFrame::Setup(Widget* widget)
	{
		if(widget == 0) return;

		if(widget->parent() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(widget->size());

		if(PushBackSubWidget(widget)) {
			assert(subs_count() == 1);
			FillSingleWidget(0, 0, 0, size().width(), size().height());
		}
	}

	Widget* SingleFrame::GetFocusedWidget() const
	{
		return focused_widget_;
	}

	Widget* SingleFrame::GetHoveredWidget() const
	{
		return hovered_widget_;
	}

	void SingleFrame::DispatchHoverEvent(const MouseEvent& event)
	{
		Widget* original_hovered_widget = hovered_widget_;

		hovered_widget_ = DispatchHoverEventsInSubWidgets(original_hovered_widget, event);

		if(original_hovered_widget != hovered_widget_) {

			if(original_hovered_widget) {
				original_hovered_widget->destroyed().disconnectOne(this,
						&SingleFrame::OnHoverWidgetDestroyed);
			}

			if(hovered_widget_) {
				events()->connect(hovered_widget_->destroyed(), this,
						&SingleFrame::OnHoverWidgetDestroyed);
			}

		}
	}

	void SingleFrame::PerformPositionUpdate(
			const PositionUpdateRequest& request)
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

	void SingleFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
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

			FillSingleWidget(0, 0, 0, request.size()->width(), request.size()->height());

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void SingleFrame::MouseHoverOutEvent(const MouseEvent& event)
	{
		if(hovered_widget_) {
			hovered_widget_->destroyed().disconnectOne(this, &SingleFrame::OnHoverWidgetDestroyed);
		}

		ClearHoverWidgets(hovered_widget_);
	}

	void SingleFrame::PreDraw(Profile& profile)
	{
		assign_profile_frame(profile);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);
	}

	ResponseType SingleFrame::Draw(Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return subs_count() ? Ignore : Accept;
	}

	void SingleFrame::PostDraw(Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());
	}

	ResponseType SingleFrame::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event);

		if(hovered_widget_) {

			AbstractWidget* widget = 0;	// widget may be focused

			widget = DispatchMousePressEvent(hovered_widget_, event);

			if(widget == 0) {
				DBG_PRINT_MSG("%s", "widget 0");
			}

			SetFocusedWidget(dynamic_cast<Widget*>(widget));
		} else {
			SetFocusedWidget(0);
		}

		return retval;
	}

	ResponseType SingleFrame::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(focused_widget_) {
			set_event_frame(event);
			retval = call_mouse_release_event(focused_widget_, event);
		}

		return retval;
	}

	ResponseType SingleFrame::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		if(pressed_ext() && focused_widget_) {
			set_event_frame(event);
			retval = call_mouse_move_event(focused_widget_, event);
		}

		return retval;
	}

	void SingleFrame::SetFocusedWidget(Widget* widget)
	{
		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &SingleFrame::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, true);
			events()->connect(focused_widget_->destroyed(), this, &SingleFrame::OnFocusedWidgetDestroyed);
		}
	}

	void SingleFrame::OnFocusedWidgetDestroyed(Widget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &SingleFrame::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void SingleFrame::OnHoverWidgetDestroyed(Widget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &SingleFrame::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

}
