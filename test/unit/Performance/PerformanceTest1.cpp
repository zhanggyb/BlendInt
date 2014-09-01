#include "PerformanceTest1.hpp"
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/StaticFrame.hpp>
#include <Common/Window.hpp>

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
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 1280, 800);

	// TODO: add test code here
	Context* context = Manage(new Context);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);
	Interface::instance->Resize(1280, 800);

    VLayout* layout1 = Manage(new VLayout);

    Frame* frame1 = Manage(new Frame);

    frame1->SetPosition(20, 20);

    int max_x = 20;
    int max_y = 20;

    //int x_pos = 50;
    //int y_pos = 5;
    for(int i = 0; i < max_y; i++)
    {
    	HLayout* layout = Manage(new HLayout);

        for(int j = 0; j < max_x; j++)
        {
        	Button* btn = Manage(new Button);
            //btn[j]->SetPreferredSize(25, btn[j]->size().height());
        	layout->PushBack(btn);
        }

        //layout[i]->SetPosition(x_pos, y_pos);
        //layout[i]->Resize(layout[i]->GetPreferredSize());

        //y_pos += 40;

        layout1->PushBack(layout);
    }

    frame1->Setup(layout1);
    frame1->Resize(frame1->GetPreferredSize());

    context->PushBack(frame1);

    RunLoop(win);
    Interface::Release();

    Terminate();

    ASSERT_TRUE(true);
}


/**
 * Test layout performance
 */
TEST_F(PerformanceTest1, Layout2)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 1280, 800);

	// TODO: add test code here
	Context* context = Manage(new Context);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);
	Interface::instance->Resize(1280, 800);

    VLayout* layout2 = Manage(new VLayout);

    StaticFrame* frame2 = Manage(new StaticFrame);

    frame2->SetPosition(620, 20);

    int max_x = 20;
    int max_y = 20;

    for(int i = 0; i < max_y; i++)
    {
    	HLayout* layout = Manage(new HLayout);

        for(int j = 0; j < max_x; j++)
        {
        	Button* btn = Manage(new Button);
            //btn[j]->SetPreferredSize(25, btn[j]->size().height());
        	layout->PushBack(btn);
        }

        layout2->PushBack(layout);
    }

    frame2->Setup(layout2);
    frame2->Resize(frame2->GetPreferredSize());

    context->PushBack(frame2);

    RunLoop(win);
    Interface::Release();

    Terminate();

    ASSERT_TRUE(true);
}

/**
 * Test layout performance
 */
TEST_F(PerformanceTest1, Layout3)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 1280, 800);

	// TODO: add test code here
	Context* context = Manage(new Context);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);
	Interface::instance->Resize(1280, 800);

    StaticFrame* mainframe = Manage(new StaticFrame);

    StaticFrame* frame1 = Manage(new StaticFrame);
    StaticFrame* frame2 = Manage(new StaticFrame);
    StaticFrame* frame3 = Manage(new StaticFrame);

    mainframe->SetPosition(100, 100);

    int max_x = 10;
    int max_y = 10;

    VLayout* layout1 = Manage(new VLayout);
    for(int i = 0; i < max_y; i++)
    {
    	HLayout* layout = Manage(new HLayout);

        for(int j = 0; j < max_x; j++)
        {
        	Button* btn = Manage(new Button);
            //btn[j]->SetPreferredSize(25, btn[j]->size().height());
        	layout->PushBack(btn);
        }

        layout1->PushBack(layout);
    }

    frame1->Setup(layout1);
    frame1->Resize(frame1->GetPreferredSize());

    VLayout* layout2 = Manage(new VLayout);
    for(int i = 0; i < max_y; i++)
    {
    	HLayout* layout = Manage(new HLayout);

        for(int j = 0; j < max_x; j++)
        {
        	Button* btn = Manage(new Button);
            //btn[j]->SetPreferredSize(25, btn[j]->size().height());
        	layout->PushBack(btn);
        }

        layout2->PushBack(layout);
    }

    frame2->Setup(layout2);
    frame2->Resize(frame2->GetPreferredSize());

    VLayout* layout3 = Manage(new VLayout);
    for(int i = 0; i < max_y; i++)
    {
    	HLayout* layout = Manage(new HLayout);

        for(int j = 0; j < max_x; j++)
        {
        	Button* btn = Manage(new Button);
            //btn[j]->SetPreferredSize(25, btn[j]->size().height());
        	layout->PushBack(btn);
        }

        layout3->PushBack(layout);
    }

    frame3->Setup(layout3);
    frame3->Resize(frame3->GetPreferredSize());

    HLayout* mainlayout = Manage(new HLayout);

    mainlayout->PushBack(layout1);
    mainlayout->PushBack(layout2);
    mainlayout->PushBack(layout3);

    mainframe->Setup(mainlayout);
    mainframe->Resize(mainframe->GetPreferredSize());

    context->PushBack(mainframe);

    RunLoop(win);
    Interface::Release();

    Terminate();

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
