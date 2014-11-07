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

	class FrameSplitter;

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

		Point GetAbsolutePosition (const Widget* widget);

		Cpp::EventRef<AbstractFrame*> destroyed ()
		{
			return *destroyed_;
		}

		static AbstractFrame* GetFrame (AbstractWidget* widget);

	protected:

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

		virtual ResponseType DispatchHoverEvent (const MouseEvent& event) = 0;

		AbstractWidget* DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType DispatchMouseMoveEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType call_key_press_event (AbstractWidget* widget, const KeyEvent& event)
		{
			return widget->KeyPressEvent(event);
		}

		ResponseType call_mouse_press_event (AbstractWidget* widget, const MouseEvent& event)
		{
			return widget->MousePressEvent(event);
		}

		ResponseType call_mouse_release_event(AbstractWidget* widget, const MouseEvent& event)
		{
			return widget->MouseReleaseEvent(event);
		}

		ResponseType call_mouse_move_event(AbstractWidget* widget, const MouseEvent& event)
		{
			return widget->MouseMoveEvent(event);
		}

		inline void set_widget_focus_status (AbstractWidget* widget, bool focus)
		{
			widget->set_focus(focus);
		}

		void set_widget_focus_event (AbstractWidget* widget, bool focus)
		{
			widget->set_focus(focus);
			widget->FocusEvent(focus);
		}

		void assign_profile_frame (Profile& profile)
		{
			profile.frame_ = this;
		}

		Widget* DispatchHoverEventsInSubWidgets (Widget* orig, const MouseEvent& event);

		void ClearHoverWidgets (AbstractWidget* hovered_widget);

		void ClearHoverWidgets (AbstractWidget* hovered_widget, const MouseEvent& event);

		static inline void delegate_dispatch_hover_event(AbstractFrame* frame, const MouseEvent& event)
		{
			frame->DispatchHoverEvent(event);
		}

		static inline void set_event_frame (const HIDEvent& event, AbstractFrame* frame)
		{
			const_cast<HIDEvent&>(event).frame_ = frame;
		}

	private:

		friend class FrameSplitter;

		Widget* DispatchHoverEventDeeper (Widget* widget, const MouseEvent& event, Point& local_position);

		inline void set_widget_mouse_hover_in_event (AbstractWidget* widget, const MouseEvent& event)
		{
			widget->set_hover(true);
			widget->MouseHoverInEvent(event);
		}

		inline void set_widget_mouse_hover_out_event (AbstractWidget* widget, const MouseEvent& event)
		{
			widget->set_hover(false);
			widget->MouseHoverOutEvent(event);
		}

		inline void set_widget_hover_status (AbstractWidget* widget, bool hover)
		{
			widget->set_hover(hover);
		}

		boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

		DisplayMode display_mode_;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTFRAME_HPP_ */
