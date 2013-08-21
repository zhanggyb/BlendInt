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

#include <BIL/Point.hpp>
#include <BIL/Size.hpp>

namespace BIL {

	class Rect
	{
	public:
		Rect ()
			: x_(0), y_(0), width_(0), height_(0), valid_(false)
		{}
		
		Rect (int x, int y, int width, int height)
			: x_(x), y_(y), width_(width), height_(height), valid_(false)
		{
			if (width_ > 0 && height_ > 0) {
				valid_ = true;
			}
		}

		Rect (const Point& bottom_left, const Point& top_right)
		: valid_(false)
		{
			x_ = bottom_left.x();
			y_ = bottom_left.y();
			width_ = top_right.x() - x_;
			height_ = top_right.y() - y_;

			if (width_ > 0 && height_ > 0) {
				valid_ = true;
			}
		}

		Rect (const Point& pos, const Size& size)
		: valid_(false)
		{
			x_ = pos.x();
			y_ = pos.y();
			width_ = size.width();
			height_ = size.height();

			if (width_ > 0 && height_ > 0) {
				valid_ = true;
			}
		}

		int x (void) const {return x_;}

		void set_x (int x) {x_ = x;}

		int y (void) const {return y_;}

		void set_y (int y) {y_ = y;}

		int width (void) const {return width_;}

		void set_width (int width) {width_ = width;}

		int height (void) const {return height_;}

		void set_height (int height) {height_ = height;}

		bool valid (void) const {return valid_;}

	private:

		int x_;
		int y_;
		int width_;
		int height_;

		bool valid_;
	};

}

#endif	// _BIL_RECT_HPP_
