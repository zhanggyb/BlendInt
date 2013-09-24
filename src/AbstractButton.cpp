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

#include <BILO/AbstractButton.hpp>

namespace BILO {

	AbstractButton::AbstractButton ()
	: down_(false), checkable_(false),
	  checked_(false), hover_(false), vertex_array_(0), valid_text_length_(0)
	{

	}

	AbstractButton::AbstractButton (Drawable *parent)
		: Widget(parent), down_(false), checkable_(false),
		  checked_(false), hover_(false), vertex_array_(0), valid_text_length_(0)
	{
		FontCache::create(font_);
	}

	AbstractButton::~AbstractButton ()
	{
		if(vertex_array_) {
			delete [] vertex_array_;
		}
	}

	void AbstractButton::set_text (const String& text)
	{
		if(text.empty()) {
			// TODO: draw blank label
			return;
		}

		text_ = text;

		Rect box = FontCache::create(font_)->calculateOutline(text_);
		updateVertexArray(box.left() + padding_.left(), padding_.bottom() + std::abs(box.bottom()), 1.0, 1.0);
		resize (box.width() + padding_.left() + padding_.right(), box.height() + padding_.top() + padding_.bottom());
	}

	void AbstractButton::set_font (const Font& font)
	{
		font_ = font;
		FontCache::create(font_);

		Rect box = FontCache::create(font_)->calculateOutline(text_);
		updateVertexArray(box.left() + padding_.left(), padding_.bottom() + std::abs(box.bottom()), 1.0, 1.0);
		resize (box.width() + padding_.left() + padding_.right(), box.height() + padding_.top() + padding_.bottom());
	}

	void AbstractButton::updateVertexArray(float x, float y, float sx, float sy)
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

	void AbstractButton::press_mouse (MouseEvent* event)
	{
		if (!contain(event->position())) return;

		if (checkable_) {
			checked_ = !checked_;
			toggled_.fire(checked_);

		} else {
			down_ = true;
			clicked_.fire();
		}
		event->accept(this);
	}

	void AbstractButton::release_mouse(MouseEvent* event)
	{
		if (! contain(event->position()))	return;

		down_ = false;

		if (checkable_) {

		} else {

		}
	}

	void AbstractButton::move_mouse (MouseEvent* event)
	{
		if (contain(event->position())) {
			hover_ = true;
            event->accept(this);
		} else {
			hover_ = false;
			down_ = false;
		}
	}

} /* namespace BILO */
