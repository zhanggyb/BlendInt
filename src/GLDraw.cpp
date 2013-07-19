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
#include <BIL/FontType.h>

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
        GLuint font = list_base;
        float height = h / .63f;

        const char *start_line = text.c_str();
        vector<string> lines;

        const char * c = text.c_str();

        //for(const char *c=text;*c;c++) {
        for (; *c; c++) {
            if (*c == '\n') {
                string line;
                for (const char *n = start_line; n < c; n++)
                    line.append(1, *n);
                lines.push_back(line);
                start_line = c + 1;
            }
        }
        if (start_line) {
            string line;
            for (const char *n = start_line; n < c; n++)
                line.append(1, *n);
            lines.push_back(line);
        }

        glPushAttrib(
                     GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glListBase(font);

        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        //This is where the text display actually happens.
        //For each line of text we reset the modelview matrix
        //so that the line's text will start in the correct position.
        //Notice that we need to reset the matrix, rather than just translating
        //down by h. This is because when each character is
        //draw it modifies the current matrix so that the next character
        //will be drawn immediatly after it.
        for (unsigned int i = 0; i < lines.size(); i++) {
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(position.getX(), position.getY() - height * i, 0);
            glMultMatrixf(modelview_matrix);

            //  The commented out raster position stuff can be useful if you need to
            //  know the length of the text that you are creating.
            //  If you decide to use it make sure to also uncomment the glBitmap command
            //  in make_dlist().
            //  glRasterPos2f(0,0);
            glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
            //  float rpos[4];
            //  glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
            //  float len=x-rpos[0];

            glPopMatrix();
        }
    }

    // copy some code from Nehe's opengl tutorial lesson 43
    void GLDraw::makeDisplayList (char ch, GLuint list_base, GLuint* tex_base)
    {
        // The first thing we do is get FreeType to render out character
        // into a bitmap. This actually requires a couple of FreeType commands:

        string fontfile =
            FontManager::gFontService->getFontPath(_drawArea->getFont());

        FontType face(fontfile);

        if (!face.isValid()) {
            cerr << __func__ << __LINE__ << " Cannot get FT_Face" << endl;
            return;
        }

        face.setCharSize(this->h, 96);

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

    void GLDraw::init (GLuint h)
    {
        // Allocate some memory to store the texture ids.
        textures = new GLuint[128];

        this->h = h;

        list_base = glGenLists(128);
        glGenTextures(128, textures);

        for (unsigned char i = 0; i < 128; i++)
            makeDisplayList(i, list_base, textures);
    }

    void GLDraw::clean (void)
    {
        glDeleteLists(list_base, 128);
        glDeleteTextures(128, textures);
        delete[] textures;
    }

} /* namespace BIL */
