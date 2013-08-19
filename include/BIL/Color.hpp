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

		enum ColorDepth {
			ColorDepth8BIT,
			ColorDepth16BIT,
			ColorDepthFloat16BIT,
			ColorDepthFloat32BIT
		};

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
											   Color* color_output);


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

			depth_ = orig.depth_;
			return *this;
		}

		Color& operator = (uint32_t color)
		{
			set_color (color);
			return *this;
		}
		
		virtual ~Color ()
		{}

		void set_color (uint32_t color)
		{
			if (color > 0xFFFFFF) {
				alpha_ = color & 0xFF;
				blue_ = (color >> 8) & 0xFF;
				green_ = (color >> 16) & 0xFF;
				red_ = (color >> 24) & 0xFF;
			} else {
				alpha_ = 0xFF;
				blue_ = color & 0xFF;
				green_ = (color >> 8) & 0xFF;
				red_ = (color >> 16) & 0xFF;
			}
			depth_ = ColorDepth8BIT;
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
			depth_ = ColorDepth8BIT;
		}

		uint32_t rgba (void) const
		{
			return (red_ << 24) & (green_ << 16) &
				(blue_ << 8) & alpha_;
		}

		unsigned char red (void) const
		{
			return red_;
		}

		unsigned char r (void) const
		{
			return red_;
		}

		void set_red (unsigned char r)
		{
			red_ = correct_in_scope(r,
									static_cast<unsigned char>(0),
									static_cast<unsigned char>(255));
		}

		unsigned char green (void) const
		{
			return green_;
		}

		unsigned char g (void) const
		{
			return green_;
		}

		void set_green (unsigned char g)
		{
			green_ = correct_in_scope(g,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		unsigned char blue (void) const
		{
			return blue_;
		}

		unsigned char b (void) const
		{
			return blue_;
		}

		void set_blue (unsigned char b)
		{
			blue_ = correct_in_scope(b,
									 static_cast<unsigned char>(0),
									 static_cast<unsigned char>(255));
		}

		unsigned char alpha (void) const
		{
			return alpha_;
		}

		unsigned char a (void) const
		{
			return alpha_;
		}

		void set_alpha (unsigned char a)
		{
			alpha_ = correct_in_scope(a,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		ColorDepth depth (void) const {return depth_;}

	private:

		unsigned char red_;
		unsigned char green_;
		unsigned char blue_;
		unsigned char alpha_;

		ColorDepth depth_;

	};

}

#endif	// _BIL_COLOR_H_
