#include "TextureAtlasTest1.hpp"
#include "TexAtlasViewer.hpp"

using namespace BlendInt;

TextureAtlasTest1::TextureAtlasTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TextureAtlasTest1::~TextureAtlasTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TextureAtlasTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("TextureAtlas - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	TexAtlasViewer* view = Manage(new TexAtlasViewer);
	view->set_name("TexAtlasViewer");
	view->SetPosition(100, 100);
	
	context->Add(view);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
