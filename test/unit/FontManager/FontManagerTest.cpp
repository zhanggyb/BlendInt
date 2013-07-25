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

    if(FontManager::service == NULL) {
	FontManager::service = FontManager::instance();
    }

    result = FontManager::service->initialize();

    delete FontManager::service;
    FontManager::service = NULL;

    CPPUNIT_ASSERT(result);
}

void FontManagerTest::loadfont1 ()
{
    bool result;

    if(FontManager::service == NULL) {
        FontManager::service = FontManager::instance();
    }

    result = FontManager::service->initialize();

    if(result) {
        result = FontManager::service->loadFont();

        if(result) {
            result = FontManager::service->getBuffer() != NULL;
        }
    }

    delete FontManager::service;
    FontManager::service = NULL;

    CPPUNIT_ASSERT(result);
}
