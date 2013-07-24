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
        : _rowspacing(1.0)
    {
    }

    TextBuffer::TextBuffer (const wstring& text)
        : _rowspacing(1.0)
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

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, texture);
            glCallLists(1, GL_UNSIGNED_BYTE, &displist);

            glDisable(GL_TEXTURE_2D);
        }
    }

    void TextBuffer::renderAt (const Point3Df& pos)
    {
        glPushAttrib(GL_LIST_BIT |
                     GL_CURRENT_BIT |
                     GL_ENABLE_BIT |
                     GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        wstring::const_iterator it;
        GLuint displist;
        int i = 0; int j = 0;
        float x = pos.getX();
        float y = pos.getY();
        for(it = _text.begin(); it != _text.end(); it++)
        {
            if(*it == '\n') {
                i = 0;
                j++;
                continue;
            }

            displist = _chardb[*it].displist;
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(x + 24 * i, y + 48 * j + 12 * _rowspacing, 0.0);
            glMultMatrixf(modelview_matrix);
            glCallLists(1, GL_UNSIGNED_BYTE, &displist);
            glPopMatrix();

            i++;
        }
    }

    void TextBuffer::clear (void)
    {

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

        font.setCharSize(10, 96); // demo code

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

        // Move the face's glyph into a Glyph object.
        FT_Glyph glyph;

        if (FT_Get_Glyph(face->glyph, &glyph))
            throw;

        // Convert the glyph to a bitmap;
        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

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
                    (i >= bitmap.rows ||
                     j >= bitmap.width)
                    ? 0 :
                    bitmap.buffer[bitmap.width * i
                                  + j];
            }
        }

        //Now we just setup some texture paramaters.
        glGenTextures(1, &texture);
        glBindTexture( GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

        //first we need to move over a little so that
        //the character has the right amount of space
        //between it and the one before it.
        glTranslatef(bitmap_glyph->left, 0, 0);

        //Now we move down a little in the case that the
        //bitmap extends past the bottom of the line
        //(this is only true for characters like 'g' or 'y'.
        glPushMatrix();
        glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);

        //Now we need to account for the fact that many of
        //our textures are filled with empty padding space.
        //We figure what portion of the texture is used by
        //the actual character and store that information in
        //the x and y variables, then when we draw the
        //quad, we will only reference the parts of the texture
        //that we contain the character itself.
        float x = (float) bitmap.width / (float) width, y = (float) bitmap.rows
            / (float) rows;

        //Here we draw the texturemaped quads.
        //The bitmap that we got from FreeType was not
        //oriented quite like we would like it to be,
        //so we need to link the texture to the quad
        //so that the result will be properly aligned.
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2f(0, bitmap.rows);
        glTexCoord2d(0, y);
        glVertex2f(0, 0);
        glTexCoord2d(x, y);
        glVertex2f(bitmap.width, 0);
        glTexCoord2d(x, 0);
        glVertex2f(bitmap.width, bitmap.rows);
        glEnd();
        glPopMatrix();
        glTranslatef(face->glyph->advance.x >> 6, 0, 0);

        //increment the raster position as if we were a bitmap font.
        //(only needed if you want to calculate text length)
        //glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

        //Finnish the display list
        glEndList();

        /*
          glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0); glVertex3f (100.0, 100.0, 0.0);
          glTexCoord2f(1.0, 0.0); glVertex3f (100.0 + 24.0, 100.0, 0.0);
          glTexCoord2f(1.0, 1.0); glVertex3f (100.0 + 24.0, 100.0 + 24.0, 0.0);
          glTexCoord2f(0.0, 1.0); glVertex3f (100.0, 24.0 + 100.0, 0.0);
          glEnd();

          glEndList();
        */

        CharData data = {texture, displist};
        _chardb[charcode] = data;

        return true;
    }

} /* namespace BIL */
