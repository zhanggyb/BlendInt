/**
 * @brief Code for character glyph
 */

#ifndef _BIL_GLYPH_H_
#define _BIL_GLYPH_H_

#include <GL/glew.h>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <wchar.h>

using namespace std;

namespace BIL {

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
		Glyph (wchar_t charcode);

		Glyph (wchar_t charcode, const string& fontname, unsigned int fontsize =
		        12, unsigned int dpi = 96);

		Glyph (const Glyph& orig);

		Glyph& operator = (const Glyph& orig);

		virtual ~Glyph ();

		void setCharacter (wchar_t charcode);

		const Metrics& getMetrics (void) const
		{
			return _metrics;
		}

		void render (void);

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

		bool makeDisplayList (void);

	private:
		/* member variables */

		wchar_t _charcode;

		GLuint _texture;

		GLuint _displist;

		unsigned int _fontsize;

		unsigned int _dpi;

		string _filename; /* font file name */

		Metrics _metrics;

	};

} /* namespace BIL */

#endif  /* _BIL_GLYPH_H_ */
