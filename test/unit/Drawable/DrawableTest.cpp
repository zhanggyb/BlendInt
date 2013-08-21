// cpp

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include <BIL/Drawable.hpp>
#include "DrawableTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (DrawableTest);

myTexture::myTexture (Traceable *parent)
    : Drawable(parent)
{
    resize (400, 400);
    set_pos (Point(20, 20));

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

void myTexture::Render (void)
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

void myTexture::ContextMenuPressEvent (ContextMenuEvent* event)
{

}

void myTexture::ContextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myTexture::MousePressEvent (MouseEvent* event)
{

}
	
void myTexture::MouseReleaseEvent (MouseEvent* event)
{

}

void myTexture::MouseMoveEvent (MouseEvent* event)
{

}

void myTexture::InputMethodEvent (unsigned int character)
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
	round_box_type_ = RoundBoxAll;
}

myDrawable1::~myDrawable1()
{

}

void myDrawable1::Render(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		DrawBox(GL_POLYGON,
				pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0);

	glDisable(GL_BLEND);
}

void myDrawable1::KeyPressEvent (KeyEvent* event)
{
}

void myDrawable1::ContextMenuPressEvent (ContextMenuEvent* event)
{

}

void myDrawable1::ContextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myDrawable1::MousePressEvent (MouseEvent* event)
{
}
	
void myDrawable1::MouseReleaseEvent (MouseEvent* event)
{

}

void myDrawable1::MouseMoveEvent (MouseEvent* event)
{

}

void myDrawable1::InputMethodEvent (unsigned int character)
{
}

void myDrawable1::CursorEnterEvent (int entered)
{
}

myDrawable2::myDrawable2(Traceable* parent)
	: Drawable (parent)
{
	round_box_type_ = RoundBoxAll;
}

myDrawable2::~myDrawable2()
{

}

void myDrawable2::Render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		DrawBoxShade(GL_POLYGON,
				pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0,
				0.5, 0.1);

	glDisable(GL_BLEND);
}

void myDrawable2::KeyPressEvent (KeyEvent* event)
{
}

void myDrawable2::ContextMenuPressEvent (ContextMenuEvent* event)
{

}

void myDrawable2::ContextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myDrawable2::MousePressEvent (MouseEvent* event)
{
}
	
void myDrawable2::MouseReleaseEvent (MouseEvent* event)
{
}

void myDrawable2::MouseMoveEvent (MouseEvent* event)
{

}

void myDrawable2::InputMethodEvent (unsigned int character)
{
}

void myDrawable2::CursorEnterEvent (int entered)
{
}

myDrawable3::myDrawable3(Traceable* parent)
	: Drawable (parent)
{
	round_box_type_ = RoundBoxAll;
}

myDrawable3::~myDrawable3()
{

}

void myDrawable3::Render(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 0.9f, 1.0f, 0.95f);

		drawRoundBox(pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0);
		DrawShadowBox(pos_.x(),
				pos_.y(),
				pos_.x() + size_.width(),
				pos_.y() + size_.height(),
				5.0,
				64);

	glDisable(GL_BLEND);
}


void myDrawable3::KeyPressEvent (KeyEvent* event)
{
}

void myDrawable3::ContextMenuPressEvent (ContextMenuEvent* event)
{

}

void myDrawable3::ContextMenuReleaseEvent (ContextMenuEvent* event)
{

}

void myDrawable3::MousePressEvent (MouseEvent* event)
{

}
	
void myDrawable3::MouseReleaseEvent (MouseEvent* event)
{

}

void myDrawable3::MouseMoveEvent (MouseEvent* event)
{

}

void myDrawable3::InputMethodEvent (unsigned int character)
{
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
    widget.set_parent (&win);

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
    widget.set_parent (&win);
    widget.set_pos(Point(50, 50));
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
    widget.set_parent (&win);
    widget.set_pos(Point(50, 50));
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
    widget.set_parent (&win);
    widget.set_pos(Point(50, 50));
    widget.resize(80, 40);

    app.run();

    CPPUNIT_ASSERT(true);
}


