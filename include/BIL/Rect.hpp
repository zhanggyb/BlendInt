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
			: x_(0), y_(0), width_(0), height_(0)
		{}
		
		Rect (int x, int y, unsigned int width, unsigned int height)
			: x_(x), y_(y), width_(width), height_(height)
		{
		}

		Rect (const Point& p1, const Point& p2)
		{
			x_ = std::min (p1.x(), p2.x());
			y_ = std::min (p1.y(), p2.y());
			width_ = std::abs (p2.x() - p1.x());
			height_ = std::abs (p2.y() - p1.y());
		}

		Rect (const Point& pos, const Size& size)
		{
			x_ = pos.x();
			y_ = pos.y();
			width_ = size.width();
			height_ = size.height();
		}

		bool Contains (const Point& point)
		{
			int diff_x = point.x() - x_;
			int diff_y = point.y() - y_;

			return diff_x >= 0 &&
					diff_x <= static_cast<int>(width_) 	&&
					diff_y >= 0 &&
					diff_y <= static_cast<int>(height_);
		}

		int x (void) const {return x_;}

		void set_x (int x) {x_ = x;}

		int y (void) const {return y_;}

		void set_y (int y) {y_ = y;}

		int width (void) const {return width_;}

		void set_width (unsigned int width) {width_ = width;}

		void set_width (int width) {width_ = static_cast<unsigned int>(width);}

		int height (void) const {return height_;}

		void set_height (unsigned int height) {height_ = height;}

		void set_height (int height) {height_ = static_cast<unsigned int>(height);}

		int left () const {
			return x_;
		}

		int right () const {
			return x_ + width_;
		}

		int top () const {
			return y_ + height_;
		}

		int bottom () const {
			return y_;
		}

		bool IsValid (void) const
		{
			return width_ > 0 && height_ > 0;
		}

	private:

		int x_;
		int y_;
		unsigned int width_;
		unsigned int height_;
	};

}

#endif	// _BIL_RECT_HPP_
