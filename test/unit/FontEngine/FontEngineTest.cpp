// cpp

#include <iostream>
#include <string>
#include <stdio.h>
#include <wchar.h>

#include <BIL/Freetype.hpp>
#include <BIL/FontConfig.hpp>

#include <cppunit/TestAssert.h>

#include "FontEngineTest.h"

using namespace BILO;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontEngineTest);

FontEngineTest::FontEngineTest ()
{

}

FontEngineTest::~FontEngineTest ()
{

}

void FontEngineTest::setUp ()
{
	if(!FontConfig::initialize()) {
		CPPUNIT_FAIL("Cannot initialize FontConfig service\n");
	}
}

void FontEngineTest::tearDown ()
{
	FontConfig::release();
}

void FontEngineTest::draw_bitmap (FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;

	for (i = x, p = 0; i < x_max; i++, p++) {
		for (j = y, q = 0; j < y_max; j++, q++) {
			if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
				continue;

			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void FontEngineTest::show_image (void)
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++)
			putchar(image[i][j] == 0 ? ' ' : image[i][j] < 128 ? '+' : '*');
		putchar('\n');
	}
}

void FontEngineTest::create1 ()
{
	bool result;

	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::create2 ()
{
	bool result1, result2;

	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font1 = new Freetype;
	font1->open(fontpath);
	Freetype *font2 = new Freetype;
	font2->open(fontpath);

	result1 = font1->valid();
	result2 = font2->valid();

	delete font1;
	font1 = NULL;
	delete font2;
	font2 = NULL;

	CPPUNIT_ASSERT(result1 && result2);
}

void FontEngineTest::create3 ()
{
	bool result1 = false;

	FontConfig* gFontService = FontConfig::instance();

	Freetype *font1 = new Freetype;
	font1->open(gFontService->getBuffer(),
									   gFontService->getBufferSize());

	result1 = font1->valid();

	delete font1;
	font1 = NULL;

	CPPUNIT_ASSERT(result1);
}

void FontEngineTest::create4 ()
{
	bool result = false;
	FT_UInt index = 0;

	Freetype font;
	font.open(Font("Sans"), 96);

	result = font.valid();

	if(result) {
		index = font.getCharIndex(L'A');
		cout << "glyph index: " << index << endl;
	}

	CPPUNIT_ASSERT(result && index > 0);
}

void FontEngineTest::create5 ()
{
	bool result;

	string fontpath = "/usr/share/fonts/TTF/DejaVuSerif-Bold.ttf";

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if(result) {

		wcout << endl << font->font().family << endl;
		if(font->font().bold) {
			cout << "Bold" << endl;
		} else {
			cout << "Regular" << endl;
		}

		if(font->font().italic) {
			cout << "Italic" << endl;
		} else {
			cout << "Normal" << endl;
		}

		cout << "Global height: " << font->height() << endl;
		cout << "Global ascender: " << font->ascender() << endl;
		cout << "Global descender: " << font->descender() << endl;
		cout << "Global max Advance: " << font->max_advance() << endl;

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::create6 ()
{
	bool result = false;
	int index = 0;

	Font font("Sans", 16);

	Freetype *fe = new Freetype;
	fe->open(font, 96);

	result = fe->valid();

	if(result) {

		wcout << endl << fe->font().family << endl;
		if(fe->font().bold) {
			cout << "Bold" << endl;
		} else {
			cout << "Regular" << endl;
		}

		if(fe->font().italic) {
			cout << "Italic" << endl;
		} else {
			cout << "Normal" << endl;
		}

		cout << "Font Size: " << fe->font().size << endl;

		index = fe->getCharIndex('A');
		cout << "CharIndex: " << index << endl;

	}

	delete fe;
	fe = NULL;

	CPPUNIT_ASSERT(result && (index != 0));
}

void FontEngineTest::checkindex1 ()
{
	bool result = false;
	int indexEn = 0; int indexCh = 0;

	Freetype *fe = new Freetype;
	fe->open(
			std::string("/usr/share/fonts/TTF/DejaVuSans.ttf"), 16);

	result = fe->valid();

	if(result) {

		wcout << endl << fe->font().family << endl;
		if(fe->font().bold) {
			cout << "Bold" << endl;
		} else {
			cout << "Regular" << endl;
		}

		if(fe->font().italic) {
			cout << "Italic" << endl;
		} else {
			cout << "Normal" << endl;
		}

		cout << "Font Size: " << fe->font().size << endl;

		indexEn = fe->getCharIndex('A');
		cout << "CharIndex of English character: " << indexEn << endl;

		indexCh = fe->getCharIndex(L'仁');
		cout << "CharIndex of CJK character: " << indexCh << endl;

	}

	delete fe;
	fe = NULL;

	CPPUNIT_ASSERT(result && (indexEn != 0) && (indexCh == 0));
}

void FontEngineTest::checkindex2 ()
{
	bool result = false;
	int indexEn = 0; int indexCh = 0;

	Freetype *fe = new Freetype;
	fe->open(
			std::string("/usr/share/fonts/TTF/DroidSansFallback.ttf"), 16);

	result = fe->valid();

	if(result) {

		wcout << endl << fe->font().family << endl;
		if(fe->font().bold) {
			cout << "Bold" << endl;
		} else {
			cout << "Regular" << endl;
		}

		if(fe->font().italic) {
			cout << "Italic" << endl;
		} else {
			cout << "Normal" << endl;
		}

		cout << "Font Size: " << fe->font().size << endl;

		indexEn = fe->getCharIndex('A');
		cout << "CharIndex of English character: " << indexEn << endl;

		indexCh = fe->getCharIndex(L'仁');
		cout << "CharIndex of CJK character: " << indexCh << endl;

	}

	delete fe;
	fe = NULL;

	CPPUNIT_ASSERT(result && (indexEn != 0) && (indexCh != 0));
}

void FontEngineTest::checkindex3 ()
{
	bool result = false;
	int indexEn = 0; int indexCh = 0;

	Freetype *fe = new Freetype;
	fe->open(
			Font("Sans", 16));

	result = fe->valid();

	if(result) {

		wcout << endl << fe->font().family << endl;
		if(fe->font().bold) {
			cout << "Bold" << endl;
		} else {
			cout << "Regular" << endl;
		}

		if(fe->font().italic) {
			cout << "Italic" << endl;
		} else {
			cout << "Normal" << endl;
		}

		cout << "Font Size: " << fe->font().size << endl;

		indexEn = fe->getCharIndex('A');
		cout << "CharIndex of English character: " << indexEn << endl;

		indexCh = fe->getCharIndex(L'仁');
		cout << "CharIndex of CJK character: " << indexCh << endl;

	}

	delete fe;
	fe = NULL;

	CPPUNIT_ASSERT(result && (indexEn != 0) && (indexCh != 0));
}

void FontEngineTest::checkindex4 ()
{
	bool result = false;
	int indexEn = 0; int indexCh = 0;

	Freetype *fe = new Freetype;
	fe->open(
			Font("Droid Sans", 16));

	result = fe->valid();

	if(result) {

		wcout << endl << fe->font().family << endl;
		if(fe->font().bold) {
			cout << "Bold" << endl;
		} else {
			cout << "Regular" << endl;
		}

		if(fe->font().italic) {
			cout << "Italic" << endl;
		} else {
			cout << "Normal" << endl;
		}

		cout << "Font Size: " << fe->font().size << endl;

		indexEn = fe->getCharIndex('A');
		cout << "CharIndex of English character: " << indexEn << endl;

		indexCh = fe->getCharIndex(L'仁');
		cout << "CharIndex of CJK character: " << indexCh << endl;

	}

	delete fe;
	fe = NULL;

	CPPUNIT_ASSERT(result && (indexEn != 0) && (indexCh == 0));
}


void FontEngineTest::get_glyph1 ()
{
	bool result;

	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	wchar_t ch = L'A';

	result = font->setCharSize(16, 72);

	if (result) {

		result = font->loadCharacter(ch, FT_LOAD_RENDER);

		if (result) {
			FT_Face face = font->getFontFace();
			cout << endl << "num_faces: " << face->num_faces << endl
				 << "face_index: " << face->face_index << endl
				 << "num_glyphs: " << face->num_glyphs << endl << "slot: "
				 << endl << "       " << "format: " << face->glyph->format
				 << endl << "       " << "bitmap: " << endl
				 << "               " << "rows: " << face->glyph->bitmap.rows
				 << endl << "               " << "width: "
				 << face->glyph->bitmap.width << endl << "               "
				 << "pitch: " << face->glyph->bitmap.pitch << endl
				 << "               " << "num_grays: "
				 << face->glyph->bitmap.num_grays << endl;

			// int rows = face->glyph->bitmap.rows;
			// int width = face->glyph->bitmap.width;

			/*
			  int i, j;
			  unsigned char charcode;
			  for (i = 0; i < rows; i++) {
			  for (j = 0; j < width; j++) {
			  charcode = *(face->glyph->bitmap.buffer + i * width + j);
			  printf("0x%2x ", charcode);
			  }
			  cout << endl;
			  }
			  cout << endl;


			  for (i = 0; i < rows; i++) {
			  for (j = 0; j < width; j++) {
			  charcode = *(face->glyph->bitmap.buffer + i * width + j);
			  putchar(charcode < 128 ? ' ' : '*');
			  }
			  cout << endl;
			  }
			  cout << endl;
			*/
		}

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::get_glyph2 ()
{
	bool result;

	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	wchar_t ch = L'仁';

	result = font->setCharSize(24, 96);

	if (result) {

		result = font->loadCharacter(ch, FT_LOAD_RENDER);

		if (result) {
			FT_Face face = font->getFontFace();
			cout << endl << "num_faces: " << face->num_faces << endl
				 << "face_index: " << face->face_index << endl
				 << "num_glyphs: " << face->num_glyphs << endl << "slot: "
				 << endl << "       " << "format: " << face->glyph->format
				 << endl << "       " << "bitmap: " << endl
				 << "               " << "rows: " << face->glyph->bitmap.rows
				 << endl << "               " << "width: "
				 << face->glyph->bitmap.width << endl << "               "
				 << "pitch: " << face->glyph->bitmap.pitch << endl
				 << "               " << "num_grays: "
				 << face->glyph->bitmap.num_grays << endl;

			// int rows = face->glyph->bitmap.rows;
			// int width = face->glyph->bitmap.width;

			/*
			  int i, j;
			  unsigned char charcode;
			  for (i = 0; i < rows; i++) {
			  for (j = 0; j < width; j++) {
			  charcode = *(face->glyph->bitmap.buffer + i * width + j);
			  printf("0x%2x ", charcode);
			  }
			  cout << endl;
			  }
			  cout << endl;

			  for (i = 0; i < rows; i++) {
			  for (j = 0; j < width; j++) {
			  charcode = *(face->glyph->bitmap.buffer + i * width + j);
			  putchar(charcode < 128 ? ' ' : '*');
			  }
			  cout << endl;
			  }
			  cout << endl;
			*/
		}

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::get_glyph3 ()
{
	bool result;

	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	wchar_t ch = L'义';

	result = font->setCharSize(24, 96);

	if (result) {

		result = font->loadCharacter(ch, FT_LOAD_RENDER);

		if (result) {
			FT_Face face = font->getFontFace();
			cout << endl << "num_faces: " << face->num_faces << endl
				 << "face_index: " << face->face_index << endl
				 << "num_glyphs: " << face->num_glyphs << endl << "slot: "
				 << endl << "       " << "format: " << face->glyph->format
				 << endl << "       " << "bitmap: " << endl
				 << "               " << "rows: " << face->glyph->bitmap.rows
				 << endl << "               " << "width: "
				 << face->glyph->bitmap.width << endl << "               "
				 << "pitch: " << face->glyph->bitmap.pitch << endl
				 << "               " << "num_grays: "
				 << face->glyph->bitmap.num_grays << endl;

			// int rows = face->glyph->bitmap.rows;
			// int width = face->glyph->bitmap.width;

			/*
			  int i, j;
			  unsigned char charcode;
			  for (i = 0; i < rows; i++) {
			  for (j = 0; j < width; j++) {
			  charcode = *(face->glyph->bitmap.buffer + i * width + j);
			  printf("0x%2x ", charcode);
			  }
			  cout << endl;
			  }
			  cout << endl;

			  for (i = 0; i < rows; i++) {
			  for (j = 0; j < width; j++) {
			  charcode = *(face->glyph->bitmap.buffer + i * width + j);
			  putchar(charcode < 128 ? ' ' : '*');
			  }
			  cout << endl;
			  }
			  cout << endl;
			*/
		}

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::get_glyph4 ()
{
	bool result;

	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	wchar_t ch = L'礼';

	result = font->setCharSize(24, 96);

	if (result) {

		FT_UInt glyph_index = font->getCharIndex(ch);

		result = font->loadGlyph(glyph_index);

		CPPUNIT_ASSERT(result != 0);

		if (result) {

			result = font->renderGlyph(FT_RENDER_MODE_NORMAL);

			if (result) {
				FT_Face face = font->getFontFace();
				cout << endl << "num_faces: " << face->num_faces << endl
					 << "face_index: " << face->face_index << endl
					 << "num_glyphs: " << face->num_glyphs << endl
					 << "slot: " << endl << "       " << "format: "
					 << face->glyph->format << endl << "       "
					 << "bitmap: " << endl << "               " << "rows: "
					 << face->glyph->bitmap.rows << endl << "               "
					 << "width: " << face->glyph->bitmap.width << endl
					 << "               " << "pitch: "
					 << face->glyph->bitmap.pitch << endl
					 << "               " << "num_grays: "
					 << face->glyph->bitmap.num_grays << endl;

				// int rows = face->glyph->bitmap.rows;
				// int width = face->glyph->bitmap.width;

				/*
				  int i, j;
				  unsigned char charcode;
				  for (i = 0; i < rows; i++) {
				  for (j = 0; j < width; j++) {
				  charcode =
				  *(face->glyph->bitmap.buffer + i * width + j);
				  printf("0x%2x ", charcode);
				  }
				  cout << endl;
				  }
				  cout << endl;

				  for (i = 0; i < rows; i++) {
				  for (j = 0; j < width; j++) {
				  charcode =
				  *(face->glyph->bitmap.buffer + i * width + j);
				  putchar(charcode < 128 ? ' ' : '*');
				  }
				  cout << endl;
				  }
				  cout << endl;
				*/
			}
		}

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::get_glyph5 ()
{
	bool result;

	FontConfig* gFontService = FontConfig::instance();

	Freetype *font = new Freetype;
	font->open(gFontService->getBuffer(),
									  gFontService->getBufferSize());

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	wchar_t ch = L'智';

	result = font->setCharSize(24, 96);

	if (result) {

		FT_UInt glyph_index = font->getCharIndex(ch);

		result = font->loadGlyph(glyph_index);

		CPPUNIT_ASSERT(result != 0);

		if (result) {

			result = font->renderGlyph(FT_RENDER_MODE_NORMAL);

			if (result) {
				//FT_Face face = font->getFontFace();

				//int rows = face->glyph->bitmap.rows;
				//int width = face->glyph->bitmap.width;

				/*
				  int i, j;
				  unsigned char charcode;
				  cout << endl;
				  for (i = 0; i < rows; i++) {
				  for (j = 0; j < width; j++) {
				  charcode =
				  *(face->glyph->bitmap.buffer + i * width + j);
				  putchar(charcode < 128 ? ' ' : '*');
				  }
				  cout << endl;
				  }
				  cout << endl;
				*/
			}
		}

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::glyph_metrics1 ()
{
	bool result;
	FontConfig* gFontService = FontConfig::instance();

	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	wchar_t ch = L'信';

	result = font->setCharSize(24, 96);

	if (result) {

		FT_UInt glyph_index = font->getCharIndex(ch);

		result = font->loadGlyph(glyph_index);

		CPPUNIT_ASSERT(result != 0);

		if (result) {

			result = font->renderGlyph(FT_RENDER_MODE_NORMAL);

			if (result) {
				FT_Face face = font->getFontFace();

				//int rows = face->glyph->bitmap.rows;
				// int width = face->glyph->bitmap.width;

				/*
				  int i, j;
				  unsigned char charcode;
				  cout << endl;
				  for (i = 0; i < rows; i++) {
				  for (j = 0; j < width; j++) {
				  charcode =
				  *(face->glyph->bitmap.buffer + i * width + j);
				  putchar(charcode < 128 ? ' ' : '*');
				  }
				  cout << endl;
				  }
				*/

				cout << "num_faces: " << face->num_faces << endl
					 << "face_index: " << face->face_index << endl
					 << "num_glyphs: " << face->num_glyphs << endl
					 << "slot: " << endl << "       " << "format: "
					 << face->glyph->format << endl << "       "
					 << "bitmap: " << endl << "               " << "rows: "
					 << face->glyph->bitmap.rows << endl << "               "
					 << "width: " << face->glyph->bitmap.width << endl
					 << "               " << "pitch: "
					 << face->glyph->bitmap.pitch << endl
					 << "               " << "num_grays: "
					 << face->glyph->bitmap.num_grays << endl << "       "
					 << "advance.x: " << face->glyph->advance.x << endl
					 << "       " << "advance.y: " << face->glyph->advance.y
					 << endl;

				// show glyph metrics
				cout << "Glyph metrics: " << endl;
				cout << "       " << "width: " << face->glyph->metrics.width
					 << endl;
				cout << "       " << "height: " << face->glyph->metrics.height
					 << endl;
				cout << "       " << "horiBearingX: "
					 << face->glyph->metrics.horiBearingX << endl;
				cout << "       " << "horiBearingY: "
					 << face->glyph->metrics.horiBearingY << endl;
				cout << "       " << "horiAdvance: "
					 << face->glyph->metrics.horiAdvance << endl;
				cout << "       " << "vertiBearingX: "
					 << face->glyph->metrics.vertBearingX << endl;
				cout << "       " << "vertiBearingY: "
					 << face->glyph->metrics.vertBearingY << endl;
				cout << "       " << "vertiAdvance: "
					 << face->glyph->metrics.vertAdvance << endl;
			}
		}

	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}

void FontEngineTest::checkkerning1()
{
	bool result;
	FontConfig* gFontService = FontConfig::instance();
	string fontpath = gFontService->getFontPath(std::string("Sans"));

	Freetype *font = new Freetype;
	font->open(fontpath);

	result = font->valid();

	if (!font->valid()) {
		delete font;
		font = NULL;
		CPPUNIT_FAIL("Cannot create font\n");
		return;
	}

	result = font->setCharSize(24, 96);

	if (result) {
		bool kerning = font->isUseKerning();
		if(kerning) {
			cout << "Sans uses kerning" << endl;
		} else {
			cout << "San does not use kerning" << endl;
		}
	}

	delete font;
	font = NULL;

	CPPUNIT_ASSERT(result);
}
