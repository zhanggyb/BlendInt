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

	Interface* app = Interface::Instance();

//	DemoFrame* frame = new DemoFrame;

//	app->Bind(frame);

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
	hlayout->Resize(400, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpand(true);
    //widget1->SetPreferredSize(40, widget1->preferred_size().height());
    //widget1->SetMaximalSize(80, widget1->maximal_size().height());
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);
	widget2->SetPreferredSize(40, widget2->preferred_size().height());
	widget2->SetMaximalSize(80, widget2->maximal_size().height());

	hlayout->Add(widget1);
	hlayout->Add(widget2);

	app->Bind(hlayout);

	RunLoop(window);

	Terminate();
}

