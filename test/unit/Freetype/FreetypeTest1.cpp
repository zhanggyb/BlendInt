#include "FreetypeTest1.hpp"
#include <BlendInt/Core/Freetype.hpp>

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

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(FreetypeTest1, Open01) {

	Freetype test;

	test.Open("/usr/share/fonts/TTF/DejaVuSans.ttf", 12, 96);

	ASSERT_TRUE(test.valid());
}

TEST_F(FreetypeTest1, Kerning02) {

	Freetype test;

	test.Open("/usr/share/fonts/TTF/Vera.ttf", 12, 96);

	if(test.isUseKerning()) {

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
