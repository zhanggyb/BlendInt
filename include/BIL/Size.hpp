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

		Size (int width, int height)
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

		int width () const {return width_;}

		void set_width (int width) {width_ = width;}

		int height () const {return height_;}

		void set_height (int height) {height_ = height;}

		int& operator [] (int index)
		{
			if(index <= 0) return width_;

			return height_;
		}

	private:
		int width_;
		int height_;
	};

}

#endif /* SIZE_HPP_ */
