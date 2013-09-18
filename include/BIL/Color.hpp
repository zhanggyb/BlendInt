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
		static void convert_shade_color (const Color& color,
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
			m_red = orig.m_red;
			m_green = orig.m_green;
			m_blue = orig.m_blue;
			m_alpha = orig.m_alpha;

			return *this;
		}

		Color& operator = (uint32_t color)
		{
			set_color (color);
			return *this;
		}
		
		~Color ()
		{}

		void highlight (const Color& orig)
		{
			m_red = orig.r() >= 240 ? 255 : (orig.r() + 15);
			m_green = orig.g() >= 240 ? 255 : (orig.g() + 15);
			m_blue = orig.b() >= 240 ? 255 : (orig.b() + 15);
			m_alpha = orig.a();
		}

		void highlight (uint32_t color)
		{
			set_color (color);
			m_red = m_red >= 240 ? 255 : (m_red + 15);
			m_green = m_green >= 240 ? 255 : (m_green + 15);
			m_blue = m_blue >= 240 ? 255 : (m_blue + 15);
		}

		unsigned char highlight_red () const
		{
			return m_red >= 240 ? 255 : (m_red + 15);
		}

		unsigned char highlight_green () const
		{
			return m_green >= 240 ? 255 : (m_green + 15);
		}

		unsigned char highlight_blue () const
		{
			return m_blue >= 240 ? 255 : (m_blue + 15);
		}

		void set_color (uint32_t color)
		{
			if (color > 0xFFFFFF) {
				m_alpha = color & 0xFF;
				m_blue = (color >> 8) & 0xFF;
				m_green = (color >> 16) & 0xFF;
				m_red = (color >> 24) & 0xFF;
			} else if (color > 0xFFFF){
				m_alpha = color & 0xFF;
				m_blue = (color >> 8) & 0xFF;
				m_green = (color >> 16) & 0xFF;
				m_red = 0x00;
			} else if (color > 0xFF) {
				m_alpha = color & 0xFF;
				m_blue = (color >> 8) & 0xFF;
				m_green = 0x00;
				m_red = 0x00;
			} else {
				m_alpha = color & 0xFF;
				m_blue = 0x00;
				m_green = 0x00;
				m_red = 0x00;
			}
	 	}

		void set_color (unsigned char r,
						unsigned char g,
						unsigned char b,
						unsigned char a = 0xFF)
		{
			m_red = correct_in_scope(r,
									static_cast<unsigned char>(0),
									static_cast<unsigned char>(255));
			m_green = correct_in_scope(g,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
			m_blue = correct_in_scope(b,
									 static_cast<unsigned char>(0),
									 static_cast<unsigned char>(255));
			m_alpha = correct_in_scope(a,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		unsigned char operator [] (int index) const
		{
			if (index <= 0) return m_red;
			if (index == 1) return m_green;
			if (index == 2) return m_blue;
			if (index >= 3) return m_alpha;

			return 0;
		}

		uint32_t rgba () const
		{
			return (m_red << 24) & (m_green << 16) &
				(m_blue << 8) & m_alpha;
		}

		unsigned char red () const
		{
			return m_red;
		}

		unsigned char r () const
		{
			return m_red;
		}

		void set_red (unsigned char r)
		{
			m_red = correct_in_scope(r,
									static_cast<unsigned char>(0),
									static_cast<unsigned char>(255));
		}

		unsigned char green () const
		{
			return m_green;
		}

		unsigned char g () const
		{
			return m_green;
		}

		void set_green (unsigned char g)
		{
			m_green = correct_in_scope(g,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		unsigned char blue () const
		{
			return m_blue;
		}

		unsigned char b () const
		{
			return m_blue;
		}

		void set_blue (unsigned char b)
		{
			m_blue = correct_in_scope(b,
									 static_cast<unsigned char>(0),
									 static_cast<unsigned char>(255));
		}

		unsigned char alpha () const
		{
			return m_alpha;
		}

		unsigned char a () const
		{
			return m_alpha;
		}

		void set_alpha (unsigned char a)
		{
			m_alpha = correct_in_scope(a,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

	private:

		unsigned char m_red;
		unsigned char m_green;
		unsigned char m_blue;
		unsigned char m_alpha;

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
