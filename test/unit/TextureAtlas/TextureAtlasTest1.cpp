#include "TextureAtlasTest1.hpp"
#include "TexAtlasViewer.hpp"
#include <BlendInt/Gui/FontCache.hpp>

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

/*
TEST_F(TextureAtlasTest1, FontCacheExt1)
{
	Init ();

    GLFWwindow* win = CreateWindow("TextureAtlas - FontCacheExt1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

    FontData data;

    data.name = "Sans";
    RefPtr<FontCacheExt> cache1 = FontCacheExt::Create(data);

    cache1->Initialize(32, 95);

//    data.name = "Droid Sans";
//    RefPtr<FontCacheExt> cache2 = FontCacheExt::Create(data);
//
//    data.name = "Droid Sans Mono";
//    RefPtr<FontCacheExt> cache3 = FontCacheExt::Create(data);

    std::cout << "count: " << cache1->count() << std::endl;;

    FontCacheExt::list();

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
*/

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
    Interface::instance->SetCurrentContext(context);

	TexAtlasViewer* view = Manage(new TexAtlasViewer);
	view->SetPosition(20, 20);
	
	context->Append(view);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
