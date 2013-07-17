// cpp

#include <BIL/FontFace.h>
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

    if(FontManager::gFontService == NULL) {
        FontManager::gFontService = FontManager::instance();
    }

    FontManager::gFontService->initialize();

    string fontpath = FontManager::gFontService->getFontPath("Sans");

    FontFace *font = new FontFace(FontManager::gFontService->getFontLibrary(), fontpath);

    result = font->isValid ();

    delete font; font = NULL;
    delete FontManager::gFontService;
    FontManager::gFontService = NULL;

    CPPUNIT_ASSERT(result);
}
