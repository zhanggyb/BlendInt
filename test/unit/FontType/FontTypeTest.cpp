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

}

void FontTypeTest::tearDown ()
{

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

    if(FontManager::gFontService == NULL) {
        FontManager::gFontService = FontManager::instance();
    }

    FontManager::gFontService->initialize();

    string fontpath = FontManager::gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid ();

    delete font; font = NULL;

    delete FontManager::gFontService;
    FontManager::gFontService = NULL;

    CPPUNIT_ASSERT(result);
}

void FontTypeTest::create2 ()
{
    bool result1, result2;

    if(FontManager::gFontService == NULL) {
        FontManager::gFontService = FontManager::instance();
    }

    FontManager::gFontService->initialize();

    string fontpath = FontManager::gFontService->getFontPath("Sans");

    FontType *font1 = new FontType(fontpath);
    FontType *font2 = new FontType(fontpath);

    result1 = font1->isValid();
    result2 = font2->isValid();

    delete font1; font1 = NULL;
    delete font2; font2 = NULL;

    delete FontManager::gFontService;
    FontManager::gFontService = NULL;

    CPPUNIT_ASSERT(result1 && result2);
}

void FontTypeTest::get_glyph1 ()
{
    bool result;

    if(FontManager::gFontService == NULL) {
        FontManager::gFontService = FontManager::instance();
    }

    FontManager::gFontService->initialize();

    string fontpath = FontManager::gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid();

    if(!font->isValid()) {
        delete font; font = NULL;
        delete FontManager::gFontService;
        FontManager::gFontService = NULL;

        CPPUNIT_FAIL ("Cannot create font\n");
        return;
    }

    wchar_t ch = L'A';

    result = font->setFontSize (16, 72);

    if (result) {

        result = font->loadChar (ch, FT_LOAD_RENDER);

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

    // Disable font service now
    delete FontManager::gFontService;
    FontManager::gFontService = NULL;

    CPPUNIT_ASSERT(result);
}
