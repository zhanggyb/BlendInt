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

#ifndef _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_
#define _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_

#include <BlendInt/Gui/AbstractWidget.hpp>

#include <BlendInt/Gui/ScrollBar.hpp>

namespace BlendInt {

	/**
	 * @brief Base class for widgets with native scroll bar
	 */
	class AbstractScrollable: public AbstractWidget
	{
	public:

		AbstractScrollable ();

		virtual ~AbstractScrollable ();

	protected:

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		ResponseType DispatchMousePressEvent (const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event)
		{
			return scrollbar->MousePressEvent(event);
		}

		ResponseType DispatchMouseReleaseEvent (const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event)
		{
			return scrollbar->MouseReleaseEvent(event);
		}

		ResponseType DispatchMouseMoveEvent (const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event)
		{
			return scrollbar->MouseMoveEvent(event);
		}

		void AdjustScrollBarGeometries (int left, int bottom, int width, int height);

		inline void draw_hbar (const RedrawEvent& event)
		{
			if(m_hbar->visiable()) {
				m_hbar->Draw(event);
			}
		}

		inline void draw_vbar (const RedrawEvent& event)
		{
			if(m_vbar->visiable()) {
				m_vbar->Draw(event);
			}
		}

		/**
		 * @brief Get the reference to the horizontal scroll bar
		 */
		const RefPtr<NativeScrollBar>& hbar () const
		{
			return m_hbar;
		}

		/**
		 * @brief Get the reference to the vertical scroll bar
		 */
		const RefPtr<NativeScrollBar>& vbar () const
		{
			return m_vbar;
		}

	private:

		RefPtr<NativeScrollBar> m_hbar;

		RefPtr<NativeScrollBar> m_vbar;

	};

}


#endif /* _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_ */
