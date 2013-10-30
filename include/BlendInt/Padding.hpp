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

#ifndef _BLENDINT_PADDING_HPP_
#define _BLENDINT_PADDING_HPP_

/**
 * @brief Margin
 *
 * used for box model of a widget
 */
namespace BlendInt {

	class Padding
	{
	public:

		Padding ()
		: m_left(5), m_right(5), m_top(5), m_bottom(5)
		{}

		Padding (int all)
		: m_left(all), m_right(all), m_top(all), m_bottom(all)
		{}

		Padding (int left, int right, int top, int bottom)
		: m_left(left), m_right(right), m_top(top), m_bottom(bottom)
		{}

		Padding (const Padding& orig)
		{
			m_left = orig.m_left;
			m_right = orig.m_right;
			m_top = orig.m_top;
			m_bottom = orig.m_bottom;
		}

		Padding& operator = (const Padding& orig)
		{
			m_left = orig.m_left;
			m_right = orig.m_right;
			m_top = orig.m_top;
			m_bottom = orig.m_bottom;

			return *this;
		}

		void set (int left, int right, int top, int bottom)
		{
			m_left = left;
			m_right = right;
			m_top = top;
			m_bottom = bottom;
		}

		int left () const {return m_left;}

		void set_left (int left) {m_left = left;}

		int right () const {return m_right;}

		void set_right (int right) {m_right = right;}

		int top () const {return m_top;}

		void set_top (int top) {m_top = top;}

		int bottom () const {return m_bottom;}

		void set_bottom (int bottom) {m_bottom = bottom;}

		bool equal (int left, int right, int top, int bottom)
		{
			return (m_left == left &&
					m_right == right &&
					m_top == top &&
					m_bottom == bottom);
		}

		bool equal (const Padding& padding)
		{
			return (m_left == padding.left() &&
					m_right == padding.right() &&
					m_top == padding.top() &&
					m_bottom == padding.bottom());
		}

	private:

		int m_left;
		int m_right;
		int m_top;
		int m_bottom;
	};

}


#endif /* PADDING_HPP_ */
