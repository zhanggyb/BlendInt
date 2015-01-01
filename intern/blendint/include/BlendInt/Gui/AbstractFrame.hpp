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

#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	class FrameSplitter;

	class AbstractFrame: public AbstractView
	{
	public:

		friend class Context;

		AbstractFrame ();

		virtual ~AbstractFrame ();

		virtual AbstractView* GetFocusedView () const = 0;

		Point GetAbsolutePosition (const AbstractWidget* view);

		Cpp::EventRef<AbstractFrame*> destroyed ()
		{
			return *destroyed_;
		}

		static AbstractFrame* GetFrame (AbstractView* view);

	protected:

		virtual ResponseType ContextMenuPressEvent (const Context* context);

		virtual ResponseType ContextMenuReleaseEvent (const Context* context);

		virtual ResponseType DispatchHoverEvent (const Context* context) = 0;

		ResponseType DispatchKeyEvent (AbstractView* view, const Context* context);

		AbstractView* DispatchMousePressEvent (AbstractView* view, const Context* context);

		ResponseType DispatchMouseMoveEvent (AbstractView* view, const Context* context);

		ResponseType DispatchMouseReleaseEvent (AbstractView* view, const Context* context);

		AbstractWidget* DispatchHoverEventsInSubWidgets (AbstractWidget* orig, const Context* context);

		AbstractFrame* CheckHoveredFrame (AbstractFrame* old, const Context* context);

		void ClearHoverWidgets (AbstractView* hovered_widget);

		void ClearHoverWidgets (AbstractView* hovered_widget, const Context* context);

		Cpp::ConnectionScope* events() const {return events_.get();}

		static inline ResponseType delegate_key_press_event (AbstractView* view, const Context* context)
		{
			return view->KeyPressEvent(context);
		}

		static inline ResponseType delegate_mouse_press_event (AbstractView* view, const Context* context)
		{
			return view->MousePressEvent(context);
		}

		static inline ResponseType delegate_mouse_release_event(AbstractView* view, const Context* context)
		{
			return view->MouseReleaseEvent(context);
		}

		static inline ResponseType delegate_mouse_move_event(AbstractView* view, const Context* context)
		{
			return view->MouseMoveEvent(context);
		}

		static inline void delegate_focus_status (AbstractView* view, bool focus)
		{
			view->set_focus(focus);
		}

		static void delegate_focus_event (AbstractView* view, bool focus)
		{
			view->set_focus(focus);
			view->FocusEvent(focus);
		}

		static inline void delegate_mouse_hover_in_event (AbstractView* view, const Context* context)
		{
			view->set_hover(true);
			view->MouseHoverInEvent(context);
		}

		static inline void delegate_mouse_hover_out_event (AbstractView* view, const Context* context)
		{
			view->set_hover(false);
			view->MouseHoverOutEvent(context);
		}

		static inline void delegate_dispatch_hover_event(AbstractFrame* frame, const Context* context)
		{
			frame->DispatchHoverEvent(context);
		}

		static inline void assign_profile_frame (Profile& profile, AbstractFrame* frame)
		{
			profile.frame_ = frame;
		}

		static void SetActiveFrame (const Context* context, AbstractFrame* frame);

	private:

		friend class FrameSplitter;

		AbstractWidget* DispatchHoverEventDeeper (AbstractWidget* view, const Context* context, Point& local_position);

		inline void set_widget_hover_status (AbstractView* view, bool hover)
		{
			view->set_hover(hover);
		}

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

		static glm::mat4 default_view_matrix;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTFRAME_HPP_ */
