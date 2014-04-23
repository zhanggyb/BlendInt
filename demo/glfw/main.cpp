/**
 * BlendInt demo
 */

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ClockWidget.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/TableLayout.hpp>
#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/RoundWidget.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/MenuItemBin.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/NumberSlider.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ToolButton.hpp>

#include <BlendInt/Service/StockItems.hpp>
#include <BlendInt/Gui/Context.hpp>

#include "GLFWContext.hpp"
#include "Window.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 640, 480);

//	Viewport3D* view3d1 = new Viewport3D;
//	view3d1->set_name("Viewport 3D1");
//	view3d1->SetPosition(20, 20);
//	view3d1->Resize(500, 400);

//	Viewport3D* view3d2 = new Viewport3D;
//	view3d2->set_name("Viewport 3D2");
//	view3d2->SetPosition(220, 20);
//	view3d2->Resize(500, 400);
//	view3d2->Register();

//	VerticalLayout* vlayout = new VerticalLayout;
//	vlayout->Add(btn1);
//	vlayout->Add(btn2);
//
//	layout->Add(view3d1);
//	layout->Add(view3d2);
//
//	layout->Add(vlayout);

//	ImageView* iv = new ImageView;
//
//	iv->SetPosition(100, 100);
//	iv->Register();

//	interface()->SetMainWidget(layout);

	/*
	MenuBar* menubar = Manage(new MenuBar);
	menubar->set_name("MenuBar");
	menubar->SetPosition(20, 600);
	menubar->Resize(600, 20);

	ScrollBar* scrollbar = Manage(new ScrollBar);

	scrollbar->set_name("ScrollBar");
	scrollbar->SetPosition(20, 560);

	TextEntry* text = Manage(new TextEntry);
	text->set_name("TextEntry");
	text->SetPosition(500, 560);

	Button* btn1 = Manage(new Button);
	btn1->set_name("Button1");
	btn1->SetText("PushButton");
	btn1->SetPosition(530, 520);

	ToggleButton* toggle1 = Manage(new ToggleButton);
	toggle1->set_name("ToggleButton1");
	toggle1->SetText("ToggleButton");
	toggle1->SetPosition(530, 480);

	ComboBox* combo = Manage(new ComboBox);
	combo->set_name("ComboBox");
	combo->SetPosition(530, 440);

	Slider* slide = Manage (new Slider);
	slide->set_name("Slider");
	slide->SetPosition(20, 520);
	slide->SetValue(20);

	Viewport3D* view3d = Manage(new Viewport3D);
	view3d->set_name("View3D");
	view3d->Resize(400, 400);
	view3d->SetPosition(20, 100);

	Widget * widget = Manage(new Widget);
	widget->set_name("Widget");
	widget->Resize(180, 180);
	widget->SetPosition(440, 240);

	Frame* frame = Manage(new Frame);
	frame->set_name("Frame");
	frame->Resize(180, 180);
	frame->SetPosition(440, 40);

	NumberSlider* num_slider = Manage(new NumberSlider);
	num_slider->set_name("NumberSlider");
	num_slider->SetPosition(20, 60);
	*/

	/*
	Widget* widget = new Widget;
	widget->set_name("ToolButton");
	widget->Resize(300, 400);

	ScrollArea* area = Manage(new ScrollArea);
	area->set_name("ScrollArea");
	area->SetPosition(100, 100);
	area->SetViewport(Manage(widget));
	*/

//	Slider* slide = Manage (new Slider);
//	slide->set_name("Slider");
//	slide->SetPosition(20, 520);
//	slide->SetValue(20);

//	ScrollBar* scrollbar = Manage(new ScrollBar);
//	scrollbar->set_name("ScrollBar");
//	scrollbar->SetPosition(200, 200);
//	scrollbar->SetPercentage(100);

	/*

	HLayout* hlayout = Manage(new HLayout);
	hlayout->set_name("HLayout");

	Button* btn1 = Manage(new Button);
	btn1->set_name("Button1");

	Button* btn2 = Manage(new Button);
	btn2->set_name("Button2");

	Button* btn3 = Manage(new Button);
	btn3->set_name("Button3");

	hlayout->Add(btn1);
	hlayout->Add(btn2);
	hlayout->Add(btn3);
	*/

	//RunLoop (win);

	/*
	Context* context = Manage(new Context);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

	Interface::instance->Resize(640, 480);

	Button* btn1 = Manage(new Button);
	btn1->set_name("Button1");
	btn1->SetPosition(200, 200);

	Frame* frame = Manage (new Frame);
	frame->set_name("Frame");
	frame->SetPosition(100, 100);

	context->Add(frame);

	context->Add(btn1);
	*/

	GLFWContext* context = Manage (new GLFWContext);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);
	context->set_max_tex_buffer_cache_size(2);

	Widget* w1 = Manage(new Widget);
	w1->set_name("widget1");
	w1->SetPosition(100, 100);
    context->Add(w1);

    Widget* w2 = Manage(new Widget);
    w2->set_name("widget2");
    w2->SetPosition(150, 150);
    w2->SetLayer(1);
    context->Add(w2);

    Widget* w3 = Manage(new Widget);
    w3->set_name("widget3");
    w3->SetPosition(200, 200);
    w3->SetLayer(2);
    context->Add(w3);

    Widget* w4 = Manage(new Widget);
    w4->set_name("widget4");
    w4->SetPosition(250, 250);
    w4->SetLayer(3);
    context->Add(w4);

    delete w4;
    delete w3;
    delete w2;

    Widget* w5 = Manage(new Widget);
    w5->set_name("widget5");
    w5->SetPosition(300, 300);
    w5->SetLayer(4);
    context->Add(w5);

    Widget* w6 = Manage(new Widget);
    w6->set_name("widget6");
    w6->SetPosition(350, 350);
    w6->SetLayer(5);
    context->Add(w6);

#ifdef DEBUG
	context->PrintLayers();
#endif

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

