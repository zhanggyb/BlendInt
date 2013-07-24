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

    if(gFontService == NULL) {
	gFontService = FontManager::instance();
    }

    result = gFontService->initialize();

    delete gFontService;
    gFontService = NULL;

    CPPUNIT_ASSERT(result);
}

void FontManagerTest::loadfont1 ()
{
    bool result;

    if(gFontService == NULL) {
        gFontService = FontManager::instance();
    }

    result = gFontService->initialize();

    if(result) {
        result = gFontService->loadFont();

        if(result) {
            result = gFontService->getBuffer() != NULL;
        }
    }

    delete gFontService;
    gFontService = NULL;

    CPPUNIT_ASSERT(result);
}
