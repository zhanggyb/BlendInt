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

#include <iostream>

#include <BlendInt/Core/Freetype.hpp>

#ifdef USE_FONTCONFIG
#include <BlendInt/Core/FontConfig.hpp>
#endif

using namespace std;

namespace BlendInt {

	Freetype::Freetype()
	: library_(0), face_(0), stroker_(NULL),
	  valid_(false), unicode_(false), height_(0),
	  ascender_(0), descender_(0), max_advance_(0),
	  dpi_(96)
	{

	}

	bool Freetype::Open (const Font& font,
						   unsigned int dpi)
	{
		// make sure close opened resources
		Close();

		FT_Error error;

		string filepath;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		filepath = fontconfig->getFontPath(font);
#else

#ifdef __APPLE__
		filepath = "/System/Library/Fonts/LucidaGrande.ttc";
#endif

#endif

		error = FT_Init_FreeType(&library_);
		if (error) {
			cerr << "Could not initialize FreeType library" << endl;
			return false;
		}

		error = FT_New_Face(library_, filepath.c_str(), 0, &face_);
		if (error == FT_Err_Unknown_File_Format) {
			cerr << "Unknown font file format: " << filepath << endl;
			Close();
			return false;
		}
		if (error) {
			cerr << "Fail to generate a new Font Face from: " << filepath << endl;
			Close ();
			return false;
		}

		valid_ = true;          // now treat it success

		error = FT_Select_Charmap(face_, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map: " << filepath
			        << endl;
		} else {
			unicode_ = true;
		}

		setCharSize(font.size, dpi);

		return true;
	}

	bool Freetype::Open (const std::string& filename,
							unsigned int size,
							unsigned int dpi)
	{
		// make sure close opened resources
		Close();

		FT_Error error;

		error = FT_Init_FreeType(&library_);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			Close ();
			return false;
		}

		error = FT_New_Face(library_, filename.c_str(), 0, &face_);
		if (error == FT_Err_Unknown_File_Format) {
			cerr << "Unknown font file format: " << " " << filename << endl;
			Close ();
			return false;
		}
		if (error) {
			cerr << "Fail to generate a new Font Face" << endl;
			Close ();
			return false;
		}

		valid_ = true;          // now treat it success

		error = FT_Select_Charmap(face_, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map: " << filename
			        << endl;
		} else {
			unicode_ = true;
		}

