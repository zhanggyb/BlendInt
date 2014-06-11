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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>

#include <BlendInt/Stock/ShaderManager.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	ToggleButton::ToggleButton ()
			: AbstractButton(), m_vao(0)
	{
		InitializeToggleButton();
	}

	ToggleButton::ToggleButton (const String& text)
			: AbstractButton(), m_vao(0)
	{
		InitializeToggleButton(text);
	}

	ToggleButton::~ToggleButton ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ToggleButton::UpdateGeometry(const WidgetUpdateRequest& request)
	{

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					UpdateTextPosition(*size_p, round_corner_type(), round_corner_radius(), text());

					VertexTool tool;
					tool.Setup(*size_p, DefaultBorderWidth(), round_corner_type(), round_corner_radius());
					tool.UpdateInnerBuffer(m_inner_buffer.get());
					tool.UpdateOuterBuffer(m_outer_buffer.get());
					tool.UpdateEmbossBuffer(m_emboss_buffer.get());
					Refresh();
					break;
				}

				case WidgetRoundCornerType: {
					const int* type_p = static_cast<const int*>(request.data());
					UpdateTextPosition(size(), *type_p, round_corner_radius(), text());

					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(), *type_p, round_corner_radius());
					tool.UpdateInnerBuffer(m_inner_buffer.get());
					tool.UpdateOuterBuffer(m_outer_buffer.get());
					tool.UpdateEmbossBuffer(m_emboss_buffer.get());
					Refresh();
					break;
				}

				case WidgetRoundCornerRadius: {
					const int* radius_p = static_cast<const int*>(request.data());
					UpdateTextPosition(size(), round_corner_type(), *radius_p, text());

					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), *radius_p);
					tool.UpdateInnerBuffer(m_inner_buffer.get());
					tool.UpdateOuterBuffer(m_outer_buffer.get());
					tool.UpdateEmbossBuffer(m_emboss_buffer.get());
					Refresh();
					break;
				}

				default:
					break;
			}

	}

	ResponseType ToggleButton::Draw (const RedrawEvent& event)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		Theme* tm = Theme::instance;

		Color color;

		if (hover()) {
			if(checked()) {
				color = tm->regular().inner_sel + 15;
			} else {
				color = tm->regular().inner + 15;
			}
		} else {
			if (checked()) {
				color = tm->regular().inner_sel;
			} else {
				color = tm->regular().inner;
			}
		}

		program->SetVertexAttrib4fv("Color", color.data());
		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner_buffer.get());

		color = tm->regular().outline;

		program->SetVertexAttrib4fv("Color", color.data());
		program->SetUniform1i("AA", 1);

		DrawTriangleStrip(0, m_outer_buffer.get());

		program->SetVertexAttrib4f("Color", 1.0f, 1.0f, 1.0f, 0.02f);
		DrawTriangleStrip(0, m_emboss_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		if(text().size()) {
			font().Print(mvp, text(), text_length(), 0);
		}

		return Accept;
	}

	void ToggleButton::InitializeToggleButton ()
	{
		set_round_corner_type(RoundAll);
		set_checkable(true);

		int h = font().GetHeight();

		set_size(h + round_corner_radius() * 2 + DefaultButtonPadding().hsum(),
						h + DefaultButtonPadding().vsum());

		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());
		m_inner_buffer = tool.GenerateInnerBuffer();
		m_outer_buffer = tool.GenerateOuterBuffer();
		m_emboss_buffer = tool.GenerateEmbossBuffer();
	}

	void ToggleButton::InitializeToggleButton (const String& text)
	{
		set_round_corner_type(RoundAll);
		set_checkable(true);
		set_text(text);

		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_corner_radius() * 2 + DefaultButtonPadding().hsum(),
							h + DefaultButtonPadding().vsum());
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width() + round_corner_radius() * 2 + DefaultButtonPadding().hsum();
			int height = h + DefaultButtonPadding().vsum();

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}

		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());
		m_inner_buffer = tool.GenerateInnerBuffer();
		m_outer_buffer = tool.GenerateOuterBuffer();
		m_emboss_buffer = tool.GenerateEmbossBuffer();
	}

}
