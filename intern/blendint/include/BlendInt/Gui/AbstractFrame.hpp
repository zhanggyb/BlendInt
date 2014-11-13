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

#include <boost/smart_ptr.hpp>

#include <BlendInt/Gui/AbstractInteractiveForm.hpp>
#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class FrameSplitter;

	class AbstractFrame: public AbstractInteractiveForm
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

		Point GetAbsolutePosition (const AbstractWidget* widget);

		Cpp::EventRef<AbstractFrame*> destroyed ()
		{
			return *destroyed_;
		}

		static AbstractFrame* GetFrame (AbstractInteractiveForm* widget);

	protected:

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType DispatchHoverEvent (const MouseEvent& event) = 0;

		AbstractInteractiveForm* DispatchMousePressEvent (AbstractInteractiveForm* widget, const MouseEvent& event);

		ResponseType DispatchMouseMoveEvent (AbstractInteractiveForm* widget, const MouseEvent& event);

		ResponseType DispatchMouseReleaseEvent (AbstractInteractiveForm* widget, const MouseEvent& event);

		ResponseType call_key_press_event (AbstractInteractiveForm* widget, const KeyEvent& event)
		{
			return widget->KeyPressEvent(event);
		}

		ResponseType call_mouse_press_event (AbstractInteractiveForm* widget, const MouseEvent& event)
		{
			return widget->MousePressEvent(event);
		}

		ResponseType call_mouse_release_event(AbstractInteractiveForm* widget, const MouseEvent& event)
		{
			return widget->MouseReleaseEvent(event);
		}

		ResponseType call_mouse_move_event(AbstractInteractiveForm* widget, const MouseEvent& event)
		{
			return widget->MouseMoveEvent(event);
		}

		inline void set_widget_focus_status (AbstractInteractiveForm* widget, bool focus)
		{
			widget->set_focus(focus);
		}

		void set_widget_focus_event (AbstractInteractiveForm* widget, bool focus)
		{
			widget->set_focus(focus);
			widget->FocusEvent(focus);
		}

		void assign_profile_frame (Profile& profile)
		{
			profile.frame_ = this;
		}

		AbstractWidget* DispatchHoverEventsInSubWidgets (AbstractWidget* orig, const MouseEvent& event);

		AbstractFrame* CheckHoveredFrame (AbstractFrame* old, const MouseEvent& event);

		void ClearHoverWidgets (AbstractInteractiveForm* hovered_widget);

		void ClearHoverWidgets (AbstractInteractiveForm* hovered_widget, const MouseEvent& event);

		Cpp::ConnectionScope* events() const {return events_.get();}

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

		AbstractWidget* DispatchHoverEventDeeper (AbstractWidget* widget, const MouseEvent& event, Point& local_position);

		inline void set_widget_mouse_hover_in_event (AbstractInteractiveForm* widget, const MouseEvent& event)
		{
			widget->set_hover(true);
			widget->MouseHoverInEvent(event);
		}

		inline void set_widget_mouse_hover_out_event (AbstractInteractiveForm* widget, const MouseEvent& event)
		{
			widget->set_hover(false);
			widget->MouseHoverOutEvent(event);
		}

		inline void set_widget_hover_status (AbstractInteractiveForm* widget, bool hover)
		{
			widget->set_hover(hover);
		}

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

		DisplayMode display_mode_;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTFRAME_HPP_ */
