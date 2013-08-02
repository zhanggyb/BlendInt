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

myDrawable::myDrawable (BasicObject *parent)
    : Drawable(parent)
{
    resize (400, 400);
    setPos (Coord2i(20, 20));

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

myDrawable::~myDrawable()
{

}

void myDrawable::render (void)
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

void myDrawable::makeCheckImage (void)
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
    //widget.initialize();
    app.initialize();

    myDrawable widget;
    widget.setParent (&win);

    app.run();

    CPPUNIT_ASSERT(true);
}
