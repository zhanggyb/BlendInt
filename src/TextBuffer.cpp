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
    {
    }

    TextBuffer::TextBuffer (const wstring& text)
    {
        append(text);
    }

    TextBuffer::~TextBuffer ()
    {
        map<wchar_t, CharData>::iterator it;
        for(it = _chardb.begin(); it != _chardb.end(); it++)
        {
            glDeleteTextures(1, &(it->second.texture));
            glDeleteLists(it->second.displist, 1);
        }
    }

    void TextBuffer::append (const wstring& text)
    {
        if(text.empty()) return;

        wstring::const_iterator it;

        for(it = text.begin(); it != text.end(); it++)
        {
            makeDisplayList(*it);
        }

        _text.append(text);
    }

    void TextBuffer::append (wchar_t charcode)
    {
        makeDisplayList(charcode);

        _text.push_back(charcode);
    }

    void TextBuffer::render (void)
    {

    }

    void TextBuffer::clear (void)
    {

    }

    bool TextBuffer::makeDisplayList (wchar_t charcode,
                                      GLuint list_base,
                                      GLuint* tex_base,
                                      unsigned int index)
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

    bool TextBuffer::makeDisplayList (wchar_t charcode)
    {
        // if the charcode is already in database, don't need to create
        // new texture and display list for it
        map<wchar_t, CharData>::iterator it;

        it = _chardb.find(charcode);
        if(it != _chardb.end()) return false;

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
