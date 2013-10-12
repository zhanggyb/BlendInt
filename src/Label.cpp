/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BlendInt/Label.hpp>

#include <BlendInt/FontCache.hpp>

namespace BlendInt {

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

		resize (m_text_outline.width() + m_padding.left() + m_padding.right(), m_text_outline.height() + m_padding.top() + m_padding.bottom());
	}

	void Label::set_font (const Font& font)
	{
		m_font = font;
		FontCache::create(m_font);

		Rect box = FontCache::create(m_font)->get_text_outline(m_text);
		resize (box.width() + m_padding.left() + m_padding.right(), box.height() + m_padding.top() + m_padding.bottom());
	}

	void Label::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(m_background.r(), m_background.g(),
				   m_background.b(), m_background.a());
		glRectf(0.0, 0.0, m_size.width(), m_size.height());

		FontCache::create(m_font)->print(m_text_outline.left() + m_padding.left(), m_padding.bottom() + std::abs(m_text_outline.bottom()), m_text);

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(m_size.width(), 0);
			glVertex2i(m_size.width(), m_size.height());
			glVertex2i(0, m_size.height());
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
} /* namespace BlendInt */
