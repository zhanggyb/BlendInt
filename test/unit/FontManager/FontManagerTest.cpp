// cpp

#include <BIL/FontConfig.h>
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

    if(FontConfig::service == NULL) {
	FontConfig::service = FontConfig::instance();
    }

    result = FontConfig::service->initialize();

    delete FontConfig::service;
    FontConfig::service = NULL;

    CPPUNIT_ASSERT(result);
}

void FontManagerTest::loadfont1 ()
{
    bool result;

    if(FontConfig::service == NULL) {
        FontConfig::service = FontConfig::instance();
    }

    result = FontConfig::service->initialize();

    if(result) {
        result = FontConfig::service->loadDefaultFontToMem();

        if(result) {
            result = FontConfig::service->getBuffer() != NULL;
        }
    }

    delete FontConfig::service;
    FontConfig::service = NULL;

    CPPUNIT_ASSERT(result);
}
