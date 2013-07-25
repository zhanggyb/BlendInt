/**
 * @brief implementations for Glyph
 */

#include <string.h>
#include <BIL/Glyph.h>
#include <BIL/FontManager.h>
#include <BIL/FontType.h>

namespace BIL {

    Glyph::Glyph (wchar_t charcode)
        : _texture(0), _displist(0)
    {
        Glyph (charcode, "Sans", 12, 96);
    }

    Glyph::Glyph (wchar_t charcode,
                  const string& fontname,
                  unsigned int fontsize,
                  unsigned int dpi)
        : _charcode(charcode), _texture(0), _displist(0)
    {
        memset (&_metrics, 0, sizeof(Metrics));
    }

    Glyph::Glyph (const Glyph& orig)
    {

    }

    Glyph& Glyph::operator = (const Glyph& orig)
    {
        return *this;
    }

    Glyph::~Glyph ()
    {
        if(glIsList(_displist)) {
            glDeleteLists(_displist, 1);
        }
        if(glIsTexture(_texture)) {
            glDeleteTextures(1, &_texture);
        }
    }

    bool Glyph::makeDisplayList (void)
    {
        FontManager* fontserv = FontManager::service;

        if(fontserv == NULL) {
            return false;
        }
        if(fontserv->isInitialized()) {
            return false;
        }

        FontType font (fontserv->getBuffer(),
                       fontserv->getBufferSize());

        if(! font.isValid()) {
            cerr << "Cannot get Font" << endl;
            return false;
        }

        font.setCharSize(_fontsize, _dpi);

        bool result = font.loadCharacter (_charcode, FT_LOAD_RENDER);
        if (!result) return false;

        FT_Face face = font.getFontFace();

        _metrics.width = face->glyph->metrics.width / 64;
        _metrics.height = face->glyph->metrics.height / 64;
        _metrics.horiBearingX = face->glyph->metrics.horiBearingX / 64;
        _metrics.horiBearingY = face->glyph->metrics.horiBearingY / 64;
        _metrics.horiAdvance = face->glyph->metrics.horiAdvance / 64;
        _metrics.vertBearingX = face->glyph->metrics.vertBearingX / 64;
        _metrics.vertBearingY = face->glyph->metrics.vertBearingY / 64;
        _metrics.vertAdvance = face->glyph->metrics.vertAdvance / 64;

        int rows = face->glyph->bitmap.rows;
        int width = face->glyph->bitmap.width;

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

        glGenTextures(1, &_texture);
        glBindTexture( GL_TEXTURE_2D, _texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // GL_LUMINANCE_ALPHA to use 2 channel data.
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, rows, 0,
                      GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fontimage);

        delete[] fontimage;

        _displist = glGenLists(1);

        glNewList(_displist, GL_COMPILE);

        glBindTexture(GL_TEXTURE_2D, _texture);

        glTranslatef(bitmap_glyph->left, 0, 0);

        glPushMatrix();
        glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);

        float x = (float) bitmap.width / (float) width,
            y = (float) bitmap.rows / (float) rows;

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
        // glTranslatef(face->glyph->advance.x >> 6, 0, 0);

        glEndList();

        return true;

    }

} // namespace BIL
