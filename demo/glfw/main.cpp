/**
 * BlendInt demo
 */

#include <iostream>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/ContextManager.hpp>

#include <BlendInt/ToggleButton.hpp>
#include <BlendInt/VerticalLayout.hpp>
#include <BlendInt/HorizontalLayout.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Button.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/ScrollWidget.hpp>
#include <BlendInt/Slider.hpp>
#include <BlendInt/Frame.hpp>
#include <BlendInt/VertexIcon.hpp>
#include <BlendInt/TableLayout.hpp>
#include <BlendInt/ScrollBar.hpp>
#include <BlendInt/VertexIcon.hpp>
#include <BlendInt/ScrollView.hpp>
#include <BlendInt/PopupWidget.hpp>
#include <BlendInt/Menu.hpp>
#include <BlendInt/RoundWidget.hpp>
#include <BlendInt/ImageView.hpp>
#include <BlendInt/TabFrame.hpp>
#include <BlendInt/MenuItemBin.hpp>
#include <BlendInt/StockIcon.hpp>
#include <BlendInt/opengl/GLTexture2D.hpp>
#include <BlendInt/TextEntry.hpp>
#include <BlendInt/ClockWidget.hpp>
#include <BlendInt/Viewport3D.hpp>

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1200, 800);

	// Add Code:

//	TextEntry* text = new TextEntry;
//	text->set_name("TextEntry");
//	text->SetPosition(200, 200);
//
//	text->Register();

//	HorizontalLayout* layout = new HorizontalLayout;
//	layout->Register();
//
//	Button* btn1 = new Button;
//	btn1->set_name("Button1");
//	btn1->SetExpandX(false);
//	btn1->SetPosition(530, 250);
//	btn1->Resize(200, 200);
//	btn1->Register();

//	Button* btn2 = new Button;
//	btn2->set_name("Button2");
//	btn2->SetExpandX(false);
//	btn2->SetPosition(530, 250);
//	btn2->Register();
//
//	Viewport3D* view3d1 = new Viewport3D;
//	view3d1->set_name("Viewport 3D1");
//	view3d1->SetPosition(20, 20);
//	view3d1->Resize(500, 400);
//	view3d1->Register();
//
//	Viewport3D* view3d2 = new Viewport3D;
//	view3d2->set_name("Viewport 3D2");
//	view3d2->SetPosition(20, 20);
//	view3d2->Resize(500, 400);
//	view3d2->Register();
//
//	VerticalLayout* vlayout = new VerticalLayout;
//	vlayout->Add(btn1);
//	vlayout->Add(btn2);
//
//	layout->Add(view3d1);
//	layout->Add(view3d2);
//
//	layout->Add(vlayout);

	ImageView* iv = new ImageView;

	iv->SetPosition(100, 100);
	iv->Register();

//	interface()->SetMainWidget(layout);

	Widget* widget = new Widget;

	widget->set_name("Widget");
	widget->Resize(200, 150);
	widget->SetPosition(700, 400);

	widget->Register();

	RunLoop (win);

	Interface::Release();

	Terminate();

	return 0;
}

