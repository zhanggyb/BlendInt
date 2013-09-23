/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BILO/Label.hpp>

#include <BILO/FontCache.hpp>
#include <BILO/ShaderManager.hpp>

namespace BILO {

	Label::Label (const String& text)
		: Widget(), background_(0x00000000), vertex_array_(0), valid_text_length_(0)
	{
		FontCache::create(font_);

		set_text(text);
	}

	Label::Label (const String& text, Drawable *parent)
		: Widget (parent), background_(0x00000000), vertex_array_(0), valid_text_length_(0)
	{
		FontCache::create(font_);

		set_text(text);
	}

	Label::~Label ()
	{
		if(vertex_array_) {
			delete [] vertex_array_;
		}
	}

	void Label::set_text (const String& label)
	{
		if(label.empty()) {
			// TODO: draw blank label
			return;
		}

		text_ = label;

		Rect box = FontCache::create(font_)->calculateOutline(text_);

		updateVertexArray(box.left() + padding_.left(), padding_.bottom() + std::abs(box.bottom()), 1.0, 1.0);

		resize (box.width() + padding_.left() + padding_.right(), box.height() + padding_.top() + padding_.bottom());
	}

	void Label::set_font (const Font& font)
	{
		font_ = font;
		FontCache::create(font_);

		Rect box = FontCache::create(font_)->calculateOutline(text_);
		updateVertexArray(box.left() + padding_.left(), padding_.bottom() + std::abs(box.bottom()), 1.0, 1.0);
		resize (box.width() + padding_.left() + padding_.right(), box.height() + padding_.top() + padding_.bottom());
	}

	void Label::updateVertexArray(float x, float y, float sx, float sy)
	{
		if(vertex_array_) {
			delete [] vertex_array_;
		}

		vertex_array_ = new Vertex2D[6 * text_.length()];

		valid_text_length_ = 0;

		String::const_iterator it;
		FontCache* fc = FontCache::create(font_);

		/* Loop through all characters */
		for (it = text_.begin(); it != text_.end(); it++)
		{
			/* Calculate the vertex and texture coordinates */
			float x2 = x + fc->queryGlyph(*it).bitmap_left * sx;

			float y2 = y + fc->queryGlyph(*it).bitmap_top * sy;

			float w = fc->queryGlyph(*it).bitmap_width * sx;
			float h = fc->queryGlyph(*it).bitmap_height * sy;

			/* Advance the cursor to the start of the next character */
			x += fc->queryGlyph(*it).advance_x * sx;
			y += fc->queryGlyph(*it).advance_y * sy;

			/* Skip glyphs that have no pixels */
			if (!w || !h)
				continue;

			(vertex_array_ + valid_text_length_)->x = x2;
			(vertex_array_ + valid_text_length_)->y = y2;
			(vertex_array_ + valid_text_length_)->s = fc->queryGlyph(*it).texture_offset_x;
			(vertex_array_ + valid_text_length_)->t = fc->queryGlyph(*it).texture_offset_y;
			valid_text_length_++;

			(vertex_array_ + valid_text_length_)->x = x2 + w;
			(vertex_array_ + valid_text_length_)->y = y2;
			(vertex_array_ + valid_text_length_)->s = fc->queryGlyph(*it).texture_offset_x + fc->queryGlyph(*it).bitmap_width / fc->queryWidth(*it);
			(vertex_array_ + valid_text_length_)->t = fc->queryGlyph(*it).texture_offset_y;
			valid_text_length_++;

			(vertex_array_ + valid_text_length_)->x = x2;
			(vertex_array_ + valid_text_length_)->y = y2 - h;
			(vertex_array_ + valid_text_length_)->s = fc->queryGlyph(*it).texture_offset_x;
			(vertex_array_ + valid_text_length_)->t = fc->queryGlyph(*it).texture_offset_y + fc->queryGlyph(*it).bitmap_height / fc->queryHeight(*it);
			valid_text_length_++;

			(vertex_array_ + valid_text_length_)->x = x2 + w;
			(vertex_array_ + valid_text_length_)->y = y2;
			(vertex_array_ + valid_text_length_)->s = fc->queryGlyph(*it).texture_offset_x + fc->queryGlyph(*it).bitmap_width / fc->queryWidth(*it);
			(vertex_array_ + valid_text_length_)->t = fc->queryGlyph(*it).texture_offset_y;
			valid_text_length_++;

			(vertex_array_ + valid_text_length_)->x = x2;
			(vertex_array_ + valid_text_length_)->y = y2 - h;
			(vertex_array_ + valid_text_length_)->s = fc->queryGlyph(*it).texture_offset_x;
			(vertex_array_ + valid_text_length_)->t = fc->queryGlyph(*it).texture_offset_y + fc->queryGlyph(*it).bitmap_height / fc->queryHeight(*it);
			valid_text_length_++;

			(vertex_array_ + valid_text_length_)->x = x2 + w;
			(vertex_array_ + valid_text_length_)->y = y2 - h;
			(vertex_array_ + valid_text_length_)->s = fc->queryGlyph(*it).texture_offset_x + fc->queryGlyph(*it).bitmap_width / fc->queryWidth(*it);
			(vertex_array_ + valid_text_length_)->t = fc->queryGlyph(*it).texture_offset_y + fc->queryGlyph(*it).bitmap_height / fc->queryHeight(*it);
			valid_text_length_++;
		}

	}

	void Label::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(background_.r(), background_.g(),
				   background_.b(), background_.a());
		glRectf(0.0, 0.0, size_.width(), size_.height());

		//text_.render();
		glUseProgram(ShaderManager::instance()->text_program().id());

		GLfloat black[4] = { 0, 0, 0, 1 };
		//GLfloat red[4] = { 1, 0, 0, 1 };
		//GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

		/* Set color to black */
		glUniform4fv(ShaderManager::instance()->text_uniform_color(), 1, black);

		/* Use the texture containing the atlas */
		glBindTexture(GL_TEXTURE_2D, FontCache::create(font_)->queryTexture(text_[0]));
		glUniform1i(ShaderManager::instance()->text_uniform_tex(), 0);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(ShaderManager::instance()->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, ShaderManager::instance()->text_vbo());
		glVertexAttribPointer(ShaderManager::instance()->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		/* Draw all the character on the screen in one go */
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6 * text_.length(), vertex_array_, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, valid_text_length_);

		glDisableVertexAttribArray(ShaderManager::instance()->text_attribute_coord());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size_.width(), 0);
			glVertex2i(size_.width(), size_.height());
			glVertex2i(0, size_.height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif
		glDisable(GL_BLEND);

		glPopMatrix();

	}

	void Label::cursorPosEvent (double xpos, double ypos)
	{
		//cout << "Cursor Position: " << xpos << " " << ypos << endl;
	}
} /* namespace BILO */
