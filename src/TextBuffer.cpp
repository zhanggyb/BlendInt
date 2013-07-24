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
        glShadeModel (GL_FLAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
        wstring::const_iterator it;

        for(it = _text.begin(); it != _text.end(); it++)
        {
            GLuint texture = _chardb[*it].texture;
            GLuint displist = _chardb[*it].displist;

#ifdef DEBUG
            if(!glIsTexture(texture)) {
                cerr << texture << " is not a texture" << endl;
            }
            if(!glIsList(displist)) {
                cerr << displist << " is not a display list" << endl;
            }
#endif
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, texture);
            glCallLists(1, GL_UNSIGNED_BYTE, &displist);

            glDisable(GL_TEXTURE_2D);
        }
    }

    void TextBuffer::clear (void)
    {

    }

    bool TextBuffer::makeDisplayList (wchar_t charcode,
                                      GLuint list_base,
                                      GLuint* tex_base,
                                      unsigned int index)
    {
        if(gFontService == NULL) {
            return false;
        }
        if(!gFontService->isInitialized()) {
            return false;
        }

        FontType font (gFontService->getBuffer(),
                       gFontService->getBufferSize());

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

        if(gFontService == NULL) {
            return false;
        }
        if(!gFontService->isInitialized()) {
            return false;
        }

        FontType font (gFontService->getBuffer(),
                       gFontService->getBufferSize());

        if (!font.isValid()) {
            cerr << "Cannot get Font" << endl;
            return false;
        }

        font.setCharSize(24, 96); // demo code

        bool result = font.loadCharacter (charcode, FT_LOAD_RENDER);

        if (!result) return false;

        FT_Face face = font.getFontFace();

        int rows = face->glyph->bitmap.rows;
        int width = face->glyph->bitmap.width;

        GLuint texture;
        GLuint displist;

        width = next_p2(width);
        rows = next_p2(rows);

        // Allocate memory for the texture data.
        GLubyte* fontimage = new GLubyte[2 * width * rows];

        //Here we fill in the data for the expanded bitmap.
        //Notice that we are using two channel bitmap (one for
        //luminocity and one for alpha), but we assign
        //both luminocity and alpha to the value that we
        //find in the FreeType bitmap.
        //We use the ?: operator so that value which we use
        //will be 0 if we are in the padding zone, and whatever
        //is the the Freetype bitmap otherwise.
        FT_Bitmap& bitmap = face->glyph->bitmap;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < width; j++) {
                fontimage[2 * (i * width + j)] = 255;
                fontimage[2 * (i * width + j) + 1] =
                    ((rows - 1 -i) >= bitmap.rows ||
                     j >= bitmap.width)
                    ? 0 :
                    bitmap.buffer[bitmap.width * (rows - 1 -i)
                                               + j];
            }
        }

        //Now we just setup some texture paramaters.
        glGenTextures(1, &texture);
        glBindTexture( GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Here we actually create the texture itself, notice
        //that we are using GL_LUMINANCE_ALPHA to indicate that
        //we are using 2 channel data.
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, rows, 0,
                      GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fontimage);

        //With the texture created, we don't need the fontimage data anymore
        delete[] fontimage;

        displist = glGenLists(1);

        glNewList(displist, GL_COMPILE);

        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f (100.0, 100.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f (100.0 + 24.0, 100.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f (100.0 + 24.0, 100.0 + 24.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f (100.0, 24.0 + 100.0, 0.0);
        glEnd();

        glEndList();

        CharData data = {texture, displist};
        _chardb[charcode] = data;

        return true;
    }

} /* namespace BIL */
