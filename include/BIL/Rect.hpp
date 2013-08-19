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

#include <BIL/Coord.hpp>

namespace BIL {

	template <typename T>
	class Rect
	{
	public:
		Rect ()
			: x_(T(0)), y_(T(0)), width_(T(0)), height_(T(0)), valid_(false)
		{}
		
		Rect (T x, T y, T width, T height)
			: x_(x), y_(y), width_(width), height_(height), valid_(false)
		{
			if (width_ > T(0.0) && height_ > T(0.0)) {
				valid_ = true;
			}
		}

		Rect (Coord2<T> bottom_left, Coord2<T> top_right)
		: valid_(false)
		{
			x_ = bottom_left.x();
			y_ = bottom_left.y();
			width_ = top_right.x() - x_;
			height_ = top_right.y() - y_;

			if (width_ > T(0.0) && height_ > T(0.0)) {
				valid_ = true;
			}
		}

		T x (void) const {return x_;}

		void set_x (T x) {x_ = x;}

		T y (void) const {return y_;}

		void set_y (T y) {y_ = y;}

		T width (void) const {return width_;}

		void set_width (T width) {width_ = width;}

		T height (void) const {return height_;}

		void set_height (T height) {height_ = height;}

		bool valid (void) const {return valid_;}

	private:

		T x_;
		T y_;
		T width_;
		T height_;

		bool valid_;
	};

	typedef Rect<int> Recti;
	typedef Rect<float> Rectf;

}

#endif	// _BIL_RECT_HPP_
