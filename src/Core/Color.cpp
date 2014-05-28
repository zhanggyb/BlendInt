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

#include <string.h>

#include <BlendInt/Core/Color.hpp>

namespace BlendInt {

	Color operator + (const Color& orig, short shade)
	{
		Color color;

		color.set_red(orig.r() + shade);
		color.set_green(orig.g() + shade);
		color.set_blue(orig.b() + shade);
		color.set_alpha(orig.a());

		return color;
	}

	ColorExt operator + (const ColorExt& orig, short shade)
	{
		ColorExt color;
		color.set_red(orig.r() + shade);
		color.set_green(orig.g() + shade);
		color.set_blue(orig.b() + shade);
		color.set_alpha(orig.a());

		return color;
	}

	ColorExt operator + (const ColorExt& color1, const ColorExt& color2)
	{
		ColorExt color;
		color.set_red(color1.r() + color2.r());
		color.set_green(color1.g() + color2.g());
		color.set_blue(color1.b() + color2.b());
		color.set_alpha(color1.a() + color2.a());

		return color;
	}

	Color make_shaded_color (const Color& color1, const Color& color2, float factor)
	{
		unsigned char faci = float_to_uchar (factor);
		unsigned char facm = 255 - faci;
		Color shaded_color;

		shaded_color.set_red((faci * color1.r() + facm * color2.r()) >> 8);
		shaded_color.set_green((faci * color1.g() + facm * color2.g()) >> 8);
		shaded_color.set_blue((faci * color1.b() + facm * color2.b()) >> 8);
		shaded_color.set_alpha((faci * color1.a() + facm * color2.a()) >> 8);

		return shaded_color;
	}

	ColorExt MakeShadedColor (const ColorExt& color1, const ColorExt& color2, float fact)
	{
		ColorExt shaded_color;

		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		shaded_color.set_red(faci * color1.r() + facm * color2.r());
		shaded_color.set_green(faci * color1.g() + facm * color2.g());
		shaded_color.set_blue(faci * color1.b() + facm * color2.b());
		shaded_color.set_alpha(faci * color1.a() + facm * color2.a());

		return shaded_color;
	}

	Color::Color()
	{
		memset (m_color_v, 0xFF, sizeof(unsigned char) * 4);
	}

	Color::Color(const Color& orig)
	{
		memcpy (m_color_v, orig.data(), sizeof(unsigned char) * 4);
	}

	Color& Color::operator = (const Color& orig)
	{
		memcpy (m_color_v, orig.data(), sizeof(unsigned char) * 4);

		return *this;
	}

	void Color::set_color(unsigned char color[4])
	{
		memcpy (m_color_v, color, sizeof(unsigned char) * 4);
	}

	void Color::set_color (uint32_t color)
	{
		if (color > 0xFFFFFF) {
			m_color_v[3] = color & 0xFF;
			m_color_v[2] = (color >> 8) & 0xFF;
			m_color_v[1] = (color >> 16) & 0xFF;
			m_color_v[0] = (color >> 24) & 0xFF;
		} else if (color > 0xFFFF){
			m_color_v[3] = color & 0xFF;
			m_color_v[2] = (color >> 8) & 0xFF;
			m_color_v[1] = (color >> 16) & 0xFF;
			m_color_v[0] = 0x00;
		} else if (color > 0xFF) {
			m_color_v[3] = color & 0xFF;
			m_color_v[2] = (color >> 8) & 0xFF;
			m_color_v[1] = 0x00;
			m_color_v[0] = 0x00;
		} else {
			m_color_v[3] = color & 0xFF;
			m_color_v[2] = 0x00;
			m_color_v[1] = 0x00;
			m_color_v[0] = 0x00;
		}
 	}

	void Color::set_color (unsigned char r,
					unsigned char g,
					unsigned char b,
					unsigned char a)
	{
		m_color_v[0] = correct_in_scope(r,
								static_cast<unsigned char>(0),
								static_cast<unsigned char>(255));
		m_color_v[1] = correct_in_scope(g,
								  static_cast<unsigned char>(0),
								  static_cast<unsigned char>(255));
		m_color_v[2] = correct_in_scope(b,
								 static_cast<unsigned char>(0),
								 static_cast<unsigned char>(255));
		m_color_v[3] = correct_in_scope(a,
								  static_cast<unsigned char>(0),
								  static_cast<unsigned char>(255));
	}

