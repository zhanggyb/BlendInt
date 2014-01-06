/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
/* ADD GLOBAL VARIABLES HERE LATER */

#include <iostream>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/Button.hpp>
#include <BlendInt/ScrollView.hpp>

using namespace BlendInt;

int init_resources(void)
{
	Interface::Initialize();
	Interface* app = Interface::Instance();
	app->Resize(1200, 800);

    Widget* widget = new Widget;
    widget->SetPosition(200, 200);
    widget->Register();
	
	return 1;
}

void onDisplay()
{
	Interface::Instance()->Draw();
	
	glutSwapBuffers();
}

void onIdle() {
	glutPostRedisplay();
}

void free_resources()
{
	Interface::Release();
}

void onReshape (int width, int height)
{
	Interface::Instance()->Resize(width, height);
}

void onKey (unsigned char key, int x, int y)
{
    std::cout << "key pressed: " << key << std::endl;
}

void onMouse (int button, int state, int x, int y)
{
}

void onMotion (int x, int y)
{
    std::cout << "position: " << x << " " << y << std::endl;
}

void onPassiveMotion (int x, int y)
{
    std::cout << "position: " << x << " " << y << std::endl;
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
        glutKeyboardFunc (onKey);
        glutMouseFunc(onMouse);
        glutMotionFunc(onMotion);
        glutPassiveMotionFunc(onPassiveMotion);
		glutIdleFunc(onIdle);
		glutMainLoop();
	}
	
	/* If the program exits in the usual way,
	 * free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}

