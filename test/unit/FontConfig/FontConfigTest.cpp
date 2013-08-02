// cpp

#include <BIL/FontConfig.h>
#include <iostream>
#include <string>

#include "FontConfigTest.h"

using namespace BIL;
using namespace std;

// CPPUNIT_TEST_SUITE_REGISTRATION(FontConfigTest);

FontConfigTest::FontConfigTest ()
{

}

FontConfigTest::~FontConfigTest ()
{

}

void FontConfigTest::setUp ()
{

}

void FontConfigTest::tearDown ()
{

}

void FontConfigTest::initialize1 ()
{
	bool result;

	FontConfig::instance();

	result = FontConfig::getService()->initialize();

	if(result) {
		result = FontConfig::release();
	}

	CPPUNIT_ASSERT(result);
}

void FontConfigTest::loadfont1 ()
{
	bool result;

	FontConfig::instance();

	result = FontConfig::getService()->initialize();

	if (result) {
		result = FontConfig::getService()->loadDefaultFontToMem();

		if (result) {
			result = FontConfig::getService()->getBuffer() != NULL;
		}
	}

	FontConfig::release();

	CPPUNIT_ASSERT(result);
}

void FontConfigTest::loadfont2 ()
{
	bool result;

	FontConfig::instance();

	result = FontConfig::getService()->initialize();

	if (result) {
		string file = FontConfig::getService()->getFontPath(Font("Source Code Pro", 10, true));

		cout << "Font file path" << file << endl;
		result = !file.empty();
	}

	FontConfig::release();

	CPPUNIT_ASSERT(result);
}
