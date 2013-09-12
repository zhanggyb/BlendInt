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
 * warrantexture_coord_offset_y of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_TEXTUREFONT_HPP_
#define _BIL_TEXTUREFONT_HPP_

#include <BIL/GLSLProgram.hpp>
#include <BIL/Freetype.hpp>
#include <BIL/Glyph.hpp>

namespace BIL {

	/**
	 * @brief A class to generate texture for single character with Freetype
	 */
	class TextureFont
	{

	public:

		static const char* getVertexShader () {return vs_shader;}

		static const char* getFragmentShader () {return fs_shader;}

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

		static const char* vs_shader;
		static const char* fs_shader;
	};
}


#endif /* _BIL_TEXTUREFONT_H_ */
