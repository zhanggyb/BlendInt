/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
/* ADD GLOBAL VARIABLES HERE LATER */

int init_resources(void)
{
	/* FILLED IN LATER */
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, 1200, 0.f, 800, 100.f, -100.f);
	return 1;
}

void onDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* FILLED IN LATER */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glTranslatef(500,
				 200,
			  0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor3ub(255, 200, 133);
	
	glBegin(GL_TRIANGLES);
	glVertex2f (200, 200);
	glVertex2f (400, 200);
	glVertex2f (300, 300);
	glEnd();
	
	glDisable(GL_BLEND);
	
	glPopMatrix();
	
	glutSwapBuffers();
}

void onIdle() {
	glutPostRedisplay();
}

void free_resources()
{
	/* FILLED IN LATER */
}

int main(int argc, char* argv[])
{
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("My First Triangle");
	
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}
	
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}
	
	/* When all init functions run without errors,
	 * the program can initialise the resources */
	if (1 == init_resources())
	{
		/* We can display it if everything goes OK */
		glutDisplayFunc(onDisplay);
		glutIdleFunc(onIdle);
		glutMainLoop();
	}
	
	/* If the program exits in the usual way,
	 * free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}