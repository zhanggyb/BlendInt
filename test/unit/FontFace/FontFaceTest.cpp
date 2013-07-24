// cpp

#include <BIL/FontType.h>
#include <BIL/FontManager.h>
#include <iostream>
#include <string>

#include "FontFaceTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (FontFaceTest);

FontFaceTest::FontFaceTest ()
{

}

FontFaceTest::~FontFaceTest ()
{

}

void FontFaceTest::setUp ()
{

}

void FontFaceTest::tearDown ()
{

}

void FontFaceTest::create1 ()
{
    bool result;

    if(gFontService == NULL) {
        gFontService = FontManager::instance();
    }

    gFontService->initialize();

    string fontpath = gFontService->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid ();

    delete font; font = NULL;
    delete gFontService;
    gFontService = NULL;

    CPPUNIT_ASSERT(result);
}
