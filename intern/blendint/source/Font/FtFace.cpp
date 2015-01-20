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

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Font/FtFace.hpp>

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

		bool Face::HasKerning ()
		{
			if(face_) {
				return face_->face_flags & FT_FACE_FLAG_KERNING;
			} else {
				return false;
			}
		}

		bool Face::CheckTrueTypePatents ()
		{
			FT_Bool ret = FT_Face_CheckTrueTypePatents(face_);
			return ret;
		}

		bool Face::SetCharSize (FT_F26Dot6 width, FT_F26Dot6 height,
		        FT_UInt h_res, FT_UInt v_res)
		{
			FT_Error err = 0;
			err = FT_Set_Char_Size(face_, width, height, h_res, v_res);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to set character size");
			}

			return (err == 0);
		}

		FT_UInt Face::GetCharIndex (FT_ULong charcode) const
		{
			FT_UInt index = FT_Get_Char_Index(face_, charcode);

			return index;
		}

		bool Face::LoadGlyph (FT_UInt glyph_index, FT_Int32 load_flags) const
		{
			FT_Error err = 0;

			err = FT_Load_Glyph(face_, glyph_index, load_flags);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to load glyph");
			}

			return (err == 0);
		}

		bool Face::RenderGlyph (FT_GlyphSlot slot,
		        FT_Render_Mode render_mode) const
		{
			FT_Error err = 0;

			err = FT_Render_Glyph(slot, render_mode);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to render glyph");
			}

			return (err == 0);
		}

		bool Face::LoadChar (FT_ULong char_code, FT_Int32 load_flags) const
		{
			FT_Error err = 0;
			err = FT_Load_Char(face_, char_code, load_flags);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to load character");
			}

			return (err == 0);
		}

		bool Face::GetKerning (FT_UInt left_glyph, FT_UInt right_glyph,
		        FT_UInt kern_mode, FT_Vector* akerning)
		{
			FT_Error err = 0;

			err = FT_Get_Kerning(face_, left_glyph, right_glyph, kern_mode, akerning);
			if(err) {
				DBG_PRINT_MSG("Fail to get kerning between: %d, %d", left_glyph, right_glyph);
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
