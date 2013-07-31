// cpp

#include <BIL/FontEngine.h>
#include <BIL/FontConfig.h>
#include <iostream>
#include <string>

#include "FontFaceTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontFaceTest);

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

	FontConfig::instance();

	FontConfig::getService()->initialize();

	string fontpath = FontConfig::getService()->getFontPath("Sans");

	FontEngine *font = new FontEngine(fontpath);

	result = font->isValid();

	delete font;
	font = NULL;

	FontConfig::release();

	CPPUNIT_ASSERT(result);
}
