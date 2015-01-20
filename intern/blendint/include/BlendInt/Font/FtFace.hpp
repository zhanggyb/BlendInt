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

#pragma once

#include <BlendInt/Font/FtLibrary.hpp>

namespace BlendInt {

	namespace Ft {

		class Face
		{
		public:

			Face ();

			~Face ();

			bool New (const Library& lib, const char* filepathname, FT_Long face_index = 0);

			bool New (const Library& lib, const FT_Byte* mem, FT_Long size, FT_Long face_index = 0);

			inline bool has_kerning ()
			{
				return face_->face_flags & FT_FACE_FLAG_KERNING;
			}

			/**
			 * @brief Check whether any of the patented opcodes are used.
			 * @return
			 * 	- true This is a TrueType font that uses one of the patented opcodes
			 * 	- false otherwise
			 *
			 * Parse all bytecode instructions of a TrueType font file to
			 * check whether any of the patented opcodes are used. This is
			 * only useful if you want to be able to use the unpatented
			 * hinter with fonts that do not use these opcodes.
			 *
			 * Note that this function parses all glyph instructions in
			 * the font file, which may be slow.
			 */
			inline bool check_truetype_patents () const
			{
				return FT_Face_CheckTrueTypePatents(face_);
			}

			inline FT_Error set_char_size (FT_F26Dot6 width, FT_F26Dot6 height, FT_UInt h_res, FT_UInt v_res) const
			{
				return FT_Set_Char_Size(face_, width, height, h_res, v_res);
			}

			inline FT_UInt get_char_index (FT_ULong charcode) const
			{
				return FT_Get_Char_Index(face_, charcode);
			}

			inline FT_Error load_glyph (FT_UInt glyph_index, FT_Int32 load_flags) const
			{
				return FT_Load_Glyph(face_, glyph_index, load_flags);
			}

			inline FT_Error render_glyph (FT_GlyphSlot slot, FT_Render_Mode render_mode) const
			{
				return FT_Render_Glyph(slot, render_mode);
			}

			inline FT_Error load_char (FT_ULong char_code, FT_Int32 load_flags) const
			{
				return FT_Load_Char(face_, char_code, load_flags);
			}

			inline FT_Error get_kerning (FT_UInt left_glyph, FT_UInt right_glyph, FT_UInt kern_mode, FT_Vector* akerning)
			{
				return FT_Get_Kerning(face_, left_glyph, right_glyph, kern_mode, akerning);
			}

			bool Done ();

			const FT_Face& face () const
			{
				return face_;
			}

		private:

			friend class Library;

			FT_Face face_;
		};

	}

}
