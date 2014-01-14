/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include <iostream>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Label.hpp>
#include <BlendInt/Button.hpp>
#include <BlendInt/ScrollView.hpp>
#include <BlendInt/HorizontalLayout.hpp>

using namespace BlendInt;

static KeyEvent global_key_event;
static MouseEvent global_mouse_event;

int init_resources(void)
{
	Interface::Initialize();
	Interface* app = Interface::Instance();
	app->Resize(1200, 800);

	int max_x = 12;
	int max_y = 20;
	HorizontalLayout* layout[max_y];
	Button* btn[max_x];
	int x_pos = 50;
	int y_pos = 5;
	for(int i = 0; i < max_y; i++)
	{
		layout[i] = new HorizontalLayout;
		layout[i]->Register();

		for(int j = 0; j < max_x; j++)
		{
			btn[j] = new Button;
			layout[i]->Add(btn[j]);
		}

		layout[i]->SetPosition(x_pos, y_pos);

		y_pos += 40;
	}
	
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
    std::cout << "key pressed: " << key << " " << x << " " << y << std::endl;
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
    global_mouse_event.set_action(MouseNone);
    global_mouse_event.set_button(MouseButtonNone);

    global_mouse_event.set_position(x, Interface::Instance()->size().height() - y);

    Interface::Instance()->DispatchMouseEvent(&global_mouse_event);
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

