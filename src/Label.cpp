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

#include <BIL/Label.hpp>

#include <BIL/FontCache.hpp>

namespace BIL {

	Label::Label (const String& text, Drawable *parent)
		: Widget (parent), background_(0x00000000)
	{
		FontCache* fc = FontCache::create(font_);
		fc->setup();

		set_text(text);
	}

	Label::~Label ()
	{
		// TODO Auto-generated destructor stub
	}

	void Label::set_text (const String& label)
	{
		if(label.empty()) {
			// TODO: draw blank label
			return;
		}

		text_ = label;

		calculateBox();

		/*
		text_.set_origin(Coord3f(
								 pos_.x() + padding_.border.l,
								 pos_.y() + padding_.border.b,
								 0.0)
						 );
		 */
	}

	void Label::set_font (const Font& font)
	{
		font_ = font;
		FontCache* fc = FontCache::create(font_);
		fc->setup();

		calculateBox();
	}


	void Label::calculateBox ()
	{
		Rect box = FontCache::create(font_)->calculateOutline(text_);
		
		box.set_width(box.width() + padding_.left() + padding_.right());
		box.set_height(box.height() + padding_.top() + padding_.bottom());

		resize (box.width(), box.height());
	}

	void Label::render ()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());
		glColor4ub(background_.r(), background_.g(),
				   background_.b(), background_.a());
		glRectf(0.0, 0.0, size_.width(), size_.height());

		//text_.render();

		glPopMatrix();
		glDisable(GL_BLEND);
	}

	void Label::cursorPosEvent (double xpos, double ypos)
	{
		//cout << "Cursor Position: " << xpos << " " << ypos << endl;
	}
} /* namespace BIL */
