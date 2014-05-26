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

#ifndef _BLENDINT_GUI_SCROLLVIEW_HPP_
#define _BLENDINT_GUI_SCROLLVIEW_HPP_

#include <BlendInt/Gui/AbstractSingleContainer.hpp>

namespace BlendInt {

	/**
	 * @brief A widget container in which a sub widget can be scrolled
	 *
	 * The sub widget is named as a viewport and it's usually larger than
	 * the ScrollView
	 */
	class ScrollView: public AbstractSingleContainer
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollView);

	public:

		ScrollView();

		virtual ~ScrollView ();

		void SetViewport (AbstractWidget* widget);

		/**
		 * @brief set the scrollable orientation
		 * @param flag
		 */
		void set_orientation(int flag)
		{
			m_orientation = flag;
		}

		/**
		 * @brief reset the viewport position to center of scroll area (counting padding)
		 */
		void CentralizeViewport ();

		int GetHPercentage ();

		int GetVPercentage ();

		void MoveViewport (int x, int y);

		void SetReletivePosition (int x, int y);

		AbstractWidget* viewport () const
		{
			return sub_widget();
		}

	protected:

		virtual void UpdateContainer (const WidgetUpdateRequest& request);

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		int m_orientation;

		/**
		 * @brief if move the viewport
		 */
		bool m_move_status;

		/**
		 * @brief the origin viewport position where start to move the viewport
		 */
		Point m_origin_pos;

		/**
		 * @brief the cursor position where start to move the viewport
		 */
		Point m_move_start_pos;
	};

}

#endif /* _BLENDINT_SCROLLVIEW_HPP_ */
