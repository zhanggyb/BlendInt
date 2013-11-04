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
		m_layout.set_position(300, 400);
		//m_hlayout.resize(400, 100);
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
		interface()->bind(&m_layout);
	}

	void unbind()
	{
		interface()->unbind(&m_layout);
	}

	void add_button()
	{
		Button* button = new Button;

		//char str[20];
		//sprintf(str, "%s%u", "button", static_cast<unsigned int>(m_buttons.size()));

		//button->set_text(str);
		//button->set_name(str);

		m_layout.add(button);
		m_buttons.push_back(button);
	}

	void remove_button()
	{
		if(m_buttons.size()) {
			Button* button = m_buttons[m_buttons.size() - 1];
			m_layout.erase(button);
			m_buttons.pop_back();
			m_layout.resize(m_layout.preferred_size());
		}
	}

private:

//	VerticalLayout *m_vlayout;

	HorizontalLayout m_layout;
//	VerticalLayout m_layout;

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
	add_button->set_position(450, 600);

	obj.connect_add(add_button);

	Button* remove_button = new Button;
	remove_button->set_text("Remove Button");
	remove_button->set_position(550, 600);

	obj.connect_remove(remove_button);

	app->bind(add_button);
	app->bind(remove_button);

	HorizontalLayout* layout = new HorizontalLayout;

	layout->set_position(100, 100);

	Button* b1 = new Button;
	Button* b2 = new Button;
	Button* b3 = new Button;
	Button* b4 = new Button;

	layout->add(b1);
	layout->add(b2);
	layout->add(b3);
	layout->add(b4);

	app->bind(layout);

	Label* label1 = new Label("Hello World");

	label1->set_position(300, 50);
	label1->resize (200, 80);

	app->bind(label1);

	Label* label2 = new Label("yes, google");
	label2->resize (200, 80);

	label2->set_position(label1->position().x() + label1->size().width(), 50);

	app->bind(label2);

	label1->set_font(Font("Droid Sans", 24));
	label2->set_font(Font("Droid Sans", 24));

//	layout->add(label1);
//	layout->add(label2);

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

