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
#include <string>

#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/Gui/Glyph.hpp>

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

		void Generate (const Freetype& freetype, wchar_t start, int size);

		bool Contain (wchar_t charcode);

		const Glyph& glyph (wchar_t charcode) const;

		const GLuint& texture () const {return m_texture;}

		unsigned int width() const {return m_width;}

		unsigned int height() const {return m_height;}

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
		GLuint m_texture;		// texture object

		unsigned int m_width;				// width of texture in pixels
		unsigned int m_height;			// height of texture in pixels

		/**
		 * The character from which to generate the texture atlas
		 */
		wchar_t m_starting_charcode;

		/**
		 * the size of how many sequent characters are in this atlas
		 * from starting_charcode_
		 */
		int m_stride;

		Glyph *m_glyph_array;

		static const char* vs_shader;
		static const char* fs_shader;
	};

}

#endif /* _BIL_TEXTUREATLAS_HPP_ */
