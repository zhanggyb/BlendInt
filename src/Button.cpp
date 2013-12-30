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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/Button.hpp>
#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	Button::Button ()
			: AbstractButton()
	{
		m_buffer.reset(new GLBufferMultiple);

		set_round_type(RoundAll);
		SetExpandX(true);
		Resize(90, 20);
		SetPreferredSize(90, 20);
	}

	Button::Button (const String& text)
			: AbstractButton()
	{
		m_buffer.reset(new GLBufferMultiple);

		set_round_type(RoundAll);
		SetExpandX(true);
		Resize(90, 20);
		set_text(text);
		SetPreferredSize(size());
	}

	Button::Button (AbstractWidget* parent)
			: AbstractButton(parent)
	{
		m_buffer.reset(new GLBufferMultiple);

		set_round_type(RoundAll);
		SetExpandX(true);
		Resize (90, 20);
		SetPreferredSize(90, 20);
	}

	Button::Button (const String& text, AbstractWidget* parent)
			: AbstractButton(parent)
	{
		m_buffer.reset(new GLBufferMultiple);

		set_round_type(RoundAll);
		SetExpandX(true);
		Resize(90, 20);
		set_text(text);
		SetPreferredSize(size());
	}

	Button::~Button ()
	{

	}

	void Button::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p, true, round_type(), radius(), m_buffer.get());
				break;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()), true, *type_p, radius(), m_buffer.get());
				break;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()), true, round_type(), *radius_p, m_buffer.get());
				break;
			}

			default:
				AbstractButton::Update(type, data);
				break;
		}
	}

	void Button::Draw ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ThemeManager* tm = ThemeManager::instance();

		// draw inner, simple fill
		if(down()) {
			glColor4ub(tm->themes()->regular.inner_sel.r(),
					tm->themes()->regular.inner_sel.g(),
					tm->themes()->regular.inner_sel.b(),
					tm->themes()->regular.inner_sel.a());
		} else {
			if(hover()) {
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

		draw_inner_buffer(m_buffer.get(), 0);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_outline_buffer(m_buffer.get(), 1);

		glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
		draw_outline_buffer(m_buffer.get(), 2);

		glDisable(GL_BLEND);

		// Draw text
		FontCache::create(font())->print(origin().x(), origin().y(), text(), valid_text_length());

		glPopMatrix();
	}

}
