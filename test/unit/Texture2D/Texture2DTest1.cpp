#include "Texture2DTest1.hpp"
#include <BlendInt/Texture2D.hpp>

using namespace BlendInt;

Texture2DTest1::Texture2DTest1()
: testing::Test()
{
	// TODO: add constructor code
}

Texture2DTest1::~Texture2DTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(Texture2DTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Texture2D - Foo1");

	// TODO: add test code here
	unsigned char image[512][512][4];
	Texture2D::MakeCheckImage (image);

	Texture2D* tex = new Texture2D;

	tex->Generate();
	tex->Bind();

	tex->SetImage (512, 512, image);
	tex->WriteToFile ("test.png");

	tex->Unbind();

	delete tex;

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
