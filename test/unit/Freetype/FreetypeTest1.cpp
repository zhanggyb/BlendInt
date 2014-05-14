#include "FreetypeTest1.hpp"
#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/Gui/Glyph.hpp>

using namespace BlendInt;

FreetypeTest1::FreetypeTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FreetypeTest1::~FreetypeTest1()
{
	// TODO: add destructor code
}

TEST_F(FreetypeTest1, Library1)
{
	FTLibrary lib;
	FTFace face;
	FTStroker stroker;

	lib.Initialize();
	lib.SetLcdFilter(FT_LCD_FILTER_DEFAULT);

	face.New(lib, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

	stroker.New(lib);

	stroker.Done();
	face.Done();
	lib.Done();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
/*
TEST_F(FreetypeTest1, Open01) {

	Freetype test;

	//test.Open("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12, 96);
	test.Open("/home/zhanggyb/.fonts/s/STKaiti.ttf", 12, 96);

	fprintf(stdout, "height: %d, max advance: %d\n", test.height(), test.max_advance());

	fprintf(stdout, "sizeof Glyph: %ld, size of GlyphExt: %ld\n", sizeof(Glyph), sizeof(GlyphExt));

	ASSERT_TRUE(test.valid());
}
*/

/*
TEST_F(FreetypeTest1, Kerning02) {

	Freetype test;

	test.Open("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12, 96);

	if(test.IsUseKerning()) {

		fprintf(stdout, "font is using kerning\n");
		FT_UInt left;
		FT_UInt right;
		FT_Vector kerning = {0, 0};

		for(char lc = 'a'; lc < 'z'; lc++) {

			for(char rc = 'a'; rc < 'z'; rc++) {

				left = test.GetCharIndex(lc);
				right = test.GetCharIndex(rc);

				kerning = test.GetKerning(left, right);

				if(kerning.x > 0) {
					fprintf (stdout, "Kerning of %c and %c is %ld\n", lc, rc, kerning.x);
				}
			}

		}

	}

	ASSERT_TRUE(test.valid());
}
*/
