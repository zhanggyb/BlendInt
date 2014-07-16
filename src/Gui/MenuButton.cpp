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
#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

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
	
	void MenuButton::ProcessSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(*request.size(), round_type(),
			        round_radius(), text());
			VertexTool tool;
			tool.Setup(*request.size(), DefaultBorderWidth(),
			        round_type(), round_radius());
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void MenuButton::ProcessRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), *request.round_type(), round_radius(),
			        text());
			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void MenuButton::ProcessRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			UpdateTextPosition(size(), round_type(), *request.round_radius(),
			        text());
			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(),
			        round_type(), *request.round_radius());
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType MenuButton::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		if (hover()) {

			RefPtr<GLSLProgram> program =
					Shaders::instance->default_triangle_program();
			program->Use();

			program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
			program->SetUniform1i("u_gamma", 0);
			program->SetUniform1i("u_AA", 1);

			program->SetVertexAttrib4fv("a_color", Theme::instance->menu_item().inner_sel.data());

			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
			glBindVertexArray(0);

			program->Reset();

		}

		if(text().size()) {
			font().Print(position(), text(), text_length(), 0);
		}

		return Accept;
	}

	void MenuButton::SetMenu (const RefPtr<Menu>& menu)
	{
		m_menu = menu;
	}

	void MenuButton::InitializeMenuButton (const String& text)
	{
		set_round_type(RoundAll);
		set_text(text);

		int h = font().GetHeight();

		if(text.empty()) {
			set_size(h + round_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right(),
							h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom());
		} else {
			set_text_length(text.length());
			Rect text_outline = font().GetTextOutline(text);

			int width = text_outline.width() + round_radius() * 2 + DefaultButtonPadding().left() + DefaultButtonPadding().right();
			int height = h + DefaultButtonPadding().top() + DefaultButtonPadding().bottom();

			set_size(width, height);

			set_pen((width - text_outline.width()) / 2,
							(height - font().GetHeight()) / 2
											+ std::abs(font().GetDescender()));
		}

		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_type(), round_radius());

		glBindVertexArray(m_vao);
		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

} /* namespace BlendInt */
