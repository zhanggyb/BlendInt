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

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	/**
	 * @brief A widget container in which a sub widget can be scrolled
	 *
	 */
	class ScrollView: public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollView);

	public:

		ScrollView();

		virtual ~ScrollView ();

		void Setup (AbstractWidget* widget);

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

		int GetHPercentage () const;

		int GetVPercentage () const;

		void MoveViewport (int x, int y);

		void SetReletivePosition (int x, int y);

		AbstractWidget* viewport () const
		{
			return first();
		}

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformMarginUpdate (const Margin& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		GLuint m_vao;

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

		RefPtr<GLArrayBuffer> inner_;
	};

}

#endif /* _BLENDINT_SCROLLVIEW_HPP_ */
