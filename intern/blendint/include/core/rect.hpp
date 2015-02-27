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

#pragma once

#include <cstdlib>
#include <algorithm>

#include <core/point.hpp>
#include <core/size.hpp>

namespace BlendInt {

	class Rect
	{
	public:

		inline Rect ()
		: x_(0), y_(0), width_(0), height_(0)
		{}
		
		inline Rect (int x, int y, int width, int height)
		{
			x_ = std::min(x, x + width);
			y_ = std::min(y, y + height);
			width_ = std::abs(width);
			height_ = std::abs(height);
		}

		inline Rect (const Point& p1, const Point& p2)
		{
			x_ = std::min(p1.x(), p2.x());
			y_ = std::min(p1.y(), p2.y());
			width_ = std::abs(p2.x() - p1.x());
			height_ = std::abs(p2.y() - p1.y());
		}

		inline Rect (const Point& pos, const Size& size)
		{
			x_ = std::min(pos.x(), pos.x() + size.width());
			y_ = std::min(pos.y(), pos.y() + size.height());
			width_ = std::abs(size.width());
			height_ = std::abs(size.height());
		}

		inline Rect (const Rect& orig)
		: x_(orig.x_), y_(orig.y_), width_(orig.width_), height_(orig.height_)
		{
		}

		inline Rect& operator = (const Rect& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			width_ = orig.width_;
			height_ = orig.height_;

			return *this;
		}

		inline bool contains (const Point& point)
		{
			if (point.x() < x_ || point.y() < y_
							|| point.x() > (x_ + width_)
							|| point.y() > (y_ + height_))
				return false;

			return true;
		}

		inline bool contains (int x, int y)
		{
			if (x < x_ || y < y_ || x > (x_ + width_)
							|| y > (y_ + height_)) {
				return false;
			}

			return true;
		}

		inline void set_position (int x, int y)
		{
			x_ = x;
			y_ = y;
		}

		inline void set_position (const Point& pos)
		{
			x_ = pos.x();
			y_ = pos.y();
		}

		inline void set_size (int width, int height)
		{
			set_width(width);
			set_height(height);
		}

		inline void set_size (const Size& size)
		{
			set_width(size.width());
			set_height(size.height());
		}

		inline int x () const {return x_;}

		inline void set_x (int x) {x_ = x;}

		inline int y () const {return y_;}

		inline void set_y (int y) {y_ = y;}

		inline int width () const {return width_;}

		inline int hcenter () const {return x_ + width_ / 2;}

		inline void set_width (int width)
		{
			x_ = width < 0 ? (x_ + width) : x_;
			width_ = std::abs(width);
		}

		inline int height () const {return height_;}

		inline int vcenter () const {return y_ + height_ / 2;}

		inline void set_height (int height)
		{
			y_ = height < 0 ? (y_ + height) : y_;
			height_ = std::abs(height);
		}

		inline int left () const
		{
			return x_;
		}

		inline int right () const
		{
			return x_ + width_;
		}

		inline int top () const
		{
			return y_ + height_;
		}

		inline int bottom () const
		{
			return y_;
		}

		inline bool is_zero (void) const
		{
			return width_ == 0 || height_ == 0;
		}

		inline void cut_left (int left)
		{
			width_ -= left;
			x_ += left;

			x_ = width_ < 0 ? (x_ + width_) : x_;
			width_ = std::abs(width_);
		}

		inline void cut_right (int right)
		{
			width_ -= right;

			x_ = width_ < 0 ? (x_ + width_) : x_;
			width_ = std::abs(width_);
		}

		inline void cut_bottom (int bottom)
		{
			height_ -= bottom;
			y_ += bottom;

			y_ = height_ < 0 ? (y_ + height_) : y_;
			height_ = std::abs(height_);
		}

		inline void cut_top (int top)
		{
			height_ -= top;

			y_ = height_ < 0 ? (y_ + height_) : y_;
			height_ = std::abs(height_);
		}

	private:

		int x_;
		int y_;
		int width_;
		int height_;
	};

}
