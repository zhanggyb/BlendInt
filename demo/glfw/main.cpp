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

	ToolButton* tbtn0 = Manage(new ToolButton);
	tbtn0->SetLayer(0);
	tbtn0->SetPosition(50, 50);
	tbtn0->Resize(200, 200);

	ToolButton* tbtn1 = Manage(new ToolButton);
	tbtn1->SetLayer(1);
	tbtn1->SetPosition(250, 250);
	tbtn1->Resize(150, 150);

	ToolButton* tbtn2 = Manage(new ToolButton);
	tbtn2->SetLayer(2);
	tbtn2->SetPosition(350, 100);
	tbtn2->Resize(100, 100);

	context->Add(tbtn0);
	context->Add(tbtn1);
	context->Add(tbtn2);

	fprintf(stdout, "size of AbstractWidget: %ld\n", sizeof(AbstractWidget));

	fprintf(stdout, "Sizeof object: %ld\n", sizeof(Object));

	fprintf(stdout, "sizeof event: %ld\n", sizeof(Cpp::Event<AbstractWidget*>));

	tbtn0->SetRoundType(RoundTopLeft | RoundTopRight | RoundBottomLeft);

	ToggleButton* t = Manage(new ToggleButton);
	t->SetPosition(100, 400);
	context->Add(t);

	RunLoop(win);

	Interface::Release();

	Terminate();

	return 0;
}

