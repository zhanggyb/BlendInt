// cpp

#include <BIL/FontType.h>
#include <BIL/FontManager.h>
#include <iostream>
#include <string>

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
