// cpp

#include <BIL/FontType.h>
#include <BIL/FontManager.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cppunit/TestAssert.h>
#include <wchar.h>

#include "FontTypeTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (FontTypeTest);

FontTypeTest::FontTypeTest ()
{

}

FontTypeTest::~FontTypeTest ()
{

}

void FontTypeTest::setUp ()
{
    bool ret = false;

    if(gFontService == NULL) {
        gFontService = FontManager::instance();
    }

    gFontService->initialize();

    ret = gFontService->loadFont(); // load default font to memory
    if(!ret) {
        // TODO: stop and show failure of this TestFixture
    }
}

void FontTypeTest::tearDown ()
{
    delete gFontService;
    gFontService = NULL;
}

void FontTypeTest::draw_bitmap (FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    for(i = x, p = 0; i < x_max; i++, p++)
    {
        for(j = y, q = 0; j < y_max; j++, q++)
        {
            if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT) continue;

            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void FontTypeTest::show_image (void)
{
    int  i, j;


    for ( i = 0; i < HEIGHT; i++ )
    {
        for ( j = 0; j < WIDTH; j++ )
            putchar( image[i][j] == 0 ? ' '
                     : image[i][j] < 128 ? '+'
                     : '*' );
        putchar( '\n' );
    }
}

void FontTypeTest::create1 ()
{
    bool result;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid ();

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}

void FontTypeTest::create2 ()
{
    bool result1, result2;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font1 = new FontType(fontpath);
    FontType *font2 = new FontType(fontpath);

    result1 = font1->isValid();
    result2 = font2->isValid();

    delete font1; font1 = NULL;
    delete font2; font2 = NULL;

    CPPUNIT_ASSERT(result1 && result2);
}

void FontTypeTest::create3 ()
{
    bool result1 = false;

    FontType *font1 = new FontType(gFontService->getBuffer(),
                                   gFontService->getBufferSize());

    result1 = font1->isValid();

    delete font1; font1 = NULL;

    CPPUNIT_ASSERT(result1);
}

void FontTypeTest::get_glyph1 ()
{
    bool result;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'A';

    result = font->setFontSize (16, 72);

    if (result) {

        result = font->loadCharacter (ch, FT_LOAD_RENDER);

        if (result) {
            FT_Face face = font->getFontFace();
            cout << endl
                 << "num_faces: " << face->num_faces << endl
                 << "face_index: " << face->face_index << endl
                 << "num_glyphs: " << face->num_glyphs << endl
                 << "slot: " << endl
                 << "       " << "format: " << face->glyph->format << endl
                 << "       " << "bitmap: " << endl
                 << "               " << "rows: " << face->glyph->bitmap.rows << endl
                 << "               " << "width: " << face->glyph->bitmap.width << endl
                 << "               " << "pitch: " << face->glyph->bitmap.pitch << endl
                 << "               " << "num_grays: " << face->glyph->bitmap.num_grays << endl;

            int rows = face->glyph->bitmap.rows;
            int width = face->glyph->bitmap.width;

            int i, j; unsigned char charcode;
            for(i = 0; i < rows; i++) {
                for(j = 0; j < width; j++) {
                    charcode = *(face->glyph->bitmap.buffer + i * width + j);
                    printf ("0x%2x ", charcode);
                }
                cout << endl;
            }
            cout << endl;

            for(i = 0; i < rows; i++) {
                for(j = 0; j < width; j++) {
                    charcode = *(face->glyph->bitmap.buffer + i * width + j);
                    putchar (charcode < 128? ' ' : '*');
                }
                cout << endl;
            }
            cout << endl;

        }

    }

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}

void FontTypeTest::get_glyph2 ()
{
    bool result;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'仁';

    result = font->setCharSize (24, 96);

    if (result) {

        result = font->loadCharacter (ch, FT_LOAD_RENDER);

        if (result) {
            FT_Face face = font->getFontFace();
            cout << endl
                 << "num_faces: " << face->num_faces << endl
                 << "face_index: " << face->face_index << endl
                 << "num_glyphs: " << face->num_glyphs << endl
                 << "slot: " << endl
                 << "       " << "format: " << face->glyph->format << endl
                 << "       " << "bitmap: " << endl
                 << "               " << "rows: " << face->glyph->bitmap.rows << endl
                 << "               " << "width: " << face->glyph->bitmap.width << endl
                 << "               " << "pitch: " << face->glyph->bitmap.pitch << endl
                 << "               " << "num_grays: " << face->glyph->bitmap.num_grays << endl;

            int rows = face->glyph->bitmap.rows;
            int width = face->glyph->bitmap.width;

            int i, j; unsigned char charcode;
            for(i = 0; i < rows; i++) {
                for(j = 0; j < width; j++) {
                    charcode = *(face->glyph->bitmap.buffer + i * width + j);
                    printf ("0x%2x ", charcode);
                }
                cout << endl;
            }
            cout << endl;

            for(i = 0; i < rows; i++) {
                for(j = 0; j < width; j++) {
                    charcode = *(face->glyph->bitmap.buffer + i * width + j);
                    putchar (charcode < 128? ' ' : '*');
                }
                cout << endl;
            }
            cout << endl;

        }

    }

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}

void FontTypeTest::get_glyph3 ()
{
    bool result;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'义';

    result = font->setCharSize (24 * 64, 0, 96, 0);

    if (result) {

        result = font->loadCharacter (ch, FT_LOAD_RENDER);

        if (result) {
            FT_Face face = font->getFontFace();
            cout << endl
                 << "num_faces: " << face->num_faces << endl
                 << "face_index: " << face->face_index << endl
                 << "num_glyphs: " << face->num_glyphs << endl
                 << "slot: " << endl
                 << "       " << "format: " << face->glyph->format << endl
                 << "       " << "bitmap: " << endl
                 << "               " << "rows: " << face->glyph->bitmap.rows << endl
                 << "               " << "width: " << face->glyph->bitmap.width << endl
                 << "               " << "pitch: " << face->glyph->bitmap.pitch << endl
                 << "               " << "num_grays: " << face->glyph->bitmap.num_grays << endl;

            int rows = face->glyph->bitmap.rows;
            int width = face->glyph->bitmap.width;

            int i, j; unsigned char charcode;
            for(i = 0; i < rows; i++) {
                for(j = 0; j < width; j++) {
                    charcode = *(face->glyph->bitmap.buffer + i * width + j);
                    printf ("0x%2x ", charcode);
                }
                cout << endl;
            }
            cout << endl;

            for(i = 0; i < rows; i++) {
                for(j = 0; j < width; j++) {
                    charcode = *(face->glyph->bitmap.buffer + i * width + j);
                    putchar (charcode < 128? ' ' : '*');
                }
                cout << endl;
            }
            cout << endl;

        }

    }

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}

void FontTypeTest::get_glyph4 ()
{
    bool result;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'礼';

    result = font->setCharSize (24 * 64, 0, 96, 0);

    if (result) {

        FT_UInt glyph_index = font->getCharIndex (ch);

        result = font->loadGlyph (glyph_index);

        CPPUNIT_ASSERT (result != 0);

        if (result) {

            result = font->renderGlyph (FT_RENDER_MODE_NORMAL);

            if (result) {
                FT_Face face = font->getFontFace();
                cout << endl
                     << "num_faces: " << face->num_faces << endl
                     << "face_index: " << face->face_index << endl
                     << "num_glyphs: " << face->num_glyphs << endl
                     << "slot: " << endl
                     << "       " << "format: " << face->glyph->format << endl
                     << "       " << "bitmap: " << endl
                     << "               " << "rows: " << face->glyph->bitmap.rows << endl
                     << "               " << "width: " << face->glyph->bitmap.width << endl
                     << "               " << "pitch: " << face->glyph->bitmap.pitch << endl
                     << "               " << "num_grays: " << face->glyph->bitmap.num_grays << endl;

                int rows = face->glyph->bitmap.rows;
                int width = face->glyph->bitmap.width;

                int i, j; unsigned char charcode;
                for(i = 0; i < rows; i++) {
                    for(j = 0; j < width; j++) {
                        charcode = *(face->glyph->bitmap.buffer + i * width + j);
                        printf ("0x%2x ", charcode);
                    }
                    cout << endl;
                }
                cout << endl;

                for(i = 0; i < rows; i++) {
                    for(j = 0; j < width; j++) {
                        charcode = *(face->glyph->bitmap.buffer + i * width + j);
                        putchar (charcode < 128? ' ' : '*');
                    }
                    cout << endl;
                }
                cout << endl;

            }
        }

    }

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}

void FontTypeTest::get_glyph5 ()
{
    bool result;

    FontType *font = new FontType(gFontService->getBuffer(),
                                  gFontService->getBufferSize());

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'智';

    result = font->setCharSize (24 * 64, 0, 96, 0);

    if (result) {

        FT_UInt glyph_index = font->getCharIndex (ch);

        result = font->loadGlyph (glyph_index);

        CPPUNIT_ASSERT (result != 0);

        if (result) {

            result = font->renderGlyph (FT_RENDER_MODE_NORMAL);

            if (result) {
                FT_Face face = font->getFontFace();

                int rows = face->glyph->bitmap.rows;
                int width = face->glyph->bitmap.width;

                int i, j; unsigned char charcode;
                cout << endl;
                for(i = 0; i < rows; i++) {
                    for(j = 0; j < width; j++) {
                        charcode = *(face->glyph->bitmap.buffer + i * width + j);
                        putchar (charcode < 128? ' ' : '*');
                    }
                    cout << endl;
                }
                cout << endl;

            }
        }

    }

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}


void FontTypeTest::glyph_metrics1 ()
{
    bool result;

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'信';

    result = font->setCharSize (24 * 64, 0, 96, 0);

    if (result) {

        FT_UInt glyph_index = font->getCharIndex (ch);

        result = font->loadGlyph (glyph_index);

        CPPUNIT_ASSERT (result != 0);

        if (result) {

            result = font->renderGlyph (FT_RENDER_MODE_NORMAL);

            if (result) {
                FT_Face face = font->getFontFace();

                int rows = face->glyph->bitmap.rows;
                int width = face->glyph->bitmap.width;

                int i, j; unsigned char charcode;
                cout << endl;
                for(i = 0; i < rows; i++) {
                    for(j = 0; j < width; j++) {
                        charcode = *(face->glyph->bitmap.buffer + i * width + j);
                        putchar (charcode < 128? ' ' : '*');
                    }
                    cout << endl;
                }
                cout << endl;

                // show glyph metrics
                cout << "Glyph metrics: " << endl;
                cout << "       " << "width: "<< face->glyph->metrics.width << endl;
                cout << "       " << "height: " << face->glyph->metrics.height << endl;
            }
        }

    }

    delete font; font = NULL;

    CPPUNIT_ASSERT(result);
}
