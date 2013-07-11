/*
 * Font.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <BIL/Font.h>

namespace BIL {

	Font::Font (const string& family, float size, bool bold, bool italic)
			: _family(family), _size(size), _bold(bold), _italic(italic), _rise(
			        0.0), _spacing(0.0), _gamma(2.0), _color(
			        Color(Color::White)), _background(Color(Color::None)), _outline(
			        false), _outlinecolor(Color(Color::White)), _underline(
			        false), _underlinecolor(Color(Color::White)), _overline(
			        false), _overlinecolor(Color(Color::White)), _strikethrough(
			        false), _strikethroughcolor(Color(Color::White))
	{
	}

	Font::~Font (void)
	{

	}

	Font::Font (const Font& orig)
	{
		_family = orig._family;
		_size = orig._size;
		_bold = orig._bold;
		_italic = orig._italic;
		_rise = orig._rise;
		_spacing = orig._spacing;
		_gamma = orig._gamma;
		_color = orig._color;
		_background = orig._background;
		_outline = orig._outline;
		_outlinecolor = orig._outlinecolor;
		_underline = orig._underline;
		_underlinecolor = orig._underlinecolor;
		_overline = orig._overline;
		_overlinecolor = orig._overlinecolor;
		_strikethrough = orig._strikethrough;
		_strikethroughcolor = orig._strikethroughcolor;
	}

	Font& Font::operator = (const Font& orig)
	{
		_family = orig._family;
		_size = orig._size;
		_bold = orig._bold;
		_italic = orig._italic;
		_rise = orig._rise;
		_spacing = orig._spacing;
		_gamma = orig._gamma;
		_color = orig._color;
		_background = orig._background;
		_outline = orig._outline;
		_outlinecolor = orig._outlinecolor;
		_underline = orig._underline;
		_underlinecolor = orig._underlinecolor;
		_overline = orig._overline;
		_overlinecolor = orig._overlinecolor;
		_strikethrough = orig._strikethrough;
		_strikethroughcolor = orig._strikethroughcolor;

		return *this;
	}

}

