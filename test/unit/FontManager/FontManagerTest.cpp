// cpp

#include <BIL/FontManager.h>
#include <iostream>
#include <string>

#include "FontManagerTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (FontManagerTest);

FontManagerTest::FontManagerTest ()
{

}

FontManagerTest::~FontManagerTest ()
{

}

void FontManagerTest::setUp ()
{

}

void FontManagerTest::tearDown ()
{

}

void FontManagerTest::initialize1 ()
{
    bool result;

    if(FontManager::gFontService == NULL) {
        FontManager::gFontService = FontManager::instance();
    }

    result = FontManager::gFontService->initialize();

    delete FontManager::gFontService;
    FontManager::gFontService = NULL;

    CPPUNIT_ASSERT(result);
}

void FontManagerTest::loadfont1 ()
{
    bool result;

    if(FontManager::gFontService == NULL) {
        FontManager::gFontService = FontManager::instance();
    }

    result = FontManager::gFontService->initialize();

    if(result) {
        result = FontManager::gFontService->loadFont();

        if(result) {
            result = FontManager::gFontService->getBuffer() != NULL;
        }
    }

    delete FontManager::gFontService;
    FontManager::gFontService = NULL;

    CPPUNIT_ASSERT(result);
}