	void Color::highlight (const Color& orig, unsigned char value)
	{
		unsigned char max = 255 - value;
		m_color_v[0] = orig.r() >= max ? 255 : (orig.r() + value);
		m_color_v[1] = orig.g() >= max ? 255 : (orig.g() + value);
		m_color_v[2] = orig.b() >= max ? 255 : (orig.b() + value);
		m_color_v[3] = orig.a();
	}

	void Color::highlight (uint32_t color, unsigned char value)
	{
		unsigned char max = 255 - value;
		set_color (color);
		m_color_v[0] = m_color_v[0] >= max ? 255 : (m_color_v[0] + value);
		m_color_v[1] = m_color_v[1] >= max ? 255 : (m_color_v[1] + value);
		m_color_v[2] = m_color_v[2] >= max ? 255 : (m_color_v[2] + value);
	}

	ColorExt::ColorExt ()
	{
		m_value[0] = 1.f;
		m_value[1] = 1.f;
		m_value[2] = 1.f;
		m_value[3] = 1.f;
	}

	ColorExt::ColorExt (uint32_t color)
	{
		SetValue(color);
	}

	ColorExt::ColorExt (unsigned char r, unsigned char g, unsigned char b,
	        unsigned char a)
	{
		SetValue(r, g, b, a);
	}

	ColorExt::ColorExt (float r, float g, float b, float a)
	{
		SetValue(r, g, b, a);
	}

	ColorExt::ColorExt (const ColorExt& orig)
	{
		m_value[0] = orig.m_value[0];
		m_value[1] = orig.m_value[1];
		m_value[2] = orig.m_value[2];
		m_value[3] = orig.m_value[3];
	}

	ColorExt& ColorExt::operator = (const ColorExt& orig)
	{
		m_value[0] = orig.m_value[0];
		m_value[1] = orig.m_value[1];
		m_value[2] = orig.m_value[2];
		m_value[3] = orig.m_value[3];
		return *this;
	}

	ColorExt& ColorExt::operator = (uint32_t color)
	{
		SetValue(color);
		return *this;
	}

	ColorExt::~ColorExt ()
	{
	}

	void ColorExt::Highlight (const ColorExt& orig, short value)
	{
		m_value[0] = glm::clamp(orig.m_value[0] + value / 255.f, 0.f, 1.f);
		m_value[1] = glm::clamp(orig.m_value[1] + value / 255.f, 0.f, 1.f);
		m_value[2] = glm::clamp(orig.m_value[2] + value / 255.f, 0.f, 1.f);
		m_value[3] = orig.a();
	}

	void ColorExt::Highlight (uint32_t color, short value)
	{
		SetValue(color);
		m_value[0] = glm::clamp(m_value[0] + value / 255.f, 0.f, 1.f);
		m_value[1] = glm::clamp(m_value[1] + value / 255.f, 0.f, 1.f);;
		m_value[2] = glm::clamp(m_value[2] + value / 255.f, 0.f, 1.f);;
	}

	void ColorExt::SetValue (float r, float g, float b, float a)
	{
		m_value[0] = glm::clamp(r, 0.f, 1.f);
		m_value[1] = glm::clamp(g, 0.f, 1.f);
		m_value[2] = glm::clamp(b, 0.f, 1.f);
		m_value[3] = glm::clamp(a, 0.f, 1.f);
	}

	void ColorExt::SetValue (unsigned char color[4])
	{
		m_value[0] = color[0] / 255.f;
		m_value[1] = color[1] / 255.f;
		m_value[2] = color[2] / 255.f;
		m_value[3] = color[3] / 255.f;
	}

	void ColorExt::SetValue (float color[4])
	{
		m_value[0] = color[0];
		m_value[1] = color[1];
		m_value[2] = color[2];
		m_value[3] = color[3];
	}

	void ColorExt::SetValue (uint32_t color)
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

	void ColorExt::SetValue (unsigned char r, unsigned char g,
	        unsigned char b, unsigned char a)
	{
		m_value[0] = r / 255.f;
		m_value[1] = g / 255.f;
		m_value[2] = b / 255.f;
		m_value[3] = a / 255.f;
	}

}
