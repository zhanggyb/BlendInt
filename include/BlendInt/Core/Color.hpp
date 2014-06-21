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

#ifndef _BLENDINT_COLOR_HPP_
#define _BLENDINT_COLOR_HPP_

#include <stdint.h>
#include <glm/glm.hpp>

#include <BlendInt/Utilities-inl.hpp>

namespace BlendInt {

	class Color;

	extern Color operator + (const Color& orig, short shade);

	extern Color operator + (const Color& color1, const Color& color2);

	extern Color MakeShadedColor (const Color& color1, const Color& color2, float fact);

	class Color
	{
	public:

		Color ()
		{
			m_value[0] = 1.f;
			m_value[1] = 1.f;
			m_value[2] = 1.f;
			m_value[3] = 1.f;
		}

		explicit Color (uint32_t color)
		{
			set_value(color);
		}

		Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			set_value(r, g, b, a);
		}

		Color(float r, float g, float b, float a)
		{
			set_value(r, g, b, a);
		}

		Color (const Color& orig)
		{
			m_value[0] = orig.m_value[0];
			m_value[1] = orig.m_value[1];
			m_value[2] = orig.m_value[2];
			m_value[3] = orig.m_value[3];
		}

		Color& operator = (const Color& orig)
		{
			m_value[0] = orig.m_value[0];
			m_value[1] = orig.m_value[1];
			m_value[2] = orig.m_value[2];
			m_value[3] = orig.m_value[3];
			return *this;
		}

		Color& operator = (uint32_t color)
		{
			set_value(color);
			return *this;
		}

		~Color ()
		{}

		void set_value (float r, float g, float b, float a)
		{
			m_value[0] = glm::clamp(r, 0.f, 1.f);
			m_value[1] = glm::clamp(g, 0.f, 1.f);
			m_value[2] = glm::clamp(b, 0.f, 1.f);
			m_value[3] = glm::clamp(a, 0.f, 1.f);
		}

		void set_value (unsigned char color[4])
		{
			m_value[0] = color[0] / 255.f;
			m_value[1] = color[1] / 255.f;
			m_value[2] = color[2] / 255.f;
			m_value[3] = color[3] / 255.f;
		}

		void set_value (float color[4])
		{
			m_value[0] = glm::clamp(color[0], 0.f, 1.f);
			m_value[1] = glm::clamp(color[1], 0.f, 1.f);
			m_value[2] = glm::clamp(color[2], 0.f, 1.f);
			m_value[3] = glm::clamp(color[3], 0.f, 1.f);
		}

		void set_value (uint32_t color)
		{
			if (color > 0xFFFFFF) {
				m_value[3] = (color & 0xFF) / 255.f;
				m_value[2] = ((color >> 8) & 0xFF) / 255.f;
				m_value[1] = ((color >> 16) & 0xFF) / 255.f;
				m_value[0] = ((color >> 24) & 0xFF) / 255.f;
			} else if (color > 0xFFFF) {
				m_value[3] = (color & 0xFF) / 255.f;
				m_value[2] = ((color >> 8) & 0xFF) / 255.f;
				m_value[1] = ((color >> 16) & 0xFF) / 255.f;
				m_value[0] = 0.f;	// 0x00
			} else if (color > 0xFF) {
				m_value[3] = (color & 0xFF) / 255.f;
				m_value[2] = ((color >> 8) & 0xFF) / 255.f;
				m_value[1] = 0.f;
				m_value[0] = 0.f;
			} else {
				m_value[3] = (color & 0xFF) / 255.f;
				m_value[2] = 0.f;
				m_value[1] = 0.f;
				m_value[0] = 0.f;
			}
		}

		void set_value (unsigned char r,
				unsigned char g,
				unsigned char b,
				unsigned char a = 0xFF)
		{
			m_value[0] = r / 255.f;
			m_value[1] = g / 255.f;
			m_value[2] = b / 255.f;
			m_value[3] = a / 255.f;
		}

		uint32_t rgba () const
		{
			return ((int)(m_value[0] * 255) << 24) |
					((int)(m_value[1] * 255) << 16) |
					((int)(m_value[2] * 255) << 8) |
					(int)(m_value[3] * 255);
		}

		unsigned char uchar_red () const
		{
			return (unsigned char)(m_value[0] * 255);
		}

		float red () const
		{
			return m_value[0];
		}

		float r () const
		{
			return m_value[0];
		}

		void set_red (float red)
		{
			m_value[0] = glm::clamp(red, 0.f, 1.f);
		}

		void set_red (unsigned char red)
		{
			m_value[0] = red / 255.f;
		}

		unsigned char uchar_green () const
		{
			return (unsigned char)(m_value[1] * 255);
		}

		float green () const
		{
			return m_value[1];
		}

		float g () const
		{
			return m_value[1];
		}

		void set_green (float green)
		{
			m_value[1] = glm::clamp(green, 0.f, 1.f);
		}

		void set_green (unsigned char green)
		{
			m_value[1] = green / 255.f;
		}

		unsigned char uchar_blue () const
		{
			return (unsigned char)(m_value[2] * 255);
		}

