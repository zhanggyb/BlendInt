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

#include <BlendInt/Gui/ComboBox.hpp>

namespace BlendInt {

	ComboBox::ComboBox ()
	: RoundWidget()
	{
		InitOnce();
	}

	ComboBox::~ComboBox ()
	{
	}

	bool ComboBox::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p,
								   round_type(),
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);

				m_shadow.Resize(*size_p);
				Refresh();
				return true;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()),
								   *type_p,
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);

				Refresh();
				return true;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()),
								   round_type(),
								   *radius_p,
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);

				Refresh();
				return true;
			}

			default:
				return RoundWidget::Update(type, data);
		}
	}

	void ComboBox::Draw(RedrawEvent* event)
	{
		ThemeManager* tm = ThemeManager::instance();

		m_shadow.Draw();

		// draw inner, simple fill
		glColor4ub(tm->themes()->regular.inner.r(),
				tm->themes()->regular.inner.g(),
				tm->themes()->regular.inner.b(),
				tm->themes()->regular.inner.a());

		//DrawInnerBuffer(m_inner_buffer.get());
		DrawInnerBuffer(m_inner_buffer);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		//DrawOutlineBuffer(m_outer_buffer.get());
		DrawOutlineBuffer(m_outer_buffer);
	}

	void ComboBox::InitOnce()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(90, 20);
		set_preferred_size(90, 20);

		GenerateFormBuffer(&size(), round_type(), radius(), m_inner_buffer.get(),
				m_outer_buffer.get(), 0);
	}

}
