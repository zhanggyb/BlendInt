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

/**
 * @brief Code for character glyph
 */

#ifndef _BIL_GLYPH_H_
#define _BIL_GLYPH_H_

#include <GL/gl.h>
#include <string>
#include <cwchar>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <BIL/Font.hpp>
#include <BIL/Rect.hpp>
#include <BIL/Tuple.hpp>

using namespace std;

namespace BIL {

	class FontEngine;

	/**
	 * @brief Store data, texture, display list for a glyph of an
	 * character
	 *
	 * Though there's no problem to create and destroy destroy each
	 * glyph for text, the fast way is to load glyph from class
	 * FontCache.
	 */
	class Glyph
	{
	public:

		struct Metrics
		{
			/** The glyph's width */
			unsigned int width;

			/** The glyph's height */
			unsigned int height;

			/** Left side bearing for horizontal layout */
			int horiBearingX;

			/** Top side bearing for horizontal layout */
			int horiBearingY;

			/** Advance width for horizontal layout */
			int horiAdvance;

			/** Left side bearing for vertical layout */
			int vertBearingX;

			/** Top side bearing for vertical layout */
			int vertBearingY;

			/** Advance height for vertical layout */
			int vertAdvance;
		};

		/**
		 * @brief Constructor for generate a glyph data with default
		 * font setting
		 */
		Glyph (wchar_t charcode, FontEngine* fontlib = NULL);

		Glyph (wchar_t charcode, const Font& font, unsigned int dpi = 96,
		        FontEngine* fontlib = NULL);

		Glyph (const Glyph& orig);

		Glyph& operator = (const Glyph& orig);

		virtual ~Glyph ();

		void set_charcode (wchar_t charcode);

		void set_font_engine (FontEngine* fontlib);

		const FontEngine* font_engine () const
		{
			return font_engine_;
		}

		unsigned int dpi () const
		{
			return dpi_;
		}

		unsigned int getFontSize () const
		{
			return font_.size;
		}

		unsigned int glyph_index () const
		{
			return glyph_index_;
		}

		const Metrics& metrics () const
		{
			return metrics_;
		}

		const Tuple2i& getBox () const
		{
			return cbox_;
		}

		Rect OutlineBox ();

		void Render ();

	private:
		/* member functions */

		///This function gets the first power of 2 >= the
		///int that we pass it.
		inline int next_p2 (int a)
		{
			int rval = 1;
			while (rval < a)
				rval <<= 1;
			return rval;
		}

		bool MakeDisplayList (void);

		void FillMetrics (const FT_Face& face);

		/**
		 * @brief delete texture and display list
		 */
		void ResetGL (void);

	private:
		/* member variables */

		FontEngine* font_engine_;

		wchar_t charcode_;

		unsigned int glyph_index_;   // 0 means 'undefined character code'

		GLuint texture_;

		GLuint displist_;

		Font font_;

		unsigned int dpi_;

		Metrics metrics_;

		Tuple2i cbox_;
	};

} /* namespace BIL */

#endif  /* _BIL_GLYPH_H_ */
