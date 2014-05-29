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
