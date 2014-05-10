/**
 * BlendInt demo
 */

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Cpp/Events.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/Types.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ClockWidget.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/HBox.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/TableLayout.hpp>
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

	HBox* hbox = Manage(new HBox);
	hbox->set_name("HBox");
	hbox->SetPosition(100, 100);
	hbox->Resize(200, 50);

	MenuButton* menubtn1 = Manage(new MenuButton("Menu"));
	menubtn1->SetPosition(100, 50);

	Button* btn1 = Manage(new Button);
	btn1->SetPosition(100, 200);

	Button* btn2 = Manage(new Button);
	btn2->SetPosition(100, 100);

	context->Add(hbox);

	hbox->Add(menubtn1);
	hbox->Add(btn1);
	hbox->Add(btn2);

	VBox* vbox = Manage(new VBox);
	vbox->set_name("VBox");
	vbox->SetPosition(200, 200);
	vbox->Resize(50, 100);

	MenuButton* menubtn2 = Manage(new MenuButton("Menu"));
	menubtn2->SetPosition(100, 50);

	Button* btn3 = Manage(new Button);
	btn3->SetPosition(100, 200);

	Button* btn4 = Manage(new Button);
	btn4->SetPosition(100, 100);

	context->Add(vbox);

	vbox->Add(menubtn2);
	vbox->Add(btn3);
	vbox->Add(btn4);

	DBG_PRINT_MSG("vbox geometry: %d %d %u %u", vbox->position().x(), vbox->position().y(), vbox->size().width(), vbox->size().height());
	DBG_PRINT_MSG("menubtn2 geometry: %d %d %u %u", menubtn2->position().x(), menubtn2->position().y(), menubtn2->size().width(), menubtn2->size().height());
	DBG_PRINT_MSG("btn3 geometry: %d %d %u %u", btn3->position().x(), btn3->position().y(), btn3->size().width(), btn3->size().height());
	DBG_PRINT_MSG("btn4 geometry: %d %d %u %u", btn4->position().x(), btn4->position().y(), btn4->size().width(), btn4->size().height());

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

