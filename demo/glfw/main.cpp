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
#include <BlendInt/GLTexture2D.hpp>
#include <BlendInt/TextEntry.hpp>
#include <BlendInt/ClockWidget.hpp>

#include <BlendInt/Window.hpp>

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Window::Initialize();
	Window::Create("glfw demo", 1280, 800);

	if(!Interface::Initialize()) {
		Window::Release();
		exit(1);
	}

	Interface::Instance()->Resize(1280, 800);

	Widget* widget = new Widget;
	widget->SetPosition(200, 200);
	widget->Register();

	ClockWidget* clock = new ClockWidget;
	clock->SetPosition(400, 200);
	clock->Register();

	Window::Run();

	Interface::Release();

	Window::Release();

	return 0;
}

