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

#ifndef _BLENDINT_GUI_TEXTUREFONT_HPP_
#define _BLENDINT_GUI_TEXTUREFONT_HPP_

#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/Gui/Glyph.hpp>

#ifdef DEBUG
#define WIDTH 72
#define HEIGHT 72
#endif

namespace BlendInt {

	/**
	 * @brief A class to generate texture for single character with Freetype
	 */
	class TextureGlyph
	{
	public:

		TextureGlyph ();

		~TextureGlyph ();

		void Load (const Freetype& freetype, wchar_t charcode);

		void generate (Freetype* freetype, wchar_t charcode);

		const Glyph& glyph () const
		{
			return m_glyph;
		}

		const GLuint& texture () const
		{
			return m_texture;
		}

		unsigned int width () const
		{
			return static_cast<unsigned int>(m_glyph.bitmap_width);
		}

		unsigned int height () const
		{
			return static_cast<unsigned int>(m_glyph.bitmap_height);
		}

	private:

		GLuint m_texture;

		Glyph m_glyph;

#ifdef DEBUG
		void draw_bitmap (FT_Bitmap* bitmap, FT_Int x, FT_Int y);

		void show_image (FT_Bitmap* bitmap);

		static unsigned char image[HEIGHT][WIDTH];
#endif
	};
}


#endif /* _BIL_TEXTUREFONT_H_ */
