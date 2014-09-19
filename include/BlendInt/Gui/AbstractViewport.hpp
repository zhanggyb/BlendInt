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

#ifndef _BLENDINT_GUI_ABSTRACTVIEWPORT_HPP_
#define _BLENDINT_GUI_ABSTRACTVIEWPORT_HPP_

#include <glm/glm.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	class AbstractViewport: public AbstractContainer
	{
	public:

		enum  DisplayMode {
			Normal,
			Modal,
			Popup
		};

		friend class Context;

		AbstractViewport ();

		virtual ~AbstractViewport ();

		inline DisplayMode display_mode () const
		{
			return display_mode_;
		}

		static AbstractViewport* GetViewport (AbstractWidget* widget);

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool RoundTypeUpdateTest (const RoundTypeUpdateRequest& request);

		virtual bool RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		ResponseType DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType assign_key_press_event (AbstractWidget* widget, const KeyEvent& event)
		{
			return widget->KeyPressEvent(event);
		}

		ResponseType assign_mouse_press_event (AbstractWidget* widget, const MouseEvent& event)
		{
			return widget->MousePressEvent(event);
		}

		ResponseType assign_mouse_release_event(AbstractWidget* widget, const MouseEvent& event)
		{
			return widget->MouseReleaseEvent(event);
		}

		ResponseType assign_mouse_move_event(AbstractWidget* widget, const MouseEvent& event)
		{
			return widget->MouseMoveEvent(event);
		}

		void set_widget_focus_event (AbstractWidget* widget, bool focus)
		{
			widget->set_focus(focus);
			widget->FocusEvent(focus);
		}

		// NOT USED, remove later
		void set_widget_focus_status (AbstractWidget* widget, bool focus)
		{
			widget->set_focus(focus);
		}

		void set_widget_hover_event (AbstractWidget* widget, bool hover)
		{
			widget->set_hover(hover);
			widget->CursorEnterEvent(hover);
		}

		// NOT USED, remove later
		void set_widget_hover_status (AbstractWidget* widget, bool hover)
		{
			widget->set_hover(hover);
		}

		void set_event_viewport (const DeviceEvent& event)
		{
			const_cast<DeviceEvent&>(event).viewport_ = this;
		}

	private:

		DisplayMode display_mode_;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTVIEWPORT_HPP_ */
