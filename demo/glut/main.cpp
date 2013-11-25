/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
/* ADD GLOBAL VARIABLES HERE LATER */

#include <BlendInt/Interface.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/Button.hpp>
#include <BlendInt/ScrollView.hpp>

int init_resources(void)
{
	using BlendInt::Interface;
	using namespace BlendInt;
	
	Interface::initialize();
	Interface* app = Interface::instance();
	app->Resize(1200, 800);
	
	// 	glMatrixMode(GL_PROJECTION);
	// 	glLoadIdentity();
	// 	glOrtho(0.f, 1200, 0.f, 800, 100.f, -100.f);

	Button* add_button = new Button;
	add_button->set_text("Add Button");
	add_button->SetPosition(600, 700);

	Button* remove_button = new Button;
	remove_button->set_text("Remove Button");
	remove_button->SetPosition(600, 650);

	app->bind(add_button);
	app->bind(remove_button);

	// -----------------------

	Button* reset_button = new Button;
	reset_button->set_text("Reset");
	reset_button->Move(640, 300);

	app->bind(reset_button);

	ScrollView* scroll_view = new ScrollView;

	scroll_view->SetPosition(200, 200);
	scroll_view->set_orientation(2);
	scroll_view->Resize(400, 400);

	Button* button = new Button;
	button->set_text("Hello World!");
//	button->Resize(80, 600);
	button->Move(205, 205);

	scroll_view->set_viewport(button);

	app->bind(scroll_view);

	Label* label = new Label("Hello World!");

	label->SetPosition(50, 50);

	label->set_text ("alsdkjflasdjflasfnvlkasefage");
	label->Resize(80, 40);
	
	app->bind(label);
	
	return 1;
}

void onDisplay()
{
	using BlendInt::Interface;
	
	Interface::instance()->Render();
	
	/* FILLED IN LATER */
// 	glMatrixMode(GL_MODELVIEW);
// 	glPushMatrix();
// 	
// 	glTranslatef(500,
// 				 200,
// 			  0);
// 	
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	
// 	glColor3ub(255, 200, 133);
// 	
// 	glBegin(GL_TRIANGLES);
// 	glVertex2f (200, 200);
// 	glVertex2f (400, 200);
// 	glVertex2f (300, 300);
// 	glEnd();
// 	
// 	glDisable(GL_BLEND);
// 	
// 	glPopMatrix();
	
	glutSwapBuffers();
}

void onIdle() {
	glutPostRedisplay();
}

void free_resources()
{
	using BlendInt::Interface;
	
	Interface::release();
}

void onReshape (int width, int height)
{
	using BlendInt::Interface;
	
	Interface::instance()->Resize(width, height);
}

void onMouse (int button, int state, int x, int y)
{
	using BlendInt::Interface;
	
	//std::cout << "mouse position: "
	//BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;
	
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("GLUT Demo");

	/* When all init functions run without errors,
	 * the program can initialise the resources */
	if (1 == init_resources())
	{
		/* We can display it if everything goes OK */
		glutDisplayFunc(onDisplay);
		glutReshapeFunc(onReshape);
		glutIdleFunc(onIdle);
		glutMainLoop();
	}
	
	/* If the program exits in the usual way,
	 * free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
