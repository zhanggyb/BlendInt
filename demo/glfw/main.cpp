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
#include <BlendInt/Gui/DirList.hpp>
#include <BlendInt/Gui/HBlock.hpp>
#include <BlendInt/Gui/VBlock.hpp>

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

	NumericalSlider* num1 = Manage(new NumericalSlider);
	NumericalSlider* num2 = Manage(new NumericalSlider);
	NumericalSlider* num3 = Manage(new NumericalSlider);

	HBlock* block = Manage(new HBlock);
	block->SetPosition(100, 20);

	block->PushBack(num1);
	block->PushBack(num2);
	block->PushBack(num3);

	Stack* stack = Manage(new Stack);
	stack->SetMargin(0, 0, 0, 0);
	stack->Add(block);

	stack->SetPosition(100, 100);
	stack->Resize(stack->GetPreferredSize());

	DBG_PRINT_MSG("num1 position: %d %d", num1->position().x(), num1->position().y());
	DBG_PRINT_MSG("num2 position: %d %d", num2->position().x(), num2->position().y());
	DBG_PRINT_MSG("num3 position: %d %d", num3->position().x(), num3->position().y());

	ColorSelector* cs = Manage(new ColorSelector);
	cs->SetPosition(200, 100);

	context->Add(cs);

	context->Add(stack);

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

