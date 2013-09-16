// cpp

#include <BIL/FontConfig.hpp>
#include <iostream>
#include <string>

#include "FontConfigTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontConfigTest);

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

	result = FontConfig::initialize();

	if(result) {
		FontConfig::release();
	}

	CPPUNIT_ASSERT(result);
}

void FontConfigTest::loadfont1 ()
{
	bool result;

	result = FontConfig::initialize();

	if (result) {
		result = FontConfig::instance()->loadDefaultFontToMem();

		if (result) {
			result = FontConfig::instance()->getBuffer() != NULL;
		}
	}

	FontConfig::release();

	CPPUNIT_ASSERT(result);
}

void FontConfigTest::loadfont2 ()
{
	bool result;

	result = FontConfig::initialize();

	if (result) {
		string file = FontConfig::instance()->getFontPath(Font("Source Code Pro", 10, true));

		cout << "Font file path" << file << endl;
		result = !file.empty();
	}

	FontConfig::release();

	CPPUNIT_ASSERT(result);
}
