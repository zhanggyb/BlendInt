/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <iostream>

#include <BlendInt/Button.hpp>
#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	Button::Button ()
			: AbstractButton()
	{
		set_roundcorner(RoundCornerAll);
		set_expand_x(true);
		resize(90, 25);
		set_preferred_size(90, 25);
	}

	Button::Button (const String& text)
			: AbstractButton()
	{
		set_roundcorner(RoundCornerAll);
		set_expand_x(true);
		set_text(text);
		set_preferred_size(size());
	}

	Button::Button (AbstractForm* parent)
			: AbstractButton(parent)
	{
		set_roundcorner(RoundCornerAll);
		set_expand_x(true);
		resize (90, 25);
		set_preferred_size(90, 25);
	}

	Button::Button (const String& text, AbstractForm* parent)
			: AbstractButton(parent)
	{
		set_roundcorner(RoundCornerAll);
		set_expand_x(true);
		set_text(text);
		set_preferred_size(size());
	}

	Button::~Button ()
	{

	}

	void Button::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ThemeManager* tm = ThemeManager::instance();

		// draw inner, simple fill
		if(m_status_down) {
			glColor4ub(tm->themes()->regular.inner_sel.r(),
					tm->themes()->regular.inner_sel.g(),
					tm->themes()->regular.inner_sel.b(),
					tm->themes()->regular.inner_sel.a());
		} else {
			if(m_status_hover) {
				glColor4ub(tm->themes()->regular.inner.highlight_red(),
						tm->themes()->regular.inner.highlight_green(),
						tm->themes()->regular.inner.highlight_blue(),
						tm->themes()->regular.inner.a());
			} else {
				glColor4ub(tm->themes()->regular.inner.r(),
						tm->themes()->regular.inner.g(),
						tm->themes()->regular.inner.b(),
						tm->themes()->regular.inner.a());
			}
		}

		m_buffer.set_index(0);
		m_buffer.bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_POLYGON, 0, m_buffer.vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		m_buffer.unbind();

		// draw outline
		m_buffer.set_index(1);
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		m_buffer.bind();
		/* outline */
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, m_buffer.vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		m_buffer.unbind();

		if(m_emboss) {
			m_buffer.set_index(2);
			m_buffer.bind();
			glEnableClientState(GL_VERTEX_ARRAY);
			for (int j = 0; j < WIDGET_AA_JITTER; j++) {
				glTranslatef(jit[j][0], jit[j][1], 0.0f);
				glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
				glVertexPointer(2, GL_FLOAT, 0, 0);
				glDrawArrays(GL_QUAD_STRIP, 0, m_buffer.vertices());
				glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
			}
			glDisableClientState(GL_VERTEX_ARRAY);
			m_buffer.unbind();
		}

		// Draw text
		FontCache::create(m_font)->print(
		        m_text_outline.left() + m_padding.left(),
		        m_padding.bottom() + std::abs(m_text_outline.bottom()), m_text);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
