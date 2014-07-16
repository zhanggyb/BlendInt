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
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ColorButton.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	ColorButton::ColorButton ()
		: AbstractButton()
	{
		InitializeColorButton();
	}

	ColorButton::~ColorButton ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void ColorButton::SetColor(const Color& color)
	{
		m_color = color;
		Refresh();
	}

	void ColorButton::ProcessSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(), round_radius(),
			        text());
			VertexTool tool;
			tool.Setup(*request.size(), DefaultBorderWidth(), round_type(),
			        round_radius());
			m_inner_buffer->Bind();
			tool.SetInnerBufferData(m_inner_buffer.get());
			m_outer_buffer->Bind();
			tool.SetOuterBufferData(m_outer_buffer.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void ColorButton::ProcessRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if (request.target() == this) {
			UpdateTextPosition(size(), *request.round_type(), round_radius(),
			        text());
			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			m_inner_buffer->Bind();
			tool.SetInnerBufferData(m_inner_buffer.get());
			m_outer_buffer->Bind();
			tool.SetOuterBufferData(m_outer_buffer.get());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void ColorButton::ProcessRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.target() == this) {
			UpdateTextPosition(size(), round_type(), *request.round_radius(),
			        text());
			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(), round_type(),
			        *request.round_radius());
			m_inner_buffer->Bind();
			tool.SetInnerBufferData(m_inner_buffer.get());
			m_outer_buffer->Bind();
			tool.SetOuterBufferData(m_outer_buffer.get());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType ColorButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;
		int outline_vertices = GetOutlineVertices(round_type());

		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4fv("a_color", m_color.data());

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertices + 2);

		program->SetUniform1i("u_AA", 1);
		program->SetVertexAttrib4fv("a_color", Theme::instance->regular().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertices * 2 + 2);

		if (emboss()) {
			program->SetVertexAttrib4f("a_color", 1.0f, 1.0f, 1.0f, 0.16f);

			program->SetUniform3f("u_position", (float) position().x(), (float) position().y() - 1.f, 0.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->Reset();

		if(text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		return Accept;
	}

	void ColorButton::InitializeColorButton ()
	{
		set_round_type(RoundAll);

		int h = font().GetHeight();

		set_size(h + round_radius() * 2 + DefaultButtonPadding().hsum(),
						h + DefaultButtonPadding().vsum());

		VertexTool tool;
		tool.Setup (size(), DefaultBorderWidth(), round_type(), round_radius());

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
