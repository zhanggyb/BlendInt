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

#include <core/types.hpp>
#include <font/ft-face.hpp>

namespace BlendInt {

	namespace Ft {

		Face::Face()
		: face_(NULL)
		{

		}

		Face::~Face ()
		{
			if(face_) {
				FT_Error err = FT_Done_Face(face_);
				if(err) {
					DBG_PRINT_MSG("Warning: %s", "Fail to destroy Freetype Face");
				}
			}
		}

		bool Face::New (const Library& lib, const char* filepathname,
		        FT_Long face_index)
		{
			FT_Error err = 0;

			if(face_) {
				err = FT_Done_Face(face_);
				if(err) {
					DBG_PRINT_MSG("%s", "Fail to destroy FreeType face");
				}
			}

			err = FT_New_Face(lib.library(), filepathname, face_index, &face_);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to create FreeType face");
			}

			return (err == 0);
		}

		bool Face::New (const Library& lib, const FT_Byte* mem, FT_Long size,
		        FT_Long face_index)
		{
			FT_Error err = 0;

			if(face_) {
				err = FT_Done_Face(face_);
				if(err) {
					DBG_PRINT_MSG("%s", "Fail to destroy FreeType face");
				}
			}

			err = FT_New_Memory_Face(lib.library(), mem, size, face_index, &face_);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to create FreeType face");
			}

			return (err == 0);
		}

		bool Face::Done ()
		{
			FT_Error err = 0;

			err = FT_Done_Face(face_);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to destroy FreeType face");
			}
			face_ = 0;

			return (err == 0);
		}

	}

}
