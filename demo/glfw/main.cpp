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

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[]) {
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init ();
	GLFWwindow* window = CreateWindow("MenuTest1 - Click1");

	// TODO: add test code here

	VertexIcon* icon = StockIcon::Instance()->menu();

	Menu* menu = new Menu;
	menu->SetRoundType(RoundAll);

	menu->SetPosition(200, 200);

	menu->AddMenuItem(icon, "MenuItem1", "Ctrl + A");
	menu->AddMenuItem(icon, "MenuItem2", "Ctrl + B");
	menu->AddMenuItem(icon, "MenuItem3", "Ctrl + C");
	menu->AddMenuItem(icon, "MenuItem4", "Ctrl + D");
	menu->AddMenuItem(icon, "MenuItem5", "Ctrl + E");

	menu->Register();

	RunLoop(window);

	Terminate();

}

