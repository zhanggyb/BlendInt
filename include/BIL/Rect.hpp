/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.	If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_RECT_HPP_
#define _BIL_RECT_HPP_

#include <cstdlib>
#include <algorithm>

#include <BIL/Point.hpp>
#include <BIL/Size.hpp>

namespace BIL {

	class Rect
	{
	public:
		Rect ()
			: m_x(0), m_y(0), m_width(0), m_height(0)
		{}
		
		Rect (int x, int y, unsigned int width, unsigned int height)
			: m_x(x), m_y(y), m_width(width), m_height(height)
		{
		}

		Rect (const Point& p1, const Point& p2)
		{
			m_x = std::min (p1.x(), p2.x());
			m_y = std::min (p1.y(), p2.y());
			m_width = std::abs (p2.x() - p1.x());
			m_height = std::abs (p2.y() - p1.y());
		}

		Rect (const Point& pos, const Size& size)
		{
			m_x = pos.x();
			m_y = pos.y();
			m_width = size.width();
			m_height = size.height();
		}

		bool contains (const Point& point)
		{
			int diff_x = point.x() - m_x;
			int diff_y = point.y() - m_y;

			return diff_x >= 0 &&
					diff_x <= static_cast<int>(m_width) 	&&
					diff_y >= 0 &&
					diff_y <= static_cast<int>(m_height);
		}

		int x (void) const {return m_x;}

		void set_x (int x) {m_x = x;}

		int y (void) const {return m_y;}

		void set_y (int y) {m_y = y;}

		int width (void) const {return m_width;}

		void set_width (unsigned int width) {m_width = width;}

		void set_width (int width) {m_width = static_cast<unsigned int>(width);}

		int height (void) const {return m_height;}

		void set_height (unsigned int height) {m_height = height;}

		void set_height (int height) {m_height = static_cast<unsigned int>(height);}

		int left () const {
			return m_x;
		}

		int right () const {
			return m_x + m_width;
		}

		int top () const {
			return m_y + m_height;
		}

		int bottom () const {
			return m_y;
		}

		bool IsValid (void) const
		{
			return m_width > 0 && m_height > 0;
		}

	private:

		int m_x;
		int m_y;
		unsigned int m_width;
		unsigned int m_height;
	};

}

#endif	// _BIL_RECT_HPP_
