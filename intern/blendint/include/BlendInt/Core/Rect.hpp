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
		: x_(0), y_(0), width_(0), height_(0)
		{}
		
		Rect (int x, int y, int width, int height)
		{
			x_ = std::min(x, x + width);
			y_ = std::min(y, y + height);
			width_ = std::abs(width);
			height_ = std::abs(height);
		}

		Rect (const Point& p1, const Point& p2)
		{
			x_ = std::min(p1.x(), p2.x());
			y_ = std::min(p1.y(), p2.y());
			width_ = std::abs(p2.x() - p1.x());
			height_ = std::abs(p2.y() - p1.y());
		}

		Rect (const Point& pos, const Size& size)
		{
			x_ = std::min(pos.x(), pos.x() + size.width());
			y_ = std::min(pos.y(), pos.y() + size.height());
			width_ = std::abs(size.width());
			height_ = std::abs(size.height());
		}

		Rect (const Rect& orig)
		: x_(orig.x_), y_(orig.y_), width_(orig.width_), height_(orig.height_)
		{
		}

		Rect& operator = (const Rect& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			width_ = orig.width_;
			height_ = orig.height_;

			return *this;
		}

		bool contains (const Point& point)
		{
			if (point.x() < x_ || point.y() < y_
							|| point.x() > (x_ + width_)
							|| point.y() > (y_ + height_))
				return false;

			return true;
		}

		bool contains (int x, int y)
		{
			if (x < x_ || y < y_ || x > (x_ + width_)
							|| y > (y_ + height_)) {
				return false;
			}

			return true;
		}

		int x (void) const {return x_;}

		void set_x (int x) {x_ = x;}

		int y (void) const {return y_;}

		void set_y (int y) {y_ = y;}

		int width (void) const {return width_;}

		void set_width (int width)
		{
			x_ = width < 0 ? (x_ + width) : x_;
			width_ = std::abs(width);
		}

		int height (void) const {return height_;}

		void set_height (int height)
		{
			y_ = height < 0 ? (y_ + height) : y_;
			height_ = std::abs(height);
		}

		int left () const
		{
			return x_;
		}

		int right () const
		{
			return x_ + width_;
		}

		int top () const
		{
			return y_ + height_;
		}

		int bottom () const
		{
			return y_;
		}

		bool is_zero (void) const
		{
			return width_ == 0 || height_ == 0;
		}

	private:

		int x_;
		int y_;
		int width_;
		int height_;
	};

}

#endif	// _BIL_RECT_HPP_
