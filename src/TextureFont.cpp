/*
 * TextureFont.cpp
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#include <BIL/TextureFont.h>

namespace BIL {

    TextureFont::TextureFont (TextureAtlas* atlas, const string& filename,
                              const float size)
        : _atlas(atlas), _height(0), _ascender(0), _descender(0),
          _filename(filename), _size(size), _outline_type(0),
          _outline_thickness(0), _hinting(1), _kerning(1), _filtering(1),
          _underline_thickness(0.0), _underline_position(0.0), _linegap (0.0)

    {
        // TODO Auto-generated constructor stub
        _lcd_weights[0] = 0x10;
        _lcd_weights[1] = 0x40;
        _lcd_weights[2] = 0x70;
        _lcd_weights[3] = 0x40;
        _lcd_weights[4] = 0x10;
    }

    TextureFont::~TextureFont ()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace BIL */
