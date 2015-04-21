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

#ifndef _BLENDINT_FREETYPE_HPP_
#define _BLENDINT_FREETYPE_HPP_

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H
#include FT_STROKER_H

#include <blendint/core/rect.hpp>

using namespace std;

namespace BlendInt {

	class FTLibrary
	{
	public:

		FTLibrary ();

		~FTLibrary ();

		bool Initialize ();

		bool SetLcdFilter (FT_LcdFilter filter);

		bool SetLcdFilterWeights (unsigned char* weights);

		bool GetVersion (FT_Int* major, FT_Int* minor, FT_Int* patch);

		bool Done ();

		const FT_Library& library () const
		{
			return m_library;
		}

	private:

		FT_Library m_library;
	};

	class FTFace
	{
	public:

		FTFace ();

		~FTFace ();

		bool New (const FTLibrary& lib, const char* filepathname, FT_Long face_index = 0);

		bool New (const FTLibrary& lib, const FT_Byte* mem, FT_Long size, FT_Long face_index = 0);

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
			return m_face;
		}

	private:

		FT_Face m_face;
	};

	class FTGlyph
	{
	public:

		FTGlyph ();

		~FTGlyph ();

		FTGlyph& operator = (const FT_Glyph orig);

		bool GetGlyph (const FTFace& face);

		bool Transform (FT_Matrix* matrix, FT_Vector* delta);

		bool ToBitmap (FT_Render_Mode render_mode, FT_Vector* origin = 0, FT_Bool destroy = 1);

		FT_BitmapGlyph GetBitmapGlyph () const
		{
			return (FT_BitmapGlyph)(m_glyph);
		}

		void Done ();

		FT_Glyph& glyph ()
		{
			return m_glyph;
		}

	private:

		FT_Glyph m_glyph;
	};

	class FTStroker
	{
	public:

		FTStroker();

		~FTStroker ();

		bool New (const FTLibrary& lib);

		void Set(FT_Fixed radius, FT_Stroker_LineCap line_cap, FT_Stroker_LineJoin line_join, FT_Fixed miter_limit);

		bool GlyphStroke (FTGlyph& glyph, FT_Bool destroy = 1);

		bool GlyphStrokeBorder (FTGlyph& glyph, FT_Bool inside = 1, FT_Bool destroy = 1);

		bool Done ();

		const FT_Stroker& stroker () const
		{
			return m_stroker;
		}

	private:

		FT_Stroker m_stroker;
	};

	class FTOutline
	{
	public:

		FTOutline ();

		~FTOutline ();

		bool New (const FTLibrary& lib, FT_UInt numPoints, FT_Int numContours);

		bool Done (const FTLibrary& lib);

	private:

		FT_Outline m_outline;
	};

} /* namespace BlendInt */

#endif /* _BIL_FREETYPE_H_ */
