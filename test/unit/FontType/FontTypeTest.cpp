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
