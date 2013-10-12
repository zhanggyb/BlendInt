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

#ifndef _BLENDINT_TEXTUREFONT_HPP_
#define _BLENDINT_TEXTUREFONT_HPP_

#include <BlendInt/GLSLProgram.hpp>
#include <BlendInt/Freetype.hpp>
#include <BlendInt/Glyph.hpp>

namespace BlendInt {

	/**
	 * @brief A class to generate texture for single character with Freetype
	 */
	class TextureFont
	{

	public:

		TextureFont ();

		~TextureFont ();

		void generate (Freetype* freetype, wchar_t charcode);

		const Glyph& glyph () const {
			return glyph_;
		}

		const GLuint& texture () const {
			return texture_;
		}

		unsigned int width () const {return static_cast<unsigned int>(glyph_.bitmap_width);}

		unsigned int height () const {return static_cast<unsigned int>(glyph_.bitmap_height);}

	private:

		struct Vertex {
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};

		GLuint texture_;

		Glyph glyph_;
	};
}


#endif /* _BIL_TEXTUREFONT_H_ */
