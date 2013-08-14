// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include "DrawableTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (DrawableTest);

myTexture::myTexture (Traceable *parent)
    : Drawable(parent)
{
    resize (400, 400);
    setPos (Coord2f(20.0, 20.0));

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glShadeModel (GL_FLAT);
    glEnable (GL_DEPTH_TEST);
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &_texName);
    glBindTexture(GL_TEXTURE_2D, _texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                 checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _checkImage);

}

myTexture::~myTexture()
{

}

void myTexture::render (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, _texName);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f (100.0, 100, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f (400.0, 100, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f (400.0, 400.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f (100.0, 400.0, 0.0);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void myTexture::KeyPressEvent (KeyEvent* event)
{
}

void myTexture::MousePressEvent (MouseEvent* event)
{

}
	
void myTexture::MouseReleaseEvent (MouseEvent* event)
{

}

void myTexture::CharEvent (unsigned int character)
{
}

void myTexture::MouseButtonEvent (int button, int action, int modes)
{
}

void myTexture::CursorPosEvent (double xpos, double ypos)
{
}

void myTexture::CursorEnterEvent (int entered)
{
}

void myTexture::makeCheckImage (void)
{
    int i, j, c;
    for(i = 0; i < checkImageHeight; i++) {
        for(j = 0; j < checkImageWidth; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8)) == 0) * 255;
            _checkImage[i][j][0] = (GLubyte)c;
            _checkImage[i][j][1] = (GLubyte)c;
            _checkImage[i][j][2] = (GLubyte)c;
            _checkImage[i][j][3] = (GLubyte)255;
        }
    }
}

myDrawable1::myDrawable1(Traceable* parent)
	: Drawable (parent)
{
	corner_ = CORNER_ALL;
}

myDrawable1::~myDrawable1()
{

}

void myDrawable1::render(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		drawBox(GL_POLYGON,
				pos_.coord.x,
				pos_.coord.y,
				pos_.coord.x + size_.vec.x,
				pos_.coord.y + size_.vec.y,
				5.0);

	glDisable(GL_BLEND);
}

void myDrawable1::KeyPressEvent (KeyEvent* event)
{
}

void myDrawable1::MousePressEvent (MouseEvent* event)
{
}
	
void myDrawable1::MouseReleaseEvent (MouseEvent* event)
{

}

void myDrawable1::CharEvent (unsigned int character)
{
}

void myDrawable1::MouseButtonEvent (int button, int action, int modes)
{
}

void myDrawable1::CursorPosEvent (double xpos, double ypos)
{
}

void myDrawable1::CursorEnterEvent (int entered)
{
}

myDrawable2::myDrawable2(Traceable* parent)
	: Drawable (parent)
{
	corner_ = CORNER_ALL;
}

myDrawable2::~myDrawable2()
{

}

void myDrawable2::render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		drawBoxShade(GL_POLYGON,
				pos_.coord.x,
				pos_.coord.y,
				pos_.coord.x + size_.vec.x,
				pos_.coord.y + size_.vec.y,
				5.0,
				0.5, 0.1);

	glDisable(GL_BLEND);
}

void myDrawable2::KeyPressEvent (KeyEvent* event)
{
}

void myDrawable2::MousePressEvent (MouseEvent* event)
{
}
	
void myDrawable2::MouseReleaseEvent (MouseEvent* event)
{
}

void myDrawable2::CharEvent (unsigned int character)
{
}

void myDrawable2::MouseButtonEvent (int button, int action, int modes)
{
}

void myDrawable2::CursorPosEvent (double xpos, double ypos)
{
}

void myDrawable2::CursorEnterEvent (int entered)
{
}

myDrawable3::myDrawable3(Traceable* parent)
	: Drawable (parent)
{
	corner_ = CORNER_ALL;
}

myDrawable3::~myDrawable3()
{

}

void myDrawable3::render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		drawRoundBox(pos_.coord.x,
				pos_.coord.y,
				pos_.coord.x + size_.vec.x,
				pos_.coord.y + size_.vec.y,
				5.0);
		drawShadowBox(pos_.coord.x,
				pos_.coord.y,
				pos_.coord.x + size_.vec.x,
				pos_.coord.y + size_.vec.y,
				5.0,
				64);

	glDisable(GL_BLEND);
}


void myDrawable3::KeyPressEvent (KeyEvent* event)
{
}

void myDrawable3::MousePressEvent (MouseEvent* event)
{

}
	
void myDrawable3::MouseReleaseEvent (MouseEvent* event)
{

}

void myDrawable3::CharEvent (unsigned int character)
{
}

void myDrawable3::MouseButtonEvent (int button, int action, int modes)
{
}

void myDrawable3::CursorPosEvent (double xpos, double ypos)
{
	cout << "Cursor Position: " << xpos << " " << ypos << endl;
}

void myDrawable3::CursorEnterEvent (int entered)
{
}

DrawableTest::DrawableTest ()
{

}

DrawableTest::~DrawableTest ()
{

}

void DrawableTest::setUp ()
{

}

void DrawableTest::tearDown ()
{

}

void DrawableTest::texture1 ()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    myTexture widget;
    widget.setParent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}

void DrawableTest::mydrawable1()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    myDrawable1 widget;
    widget.setParent (&win);
    widget.setPos(Coord2f(50.0, 50.0));
    widget.resize(80, 40);

    app.run();

    CPPUNIT_ASSERT(true);
}

void DrawableTest::mydrawable2()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    myDrawable2 widget;
    widget.setParent (&win);
    widget.setPos(Coord2f(50.0, 50.0));
    widget.resize(80, 40);

    app.run();

    CPPUNIT_ASSERT(true);
}

void DrawableTest::mydrawable3()
{
    Application app;

    Window win(640, 480, "640 x 480 Window, test drawable3", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    myDrawable3 widget;
    widget.setParent (&win);
    widget.setPos(Coord2f(50.0, 50.0));
    widget.resize(80, 40);

    app.run();

    CPPUNIT_ASSERT(true);
}


