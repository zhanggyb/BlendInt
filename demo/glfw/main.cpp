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
#include <BlendInt/Gui/HLayout.hpp>
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
#include <BlendInt/Gui/VLayout.hpp>
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

	VLayout* vlayout = Manage(new VLayout);

	vlayout->SetPosition(20, 20);

	TextEntry* path_entry = Manage(new TextEntry);
	path_entry->SetRoundType(RoundAll);
	//m_path_entry->SetPreferredSize(65536, m_path_entry->preferred_size().height());
	Button* btn_open = Manage(new Button);
	//btn_open->SetExpandX(false);
	//m_open->SetText("Open");

	HLayout* dir_layout = Manage(new HLayout);
	dir_layout->SetMargin(0, 0, 0, 0);
	dir_layout->Add(path_entry);
	dir_layout->Add(btn_open);

	DBG_PRINT_MSG("dir layout is expandable: %s", dir_layout->expand_x() ? "true" : "false");

	TextEntry* file_entry = Manage(new TextEntry);
	file_entry->SetRoundType(RoundAll);
	Button* btn_cancel = Manage(new Button);
	//btn_cancel->SetExpandX(false);
	//m_cancel->SetText("Cancel");

	HLayout* file_layout = Manage(new HLayout);
	file_layout->SetMargin(0, 0, 0, 0);
	file_layout->Add(file_entry);
	file_layout->Add(btn_cancel);

	Widget* widget = Manage(new Widget);
	widget->SetExpand(true);

	vlayout->Add(dir_layout);
	vlayout->Add(file_layout);
	vlayout->Add(widget);

	context->Add(vlayout);

	vlayout->Resize(400, 400);

	DBG_PRINT_MSG("dir layout size: %u, %u", dir_layout->size().width(), dir_layout->size().height());

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

