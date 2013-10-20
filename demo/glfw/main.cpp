/**
 * BlendInt demo
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <typeinfo>

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
#include <BlendInt/GLBuffer.hpp>
#include <BlendInt/VertexIcon.hpp>

using namespace BlendInt;
using namespace std;

class DoEvent
{
public:

	DoEvent()
	: i(0)
	{
		m_hlayout.set_pos(300, 400);
		m_hlayout.resize(400, 100);
//		m_hlayout.set_sizing_mode(LayoutFixed);
//		m_hlayout.set_margin(5, 5, 5, 5);
//		m_hlayout.set_space (2);
//		m_layout.set_alignment(AlignLeft);

//		m_vlayout = new VerticalLayout;
//		m_vlayout->set_space (2);

//		m_hlayout.add(m_vlayout);

	}

	~DoEvent()
	{

	}

	void connect_add(Button* button)
	{
		m_events.connect(button->clicked(), this, &DoEvent::add_button);
	}

	void connect_remove(Button* button)
	{
		m_events.connect(button->clicked(), this, &DoEvent::remove_button);
	}

	void bind()
	{
		interface()->bind(&m_hlayout);
	}

	void unbind()
	{
		interface()->unbind(&m_hlayout);
	}

	void add_button()
	{
		Button* button = new Button;

		char str[20];
		sprintf(str, "%s%u", "button", static_cast<unsigned int>(m_buttons.size()));

		button->set_text(str);
		button->set_name(str);

		m_hlayout.add(button);
		m_buttons.push_back(button);
	}

	void remove_button()
	{
		Button* button = m_buttons[m_buttons.size() - 1];
		m_hlayout.erase(button);
		m_buttons.pop_back();
	}

private:

//	VerticalLayout *m_vlayout;

	HorizontalLayout m_hlayout;

	std::vector<Button*> m_buttons;

	int i;

	Cpp::ConnectionScope m_events;
};

static void cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

static void cbWindowSize(GLFWwindow* window, int w, int h) {
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

static void cbKey(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

static void cbMouseButton(GLFWwindow* window, int button, int action,
		int mods) {
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

static void cbCursorPos(GLFWwindow* window, double xpos, double ypos) {
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}

int main(int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, __func__, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		return -1;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	DoEvent obj;

	obj.bind();

	Button* add_button = new Button;
	add_button->set_text("Add Button");
	add_button->set_pos(450, 600);

	obj.connect_add(add_button);

	Button* remove_button = new Button;
	remove_button->set_text("Remove Button");
	remove_button->set_pos(550, 600);

	obj.connect_remove(remove_button);

	app->bind(add_button);
	app->bind(remove_button);

	Button* b1 = new Button;
	Button* b2 = new Button;

	HorizontalLayout* h1 = new HorizontalLayout;
	h1->set_name("hlayout");

	h1->add(b1);
	h1->add(b2);

	Button* b3 = new Button;
	Button* b4 = new Button;

	VerticalLayout* v1 = new VerticalLayout;
	v1->set_name("vlayout");
	v1->add(b3);
	v1->add(b4);

//	HorizontalLayout* h2 = new HorizontalLayout;
//	h2->add(b3);
//	h2->add(b4);

	h1->add(v1);

	h1->set_pos(500, 200);

//	v1->resize(200, 60);

	app->bind(h1);

	Button* b5 = new Button;
//
	v1->add(b5);
//
//	Button* b6 = new Button;
//
//	v1->add(b6);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	obj.unbind();

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	return 0;
}

