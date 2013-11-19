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

#include <BlendInt/ToggleButton.hpp>
#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	ToggleButton::ToggleButton ()
			: AbstractButton()
	{
		m_buffer.reset(new GLBuffer);

		set_round_type(RoundAll);
		SetExpandX(true);
		set_checkable(true);
		Resize(90, 20);
		SetPreferredSize(90, 20);
	}

	ToggleButton::ToggleButton (const String& text)
			: AbstractButton()
	{
		m_buffer.reset(new GLBuffer);

		set_round_type(RoundAll);
		SetExpandX(true);
		set_checkable(true);
		set_text(text);
		SetPreferredSize(size());
	}

	ToggleButton::ToggleButton (AbstractWidget* parent)
			: AbstractButton(parent)
	{
		m_buffer.reset(new GLBuffer);

		set_round_type(RoundAll);
		SetExpandX(true);
		set_checkable(true);
		Resize (90, 20);
		SetPreferredSize(90, 20);
	}

	ToggleButton::ToggleButton (const String& text, AbstractWidget* parent)
			: AbstractButton(parent)
	{
		m_buffer.reset(new GLBuffer);

		set_round_type(RoundAll);
		SetExpandX(true);
		set_checkable(true);
		set_text(text);
		SetPreferredSize(size());
	}

	ToggleButton::~ToggleButton ()
	{

	}

	void ToggleButton::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				generate_form_buffer(size_p, true, round_type(), radius(), m_buffer.get());
				break;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				generate_form_buffer(&(size()), true, *type_p, radius(), m_buffer.get());
				break;
			}
			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				generate_form_buffer(&(size()), true, round_type(), *radius_p, m_buffer.get());
				break;
			}

			default:
				AbstractButton::Update(type, data);
				break;
		}

	}

	void ToggleButton::Render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		ThemeManager* tm = ThemeManager::instance();

		// draw inner, simple fill
		if (hover()) {
			if(checked()) {
				glColor4ub(tm->themes()->regular.inner_sel.highlight_red(),
				        tm->themes()->regular.inner_sel.highlight_green(),
				        tm->themes()->regular.inner_sel.highlight_blue(),
				        tm->themes()->regular.inner_sel.a());
			} else {
				glColor4ub(tm->themes()->regular.inner.highlight_red(),
						tm->themes()->regular.inner.highlight_green(),
						tm->themes()->regular.inner.highlight_blue(),
						tm->themes()->regular.inner.a());
			}
		} else {
			if (checked()) {
				glColor4ub(tm->themes()->regular.inner_sel.r(),
				        tm->themes()->regular.inner_sel.g(),
				        tm->themes()->regular.inner_sel.b(),
				        tm->themes()->regular.inner_sel.a());
			} else {
				glColor4ub(tm->themes()->regular.inner.r(),
				        tm->themes()->regular.inner.g(),
				        tm->themes()->regular.inner.b(),
				        tm->themes()->regular.inner.a());
			}
		}

		draw_inner_buffer(m_buffer.get(), 0);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		glColor4ubv(tcol);

		/* outline */
		draw_outline_buffer(m_buffer.get(), 1);

		glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
		draw_outline_buffer(m_buffer.get(), 2);

		glDisable(GL_BLEND);

		FontCache::create(font())->print(origin().x(), origin().y(), text(), valid_text_length());

		glPopMatrix();
	}

}
