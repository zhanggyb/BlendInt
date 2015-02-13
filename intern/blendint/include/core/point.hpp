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

#ifndef _BLENDINT_POINT_HPP_
#define _BLENDINT_POINT_HPP_

namespace BlendInt {

	/**
	 * @brief Point position
	 *
	 * only used in widgets/window to define the position
	 */
	class Point
	{
	public:

		Point ()
				: x_(0), y_(0)
		{
		}

		Point (int x, int y)
				: x_(x), y_(y)
		{

		}

		Point (const Point& orig)
		: x_(orig.x_), y_(orig.y_)
		{

		}

		Point& operator = (const Point& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;

			return *this;
		}

		void reset (int x = 0, int y = 0)
		{
			x_ = x;
			y_ = y;
		}

		int x (void) const
		{
			return x_;
		}

		void set_x (int x)
		{
			x_ = x;
		}

		int y (void) const
		{
			return y_;
		}

		void set_y (int y)
		{
			y_ = y;
		}

		inline bool equal (const Point& point)
		{
			return (x_ == point.x() && y_ == point.y());
		}

		inline bool equal (int x, int y)
		{
			return (x_ == x && y_ == y);
		}

	private:
		int x_;
		int y_;
	};

	extern inline bool operator == (const Point& point1, const Point& point2)
	{
		return ((point1.x() == point2.x())
				&& (point1.y() == point2.y()));
	}

	extern inline Point operator + (const Point& point1, const Point& point2)
	{
		return Point(point1.x() + point2.x(), point1.y() + point2.y());
	}

	extern inline Point operator - (const Point& point1, const Point& point2)
	{
		return Point(point1.x() - point2.x(), point1.y() - point2.y());
	}

}

#endif /* _BLENDINT_POINT_HPP_ */
