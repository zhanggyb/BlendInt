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
#include <BlendInt/Gui/Clock.hpp>
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

#include <BlendInt/Gui/VirtualWindow.hpp>
#include <BlendInt/Gui/Decoration.hpp>

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

	VirtualWindow* vw = Manage(new VirtualWindow);
	DBG_SET_NAME(vw, "virtual window");
	vw->SetPosition(100, 100);

	Button* btn1 = Manage(new Button("Hello World 1!"));
	DBG_SET_NAME(btn1, "button1");
	btn1->SetPosition(600, 200);

	Button* btn2 = Manage(new Button("Hello World 2!"));
	DBG_SET_NAME(btn2, "button2");
	btn2->SetPosition(600, 300);

	Button* btn3 = Manage(new Button("Hello World 3!"));
	DBG_SET_NAME(btn3, "button3");
	btn3->SetPosition(600, 400);

	FramePanel* fm = Manage(new FramePanel);
	DBG_SET_NAME(fm, "Frame");
	fm->SetPosition(300, 400);

	fm->Setup(btn3);

	context->AddWidget(btn1);
	context->AddWidget(btn2);
	context->AddWidget(fm);

	context->AddWidget(vw);

#ifdef DEBUG
	//context->PrintSections();
#endif

	RunLoop(win);

#ifdef DEBUG
	//context->PrintSections();
#endif

	Interface::Release();

	Terminate();

	assert(Object::CheckAllocatedObjects());

	return 0;
}

