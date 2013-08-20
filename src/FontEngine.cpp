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

#include <iostream>

#include <BIL/FontEngine.hpp>
#include <BIL/FontConfig.hpp>

using namespace std;

namespace BIL {

	FontEngine::FontEngine(const Font& font,
						   unsigned int dpi)
		: _library(NULL), _face(NULL), _stroker(NULL),
		  _valid(false), _unicode(false), _height(0),
		  _ascender(0), _descender(0), _maxAdvance(0),
		  _font(font), _dpi(dpi)
	{
		FT_Error error;

		if(FontConfig::getService() == NULL) {
			if(! FontConfig::instance()) {
				_valid = false;
				return;
			}
		}
		FontConfig* fontconfig = FontConfig::getService();
		if(! fontconfig->isInitialized()) {
			fontconfig->initialize();
		}

		string filename = fontconfig->getFontPath(font);

		error = FT_Init_FreeType(&_library);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			return;
		}

		error = FT_New_Face(_library, filename.c_str(), 0, &_face);
		if (error == FT_Err_Unknown_File_Format) {
			cerr << "Unknown font file format: " << filename << endl;
			return;
		}
		if (error) {
			cerr << "Fail to generate a new Font Face" << endl;
			return;
		}

		error = FT_Stroker_New(_library, &_stroker);

		if (error) {
			cerr << "Fail to load Stroker" << endl;
			return;
		}

		_valid = true;          // now treat it success

		error = FT_Select_Charmap(_face, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map: " << filename
			        << endl;
		} else {
			_unicode = true;
		}

