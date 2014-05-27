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
#include <BlendInt/Gui/NumberSlider.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/SpaceArea.hpp>

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
	Interface::instance->SetCurrentContext(context);

	VBox* layout = Manage(new VBox);
	layout->SetMargin(2, 2, 2, 2);
	layout->SetSpace(4);

	TextEntry* path_entry = Manage(new TextEntry);
	path_entry->SetRoundCornerType (RoundAll);

	Button* open = Manage(new Button(String(L"Open")));
	HBox* dir_layout = Manage(new HBox);
	dir_layout->PushBack(path_entry);
	dir_layout->PushBack(open);

	TextEntry* file_entry = Manage(new TextEntry);
	file_entry->SetRoundCornerType (RoundAll);
	Button* cancel = Manage(new Button(L"Cancel"));
	HBox* file_layout = Manage(new HBox);
	file_layout->PushBack(file_entry);
	file_layout->PushBack(cancel);

	layout->PushBack(dir_layout);
	layout->PushBack(file_layout);

	layout->SetPosition(100, 100);
	context->Add(layout);

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

