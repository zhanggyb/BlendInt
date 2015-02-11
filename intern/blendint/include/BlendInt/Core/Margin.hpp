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

/**
 * @brief Margin
 *
 * used for box model of a widget
 */
namespace BlendInt {

	class Margin
	{
	public:

		Margin ()
		: left_(5), right_(5), top_(5), bottom_(5)
		{}

		Margin (int all)
		: left_(all), right_(all), top_(all), bottom_(all)
		{}

		Margin (int left, int right, int top, int bottom)
		: left_(left), right_(right), top_(top), bottom_(bottom)
		{}

		void set_value (int left, int right, int top, int bottom)
		{
			left_ = left;
			right_ = right;
			top_ = top;
			bottom_ = bottom;
		}

		int left () const {return left_;}

		void set_left (int left) {left_ = left;}

		int right () const {return right_;}

		void set_right (int right) {right_ = right;}

		int top () const {return top_;}

		void set_top (int top) {top_ = top;}

		int bottom () const {return bottom_;}

		void set_bottom (int bottom) {bottom_ = bottom;}

		int hsum () const
		{
			return left_ + right_;
		}

		int vsum () const
		{
			return top_ + bottom_;
		}

		bool equal (int left, int right, int top, int bottom)
		{
			return (left_ == left &&
					right_ == right &&
					top_ == top &&
					bottom_ == bottom);
		}

		bool equal (const Margin& margin)
		{
			return (left_ == margin.left() &&
					right_ == margin.right() &&
					top_ == margin.top() &&
					bottom_ == margin.bottom());
		}

	private:

		friend inline bool operator == (const Margin& src, const Margin& dst);

		int left_;
		int right_;
		int top_;
		int bottom_;
	};

	inline bool operator == (const Margin& src, const Margin& dst)
	{
		return (src.left_ == dst.left_) &&
				(src.right_ == dst.right_) &&
				(src.top_ == dst.top_) &&
				(src.bottom_ == dst.bottom_);
	}

}
