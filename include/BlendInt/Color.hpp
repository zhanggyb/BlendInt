/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_COLOR_HPP_
#define _BLENDINT_COLOR_HPP_

#include <stdint.h>

#include <BlendInt/Utilities-inl.hpp>

namespace BlendInt {

	class Color;

	/**
	 * @brief add r,g,b with a given value
	 * @param orig
	 * @param shade
	 * @return a new Color
	 *
	 * This function mostly used to calculate a shaded color in Widget drawing
	 */
	extern Color operator + (const Color& orig, short shade);

	/**
	 * @brief make a shader color between 2 given colors
	 * @param[in] color1
	 * @param[in] color2
	 * @param[in] factor must be within 0.0 ~ 1.0
	 * @return
	 */
	extern Color make_shade_color (const Color& color1, const Color& color2, float factor);

	class Color
	{
	public:

		Color ();

		explicit Color (uint32_t color)
		{
			set_color (color);
		}

		Color (unsigned char r, unsigned char g, unsigned char b,
			   unsigned char a)
		{
			set_color (r, g, b, a);
		}

		Color (const Color& orig);

		Color& operator = (const Color& orig);

		Color& operator = (uint32_t color)
		{
			set_color (color);
			return *this;
		}

		~Color ()
		{}

		void highlight (const Color& orig, unsigned char value = 15);

		void highlight (uint32_t color, unsigned char value = 15);

		unsigned char highlight_red () const
		{
			return m_color_v[0] >= 240 ? 255 : (m_color_v[0] + 15);
		}

		unsigned char highlight_green () const
		{
			return m_color_v[1] >= 240 ? 255 : (m_color_v[1] + 15);
		}

		unsigned char highlight_blue () const
		{
			return m_color_v[2] >= 240 ? 255 : (m_color_v[2] + 15);
		}

		void set_color (unsigned char color[4]);

		void set_color (uint32_t color);

		void set_color (unsigned char r,
						unsigned char g,
						unsigned char b,
						unsigned char a = 0xFF);

		unsigned char operator [] (int index) const
		{
			return m_color_v[index];
		}

		uint32_t rgba () const
		{
			return (m_color_v[0] << 24) & (m_color_v[1] << 16) &
				(m_color_v[2] << 8) & m_color_v[3];
		}

		unsigned char red () const
		{
			return m_color_v[0];
		}

		unsigned char r () const
		{
			return m_color_v[0];
		}

		void set_red (unsigned char r)
		{
			m_color_v[0] = correct_in_scope(r,
									static_cast<unsigned char>(0),
									static_cast<unsigned char>(255));
		}

		unsigned char green () const
		{
			return m_color_v[1];
		}

		unsigned char g () const
		{
			return m_color_v[1];
		}

		void set_green (unsigned char g)
		{
			m_color_v[1] = correct_in_scope(g,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		unsigned char blue () const
		{
			return m_color_v[2];
		}

		unsigned char b () const
		{
			return m_color_v[2];
		}

		void set_blue (unsigned char b)
		{
			m_color_v[2] = correct_in_scope(b,
									 static_cast<unsigned char>(0),
									 static_cast<unsigned char>(255));
		}

		unsigned char alpha () const
		{
			return m_color_v[3];
		}

		unsigned char a () const
		{
			return m_color_v[3];
		}

		void set_alpha (unsigned char a)
		{
			m_color_v[3] = correct_in_scope(a,
									  static_cast<unsigned char>(0),
									  static_cast<unsigned char>(255));
		}

		void add (short shade)
		{
			set_red(m_color_v[0] + shade);
			set_green(m_color_v[1] + shade);
			set_blue(m_color_v[2] + shade);
		}

		const unsigned char* data () const {return m_color_v;}

	private:

		/**
		 * @brief Array to store color data
		 *
		 * [0]: red
		 * [1]: green
		 * [2]: blue
		 * [3]: alpha
		 */
		unsigned char m_color_v[4];
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

#endif	// _BLENDINT_COLOR_HPP_
