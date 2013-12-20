/**
 * BlendInt demo
 */

#include <GL/glew.h>

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
#include <BlendInt/GLBuffer.hpp>
#include <BlendInt/VertexIcon.hpp>
#include <BlendInt/ScrollView.hpp>
#include <BlendInt/PopupWidget.hpp>
#include <BlendInt/Menu.hpp>
#include <BlendInt/RoundWidget.hpp>
#include <BlendInt/ImageView.hpp>
#include <BlendInt/TabFrame.hpp>

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[]) {
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* window = CreateWindow("GLFW3 Demo");

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	//widget1->SetExpandX(true);
	//widget1->SetMinimalSize(10, widget1->minimal_size().height());
	Widget* widget2 = new Widget;
    widget2->SetMinimalSize(30, widget2->minimal_size().height());
	Widget* widget3 = new Widget;
    widget3->SetMinimalSize(20, widget3->minimal_size().height());
	Widget* widget4 = new Widget;

	hlayout->Add(widget1);
	hlayout->Add(widget2);
	hlayout->Add(widget3);
	hlayout->Add(widget4);

	hlayout->Resize(80, 100);

	Interface::Instance()->Bind(hlayout);

	RunLoop(window);

	Terminate();
}

