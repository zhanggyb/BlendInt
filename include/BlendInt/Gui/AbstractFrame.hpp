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

#ifndef _BLENDINT_GUI_ABSTRACTFRAME_HPP_
#define _BLENDINT_GUI_ABSTRACTFRAME_HPP_

#include <glm/glm.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class AbstractFrame: public AbstractWidget
	{
	public:

		enum  DisplayMode {
			Normal,
			Modal,
			Popup
		};

		friend class Context;

		AbstractFrame ();

		virtual ~AbstractFrame ();

		inline DisplayMode display_mode () const
		{
			return display_mode_;
		}

		virtual void SetCursorFollowedWidget (Widget* widget);

		Point GetGlobalPosition (const Widget* widget);

		Cpp::EventRef<AbstractFrame*> destroyed ()
		{
			return *destroyed_;
		}

		static AbstractFrame* GetFrame (AbstractWidget* widget);

	protected:

		virtual void DispatchHoverEvent (const MouseEvent& event) = 0;

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool RoundTypeUpdateTest (const RoundTypeUpdateRequest& request);

		virtual bool RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		ResponseType DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType DispatchMouseMoveEvent (AbstractWidget* widget, const MouseEvent& event);

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

		void set_widget_focus_status (AbstractWidget* widget, bool focus)
		{
			widget->set_focus(focus);
		}

		void set_widget_hover_status (AbstractWidget* widget, bool hover)
		{
			widget->set_hover(hover);
		}

		void set_widget_mouse_hover_in_event (AbstractWidget* widget, const MouseEvent& event)
		{
			widget->set_hover(true);
			widget->MouseHoverInEvent(event);
		}

		void set_widget_mouse_hover_out_event (AbstractWidget* widget, const MouseEvent& event)
		{
			widget->set_hover(false);
			widget->MouseHoverOutEvent(event);
		}

		void set_event_frame (const DeviceEvent& event)
		{
			const_cast<DeviceEvent&>(event).frame_ = this;
		}

		void assign_frame (Profile& profile)
		{
			profile.frame_ = this;
		}

		static inline void assign_dispatch_hover_event(AbstractFrame* frame, const MouseEvent& event)
		{
			frame->DispatchHoverEvent(event);
		}

	private:

		boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

		DisplayMode display_mode_;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTFRAME_HPP_ */
