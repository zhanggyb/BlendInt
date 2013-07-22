/*
 * TextBuffer.cpp
 *
 *  Created on: 2013年7月12日
 *      Author: zhanggyb
 */

#include <BIL/FontType.h>
#include <BIL/FontManager.h>
#include <BIL/TextBuffer.h>

namespace BIL {

    TextBuffer::TextBuffer ()
        : _textures(NULL)
    {
        _textures = new GLuint[128];

        _displist = glGenLists(128);
        glGenTextures(128, _textures);
    }

    TextBuffer::TextBuffer (const wstring& text)
        : _textures(NULL), _text(text)
    {
        _textures = new GLuint[128];

        _displist = glGenLists(128);
        glGenTextures(128, _textures);
    }

    TextBuffer::~TextBuffer ()
    {
        glDeleteLists(_displist, 128);
        if(_textures != NULL) {
            glDeleteTextures(128, _textures);
            delete [] _textures;
            _textures = NULL;
        }
    }

    void TextBuffer::append (const wstring& text)
    {

    }

    void TextBuffer::append (wchar_t charcode)
    {

    }

    void TextBuffer::render (void)
    {

    }

    void TextBuffer::clear (void)
    {

    }

    bool TextBuffer::makeDisplayList (wchar_t charcode,
                                      GLuint list_base,
                                      GLuint* tex_base)
    {
        if(FontManager::gFontService == NULL) {
            return false;
        }
        if(!FontManager::gFontService->isInitialized()) {
            return false;
        }

        FontType font (FontManager::gFontService->getBuffer(),
                       FontManager::gFontService->getBufferSize());

        if (!font.isValid()) {
            cerr << "Cannot get Font" << endl;
            return false;
        }

        font.setCharSize(12, 72); // demo code

        bool result = font.loadCharacter (charcode, FT_LOAD_RENDER);

        if (!result) return false;

        int width = font.getFontFace()->glyph->bitmap.width;
        int height = font.getFontFace()->glyph->bitmap.rows;

        cout << width << endl;
        cout << height << endl;

        unsigned char *fontimage = new unsigned char [width * height];
        int i, j;

        for(i = 0; i < height; i++) {
            for(j = 0; j < width; j++) {
                fontimage[i*width + j] =
                    *(font.getFontFace()->glyph->bitmap.buffer + i * width + j);
            }
        }

        delete [] fontimage;

        return true;
    }

} /* namespace BIL */
