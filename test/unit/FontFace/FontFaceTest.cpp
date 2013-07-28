// cpp

#include <BIL/FontType.h>
#include <BIL/FontConfig.h>
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

    if(FontConfig::service == NULL) {
        FontConfig::service = FontConfig::instance();
    }

    FontConfig::service->initialize();

    string fontpath = FontConfig::service->getFontPath("Sans");

    FontType *font = new FontType(fontpath);

    result = font->isValid ();

    delete font; font = NULL;
    delete FontConfig::service;
    FontConfig::service = NULL;

    CPPUNIT_ASSERT(result);
}
