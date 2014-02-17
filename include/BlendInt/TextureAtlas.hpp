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

#ifndef _BLENDINT_TEXTUREATLAS_HPP_
#define _BLENDINT_TEXTUREATLAS_HPP_

#include <vector>
#include <BlendInt/Vector.hpp>
#include <string>

#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/Freetype.hpp>
#include <BlendInt/Glyph.hpp>

namespace BlendInt {

	/**
	 * A texture atlas is basically a big texture which contains many
	 * small images that are packed together.
	 */
	class TextureAtlas
	{
	public:

		TextureAtlas ();

		~TextureAtlas ();

		void initialize ();

		void generate (Freetype* freetype, wchar_t start, int size);

		bool contains (wchar_t charcode);

		void render_text(const wchar_t* text, float x, float y, float sx, float sy);

		const Glyph& glyph (wchar_t charcode) const;

		const GLuint& texture () const {return texture_;}

		unsigned int width() const {return width_;}

		unsigned int height() const {return height_;}

	private:

		struct Vertex {
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};

		/**
		 * Texture ID
		 */
		GLuint texture_;		// texture object

		GLSLProgram* m_program;

		GLint uniform_tex_;

		GLint attribute_coord_;
		GLint uniform_color_;

		GLuint vbo_;

		unsigned int width_;				// width of texture in pixels
		unsigned int height_;			// height of texture in pixels

		/**
		 * The character from which to generate the texture atlas
		 */
		wchar_t starting_charcode_;

		/**
		 * the size of how many sequent characters are in this atlas
		 * from starting_charcode_
		 */
		int stride_;

		Glyph *glyph_array_;

		static const char* vs_shader;
		static const char* fs_shader;
	};

}

#endif /* _BIL_TEXTUREATLAS_HPP_ */
