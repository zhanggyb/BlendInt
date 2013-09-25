/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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

namespace BILO {

	Label::Label (const String& text)
		: Widget(), m_background(0x00000000)
	{
		FontCache::create(m_font);

		set_text(text);
	}

	Label::Label (const String& text, Drawable *parent)
		: Widget (parent), m_background(0x00000000)
	{
		FontCache::create(m_font);

		set_text(text);
	}

	Label::~Label ()
	{

	}

	void Label::set_text (const String& label)
	{
		if(label.empty()) {
			// TODO: draw blank label
			return;
		}

		m_text = label;

		m_text_outline = FontCache::create(m_font)->get_text_outline(m_text);

		resize (m_text_outline.width() + padding_.left() + padding_.right(), m_text_outline.height() + padding_.top() + padding_.bottom());
	}

	void Label::set_font (const Font& font)
	{
		m_font = font;
		FontCache::create(m_font);

		Rect box = FontCache::create(m_font)->get_text_outline(m_text);
		resize (box.width() + padding_.left() + padding_.right(), box.height() + padding_.top() + padding_.bottom());
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

		glColor4ub(m_background.r(), m_background.g(),
				   m_background.b(), m_background.a());
		glRectf(0.0, 0.0, size_.width(), size_.height());

		FontCache::create(m_font)->print(m_text_outline.left() + padding_.left(), padding_.bottom() + std::abs(m_text_outline.bottom()), m_text);

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
