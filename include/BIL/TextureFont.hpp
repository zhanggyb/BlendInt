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

namespace BIL {

	/**
	 * @brief A class to generate texture for single character with Freetype
	 */
	class TextureFont
	{

	public:

		struct GlyphMetrics
		{
			float bitmap_width;
			float bitmap_height;

			float bitmap_left;
			float bitmap_top;

			float advance_x;
			float advance_y;
		};

		static const char* getVertexShader () {return vs_shader;}

		static const char* getFragmentShader () {return fs_shader;}

		TextureFont ();

		~TextureFont ();

		void generate (Freetype* freetype, wchar_t charcode);

		const GlyphMetrics& glyph_metrics () const {
			return glyph_metrics_;
		}

		const GLuint& texture () const {
			return texture_;
		}

	private:

		struct Vertex {
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};

		GLuint texture_;

		GlyphMetrics glyph_metrics_;

		static const char* vs_shader;
		static const char* fs_shader;
	};
}


#endif /* _BIL_TEXTUREFONT_H_ */
