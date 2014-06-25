/**
 * BlendInt demo
 */

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Core/Types.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/TextureAtlas2D.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ClockWidget.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/HBox.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/MenuItemBin.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/SpaceArea.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Gui/TabButton.hpp>
#include <BlendInt/Gui/TabHeader.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/ColorButton.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/HBlock.hpp>
#include <BlendInt/Gui/VBlock.hpp>
#include <BlendInt/Gui/FramePanel.hpp>

#include "GLFWContext.hpp"
#include "Window.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 1280, 800);

	GLFWContext* context = Manage (new GLFWContext);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);
	context->Resize(1280, 800);

	/*
	Button* btn1 = Manage(new Button);
	DBG_SET_NAME(btn1, "Button 1");
	btn1->Resize(100, 50);
	btn1->SetPosition(200, 100);

	FramePanel* f1 = Manage(new FramePanel);
	DBG_SET_NAME(f1, "Frame 1");
	f1->SetMargin(20, 20, 20, 20);
	f1->SetPosition(400, 100);

	FramePanel* f2 = Manage(new FramePanel);
	DBG_SET_NAME(f2, "Frame 2");
	f2->SetMargin(20, 20, 20, 20);
	f2->SetPosition(400, 100);

	Button* btn2 = Manage(new Button);
	DBG_SET_NAME(btn2, "Button 2");

	f2->Setup(btn2);
	f1->Setup(f2);

	ColorSelector* cs = Manage(new ColorSelector);
	cs->SetPosition(200, 200);

	context->Add(cs);

	context->Add(btn1);
	context->Add(f1);

	f1->Resize(200, 400);
	*/

	Button* btn = Manage(new Button);
	DBG_SET_NAME(btn, "Button");
	btn->Resize(500, 400);

	ScrollArea* sa = Manage(new ScrollArea);
	DBG_SET_NAME(sa, "ScrollArea");
	sa->SetPosition(100, 100);

	sa->SetViewport(btn);

	sa->Resize(800, 600);

	context->Add(sa);

	RunLoop(win);

	Interface::Release();

	Terminate();

	assert(Object::CheckAllocatedObjects());

	return 0;
}

