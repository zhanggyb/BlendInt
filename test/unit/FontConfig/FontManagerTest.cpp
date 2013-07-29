// cpp

#include <BIL/FontConfig.h>
#include <iostream>
#include <string>

#include "FontManagerTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontManagerTest);

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

	FontConfig::instance();

	result = FontConfig::getService()->initialize();

	if(result) {
		result = FontConfig::release();
	}

	CPPUNIT_ASSERT(result);
}

void FontManagerTest::loadfont1 ()
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

void FontManagerTest::loadfont2 ()
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