		float blue () const
		{
			return m_value[2];
		}

		float b () const
		{
			return m_value[2];
		}

		void set_blue (float blue)
		{
			m_value[2] = glm::clamp(blue, 0.f, 1.f);
		}

		void set_blue (unsigned char blue)
		{
			m_value[2] = blue / 255.f;
		}

		unsigned char uchar_alpha () const
		{
			return (unsigned char)(m_value[3] * 255);
		}

		float alpha () const
		{
			return m_value[3];
		}

		float a () const
		{
			return m_value[3];
		}

		void set_alpha (float alpha)
		{
			m_value[3] = glm::clamp(alpha, 0.f, 1.f);
		}

		void set_alpha (unsigned char alpha)
		{
			m_value[3] = alpha / 255.f;
		}

		const float* data () const {return m_value;}

		// CSS Named Colors

		// Standard Colors
		static const unsigned int Aqua = 	0x00FFFFFF;
		static const unsigned int Black =	0x000000FF;
		static const unsigned int Blue = 	0x0000FFFF;
		static const unsigned int Fuchsia =	0xFF00FFFF;
		static const unsigned int Gray = 	0x808080FF;
		static const unsigned int Green =	0x008000FF;
		static const unsigned int Lime =	0x00FF00FF;
		static const unsigned int Maroon =	0x800000FF;
		static const unsigned int Navy = 	0x000080FF;
		static const unsigned int Olive = 	0x808000FF;
		static const unsigned int Orange =	0xFFA500FF;
		static const unsigned int Purple = 	0x800080FF;
		static const unsigned int Red =		0xFF0000FF;
		static const unsigned int Silver = 	0xC0C0C0FF;
		static const unsigned int Teal = 	0x008080FF;
		static const unsigned int White = 	0xFFFFFFFF;
		static const unsigned int Yellow =	0xFFFF00FF;

		// Extended Colors

