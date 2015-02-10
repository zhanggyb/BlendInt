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

#pragma once

#include <glm/glm.hpp>

#include <boost/smart_ptr.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	class FrameSplitter;

	class AbstractFrame: public AbstractView
	{
	public:

		friend class AbstractWindow;

		AbstractFrame ();

		AbstractFrame (int width, int height);

		virtual ~AbstractFrame ();

		Point GetAbsolutePosition (const AbstractWidget* widget);

		Point GetRelativePosition (const AbstractWidget* widget);

		Cpp::EventRef<AbstractFrame*> destroyed ()
		{
			return *destroyed_;
		}

		static AbstractFrame* GetFrame (AbstractView* view);

	protected:

		virtual Response PerformContextMenuPress (AbstractWindow* context);

		virtual Response PerformContextMenuRelease (AbstractWindow* context);

		virtual Response DispatchHoverEvent (AbstractWindow* context) = 0;

		Response DispatchKeyEvent (AbstractView* view, AbstractWindow* context);

		AbstractView* DispatchMousePressEvent (AbstractView* view, AbstractWindow* context);

		Response DispatchMouseMoveEvent (AbstractView* view, AbstractWindow* context);

		Response DispatchMouseReleaseEvent (AbstractView* view, AbstractWindow* context);

		AbstractWidget* DispatchHoverEventsInWidgets (AbstractWidget* orig, AbstractWindow* context);

		void ClearHoverWidgets (AbstractView* hovered_widget);

		void ClearHoverWidgets (AbstractView* hovered_widget, AbstractWindow* context);

		const boost::scoped_ptr<Cpp::ConnectionScope>& events()
		{
			if(!events_) {
				events_.reset(new Cpp::ConnectionScope);
			}

			return events_;
		}

		static inline Response delegate_key_press_event (AbstractView* view, AbstractWindow* context)
		{
			return view->PerformKeyPress(context);
		}

		static inline Response delegate_mouse_press_event (AbstractView* view, AbstractWindow* context)
		{
			return view->PerformMousePress(context);
		}

		static inline Response delegate_mouse_release_event(AbstractView* view, AbstractWindow* context)
		{
			return view->PerformMouseRelease(context);
		}

		static inline Response delegate_mouse_move_event(AbstractView* view, AbstractWindow* context)
		{
			return view->PerformMouseMove(context);
		}

		static inline void delegate_focus_status (AbstractView* view, bool focus)
		{
			view->set_focus(focus);
		}

		static void delegate_focus_on (AbstractView* view, AbstractWindow* context)
		{
			view->set_focus(true);
			view->PerformFocusOn(context);
		}

		static void delegate_focus_off (AbstractView* view, AbstractWindow* context)
		{
			view->set_focus(false);
			view->PerformFocusOff(context);
		}

		static inline void delegate_mouse_hover_in_event (AbstractView* view, AbstractWindow* context)
		{
			view->set_hover(true);
			view->PerformHoverIn(context);
		}

		static inline void delegate_mouse_hover_out_event (AbstractView* view, AbstractWindow* context)
		{
			view->set_hover(false);
			view->PerformHoverOut(context);
		}

		static inline Response delegate_dispatch_hover_event(AbstractFrame* frame, AbstractWindow* context)
		{
			return frame->DispatchHoverEvent(context);
		}

		/**
		 * @brief Render to texture
		 * @param[in] frame
		 * @param[in] context
		 * @param[out] texture
		 */
		static bool RenderSubFramesToTexture (
			AbstractFrame* frame,
			AbstractWindow* context,
			const glm::mat4& projection,
			const glm::mat3& model,
			GLTexture2D* texture);

	private:

		friend class FrameSplitter;

		AbstractWidget* DispatchHoverEventDeeper (AbstractWidget* view, AbstractWindow* context, Point& local_position);

		inline void set_widget_hover_status (AbstractView* view, bool hover)
		{
			view->set_hover(hover);
		}

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractFrame*> > destroyed_;

		static glm::mat4 default_view_matrix;
	};

}
