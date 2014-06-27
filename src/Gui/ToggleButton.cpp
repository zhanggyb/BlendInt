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

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	ToggleButton::ToggleButton ()
	: AbstractButton()
	{
		InitializeToggleButton();
	}

	ToggleButton::ToggleButton (const String& text)
	: AbstractButton()
	{
		InitializeToggleButton(text);
	}

	ToggleButton::~ToggleButton ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	bool ToggleButton::IsExpandX () const
	{
		return true;
	}

	void ToggleButton::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if (request.target() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
					        static_cast<const Size*>(request.data());
					UpdateTextPosition(*size_p, round_corner_type(),
					        round_corner_radius(), text());
					VertexTool tool;
					tool.Setup(*size_p, DefaultBorderWidth(),
					        round_corner_type(), round_corner_radius());
					m_inner_buffer->Bind();
					tool.SetInnerBufferData(m_inner_buffer.get());
					m_outer_buffer->Bind();
					tool.SetOuterBufferData(m_outer_buffer.get());

					set_size(*size_p);
					Refresh();
					break;
				}

				case WidgetRoundCornerType: {
					const int* type_p = static_cast<const int*>(request.data());
					UpdateTextPosition(size(), *type_p, round_corner_radius(),
					        text());
					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(), *type_p,
					        round_corner_radius());
					m_inner_buffer->Bind();
					tool.SetInnerBufferData(m_inner_buffer.get());
					m_outer_buffer->Bind();
					tool.SetOuterBufferData(m_outer_buffer.get());

					set_round_corner_type(*type_p);
					Refresh();
					break;
				}

				case WidgetRoundCornerRadius: {
					const float* radius_p =
					        static_cast<const float*>(request.data());
					UpdateTextPosition(size(), round_corner_type(), *radius_p,
					        text());
					VertexTool tool;
					tool.Setup(size(), DefaultBorderWidth(),
					        round_corner_type(), *radius_p);
					m_inner_buffer->Bind();
					tool.SetInnerBufferData(m_inner_buffer.get());
					m_outer_buffer->Bind();
					tool.SetOuterBufferData(m_outer_buffer.get());

					set_round_corner_radius(*radius_p);
					Refresh();
					break;
				}

				default:
					break;
			}
		}

		ReportGeometryUpdate(request);
	}

	ResponseType ToggleButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		if (hover()) {
			Color color;
			if (checked()) {
				color = Theme::instance->regular().inner_sel + 15;
			} else {
				color = Theme::instance->regular().inner + 15;
			}
			program->SetVertexAttrib4fv("Color", color.data());
		} else {
			if (checked()) {
				program->SetVertexAttrib4fv("Color",
								Theme::instance->regular().inner_sel.data());
			} else {
				program->SetVertexAttrib4fv("Color",
								Theme::instance->regular().inner.data());
			}
		}

		program->SetUniform1i("AA", 0);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_corner_type()) + 2);

		program->SetUniform1i("AA", 1);
		program->SetVertexAttrib4fv("Color", Theme::instance->regular().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		if (emboss()) {
			program->SetVertexAttrib4f("Color", 1.0f, 1.0f, 1.0f, 0.16f);

			glm::mat4 emboss_mvp = glm::translate(mvp,
							glm::vec3(0.f, -1.f, 0.f));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
							glm::value_ptr(emboss_mvp));
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_corner_type()) * 2);
		}

		glBindVertexArray(0);
		program->Reset();

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

		VertexTool tool;
		tool.Setup (size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());

		glGenVertexArrays(2, m_vao);
		glBindVertexArray(m_vao[0]);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_inner_buffer->Generate();
		m_inner_buffer->Bind();
		tool.SetInnerBufferData(m_inner_buffer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_outer_buffer->Generate();
		m_outer_buffer->Bind();
		tool.SetOuterBufferData(m_outer_buffer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
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

		VertexTool tool;
		tool.Setup (size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());

		glGenVertexArrays(2, m_vao);
		glBindVertexArray(m_vao[0]);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_inner_buffer->Generate();
		m_inner_buffer->Bind();
		tool.SetInnerBufferData(m_inner_buffer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_outer_buffer->Generate();
		m_outer_buffer->Bind();
		tool.SetOuterBufferData(m_outer_buffer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

}
