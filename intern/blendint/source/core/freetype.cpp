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
#include <core/freetype.hpp>

namespace BlendInt {

	FTGlyph::FTGlyph ()
	: m_glyph(0)
	{

	}

	FTGlyph::~FTGlyph ()
	{
		Done();
	}

	FTGlyph& FTGlyph::operator =(const FT_Glyph orig)
	{
		if(m_glyph) {
			FT_Done_Glyph(m_glyph);
			m_glyph = 0;
		}
		m_glyph = orig;

		return *this;
	}

	bool FTGlyph::GetGlyph(const FTFace& face)
	{
		if(m_glyph) {
			FT_Done_Glyph(m_glyph);
			m_glyph = 0;
		}

		FT_Error err = FT_Get_Glyph(face.face()->glyph, &m_glyph);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to get glyph");
		}

		return (err == 0);
	}

	bool FTGlyph::Transform(FT_Matrix * matrix, FT_Vector* delta)
	{
		FT_Error err = FT_Glyph_Transform(m_glyph, matrix, delta);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to transform glyph");
		}

		return (err == 0);
	}

	bool FTGlyph::ToBitmap(FT_Render_Mode render_mode, FT_Vector* origin, FT_Bool destroy)
	{
		if(m_glyph) {

			FT_Error err = FT_Glyph_To_Bitmap(&m_glyph, render_mode, origin, destroy);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to convert glyph object to a bitmap");
			}

			return (err == 0);
		} else {
			return false;
		}
	}

	void FTGlyph::Done ()
	{
		if(m_glyph) {
			FT_Done_Glyph(m_glyph);
			m_glyph = 0;
		}
	}

	FTLibrary::FTLibrary()
	: m_library(0)
	{

	}

	FTLibrary::~FTLibrary()
	{
		if(m_library) {
			FT_Error err = FT_Done_FreeType(m_library);
			if(err) {
				// TODO: print error message
			}
		}
	}

	bool FTLibrary::Initialize()
	{
		FT_Error err = 0;

		if(m_library) {
			err = FT_Done_FreeType(m_library);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to destroy FreeType library");
			}
		}

		err = FT_Init_FreeType(&m_library);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to initialize FreeType library");
		}

		return (err == 0);
	}
	
	bool FTLibrary::SetLcdFilter (FT_LcdFilter filter)
	{
		FT_Error err = FT_Library_SetLcdFilter(m_library, filter);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to set FreeType Lcd Filter");
		}

		return (err == 0);
	}
	
	bool FTLibrary::SetLcdFilterWeights (unsigned char* weights)
	{
		FT_Error err = FT_Library_SetLcdFilterWeights(m_library, weights);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to set FreeType Lcd Filter Weights");
		}

		return (err == 0);
	}
	
	bool FTLibrary::GetVersion (FT_Int* major, FT_Int* minor, FT_Int* patch)
	{
		if(m_library) {
			FT_Library_Version(m_library, major, minor, patch);
			return true;
		}

		return false;
	}

	bool FTLibrary::Done()
	{
		FT_Error err = FT_Done_FreeType(m_library);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to destroy FreeType library");
		}
		m_library = 0;

		return (err == 0);
	}

	FTFace::FTFace ()
	: m_face(0)
	{

	}

	FTFace::~FTFace()
	{
		if(m_face) {
			FT_Error err = FT_Done_Face(m_face);
			if(err) {
				// TODO: print error message
			}

		}
	}

	bool FTFace::New(const FTLibrary& lib, const char* filepathname, FT_Long face_index)
	{
		FT_Error err = 0;

		if(m_face) {
			err = FT_Done_Face(m_face);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to destroy FreeType face");
			}
		}

		err = FT_New_Face(lib.library(), filepathname, face_index, &m_face);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to create FreeType face");
		}

		return (err == 0);
	}

	bool FTFace::New (const FTLibrary& lib, const FT_Byte* mem, FT_Long size, FT_Long face_index)
	{
		FT_Error err = 0;

		if(m_face) {
			err = FT_Done_Face(m_face);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to destroy FreeType face");
			}
		}

		err = FT_New_Memory_Face(lib.library(), mem, size, face_index, &m_face);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to create FreeType face");
		}

		return (err == 0);
	}
	
	bool FTFace::HasKerning()
	{
		if(m_face) {
			return m_face->face_flags & FT_FACE_FLAG_KERNING;
		} else {
			return false;
		}
	}

	bool FTFace::CheckTrueTypePatents ()
	{
		FT_Bool ret = FT_Face_CheckTrueTypePatents(m_face);

		return ret;
	}
	
	bool FTFace::SetCharSize (FT_F26Dot6 width, FT_F26Dot6 height,
					FT_UInt h_res, FT_UInt v_res)
	{
		FT_Error err = 0;
		err = FT_Set_Char_Size(m_face, width, height, h_res, v_res);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to set character size");
		}

		return (err == 0);
	}
	
	bool FTFace::LoadChar (FT_ULong char_code, FT_Int32 load_flags) const
	{
		FT_Error err = 0;
		err = FT_Load_Char(m_face, char_code, load_flags);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to load character");
		}

		return (err == 0);
	}
	
	FT_UInt FTFace::GetCharIndex (FT_ULong charcode) const
	{
		FT_UInt index = FT_Get_Char_Index(m_face, charcode);

		return index;
	}
	
	bool FTFace::LoadGlyph (FT_UInt glyph_index, FT_Int32 load_flags) const
	{
		FT_Error err = 0;

		err = FT_Load_Glyph(m_face, glyph_index, load_flags);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to load glyph");
		}

		return (err == 0);
	}
	
	bool FTFace::RenderGlyph (FT_GlyphSlot slot, FT_Render_Mode render_mode) const
	{
		FT_Error err = 0;

		err = FT_Render_Glyph(slot, render_mode);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to render glyph");
		}

		return (err == 0);
	}
	
	bool FTFace::GetKerning (FT_UInt left_glyph, FT_UInt right_glyph,
					FT_UInt kern_mode, FT_Vector* akerning)
	{
		FT_Error err = 0;

		err = FT_Get_Kerning(m_face, left_glyph, right_glyph, kern_mode, akerning);
		if(err) {
			DBG_PRINT_MSG("Fail to get kerning between: %d, %d", left_glyph, right_glyph);
		}

		return (err == 0);
	}

	bool FTFace::Done ()
	{
		FT_Error err = 0;

		err = FT_Done_Face(m_face);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to destroy FreeType face");
		}
		m_face = 0;

		return (err == 0);
	}

	FTStroker::FTStroker()
	: m_stroker(0)
	{

	}

	FTStroker::~FTStroker ()
	{
		if(m_stroker) {
			FT_Stroker_Done(m_stroker);
		}
	}

	bool FTStroker::New (const FTLibrary& lib)
	{
		FT_Error err = 0;

		if(m_stroker) {
			FT_Stroker_Done(m_stroker);
			m_stroker = 0;
		}

		err = FT_Stroker_New(lib.library(), &m_stroker);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to create FreeType stroker");
		}

		return (err == 0);
	}
	
	void FTStroker::Set (FT_Fixed radius, FT_Stroker_LineCap line_cap,
					FT_Stroker_LineJoin line_join, FT_Fixed miter_limit)
	{
		FT_Stroker_Set(m_stroker, radius, line_cap, line_join, miter_limit);
	}
	
	bool FTStroker::GlyphStroke (FTGlyph& glyph, FT_Bool destroy)
	{
		FT_Error err = 0;

		err = FT_Glyph_Stroke(&(glyph.glyph()), m_stroker, destroy);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to stroke the glyph");
		}

		return (err == 0);
	}

	bool FTStroker::GlyphStrokeBorder(FTGlyph& glyph, FT_Bool inside, FT_Bool destroy)
	{
		FT_Error err = 0;

		err = FT_Glyph_StrokeBorder(&(glyph.glyph()), m_stroker, inside, destroy);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to stroke the border of a glyph");
		}

		return (err == 0);
	}

	bool FTStroker::Done()
	{
		FT_Error err = 0;

		if(m_stroker) {
			FT_Stroker_Done(m_stroker);
			m_stroker = 0;
		}

		return (err == 0);
	}

	FTOutline::FTOutline()
	{

	}

	FTOutline::~FTOutline()
	{

	}

	bool FTOutline::New(const FTLibrary& lib, FT_UInt numPoints, FT_Int numContours)
	{
		FT_Error err = 0;

		err = FT_Outline_New(lib.library(), numPoints, numContours, &m_outline);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to create a new FreeType outline");
		}
		return (err == 0);
	}

	bool FTOutline::Done (const FTLibrary& lib)
	{
		FT_Error err = 0;

		err = FT_Outline_Done(lib.library(), &m_outline);
		if(err) {
			DBG_PRINT_MSG("%s", "Fail to destroy FreeType outline");
		}

		return (err == 0);
	}

} /* namespace BlendInt */
