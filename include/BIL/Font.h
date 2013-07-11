/*
 * Font.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <string>

#include <BIL/Color.h>

using namespace std;

namespace BIL {

	class Font
	{
	public:

		Font (const string& family = string("Sans"), float size = 10,
		        bool bold = false, bool italic = false);

		Font (const Font& orig);

		Font& operator = (const Font& orig);

		bool bold (void) const {return _bold;}

		void setBold (bool bold) {_bold = bold;}

		bool italic (void) const {return _italic;}

		void setItalic (bool italic) {_italic = italic;}

		float size (void) const {return _size;}

		void setSize (float size) {_size = size;}

		const string& family (void) const {return _family;}

		void setFamily (const string& family) {_family = family;}

		virtual ~Font ();

	private:

		/** the font family, e.g. "Droid Sans" */
		string _family;

		/** font size */
		float _size;

		/** whether text is bold */
		bool _bold;

		/** whether text is italic */
		bool _italic;

		/** vertical displacemen from the baseline */
		float _rise;

		/** spacing between 2 letters */
		float _spacing;

		/** Gamma correction */
		float _gamma;

		/** Text color */
		Color _color;

		/** background color */
		Color _background;

		/** whether outline is active */
		bool _outline;

		/** Outline color */
		Color _outlinecolor;

		/** whether underline is active */
		bool _underline;

		/** Underline color */
		Color _underlinecolor;

		/** Whether overline is active */
		bool _overline;

		/** Overline color */
		Color _overlinecolor;

		/** Whether strikethrough is active */
		bool _strikethrough;

		/** Strikethrough color */
		Color _strikethroughcolor;


	};

} /* namespace BIL */
#endif /* FONT_H_ */
