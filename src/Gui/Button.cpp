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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	Button::Button ()
		: AbstractButton()
	{
		set_round_corner_type(RoundAll);
		set_drop_shadow(true);
		int h = font().GetHeight();
		set_size(h + round_corner_radius() * 2 + DefaultButtonPadding().hsum(),
						h + DefaultButtonPadding().vsum());

		InitializeButton();
	}

	Button::Button (const String& text)
		: AbstractButton()
	{
		InitializeButton(text);
	}

	Button::~Button ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void Button::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {
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

	ResponseType Button::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program =
						Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		if (down()) {
			program->SetVertexAttrib4fv("a_color",
							Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				Color color = Theme::instance->regular().inner + 15;
				program->SetVertexAttrib4fv("a_color", color.data());
			} else {
				program->SetVertexAttrib4fv("a_color",
								Theme::instance->regular().inner.data());
			}
		}

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_corner_type()) + 2);

		program->SetUniform1i("u_AA", 1);
		program->SetVertexAttrib4fv("a_color", Theme::instance->regular().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		if (emboss()) {
			program->SetVertexAttrib4f("a_color", 1.0f, 1.0f, 1.0f, 0.16f);

			program->SetUniform3f("u_position", (float) position().x(), (float) position().y() - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_corner_type()) * 2);
		}

		glBindVertexArray(0);
		program->Reset();

		if(text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		return Accept;
	}

	void Button::InitializeButton ()
	{
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

	void Button::InitializeButton (const String& text)
	{
		set_round_corner_type(RoundAll);
		set_drop_shadow(true);
		set_text(text);

		int left = DefaultButtonPadding().left() * Theme::instance->pixel();
		int right = DefaultButtonPadding().right() * Theme::instance->pixel();
		int top = DefaultButtonPadding().top() * Theme::instance->pixel();
		int bottom = DefaultButtonPadding().bottom() * Theme::instance->pixel();
		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_corner_radius() * 2 * Theme::instance->pixel() + left + right,
							h + top + bottom);
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width()
							+ round_corner_radius() * 2 * Theme::instance->pixel()
							+ left + right;
			int height = h + top + bottom;

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

