/**
 * BlendInt demo
 */

#include <iostream>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Service/ContextManager.hpp>

#include <BlendInt/UI/ToggleButton.hpp>
#include <BlendInt/UI/VerticalLayout.hpp>
#include <BlendInt/UI/HorizontalLayout.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/UI/Button.hpp>
#include <BlendInt/UI/Label.hpp>
#include <BlendInt/UI/ScrollWidget.hpp>
#include <BlendInt/UI/Slider.hpp>
#include <BlendInt/UI/Frame.hpp>
#include <BlendInt/UI/VertexIcon.hpp>
#include <BlendInt/UI/TableLayout.hpp>
#include <BlendInt/UI/ScrollBar.hpp>
#include <BlendInt/UI/VertexIcon.hpp>
#include <BlendInt/UI/ScrollView.hpp>
#include <BlendInt/UI/PopupWidget.hpp>
#include <BlendInt/UI/Menu.hpp>
#include <BlendInt/UI/RoundWidget.hpp>
#include <BlendInt/UI/ImageView.hpp>
#include <BlendInt/UI/TabFrame.hpp>
#include <BlendInt/UI/MenuItemBin.hpp>
#include <BlendInt/Service/StockIcon.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/UI/TextEntry.hpp>
#include <BlendInt/UI/ClockWidget.hpp>
#include <BlendInt/UI/Viewport3D.hpp>

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

	Button* btn1 = new Button;
	btn1->set_name("Button1");
	btn1->SetExpandX(false);
	btn1->SetPosition(630, 250);
	btn1->Resize(200, 200);
	btn1->Register();

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

	RunLoop (win);

	Interface::Release();

	Terminate();

	return 0;
}

