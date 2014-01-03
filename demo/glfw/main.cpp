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

#include "Window.hpp"

#include "DemoFrame.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[]) {
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init ();
	GLFWwindow* window = CreateWindow("MenuTest1 - Click1");

	// TODO: add test code here

	Button* button = new Button("Hello");
	button->SetPosition(200, 400);
	button->Register();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		Interface::Instance()->Draw();

#ifdef DEBUG
		// render icons:
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(100, 100, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(55, 55, 55, 225);

		Interface::Instance()->DispatchRender(StockIcon::Instance()->menu());

		glTranslatef(50, 0, 0);

		Interface::Instance()->DispatchRender(StockIcon::Instance()->circle());

		glTranslatef(50, 0, 0);

		Interface::Instance()->DispatchRender(StockIcon::Instance()->checkbox());

		glTranslatef(50, 0, 0);

		Interface::Instance()->DispatchRender(StockIcon::Instance()->num());

		glDisable(GL_BLEND);
		glPopMatrix();

#endif

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Terminate();

}

