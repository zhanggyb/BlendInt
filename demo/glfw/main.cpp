/**
 * BlendInt demo
 */

#include <iostream>

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
#include <BlendInt/Gui/ScrollWidget.hpp>
#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/TableLayout.hpp>
#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/PopupWidget.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/RoundWidget.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/MenuItemBin.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>

#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Service/StockItems.hpp>

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 640, 480);

	// Add Code:

	TextEntry* text = Manage(new TextEntry);
	text->set_name("TextEntry");
	text->SetPosition(140, 200);

//	HorizontalLayout* layout = new HorizontalLayout;
//	layout->Register();

//	Button* btn1 = Manage(new Button);
//	btn1->set_name("Button1");
//	btn1->SetText("Button1");
	//btn1->SetRadius(10.0);
	//btn1->SetExpandX(false);
//	btn1->SetPosition(100, 250);
	//btn1->Resize(100, 100);

//	ToggleButton* toggle1 = Manage(new ToggleButton);
//	toggle1->set_name("ToggleButton1");
	//toggle1->SetExpandX(false);
//	toggle1->SetPosition(100, 450);
	//toggle1->Resize(100, 100);

	//	Button* btn2 = new Button;
//	btn2->set_name("Button2");
//	btn2->SetExpandX(false);
//	btn2->SetPosition(600, 250);
//	btn2->Resize(100, 100);
//	btn2->SetLayer(1);
//	btn2->Register();


//	Button* btn2 = new Button;
//	btn2->set_name("Button2");
//	btn2->SetExpandX(false);
//	btn2->SetPosition(530, 250);
//	btn2->Register();

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

//	ComboBox* combo = Manage(new ComboBox);
//	combo->set_name("ComboBox");
//	combo->SetPosition(200, 200);
//	combo->Resize(100, 20);

	Slider* slide = Manage (new Slider);
	slide->set_name("Slider");
	slide->SetPosition(200, 400);
	slide->set_value(20);

	RunLoop (win);

	Interface::Release();

	Terminate();

	return 0;
}

