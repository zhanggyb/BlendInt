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
#include <font/ft-library.hpp>

namespace BlendInt {

	namespace Ft {

		Library::Library()
		: library_(NULL)
		{

		}

		Library::~Library ()
		{
			if(library_) {
				FT_Error err = FT_Done_FreeType(library_);
				if(err) {
					DBG_PRINT_MSG("%s", "Fail to destroy FreeType library");
				}
			}
		}

		bool Library::Init ()
		{
			FT_Error err = 0;

			if(library_) {
				err = FT_Done_FreeType(library_);
				if(err) {
					DBG_PRINT_MSG("%s", "Fail to destroy FreeType library");
				}
			}

			err = FT_Init_FreeType(&library_);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to initialize FreeType library");
			}

			return (err == 0);
		}

		bool Library::SetLcdFilter (FT_LcdFilter filter)
		{
			FT_Error err = FT_Library_SetLcdFilter(library_, filter);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to set FreeType Lcd Filter");
			}

			return (err == 0);
		}

		bool Library::SetLcdFilterWeights (unsigned char* weights)
		{
			FT_Error err = FT_Library_SetLcdFilterWeights(library_, weights);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to set FreeType Lcd Filter Weights");
			}

			return (err == 0);
		}

		bool Library::GetVersion (FT_Int* major, FT_Int* minor, FT_Int* patch)
		{
			if(library_) {
				FT_Library_Version(library_, major, minor, patch);
				return true;
			}

			return false;
		}

		bool Library::Done ()
		{
			FT_Error err = FT_Done_FreeType(library_);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to destroy FreeType library");
			}
			library_ = NULL;

			return (err == 0);
		}

	}

}
