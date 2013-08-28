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

#ifndef _BIL_COLOR_H_
#define _BIL_COLOR_H_

#include <stdint.h>

#include <BIL/Utilities-inl.hpp>

namespace BIL {

	class Color
	{
	public:

		/**
		 * @brief Convert shade colors
		 * @param[in] color the initial color
		 * @param[in] shadetop shade of top: -100 - 100
		 * @param[in] shadedown shade of down: -100 - 100
		 * @param[out] coltop Color at the top
		 * @param[out] coldown Color at the bottom
		 */
		static void ConvertShadeColor (const Color& color,
									   short shadetop,
									   short shadedown,
									   Color* top_color,
									   Color* bottom_color);

		/**
		 * @brief Conver shade colors for round box
		 */
		static void ConvertRoundBoxShadeColor (const Color& color1,
											   const Color& color2,
											   float factor,
											   unsigned char color_output[4]);


	public:

		explicit Color (uint32_t color = 0xFFFFFFFF)
		{
			set_color (color);
		}

		Color (unsigned char r, unsigned char g, unsigned char b,
			   unsigned char a)
		{
			set_color (r, g, b, a);
		}

		Color& operator = (const Color& orig)
		{
			red_ = orig.red_;
			green_ = orig.green_;
			blue_ = orig.blue_;
			alpha_ = orig.alpha_;

			return *this;
		}

		Color& operator = (uint32_t color)
		{
			set_color (color);
			return *this;
		}
		
		virtual ~Color ()
		{}

		void HighlightFrom (const Color& orig)
		{
			red_ = orig.r() >= 240 ? 255 : (orig.r() + 15);
			green_ = orig.g() >= 240 ? 255 : (orig.g() + 15);
			blue_ = orig.b() >= 240 ? 255 : (orig.b() + 15);
			alpha_ = orig.a();
		}

		void HighlightFrom (uint32_t color)
		{
			set_color (color);
			red_ = red_ >= 240 ? 255 : (red_ + 15);
			green_ = green_ >= 240 ? 255 : (green_ + 15);
			blue_ = blue_ >= 240 ? 255 : (blue_ + 15);
		}

		void set_color (uint32_t color)
		{
			if (color > 0xFFFFFF) {
				alpha_ = color & 0xFF;
				blue_ = (color >> 8) & 0xFF;
				green_ = (color >> 16) & 0xFF;
				red_ = (color >> 24) & 0xFF;
			} else if (color > 0xFFFF){
				alpha_ = color & 0xFF;
				blue_ = (color >> 8) & 0xFF;
				green_ = (color >> 16) & 0xFF;
				red_ = 0x00;
			} else if (color > 0xFF) {
				alpha_ = color & 0xFF;
				blue_ = (color >> 8) & 0xFF;
				green_ = 0x00;
				red_ = 0x00;
			} else {
				alpha_ = color & 0xFF;
				blue_ = 0x00;
				green_ = 0x00;
				red_ = 0x00;
			}
	 	}

		void set_color (unsigned char r,
						unsigned char g,
						unsigned char b,
						unsigned char a = 0xFF)
		{
			red_ = correct_in_scope(r,
									static_cast<unsigned char>(0),
									static_cast<unsigned char>(255));
			green_ = correct_in_scope(g,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
			blue_ = correct_in_scope(b,
									 static_cast<unsigned char>(0),
									 static_cast<unsigned char>(255));
			alpha_ = correct_in_scope(a,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		unsigned char operator [] (int index) const
		{
			if (index <= 0) return red_;
			if (index == 1) return green_;
			if (index == 2) return blue_;
			if (index >= 3) return alpha_;

			return 0;
		}

		uint32_t rgba () const
		{
			return (red_ << 24) & (green_ << 16) &
				(blue_ << 8) & alpha_;
		}

		unsigned char red () const
		{
			return red_;
		}

		unsigned char r () const
		{
			return red_;
		}

		void set_red (unsigned char r)
		{
			red_ = correct_in_scope(r,
									static_cast<unsigned char>(0),
									static_cast<unsigned char>(255));
		}

		unsigned char green () const
		{
			return green_;
		}

		unsigned char g () const
		{
			return green_;
		}

		void set_green (unsigned char g)
		{
			green_ = correct_in_scope(g,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		unsigned char blue () const
		{
			return blue_;
		}

		unsigned char b () const
		{
			return blue_;
		}

		void set_blue (unsigned char b)
		{
			blue_ = correct_in_scope(b,
									 static_cast<unsigned char>(0),
									 static_cast<unsigned char>(255));
		}

		unsigned char alpha () const
		{
			return alpha_;
		}

		unsigned char a () const
		{
			return alpha_;
		}

		void set_alpha (unsigned char a)
		{
			alpha_ = correct_in_scope(a,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

	private:

		unsigned char red_;
		unsigned char green_;
		unsigned char blue_;
		unsigned char alpha_;

	};

	/*
	Color operator + (const Color& color1, const Color& color2)
	{
		Color color;

		color.set_red (correct_in_scope(color1.r() + color2.r(), 0, 255));
		color.set_green (correct_in_scope(color1.g() + color2.g(), 0, 255));
		color.set_blue (correct_in_scope(color1.b() + color2.b(), 0, 255));
		color.set_alpha (correct_in_scope(color1.a() + color2.a(), 0, 255));

		return color;
	}
	*/
}

#endif	// _BIL_COLOR_H_
