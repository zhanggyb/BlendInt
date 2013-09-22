// cpp

#include <iostream>
#include <string>

#include <BILO/String.hpp>

#include "StringTest.h"

using namespace BILO;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION (StringTest);

StringTest::StringTest ()
{

}

StringTest::~StringTest ()
{

}

void StringTest::setUp ()
{

}

void StringTest::tearDown ()
{

}

void StringTest::create1 ()
{
	String str;

    CPPUNIT_ASSERT(str.length() == 0);
}

void StringTest::create2 ()
{
    String str1("Hello");
	String str2(L"Hello");

	CPPUNIT_ASSERT(str1.length() == 5 && str2.length() == 5);
}

void StringTest::create3 ()
{
	String str1("This is a long string", 10);

    CPPUNIT_ASSERT(str1.length() == 10);
}


void StringTest::create4 ()
{
	String str1;
	String str2 = L"String will be cleared";

	str1 = "Hello";
	str2 = L"Hello";

    CPPUNIT_ASSERT(str1.length() == 5 &&
                   str2.length() == 5);
}


void StringTest::create5 ()
{
	std::string src1 = "Hello";
	std::wstring src2 = L"Hello";

	String str1 = src1;
	String str2 = src2;

    CPPUNIT_ASSERT(str1 == str2);
}

