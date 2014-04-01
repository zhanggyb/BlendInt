#include "ImageTest1.hpp"
#include <BlendInt/Core/Image.hpp>

using namespace BlendInt;

ImageTest1::ImageTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ImageTest1::~ImageTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ImageTest1, Foo1)
{
    // TODO: add test code here
	Image io;

	io.Read("demo.png");

	DBG_PRINT_MSG("image: width: %d, height: %d, channel: %d", io.width(), io.height(), io.channels());

	ASSERT_TRUE(true);
}
