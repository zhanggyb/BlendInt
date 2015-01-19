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

		class Face: public Object
		{
		public:

			Face ();

			virtual ~Face ();

			bool New (const Library& lib, const char* filepathname, FT_Long face_index = 0);

			bool New (const Library& lib, const FT_Byte* mem, FT_Long size, FT_Long face_index = 0);

			bool HasKerning ();

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
			bool CheckTrueTypePatents ();

			bool SetCharSize (FT_F26Dot6 width, FT_F26Dot6 height, FT_UInt h_res, FT_UInt v_res);

			FT_UInt GetCharIndex (FT_ULong charcode) const;

			bool LoadGlyph (FT_UInt glyph_index, FT_Int32 load_flags) const;

			bool RenderGlyph (FT_GlyphSlot slot, FT_Render_Mode render_mode) const;

			bool LoadChar (FT_ULong char_code, FT_Int32 load_flags) const;

			bool GetKerning (FT_UInt left_glyph, FT_UInt right_glyph, FT_UInt kern_mode, FT_Vector* akerning);

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
