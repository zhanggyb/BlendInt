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

#include <BIL/FontEngine.hpp>

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
			float advance_x;			// advance.x
			float advance_y;			// advance.y
			
			float bitmap_width;
			float bitmap_height;

			float bitmap_left;
			float bitmap_top;

			float offset_x;		// x offset of glyph in texture coordinates
			float offset_y;		// y offset of glyph in texture coordinates
		};

		TextureAtlas ();

		~TextureAtlas ();

		void load (FT_Face face, int height);

	private:
		
		GLuint texture_;		// texture object

		int width_;				// width of texture in pixels
		int height_;			// height of texture in pixels

		CharacterInfo charcode_[128]; // character information

		GLint texture_uniform_;
	};

}


#endif /* _BIL_TEXTUREATLAS_HPP_ */
