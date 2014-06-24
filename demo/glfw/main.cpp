/**
 * BlendInt demo
 */

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

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
	Interface::instance->SetCurrentContext(context);
	context->Resize(1280, 800);

	FramePanel* fp1 = Manage(new FramePanel);
	DBG_SET_NAME(fp1, "Frame 1");
	fp1->SetPosition(200, 200);

	FramePanel* fp2 = Manage(new FramePanel);
	DBG_SET_NAME(fp2, "Frame 2");
	fp2->SetPosition(200, 200);

	FramePanel* fp3 = Manage(new FramePanel);
	DBG_SET_NAME(fp3, "Frame 3");
	fp3->SetPosition(200, 200);

	Button* btn1 = Manage(new Button);
	DBG_SET_NAME(btn1, "Button 1");

	Button* btn2 = Manage(new Button);
	DBG_SET_NAME(btn2, "Button 2");

	HBox* hbox = Manage(new HBox);
	hbox->SetPosition(100, 100);
	DBG_SET_NAME(hbox, "HBox");

	context->Add(hbox);

	hbox->PushBack(fp1);

	fp1->Setup(fp2);
	fp2->Setup(btn1);

	hbox->PushBack(fp3);
	hbox->PushBack(btn2);

	hbox->Resize(hbox->GetPreferredSize());

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