		setCharSize(size, dpi);
		return true;
	}

	bool Freetype::Open (const FT_Byte* buffer,
							FT_Long bufsize,
							FT_Long index,
							unsigned int size,
							unsigned int dpi)
	{
		// make sure close opened resources
		Close();

		FT_Error error;

		error = FT_Init_FreeType(&library_);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			Close ();
			return false;
		}

		error = FT_New_Memory_Face(library_, buffer, bufsize, index, &face_);
		if (error) {
			cerr << "Fail to generate a new Font Face from memory" << endl;
			Close ();
			return false;
		}

		valid_ = true;          // now treat it success

		error = FT_Select_Charmap(face_, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map" << endl;
		} else {
			unicode_ = true;
		}
		
		setCharSize(size, dpi);
		return true;
	}

	void Freetype::Close ()
	{
		if (stroker_) {
			FT_Stroker_Done(stroker_);
			stroker_ = 0;
		}

		if (face_) {
			FT_Done_Face(face_);
			face_ = 0;
		}

		if (library_) {
			FT_Done_FreeType(library_);
			library_ = 0;
		}

		valid_ = false;
		unicode_ = false;
	}

	Freetype::~Freetype ()
	{
		Close();
	}


	bool Freetype::isUseKerning (void)
	{
		if(!valid_)
			return false;

		return FT_HAS_KERNING(face_);
	}

	bool Freetype::loadGlyph (FT_UInt glyph_index, FT_Int32 load_flags)
	{
		if (!valid_)
			return false;

		FT_Error error;

		error = FT_Load_Glyph(face_, glyph_index, load_flags);

		if (error) {
			cerr << "Fail to load glyph" << endl;
			return false;
		}

		return true;
	}

	/*
	bool FontEngine::setFontSize (unsigned int size, unsigned int dpi)
	{
		if (!_valid)
			return false;

		FT_Error error;

		_font.size = size; _dpi = dpi;

		error = FT_Set_Char_Size(_face, 0, (FT_F26Dot6) (_font.size * 64), _dpi, _dpi);
		if (error) {
			cerr << "The current font don't support the size, " << size
			        << " and dpi " << _dpi << endl;
			return false;
		}

		return true;
	}
	*/

	bool Freetype::setCharSize (unsigned int size, unsigned int dpi)
	{
		if (!valid_)
			return false;

		FT_Error error;
		dpi_ = dpi;

		// size_t hres = 64;

		/* Set char size */
		// error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);
		//For some twisted reason, Freetype measures font size
		//in terms of 1/64ths of pixels.  Thus, to make a font
		//h pixels high, we need to request a size of h*64.
		//(h << 6 is just a prettier way of writting h*64)
		error = FT_Set_Char_Size(face_, (long) (size << 6), 0, dpi_, 0);
		if (error) {
			cerr << "The current font don't support the size, " << size
			        << " and dpi " << dpi_ << endl;
			return false;
		}

		height_ = face_->size->metrics.height >> 6;
		ascender_ = face_->size->metrics.ascender >> 6;
		descender_ = face_->size->metrics.descender >> 6;
		max_advance_ = face_->size->metrics.max_advance >> 6;

		return true;
	}

	bool Freetype::setPixelSize(unsigned int width, unsigned int height)
	{
		FT_Error error;
		if(!valid_) return false;

		error = FT_Set_Pixel_Sizes (face_, width, height);
		if (error) {
			cerr << "Fail to set pixel sizes" << endl;
			return false;
		}

		return true;
	}

	FT_UInt Freetype::getCharIndex (const FT_ULong charcode)
	{
		if (!valid_)
			return 0;

		return (FT_Get_Char_Index(face_, charcode));
	}

	bool Freetype::LoadCharacter (FT_ULong charcode, FT_Int32 load_flags)
	{
		if (!valid_)
			return false;

		FT_Error error;

		error = FT_Load_Char(face_, charcode, load_flags);
		if (error) {
			cerr << "Fail to load Character" << endl;
			return false;
		}

		return true;
	}

	bool Freetype::renderGlyph (FT_Render_Mode render_mode)
	{
		if (!valid_)
			return false;
		FT_Error error;

		error = FT_Render_Glyph(face_->glyph, // glyph slot
		        render_mode);
		if (error) {
			cerr << "Fail to Render glyph" << endl;
			return false;
		}

		return true;
	}

	FT_GlyphSlot Freetype::GetGlyphSlot () const
	{
		if(!valid_)
			return 0;

		return face_->glyph;
	}

	bool Freetype::getKerning (FT_UInt left_glyph, FT_UInt right_glyph,
	        FT_UInt kern_mode, FT_Vector* akerning)
	{
		if ((!valid_) || (face_ == NULL))
			return false;

		FT_Error error;
		error = FT_Get_Kerning(face_, left_glyph, right_glyph, kern_mode,
		        akerning);
		if (error) {
			cerr << "Fail to get kerning" << endl;
			return false;
		}

		return true;
	}

//	Tuple2l Freetype::getKerning (const wchar_t& left, const wchar_t& right,
//	        FT_UInt kerning_mode)
//	{
//		Tuple2l ret;	// {0, 0}
//
//		if ((!valid_) || (!face_))
//			return ret;
//
//		if (! FT_HAS_KERNING(face_))  return ret;
//
//		FT_Vector kerning;
//
//		bool result = getKerning(getCharIndex(left),
//		        getCharIndex(right), kerning_mode, &kerning);
//
//		if (!result)
//			return ret;
//
//		ret.vec.x = kerning.x;
//		ret.vec.y = kerning.y;
//
//		return ret;
//	}

	bool Freetype::setLcdFilter (FT_LcdFilter filter)
	{
		if (!valid_)
			return false;

		FT_Error error;

		error = FT_Library_SetLcdFilter(library_, filter);

		if (error) {
			cerr << "Fail to set lcd filter" << endl;
			return false;
		}

		return true;
	}

	bool Freetype::setLcdFilterWeights (unsigned char* weights)
	{
		if (!valid_)
			return false;

		FT_Error error;

		error = FT_Library_SetLcdFilterWeights(library_, weights);
		if (error) {
			cerr << "Fail to set lcd filter weights" << endl;
			return false;
		}

		return true;
	}

} /* namespace BlendInt */
