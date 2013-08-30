/* Test case for FontFace */

#ifndef _SHADER_TEST_H
#define _SHADER_TEST_H

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Widget.hpp>
#include <BIL/Program.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace BIL::GL;
using namespace std;

class ShaderWidget: public Widget
{
public:

	ShaderWidget ();

	virtual ~ShaderWidget ()
	{
	}

protected:

	virtual void render ();

private:

	Program program_;
};

class ShaderTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ShaderTest);

	CPPUNIT_TEST(shader_load1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	ShaderTest ();

	virtual ~ShaderTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void shader_load1 ();

};

#endif  /* _SHADER_TEST_H */
