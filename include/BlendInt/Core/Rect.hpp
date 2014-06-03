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

#ifndef _BLENDINT_RECT_HPP_
#define _BLENDINT_RECT_HPP_

#include <cstdlib>
#include <algorithm>

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>

namespace BlendInt {

	class Rect
	{
	public:
		Rect ()
		: m_x(0), m_y(0), m_width(0), m_height(0)
		{}
		
		Rect (int x, int y, int width, int height)
		{
			m_x = std::min(x, x + width);
			m_y = std::min(y, y + height);
			m_width = std::abs(width);
			m_height = std::abs(height);
		}

		Rect (const Point& p1, const Point& p2)
		{
			m_x = std::min(p1.x(), p2.x());
			m_y = std::min(p1.y(), p2.y());
			m_width = std::abs(p2.x() - p1.x());
			m_height = std::abs(p2.y() - p1.y());
		}

		Rect (const Point& pos, const Size& size)
		{
			m_x = std::min(pos.x(), pos.x() + size.width());
			m_y = std::min(pos.y(), pos.y() + size.height());
			m_width = std::abs(size.width());
			m_height = std::abs(size.height());
		}

		Rect (const Rect& orig)
		: m_x(orig.m_x), m_y(orig.m_y), m_width(orig.m_width), m_height(orig.m_height)
		{
		}

		Rect& operator = (const Rect& orig)
		{
			m_x = orig.m_x;
			m_y = orig.m_y;
			m_width = orig.m_width;
			m_height = orig.m_height;

			return *this;
		}

		bool contains (const Point& point)
		{
			if (point.x() < m_x || point.y() < m_y
							|| point.x() > (m_x + m_width)
							|| point.y() > (m_y + m_height))
				return false;

			return true;
		}

		bool contains (int x, int y)
		{
			if (x < m_x || y < m_y || x > (m_x + m_width)
							|| y > (m_y + m_height)) {
				return false;
			}

			return true;
		}

		int x (void) const {return m_x;}

		void set_x (int x) {m_x = x;}

		int y (void) const {return m_y;}

		void set_y (int y) {m_y = y;}

		int width (void) const {return m_width;}

		void set_width (int width)
		{
			m_x = width < 0 ? (m_x + width) : m_x;
			m_width = std::abs(width);
		}

		int height (void) const {return m_height;}

		void set_height (int height)
		{
			m_y = height < 0 ? (m_y + height) : m_y;
			m_height = std::abs(height);
		}

		int left () const
		{
			return m_x;
		}

		int right () const
		{
			return m_x + m_width;
		}

		int top () const
		{
			return m_y + m_height;
		}

		int bottom () const
		{
			return m_y;
		}

		bool is_zero (void) const
		{
			return m_width == 0 || m_height == 0;
		}

	private:

		int m_x;
		int m_y;
		int m_width;
		int m_height;
	};

}

#endif	// _BIL_RECT_HPP_
