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
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_TEXTUREATLAS_HPP_
#define _BIL_TEXTUREATLAS_HPP_

#include <vector>
#include <BIL/Vector.hpp>
#include <string>

#include <BIL/GLSLProgram.hpp>

namespace BIL {

	/**
	 * A texture atlas is basically a big texture which contains many
	 * small images that are packed together.
	 */
	class TextureAtlas
	{
	public:

		struct CharacterInfo
		{
			float advance_x;
			float advance_y;

			float bitmap_width;
			float bitmap_height;

			float bitmap_left;
			float bitmap_top;

			float texture_coord_offset_x;
			float texture_coord_offset_y;
		};

		TextureAtlas (const std::string& filename);

		~TextureAtlas ();

		void initialize ();

		void generate ();

		void render_text(const char *text, float x, float y, float sx, float sy);

	private:

		struct point {
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};
		/**
		 * Texture ID
		 */
		GLuint texture_;		// texture object

		GLSLProgram program_;

		GLint uniform_tex_;

		GLint attribute_coord_;
		GLint uniform_color_;

		GLuint vbo_;

		unsigned int width_;				// width of texture in pixels
		unsigned int height_;			// height of texture in pixels

		CharacterInfo c_[128];

		std::string filename_;

		unsigned int font_size_;

		static const char* vs_shader;
		static const char* fs_shader;
	};

}

#endif /* _BIL_TEXTUREATLAS_HPP_ */
