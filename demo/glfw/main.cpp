/**
 * BlendInt demo
 */

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
#include <BlendInt/VertexIcon.hpp>
#include <BlendInt/ScrollView.hpp>
#include <BlendInt/PopupWidget.hpp>
#include <BlendInt/Menu.hpp>
#include <BlendInt/RoundWidget.hpp>
#include <BlendInt/ImageView.hpp>
#include <BlendInt/TabFrame.hpp>
#include <BlendInt/MenuItemBin.hpp>
#include <BlendInt/StockIcon.hpp>
#include <BlendInt/Texture2D.hpp>

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[]) {
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init ();
	GLFWwindow* window = CreateWindow("MenuTest1 - Click1");

	// TODO: add test code here
	unsigned char image[512][512][4];
	Texture2D::MakeCheckImage (image);

	Texture2D* tex = new Texture2D;

	tex->Generate();
	tex->Bind();

	tex->SetImage (512, 512, image);
	tex->WriteToFile ("test.png");

	tex->Unbind();

	delete tex;

	RunLoop(window);

	Terminate();

}

