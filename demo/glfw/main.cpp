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
#include <BlendInt/GLTexture2D.hpp>
#include <BlendInt/TextEntry.hpp>

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[]) {
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init ();
	GLFWwindow* window = CreateWindow("MenuTest1 - Click1");

	// TODO: add test code here

	int max_x = 50;
	int max_y = 20;
	HorizontalLayout* layout[max_y];
	Button* btn[max_x];
	int x_pos = 5;
	int y_pos = 5;
	for(int i = 0; i < max_y; i++)
	{
		layout[i] = new HorizontalLayout;
		layout[i]->Register();

		for(int j = 0; j < max_x; j++)
		{
			btn[j] = new Button;
			btn[j]->Resize(24, 24);
			btn[j]->SetPreferredSize(24, 24);
			layout[i]->Add(btn[j]);
		}

		layout[i]->SetPosition(x_pos, y_pos);

		y_pos += 40;
	}

	RunLoop(window);

	Terminate();

}

