#include "BezierTest1.hpp"
#include <BlendInt/Gui/CubicBezierCurve.hpp>

using namespace BlendInt;

BezierTest1::BezierTest1()
: testing::Test()
{
	// TODO: add constructor code
}

BezierTest1::~BezierTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(BezierTest1, Foo1)
{
	CubicBezierCurve bezier;
	bezier.push_back(0.f, 0.f);
	bezier.push_back(10.f, 10.f);

	bezier.Unpack();

	ASSERT_TRUE(true);
}