		static const unsigned int AliceBlue = 0xF0F8FFFF;
		static const unsigned int AntiqueWhite = 0xFAEBD7FF;
		static const unsigned int Aquamarine = 0x7FFFD4FF;
		static const unsigned int Azure = 0xF0FFFFFF;
		static const unsigned int Beige = 0xF5F5DCFF;
		static const unsigned int Bisque = 0xFFE4C4FF;
		static const unsigned int BlanchedAlmond = 0xFFEBCDFF;
		static const unsigned int BlueViolet = 0x8A2BE2FF;
		static const unsigned int Brown = 0xA52A2AFF;
		static const unsigned int BurlyWood = 0xDEB887FF;
		static const unsigned int CadetBlue = 0x5F9EA0FF;
		static const unsigned int Chartreuse = 0x7FFF00FF;
		static const unsigned int Chocolate = 0xD2691EFF;
		static const unsigned int Coral = 0xFF7F50FF;
		static const unsigned int CornflowerBlue = 0x6495EDFF;
		static const unsigned int Cornsilk = 0xFFF8DCFF;
		static const unsigned int Crimson = 0xDC143CFF;
		static const unsigned int Cyan = 0x00FFFFFF;
		static const unsigned int DarkBlue = 0x00008BFF;
		static const unsigned int DarkCyan = 0x008B8BFF;
		static const unsigned int DarkGoldenRod = 0xB8860BFF;
		static const unsigned int DarkGray = 0xA9A9A9FF;
		static const unsigned int DarkGreen = 0x006400FF;
		static const unsigned int DarkKhaki = 0xBDB76BFF;
		static const unsigned int DarkMagenta = 0x8B008BFF;
		static const unsigned int DarkOliveGreen = 0x556B2FFF;
		static const unsigned int DarkOrange = 0xFF8C00FF;
		static const unsigned int DarkOrchid = 0x9932CCFF;
		static const unsigned int DarkRed = 0x8B0000FF;
		static const unsigned int DarkSalmon = 0xE9967AFF;
		static const unsigned int DarkSeaGreen = 0x8FBC8FFF;
		static const unsigned int DarkSlateBlue = 0x483D8BFF;
		static const unsigned int DarkSlateGray = 0x2F4F4FFF;
		static const unsigned int DarkTurquoise = 0x00CED1FF;
		static const unsigned int DarkViolet = 0x9400D3FF;
		static const unsigned int DeepPink = 0xFF1493FF;
		static const unsigned int DeepSkyBlue = 0x00BFFFFF;
		static const unsigned int DimGray = 0x696969FF;
		static const unsigned int DodgerBlue = 0x1E90FFFF;
		static const unsigned int FireBrick = 0xB22222FF;
		static const unsigned int FloralWhite = 0xFFFAF0FF;
		static const unsigned int ForestGreen = 0x228B22FF;
		static const unsigned int Gainsboro = 0xDCDCDCFF;
		static const unsigned int GhostWhite = 0xF8F8FFFF;
		static const unsigned int Gold = 0xFFD700FF;
		static const unsigned int GoldenRod = 0xDAA520FF;
		static const unsigned int GreenYellow = 0xADFF2FFF;
		static const unsigned int HoneyDew = 0xF0FFF0FF;
		static const unsigned int HotPink = 0xFF69B4FF;
		static const unsigned int IndianRed = 0xCD5C5CFF;
		static const unsigned int Indigo = 0x4B0082FF;
		static const unsigned int Ivory = 0xFFFFF0FF;
		static const unsigned int Khaki = 0xF0E68CFF;
		static const unsigned int Lavender = 0xE6E6FAFF;
		static const unsigned int LavenderBlush = 0xFFF0F5FF;
		static const unsigned int LawnGreen = 0x7CFC00FF;
		static const unsigned int LemonChiffon = 0xFFFACDFF;
		static const unsigned int LightBlue = 0xADD8E6FF;
		static const unsigned int LightCoral = 0xF08080FF;
		static const unsigned int LightCyan = 0xE0FFFFFF;
		static const unsigned int LightGoldenRodYellow = 0xFAFAD2FF;
		static const unsigned int LightGray = 0xD3D3D3FF;
		static const unsigned int LightGreen = 0x90EE90FF;
		static const unsigned int LightPink = 0xFFB6C1FF;
		static const unsigned int LightSalmon = 0xFFA07AFF;
		static const unsigned int LightSeaGreen = 0x20B2AAFF;
		static const unsigned int LightSkyBlue = 0x87CEFAFF;
		static const unsigned int LightSlateGray = 0x778899FF;
		static const unsigned int LightSteelBlue = 0xB0C4DEFF;
		static const unsigned int LightYellow = 0xFFFFE0FF;
		static const unsigned int LimeGreen = 0x32CD32FF;
		static const unsigned int Linen = 0xFAF0E6FF;
		static const unsigned int Magenta = 0xFF00FFFF;
		static const unsigned int MediumAquaMarine = 0x66CDAAFF;
		static const unsigned int MediumBlue = 0x0000CDFF;
		static const unsigned int MediumOrchid = 0xBA55D3FF;
		static const unsigned int MediumPurple = 0x9370DBFF;
		static const unsigned int MediumSeaGreen = 0x3CB371FF;
		static const unsigned int MediumSlateBlue = 0x7B68EEFF;
		static const unsigned int MediumSpringGreen = 0x00FA9AFF;
		static const unsigned int MediumTurquoise = 0x48D1CCFF;
		static const unsigned int MediumVioletRed = 0xC71585FF;
		static const unsigned int MidnightBlue = 0x191970FF;
		static const unsigned int MintCream =	 0xF5FFFAFF;
		static const unsigned int MistyRose = 0xFFE4E1FF;
		static const unsigned int Moccasin = 0xFFE4B5FF;
		static const unsigned int NavajoWhite = 0xFFDEADFF;
		static const unsigned int OldLace = 0xFDF5E6FF;
		static const unsigned int OliveDrab = 0x6B8E23FF;
		static const unsigned int OrangeRed = 0xFF4500FF;
		static const unsigned int Orchid = 0xDA70D6FF;
		static const unsigned int PaleGoldenRod = 0xEEE8AAFF;
		static const unsigned int PaleGreen = 0x98FB98FF;
		static const unsigned int PaleTurquoise = 0xAFEEEEFF;
		static const unsigned int PaleVioletRed = 0xDB7093FF;
		static const unsigned int PapayaWhip = 0xFFEFD5FF;
		static const unsigned int PeachPuff = 0xFFDAB9FF;
		static const unsigned int Peru = 0xCD853FFF;
		static const unsigned int Pink = 0xFFC0CBFF;
		static const unsigned int Plum = 0xDDA0DDFF;
		static const unsigned int PowderBlue = 0xB0E0E6FF;
		static const unsigned int RosyBrown = 0xBC8F8FFF;
		static const unsigned int RoyalBlue = 0x4169E1FF;
		static const unsigned int SaddleBrown = 0x8B4513FF;
		static const unsigned int Salmon = 0xFA8072FF;
		static const unsigned int SandyBrown = 0xF4A460FF;
		static const unsigned int SeaGreen = 0x2E8B57FF;
		static const unsigned int SeaShell = 0xFFF5EEFF;
		static const unsigned int Sienna = 0xA0522DFF;
		static const unsigned int SkyBlue = 0x87CEEBFF;
		static const unsigned int SlateBlue = 0x6A5ACDFF;
		static const unsigned int SlateGray = 0x708090FF;
		static const unsigned int Snow = 0xFFFAFAFF;
		static const unsigned int SpringGreen = 0x00FF7FFF;
		static const unsigned int SteelBlue = 0x4682B4FF;
		static const unsigned int Tan = 0xD2B48CFF;
		static const unsigned int Thistle = 0xD8BFD8FF;
		static const unsigned int Tomato = 0xFF6347FF;
		static const unsigned int Turquoise = 0x40E0D0FF;
		static const unsigned int Violet = 0xEE82EEFF;
		static const unsigned int Wheat = 0xF5DEB3FF;
		static const unsigned int WhiteSmoke = 0xF5F5F5FF;
		static const unsigned int YellowGreen = 0x9ACD32FF;

	private:

		float m_value[4];
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
