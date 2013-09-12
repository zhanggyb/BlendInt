/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <iostream>
#include <algorithm>

#include <BIL/Freetype.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/TextBuffer.hpp>

#include <BIL/Tuple.hpp>

using namespace std;

namespace BIL {

	/*
	 TextBuffer::TextBuffer (const Font& font)
	 : rowspacing_(1.0),
	 foreground_(0xFFFFFFFF),
	 background_(0x00000000)
	 {
	 set_font(font);
	 //glShadeModel(GL_FLAT);
	 //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	 }
	 */

	TextBuffer::TextBuffer (const String& text, const Font& font)
			: line_spacing_(1.0), foreground_(0x000000FF), background_(
			        0x00000000)
	{
		setFont(font);
		append(text);
	}

	TextBuffer::~TextBuffer ()
	{
	}

	void TextBuffer::setFont (const Font& font)
	{
		FontCache::create(font);
	}

	void TextBuffer::append (const String& text)
	{
		if (text.empty())
			return;

		text_.append(text);
	}

	void TextBuffer::append (wchar_t charcode)
	{
		text_.push_back(charcode);
	}

	Rect TextBuffer::calculateOutline ()
	{
		String::const_iterator it;
		String::const_iterator next;
		Tuple2l kerning;
		int xmin = 0;
		int ymin = 0;
		int xmax = 0;
		int ymax = 0;

		for (it = text_.begin(); it != text_.end(); it++)
		{
			xmax = fontcache_->queryGlyph(*it).advance_x + xmax;
			ymin = std::min(static_cast<int>(fontcache_->queryGlyph(*it).bitmap_top - fontcache_->queryGlyph(*it).bitmap_height), ymin);
			ymax = std::max(static_cast<int>(fontcache_->queryGlyph(*it).bitmap_top), ymax);
		}

		return Rect(Point(xmin, ymin), Point(xmax, ymax));
	}

	Size TextBuffer::calculateOutlineBoxSize ()
	{
		String::const_iterator it;
		String::const_iterator next;
		Size box;
		Tuple2l kerning;

		unsigned int line_width = 0;
		unsigned int line = 1;
		//int left_edge = 0;
		//int right_edge = 0;

		for (it = text_.begin(); it != text_.end(); it++) {
			if (*it == '\n') {
				line++;
				box.set_width(std::max(line_width, box.width()));
				line_width = 0;
				continue;
			}

			next = it + 1;
			if (next != text_.end()) {
				kerning = fontcache_->getKerning(*it, *next);
			}
			line_width = fontcache_->queryGlyph(*it).advance_x + kerning.vec.x
			        + line_width;
		}

		if (line == 1) {
			box.set_width(line_width);
		}
		box.set_height(
		        static_cast<unsigned int>(fontcache_->getHeight() * line
		                + (line - 1) * line_spacing_));

		return box;
	}

	void TextBuffer::render ()
	{
		//glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glColor4ub(foreground_.r(), foreground_.g(), foreground_.b(),
		        foreground_.a());

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(origin_.x(), origin_.y() - fontcache_->getDescender(),
		        origin_.z());

		int line = 0;
		String::const_iterator it;
		for (it = text_.begin(); it != text_.end(); it++) {
			if (*it == '\n') {
				line++;
				glLoadIdentity();
				glTranslatef(origin_.x(),
				        origin_.y()
				                - line_spacing_ * fontcache_->getHeight()
				                        * line, origin_.z());
				continue;
			}

			/*
			 glyph = fontcache_->query(*it);
			 if (glyph) {
			 glyph->Render();
			 glTranslatef(glyph->metrics().horiAdvance, 0, 0);
			 }
			 */
		}

		glPopMatrix();
		glDisable(GL_BLEND);

	}

	void TextBuffer::clear (void)
	{
		text_.clear();
	}

} /* namespace BIL */
