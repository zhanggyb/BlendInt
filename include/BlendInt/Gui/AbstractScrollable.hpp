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

		Cpp::EventRef<int> hbar_moved ()
		{
			return m_hbar_moved;
		}

		Cpp::EventRef<int> vbar_moved ()
		{
			return m_vbar_moved;
		}

	protected:

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		ResponseType DispatchMousePressEvent (const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event);

		ResponseType DispatchMouseReleaseEvent (const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event);

		ResponseType DispatchMouseMoveEvent (const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event);

		ResponseType DispatchDrawEvent (const RefPtr<NativeScrollBar>& scrollbar, const Profile& profile);

		void AdjustScrollBarGeometries (int left, int bottom, int width, int height);

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

		Cpp::Event<int> m_hbar_moved;

		Cpp::Event<int> m_vbar_moved;

	};

}


#endif /* _BLENDINT_GUI_ABSTRACTSCROLLABLE_HPP_ */
