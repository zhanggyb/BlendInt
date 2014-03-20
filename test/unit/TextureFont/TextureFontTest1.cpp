#include "TextureFontTest1.hpp"
#include <BlendInt/Gui/TextureFont.hpp>

using namespace BlendInt;

TextureFontTest1::TextureFontTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TextureFontTest1::~TextureFontTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TextureFontTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("TextureFont - Foo1");

    // TODO: add test code here

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
