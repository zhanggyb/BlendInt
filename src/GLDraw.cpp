/*
 * GLDraw.cpp
 *
 *  Created on: 2013年7月8日
 *      Author: zhanggyb
 */

// Load glew first
#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/GLDraw.h>

#include <BIL/FontManager.h>
#include <BIL/FontFace.h>

#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

namespace BIL {

	GLDraw::GLDraw (Drawable *obj)
			: _drawArea(obj)
	{
		// TODO Auto-generated constructor stub

	}

	GLDraw::~GLDraw ()
	{
		// TODO Auto-generated destructor stub
	}

	void GLDraw::setPointSize (GLfloat size)
	{
		glPointSize(size);
	}

	void GLDraw::setLineWidth (GLfloat width)
	{
		glLineWidth(width);
	}

	void GLDraw::setLineStyle (GLfloat style)
	{
	}

	void GLDraw::setColor (const Color& color)
	{
		glColor4f(color.red(), color.green(), color.blue(), color.alpha());
	}

	void GLDraw::line (const Point2Di& from, const Point2Di& to)
	{
	}

	void GLDraw::line (const Point3Di& from, const Point3Di& to)
	{
		glBegin(GL_LINES);
		glVertex3i(from.getX(), from.getY(), from.getZ());
		glVertex3i(to.getX(), to.getY(), to.getZ());
		glEnd();
	}

	void GLDraw::line (const Point3Df& from, const Point3Df& to)
	{
		glBegin(GL_LINES);
		glVertex3f(from.getX(), from.getY(), from.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
		glEnd();
	}

	void GLDraw::rect (GLint x, GLint y, GLint w, GLint h)
	{
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);
		glEnd();
	}

	void GLDraw::rect (GLfloat x, GLfloat y, GLfloat w, GLfloat h)
	{
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
	}

	void GLDraw::rect (const Rect2Di& rect)
	{
		glBegin(GL_QUADS);
		glVertex2i(rect.getX(), rect.getY());
		glVertex2i(rect.getX() + rect.getWidth(), rect.getY());
		glVertex2i(rect.getX() + rect.getWidth(),
		        rect.getY() + rect.getHeight());
		glVertex2i(rect.getX(), rect.getY() + rect.getHeight());
		glEnd();
	}

	void GLDraw::text (const std::string& text, Point3Df position)
	{
	}

	// copy some code from Nehe's opengl tutorial lesson 43
	void GLDraw::makeDisplayList (char ch, GLuint list_base, GLuint* tex_base)
	{
		// The first thing we do is get FreeType to render out character
		// into a bitmap. This actually requires a couple of FreeType commands:

		string fontfile = FontManager::gFontManager->getFontPath(
		        _drawArea->getFont());

		FontFace face(FontManager::gFontManager->getFontLibrary(), fontfile);

		if (!face.isValid())
			return;

		// load Glyph for our character.
		face.loadGlyph(ch);

		// Move the face's glyph into a Glyph object.
		FT_Glyph glyph;

		if (FT_Get_Glyph(face.getFontFace()->glyph, &glyph))
			throw;

		// Convert the glyph to a bitmap;
		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

		// This reference will make accessing the bitmap easier
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		// Use our healper funciton to get the widths of
		// the bitmap data that we will need in order to create
		// out texture
		int width = next_p2(bitmap.width);
		int height = next_p2(bitmap.rows);

		// Allocate memory for the texture data.
		GLubyte* expanded_data = new GLubyte[2 * width * height];

		//Here we fill in the data for the expanded bitmap.
		//Notice that we are using two channel bitmap (one for
		//luminocity and one for alpha), but we assign
		//both luminocity and alpha to the value that we
		//find in the FreeType bitmap.
		//We use the ?: operator so that value which we use
		//will be 0 if we are in the padding zone, and whatever
		//is the the Freetype bitmap otherwise.
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				expanded_data[2 * (i + j * width)] = 255;
				expanded_data[2 * (i + j * width) + 1] =
				        (i >= bitmap.width || j >= bitmap.rows) ?
				                0 : bitmap.buffer[i + bitmap.width * j];
			}
		}

		//Now we just setup some texture paramaters.
		glBindTexture( GL_TEXTURE_2D, tex_base[ch]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Here we actually create the texture itself, notice
		//that we are using GL_LUMINANCE_ALPHA to indicate that
		//we are using 2 channel data.
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		        GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

		//With the texture created, we don't need to expanded data anymore
		delete[] expanded_data;

		//So now we can create the display list
		glNewList(list_base + ch, GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D, tex_base[ch]);

		//first we need to move over a little so that
		//the character has the right amount of space
		//between it and the one before it.
		glTranslatef(bitmap_glyph->left, 0, 0);

		//Now we move down a little in the case that the
		//bitmap extends past the bottom of the line
		//(this is only true for characters like 'g' or 'y'.
		glPushMatrix();
		glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);

		//Now we need to account for the fact that many of
		//our textures are filled with empty padding space.
		//We figure what portion of the texture is used by
		//the actual character and store that information in
		//the x and y variables, then when we draw the
		//quad, we will only reference the parts of the texture
		//that we contain the character itself.
		float x = (float) bitmap.width / (float) width, y = (float) bitmap.rows
		        / (float) height;

		//Here we draw the texturemaped quads.
		//The bitmap that we got from FreeType was not
		//oriented quite like we would like it to be,
		//so we need to link the texture to the quad
		//so that the result will be properly aligned.
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2f(0, bitmap.rows);
		glTexCoord2d(0, y);
		glVertex2f(0, 0);
		glTexCoord2d(x, y);
		glVertex2f(bitmap.width, 0);
		glTexCoord2d(x, 0);
		glVertex2f(bitmap.width, bitmap.rows);
		glEnd();
		glPopMatrix();
		glTranslatef(face.getFontFace()->glyph->advance.x >> 6, 0, 0);

		//increment the raster position as if we were a bitmap font.
		//(only needed if you want to calculate text length)
		//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

		//Finnish the display list
		glEndList();
	}

} /* namespace BIL */

