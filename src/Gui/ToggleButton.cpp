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

#include <BlendInt/Gui/ToggleButton.hpp>

#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	ToggleButton::ToggleButton ()
			: AbstractButton()
	{
		InitializeOnce();
	}

	ToggleButton::ToggleButton (const String& text)
			: AbstractButton()
	{
		InitializeOnce(text);
	}

	ToggleButton::ToggleButton (AbstractWidget* parent)
			: AbstractButton(parent)
	{
		InitializeOnce();
	}

	ToggleButton::ToggleButton (const String& text, AbstractWidget* parent)
			: AbstractButton(parent)
	{
		InitializeOnce(text);
	}

	ToggleButton::~ToggleButton ()
	{
	}

	bool ToggleButton::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p, round_type(), radius(),
				        m_inner_buffer.get(), m_outer_buffer.get(),
				        m_emboss_buffer.get());

				Refresh();
				return true;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()), *type_p, radius(),
				        m_inner_buffer.get(), m_outer_buffer.get(),
				        m_emboss_buffer.get());

				Refresh();
				return true;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()), round_type(), *radius_p,
				        m_inner_buffer.get(), m_outer_buffer.get(),
				        m_emboss_buffer.get());

				Refresh();
				return true;
			}

			default:
				return true;
		}

	}

	void ToggleButton::Draw ()
	{
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

		DrawInnerBuffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		glColor4ubv(tcol);

		/* outline */
		DrawOutlineBuffer(m_outer_buffer.get());

		glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
		DrawOutlineBuffer(m_emboss_buffer.get());

		if(text().size())
			FontCache::create(font())->print(origin().x(), origin().y(), text(), valid_text_length());
	}

	void ToggleButton::InitializeOnce ()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_emboss_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);
		set_expand_x(true);
		set_checkable(true);
		set_size(90, 20);
		set_preferred_size(90, 20);

		GenerateFormBuffer(&size(), round_type(), radius(),
		        m_inner_buffer.get(), m_outer_buffer.get(),
		        m_emboss_buffer.get());
	}

	void ToggleButton::InitializeOnce (const String& text)
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_emboss_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);
		set_expand_x(true);
		set_checkable(true);
		set_text(text);	// this change the button size
		set_preferred_size(size());

		GenerateFormBuffer(&size(), round_type(), radius(),
		        m_inner_buffer.get(), m_outer_buffer.get(),
		        m_emboss_buffer.get());
	}

}
