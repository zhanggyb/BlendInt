#include "PerformanceTest1.hpp"
#include <BlendInt/Button.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/Window.hpp>

using namespace BlendInt;

PerformanceTest1::PerformanceTest1()
: testing::Test()
{
	// TODO: add constructor code
}

PerformanceTest1::~PerformanceTest1()
{
	// TODO: add destructor code
}

/**
 * Test layout performance
 */
TEST_F(PerformanceTest1, Layout1)
{
    Window::Initialize();
    Window* win = Window::Create("Button Test - Foo1", 1024, 768);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }

    win->Resize(1280, 800);
	// TODO: add test code here


    int max_x = 12;
    int max_y = 20;
    HorizontalLayout* layout[max_y];
    Button* btn[max_x];
    int x_pos = 50;
    int y_pos = 5;
    for(int i = 0; i < max_y; i++)
    {
        layout[i] = new HorizontalLayout;
        layout[i]->Register();

        for(int j = 0; j < max_x; j++)
        {
            btn[j] = new Button;
            layout[i]->Add(btn[j]);
        }
    
        layout[i]->SetPosition(x_pos, y_pos);

        y_pos += 40;
    }

    Window::Run();
    Interface::Release();

    Window::Release();

    ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
/*
TEST_F(PerformanceTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Performance - Foo1");

	// TODO: add test code here

    int max = 800;
    Button* btn[max];
    int x_pos = 0;
    int y_pos = 0;
    for(int i = 0, j = 1; i < max; i++, j++)
    {
        btn[i] = new Button;
        btn[i]->SetPosition(x_pos, y_pos);
        btn[i]->Resize(48, 24);
        btn[i]->Register();

        x_pos += 50;
        if(j % 25 == 0) {
            x_pos = 0;
            y_pos += 25;
        }
    }

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
*/

/*
TEST_F(PerformanceTest1, Performance2)
{
	Init ();
	GLFWwindow* window = CreateWindow("Performance - 2");

	// TODO: add test code here

    int max = 800;
    Widget* label[max];
    int x_pos = 0;
    int y_pos = 0;
    for(int i = 0, j = 1; i < max; i++, j++)
    {
        label[i] = new Widget;
        label[i]->SetPosition(x_pos, y_pos);
        label[i]->Resize(48, 24);
        label[i]->Register();

        x_pos += 50;
        if(j % 25 == 0) {
            x_pos = 0;
            y_pos += 25;
        }
    }

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
*/
