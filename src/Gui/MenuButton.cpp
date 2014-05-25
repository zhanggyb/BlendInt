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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {
	
	MenuButton::MenuButton (const String& text)
	: AbstractButton(), m_vao(0)
	{
		InitializeMenuButton(text);
	}
	
	MenuButton::~MenuButton ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	void MenuButton::UpdateGeometry (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {
			switch (request.type()) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				UpdateTextPosition(*size_p, round_corner_type(), round_corner_radius(), text());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(*size_p,
								   round_corner_type(),
								   round_corner_radius(),
								   m_inner.get(),
								   0,
								   0);
				glBindVertexArray(0);
				Refresh();
				break;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(request.data());
				UpdateTextPosition(size(), *type_p, round_corner_radius(), text());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   *type_p,
								   round_corner_radius(),
								   m_inner.get(),
								   0,
								   0);
				glBindVertexArray(0);
				Refresh();
				break;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(request.data());
				UpdateTextPosition(size(), round_corner_type(), *radius_p, text());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   round_corner_type(),
								   *radius_p,
								   m_inner.get(),
								   0,
								   0);
				glBindVertexArray(0);
				Refresh();
				break;
			}

			default:
				AbstractButton::UpdateGeometry(request);
			}

		}
	}
	
	ResponseType MenuButton::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		if (hover()) {
			glBindVertexArray(m_vao);

			RefPtr<GLSLProgram> program =
							ShaderManager::instance->default_triangle_program();
			program->Use();

			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
							glm::value_ptr(mvp));

			Theme* tm = Theme::instance;

			glm::vec4 color(tm->menu_item().inner_sel.r() / 255.f,
							tm->menu_item().inner_sel.g() / 255.f,
							tm->menu_item().inner_sel.b() / 255.f,
							tm->menu_item().inner_sel.a() / 255.f);

			program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
			program->SetUniform1i("AA", 1);

			glEnableVertexAttribArray(0);

			DrawTriangleFan(program, mvp, 0, m_inner.get());

			glDisableVertexAttribArray(0);
			program->Reset();

			glBindVertexArray(0);
		}

		if(text().size()) {
			font().Print(mvp, text(), text_length(), 0);
		}

		return Accept;
	}

	void MenuButton::SetMenu (const RefPtr<Menu>& menu)
	{
		m_menu = menu;
	}

	void MenuButton::InitializeMenuButton (const String& text)
	{
		set_round_corner_type(RoundAll);
		set_text(text);

		unsigned int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_corner_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right(),
							h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom());
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			unsigned int width = text_outline.width() + round_corner_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right();
			unsigned int height = h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom();

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner.reset(new GLArrayBuffer);

		GenerateFormBuffer(
						size(),
						round_corner_type(),
						round_corner_radius(),
						m_inner.get(),
						0,
						0);

		glBindVertexArray(0);
	}

} /* namespace BlendInt */

