/* Test case for FontFace */

#ifndef _DRAWABLE_TEST_H
#define _DRAWABLE_TEST_H

#include <BIL/Application.h>
#include <BIL/Window.h>
#include <BIL/Drawable.h>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace std;

class myDrawable: public Drawable
{
 public:

    myDrawable (BasicObject *parent = NULL);

    virtual ~myDrawable ();

 protected:

    void render (void);

 private:
    void makeCheckImage(void);

    static const int checkImageWidth = 64;
    static const int checkImageHeight = 64;

    GLuint _texName;
    GLubyte _checkImage[checkImageHeight][checkImageWidth][4];
};


class DrawableTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(DrawableTest);

    CPPUNIT_TEST(texture1);

    CPPUNIT_TEST_SUITE_END();

 public:

    DrawableTest();

    virtual ~DrawableTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void texture1 ();
};

#endif  /* _DRAWABLE_TEST_H */