		setCharSize(font.size, dpi);
	}

	FontEngine::FontEngine (const string& filename,
							unsigned int size,
							unsigned int dpi)
		: _library(NULL), _face(NULL), _stroker(NULL),
		  _valid(false), _unicode(false), _height(0),
		  _ascender(0), _descender(0), _maxAdvance(0),
		  _dpi(dpi)
	{
		FT_Error error;

		error = FT_Init_FreeType(&_library);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			return;
		}

		error = FT_New_Face(_library, filename.c_str(), 0, &_face);
		if (error == FT_Err_Unknown_File_Format) {
			cerr << "Unknown font file format: " << filename << endl;
			return;
		}
		if (error) {
			cerr << "Fail to generate a new Font Face" << endl;
			return;
		}

		_font.family = _face->family_name;
		_font.size = size;
		_font.italic = (_face->style_flags & FT_STYLE_FLAG_ITALIC) ? true : false;
		_font.bold = (_face->style_flags & FT_STYLE_FLAG_BOLD) ? true : false;

		error = FT_Stroker_New(_library, &_stroker);

		if (error) {
			cerr << "Fail to load Stroker" << endl;
			return;
		}

		_valid = true;          // now treat it success

		error = FT_Select_Charmap(_face, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map: " << filename
			        << endl;
		} else {
			_unicode = true;
		}

		setCharSize(size, dpi);
	}

	FontEngine::FontEngine (const FT_Byte* buffer,
							FT_Long bufsize,
							FT_Long index,
							unsigned int size,
							unsigned int dpi)
		: _library(NULL), _face(NULL), _stroker(NULL),
		  _valid(false), _unicode(false), _height(0),
		  _ascender(0), _descender(0), _maxAdvance(0),
		  _dpi(dpi)
	{
		FT_Error error;

		error = FT_Init_FreeType(&_library);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			return;
		}

		error = FT_New_Memory_Face(_library, buffer, bufsize, index, &_face);
		if (error) {
			cerr << "Fail to generate a new Font Face from memory" << endl;
			return;
		}

		_font.family = _face->family_name;
		_font.size = size;
		_font.italic = (_face->style_flags & FT_STYLE_FLAG_ITALIC) ? true : false;
		_font.bold = (_face->style_flags & FT_STYLE_FLAG_BOLD) ? true : false;

		error = FT_Stroker_New(_library, &_stroker);

		if (error) {
			cerr << "Fail to load Stroker" << endl;
			return;
		}

		_valid = true;          // now treat it success

		error = FT_Select_Charmap(_face, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map" << endl;
		} else {
			_unicode = true;
		}
		
		setCharSize(size, dpi);
	}

	FontEngine::~FontEngine ()
	{
		if (_stroker != NULL) {
			FT_Stroker_Done(_stroker);
			_stroker = NULL;
		}

		if (_face != NULL) {
			FT_Done_Face(_face);
			_face = NULL;
		}

		if (_library != NULL) {
			FT_Done_FreeType(_library);
			_library = NULL;
		}
	}


	bool FontEngine::isUseKerning (void)
	{
		if(!_valid)
			return false;

		return FT_HAS_KERNING(_face);
	}

	bool FontEngine::loadGlyph (FT_UInt glyph_index, FT_Int32 load_flags)
	{
		if (!_valid)
			return false;

		FT_Error error;

		error = FT_Load_Glyph(_face, glyph_index, load_flags);

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

	bool FontEngine::setCharSize (unsigned int size, unsigned int dpi)
	{
		FT_Error error;

		if (!_valid)
			return false;

		_font.size = size; _dpi = dpi;

		// size_t hres = 64;

		/* Set char size */
		// error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);
		//For some twisted reason, Freetype measures font size
		//in terms of 1/64ths of pixels.  Thus, to make a font
		//h pixels high, we need to request a size of h*64.
		//(h << 6 is just a prettier way of writting h*64)
		error = FT_Set_Char_Size(_face, (long) (_font.size << 6), 0, _dpi, 0);
		if (error) {
			cerr << "The current font don't support the size, " << size
			        << " and dpi " << _dpi << endl;
			return false;
		}

		_height = _face->size->metrics.height >> 6;
		_ascender = _face->size->metrics.ascender >> 6;
		_descender = _face->size->metrics.descender >> 6;
		_maxAdvance = _face->size->metrics.max_advance >> 6;

		return true;
	}

	FT_UInt FontEngine::getCharIndex (const FT_ULong charcode)
	{
		if (!_valid)
			return 0;

		return (FT_Get_Char_Index(_face, charcode));
	}

	bool FontEngine::loadCharacter (FT_ULong charcode, FT_Int32 load_flags)
	{
		if (!_valid)
			return false;

		FT_Error error;

		error = FT_Load_Char(_face, charcode, load_flags);
		if (error) {
			cerr << "Fail to load Character" << endl;
			return false;
		}

		return true;
	}

	bool FontEngine::renderGlyph (FT_Render_Mode render_mode)
	{
		if (!_valid)
			return false;
		FT_Error error;

		error = FT_Render_Glyph(_face->glyph, // glyph slot
		        render_mode);
		if (error) {
			cerr << "Fail to render glyph" << endl;
			return false;
		}

		return true;
	}

	bool FontEngine::getKerning (FT_UInt left_glyph, FT_UInt right_glyph,
	        FT_UInt kern_mode, FT_Vector* akerning)
	{
		if ((!_valid) || (_face == NULL))
			return false;

		FT_Error error;
		error = FT_Get_Kerning(_face, left_glyph, right_glyph, kern_mode,
		        akerning);
		if (error) {
			cerr << "Fail to get kerning" << endl;
			return false;
		}

		return true;
	}

	Tuple2l FontEngine::getKerning (const Glyph& left, const Glyph& right,
	        FT_UInt kerning_mode)
	{
		Tuple2l ret;	// {0, 0}

		if ((!_valid) || (_face == NULL))
			return ret;

		if (! FT_HAS_KERNING(_face))  return ret;

		FT_Vector kerning;

		bool result = getKerning(left.getGlyphIndex(),
		        right.getGlyphIndex(), kerning_mode, &kerning);

		if (!result)
			return ret;

		ret.vec.x = kerning.x;
		ret.vec.y = kerning.y;

		return ret;
	}

	bool FontEngine::setLcdFilter (FT_LcdFilter filter)
	{
		if (!_valid)
			return false;

		FT_Error error;

		error = FT_Library_SetLcdFilter(_library, filter);

		if (error) {
			cerr << "Fail to set lcd filter" << endl;
			return false;
		}

		return true;
	}

	bool FontEngine::setLcdFilterWeights (unsigned char* weights)
	{
		if (!_valid)
			return false;

		FT_Error error;

		error = FT_Library_SetLcdFilterWeights(_library, weights);
		if (error) {
			cerr << "Fail to set lcd filter weights" << endl;
			return false;
		}

		return true;
	}

} /* namespace BIL */

