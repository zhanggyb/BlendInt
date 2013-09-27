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

#include <BILO/ToggleButton.hpp>
#include <BILO/ShaderManager.hpp>

namespace BILO {

	ToggleButton::ToggleButton ()
			: AbstractButton()
	{
		setCheckable(true);
		resize(90, 25);
	}

	ToggleButton::ToggleButton (const String& text)
			: AbstractButton()
	{
		setCheckable(true);
		set_text(text);
	}

	ToggleButton::ToggleButton (Drawable* parent)
			: AbstractButton(parent)
	{
		setCheckable(true);
		resize (90, 25);
	}

	ToggleButton::ToggleButton (const String& text, Drawable* parent)
			: AbstractButton(parent)
	{
		setCheckable(true);
		set_text(text);
	}

	ToggleButton::~ToggleButton ()
	{

	}

	void ToggleButton::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		if (m_buffer.is_buffer(0)) {
			Theme* tm = Theme::instance();

			if (m_status_checked) {
				glColor4ub(tm->themeUI()->wcol_regular.inner_sel.r(),
				        tm->themeUI()->wcol_regular.inner_sel.g(),
				        tm->themeUI()->wcol_regular.inner_sel.b(),
				        tm->themeUI()->wcol_regular.inner_sel.a() * 0.5f);
			} else {
				if (m_status_hover) {
					glColor4ub(
					        tm->themeUI()->wcol_regular.inner.highlight_red(),
					        tm->themeUI()->wcol_regular.inner.highlight_green(),
					        tm->themeUI()->wcol_regular.inner.highlight_blue(),
					        tm->themeUI()->wcol_regular.inner.a() * 0.5f);
				} else {
					glColor4ub(tm->themeUI()->wcol_regular.inner.r(),
					        tm->themeUI()->wcol_regular.inner.g(),
					        tm->themeUI()->wcol_regular.inner.b(),
					        tm->themeUI()->wcol_regular.inner.a() * 0.5f);
				}
			}

			m_buffer.bind(GL_ARRAY_BUFFER);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawArrays(GL_POLYGON, 0, 4);

			glColor4ub(tm->themeUI()->wcol_regular.outline.r(),
			        tm->themeUI()->wcol_regular.outline.g(),
			        tm->themeUI()->wcol_regular.outline.b(),
			        tm->themeUI()->wcol_regular.outline.a());

			glDrawArrays(GL_LINE_LOOP, 0, 4);

			glDisableClientState(GL_VERTEX_ARRAY);

			m_buffer.unbind(GL_ARRAY_BUFFER);
		}

		FontCache::create(m_font)->print(
		        m_text_outline.left() + m_padding.left(),
		        m_padding.bottom() + std::abs(m_text_outline.bottom()), m_text);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
