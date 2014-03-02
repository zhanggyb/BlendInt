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

#ifndef _BLENDINT_SCROLLVIEW_HPP_
#define _BLENDINT_SCROLLVIEW_HPP_

#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	/**
	 * @brief A widget container in which a sub widget can be scrolled
	 *
	 * The sub widget is named as a viewport and it's usually larger than
	 * the ScrollView
	 */
	class ScrollView: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollView);

	public:

		ScrollView();

		ScrollView(AbstractWidget* parent);

		/**
		 * @brief set the scrollable orientation
		 * @param flag
		 */
		void set_orientation(int flag)
		{
			m_orientation = flag;
		}

		void set_viewport (AbstractWidget* viewport);

		/**
		 * @brief reset the viewport position to center of scroll area (counting padding)
		 */
		void reset_viewport_position ();

		const AbstractWidget* viewport () const {return m_viewport;}

	protected:

		virtual void Update (int type, const void* data);

		virtual void Draw ();

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

	private:

		int m_orientation;

		/**
		 * @brief if move the viewport
		 */
		bool m_move_status;

		AbstractWidget* m_viewport;

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
