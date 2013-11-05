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

#include <BlendInt/Widget.hpp>

namespace BlendInt {

	/**
	 * @brief A widget container in which the sub widget can be scrolled
	 */
	class ScrollView: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollView);

	public:

		ScrollView();

		ScrollView(AbstractForm* parent);

		/**
		 * @brief set the scrollable orientation
		 * @param flag
		 */
		void set_orientation(int flag)
		{
			m_orientation = flag;
		}

		void set_viewport (AbstractForm* viewport);

		const AbstractForm* viewport () const {return m_viewport;}

	protected:

		virtual void render ();

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

	private:

		int m_orientation;

		/**
		 * @brief if move the viewport
		 */
		bool m_move_status;

		AbstractForm* m_viewport;

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
