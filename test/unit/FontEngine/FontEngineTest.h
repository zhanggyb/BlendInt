/* Test case for FontFace */

#ifndef _FONTENGINE_TEST_H
#define _FONTENGINE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

#define WIDTH 640
#define HEIGHT 480

using namespace std;

class FontEngineTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontEngineTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(create2);
    CPPUNIT_TEST(create3);
    CPPUNIT_TEST(create4);
    CPPUNIT_TEST(create5);
    CPPUNIT_TEST(create6);
    CPPUNIT_TEST(get_glyph1);
    CPPUNIT_TEST(get_glyph2);
    CPPUNIT_TEST(get_glyph3);
    CPPUNIT_TEST(get_glyph4);
    CPPUNIT_TEST(get_glyph5);
    CPPUNIT_TEST(glyph_metrics1);
    CPPUNIT_TEST_SUITE_END();

 public:

    FontEngineTest();

    virtual ~FontEngineTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    /* Test case */
    void create1 ();
    void create2 ();
    void create3 ();
    void create4 ();
    void create5 ();
    void create6 ();
    void get_glyph1 ();
    void get_glyph2 ();
    void get_glyph3 ();
    void get_glyph4 ();
    void get_glyph5 ();         /* test new Face from memory */
    void glyph_metrics1 ();

    /* draw bitmap */
    void draw_bitmap (FT_Bitmap* bitmap, FT_Int x, FT_Int y);

    void show_image(void);

    unsigned char image[HEIGHT][WIDTH];
};

#endif  /* _FONTENGINE_TEST_H */
