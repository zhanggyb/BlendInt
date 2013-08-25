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
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_SIZE_HPP_
#define _BIL_SIZE_HPP_

#include <BIL/Point.hpp>
#include <BIL/Coord.hpp>

namespace BIL {

	/**
	 * @brief Size
	 *
	 * only used in widgets/windows to define its size
	 */
	class Size
	{
	public:

		Size ()
				: width_(0), height_(0)
		{

		}

		Size (unsigned int width, unsigned int height)
				: width_(width), height_(height)
		{

		}

		Size (const Size& orig)
				: width_(orig.width_), height_(orig.height_)
		{

		}

		Size& operator = (const Size& orig)
		{
			width_ = orig.width_;
			height_ = orig.height_;
			return *this;
		}

		bool IsValid () const
		{
			return width_ > 0 && height_ > 0;
		}

		bool Contains (const Point& pos)
		{
			if (pos.x() < 0 || pos.y() < 0 ||
				pos.x() > static_cast<int>(width_) || pos.y() > static_cast<int>(height_))
			{
				return false;
			}
			
			return true;
		}

		bool Contains (const Coord2f& pos)
		{
			if (pos.x() < 0 || pos.y() < 0 ||
				pos.x() > width_ || pos.y() > height_)
			{
				return false;
			}
			
			return true;
		}

		unsigned int width () const {return width_;}

		void set_width (unsigned int width) {width_ = width;}

		void set_width (int width) {width_ = static_cast<unsigned int>(width);}

		unsigned int height () const {return height_;}

		void set_height (unsigned int height) {height_ = height;}

		void set_height (int height) {height_ = static_cast<unsigned int>(height);}

		unsigned int& operator [] (int index)
		{
			if(index <= 0) return width_;

			return height_;
		}

	private:
		unsigned int width_;
		unsigned int height_;
	};

}

#endif /* SIZE_HPP_ */
