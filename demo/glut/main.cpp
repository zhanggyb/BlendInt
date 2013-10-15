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

int init_resources(void)
{
	using BlendInt::Interface;
	using BlendInt::Label;
	
	Interface::initialize();
	Interface* app = Interface::instance();
	app->resize(1200, 800);
	
	// 	glMatrixMode(GL_PROJECTION);
	// 	glLoadIdentity();
	// 	glOrtho(0.f, 1200, 0.f, 800, 100.f, -100.f);
	
	Label* label = new Label("Hello World!");
	label->set_pos (200, 200);
		
	app->bind(label);
	
	return 1;
}

void onDisplay()
{
	using BlendInt::Interface;
	
	Interface::instance()->render();
	
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
	
	Interface::instance()->resize(width, height);
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
