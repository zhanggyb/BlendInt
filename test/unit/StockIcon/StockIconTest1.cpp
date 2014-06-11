#include "StockIconTest1.hpp"
#include <BlendInt/Stock/Icons.hpp>

using namespace BlendInt;

StockIconTest1::StockIconTest1()
: testing::Test()
{
	// TODO: add constructor code
}

StockIconTest1::~StockIconTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(StockIconTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("StockIcon - Foo1");

	// TODO: add test code here

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		Interface::instance->Draw();

#ifdef DEBUG
        /*
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
        */
#endif

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(StockIconTest1, Foo2)
{
	Init ();
	GLFWwindow* window = CreateWindow("StockIcon - Foo2");

	// TODO: add test code here

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		Interface::instance->Draw();

#ifdef DEBUG
        /*
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
        */
#endif

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Terminate();

	ASSERT_TRUE(true);
}
