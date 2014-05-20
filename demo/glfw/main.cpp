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
#include <BlendInt/Gui/RoundWidget.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/MenuItemBin.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/NumberSlider.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ToolButton.hpp>

#include <BlendInt/Service/StockItems.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/DirList.hpp>

#include "GLFWContext.hpp"
#include "Window.hpp"

using namespace BlendInt;
using namespace std;

namespace BI = BlendInt;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("GLFW3 Demo", 640, 480);

	GLFWContext* context = Manage (new GLFWContext);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

	ToolButton* tbtn0 = Manage(new ToolButton);
	tbtn0->set_name("ToolButton0");
	tbtn0->SetLayer(0);
	tbtn0->SetPosition(200, 200);
	tbtn0->Resize(200, 200);

	ToolButton* tbtn1 = Manage(new ToolButton);
	tbtn1->set_name("ToolButton1");
	tbtn1->SetLayer(1);
	tbtn1->SetPosition(250, 250);
	tbtn1->Resize(150, 150);

	ToolButton* tbtn2 = Manage(new ToolButton);
	tbtn2->set_name("ToolButton2");
	tbtn2->SetLayer(2);
	tbtn2->SetPosition(300, 300);
	tbtn2->Resize(100, 100);

	context->Add(tbtn0);
	context->Add(tbtn1);
	context->Add(tbtn2);

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

