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

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	Button::Button ()
		: AbstractRoundButton(), m_vao(0)
	{
		InitializeButton();
	}

	Button::Button (const String& text)
		: AbstractRoundButton(), m_vao(0)
	{
		InitializeButton(text);
	}

	Button::~Button ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Button::Update(const UpdateRequest& request)
	{
		if(request.source() == Predefined) {
			switch (request.type()) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(*size_p,
								   round_type(),
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   m_emboss_buffer.get());
				glBindVertexArray(0);
				Refresh();
				break;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   *type_p,
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   m_emboss_buffer.get());
				glBindVertexArray(0);
				Refresh();
				break;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   round_type(),
								   *radius_p,
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   m_emboss_buffer.get());
				glBindVertexArray(0);
				Refresh();
				break;
			}

			default:
				AbstractButton::Update(request);
			}

		}
	}

	ResponseType Button::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);

		Theme* tm = Theme::instance;

		glm::vec4 color;

		// draw inner, simple fill
		if(down()) {
			color.r = tm->regular().inner_sel.r() / 255.f;
			color.g = tm->regular().inner_sel.g() / 255.f;
			color.b = tm->regular().inner_sel.b() / 255.f;
			color.a = tm->regular().inner_sel.a() / 255.f;
		} else {
			if(hover()) {
				color.r = tm->regular().inner.highlight_red() / 255.f;
				color.g = tm->regular().inner.highlight_green() / 255.f;
				color.b = tm->regular().inner.highlight_blue() / 255.f;
				color.a = tm->regular().inner.a() / 255.f;
			} else {
				color.r = tm->regular().inner.r() / 255.f;
				color.g = tm->regular().inner.g() / 255.f;
				color.b = tm->regular().inner.b() / 255.f;
				color.a = tm->regular().inner.a() / 255.f;
			}
		}

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner_buffer.get());

		color.r = tm->regular().outline.r() / 255.f;
		color.g = tm->regular().outline.g() / 255.f;
		color.b = tm->regular().outline.b() / 255.f;
		color.a = tm->regular().outline.a() / 255.f;

		program->SetUniform1i("AA", 1);

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		DrawTriangleStrip(0, m_outer_buffer.get());

		program->SetVertexAttrib4f("Color", 1.0f, 1.0f, 1.0f, 0.02f);
		DrawTriangleStrip(0, m_emboss_buffer.get());

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		if(text().size()) {
			font().Print(mvp, origin().x(), origin().y(), text(), text_length(), 0);
		}

		return Accept;
	}

	void Button::InitializeButton ()
	{
		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(90, 20);
		set_preferred_size(90, 20);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_emboss_buffer.reset(new GLArrayBuffer);

		GenerateFormBuffer(
						size(),
						round_type(),
						radius(),
						m_inner_buffer.get(),
						m_outer_buffer.get(),
						m_emboss_buffer.get());

		glBindVertexArray(0);
	}

	void Button::InitializeButton (const String& text)
	{
		set_round_type(RoundAll);
		set_expand_x(true);
		set_size(90, 20);
		set_preferred_size(90, 20);
		set_text(text);

		bool cal_width = true;
		set_text_outline(font().get_text_outline(text));

		set_text_length(text.length());

		if(size().height() < text_outline().height()) {
			if(expand_y()) {
				set_size(size().width(), text_outline().height());
			} else {
				set_text_length(0);
				cal_width = false;
			}
		}

		if(size().width() < text_outline().width()) {
			if(expand_x()) {
				set_size(text_outline().width(), size().height());
			} else {
				if(cal_width) set_text_length(GetValidTextSize());
			}
		}

		set_origin((size().width() - text_outline().width()) / 2,
				(size().height() - font().get_height()) / 2 + std::abs(font().get_descender()));

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_emboss_buffer.reset(new GLArrayBuffer);

		GenerateFormBuffer(
						size(),
						round_type(),
						radius(),
						m_inner_buffer.get(),
						m_outer_buffer.get(),
						m_emboss_buffer.get());

		glBindVertexArray(0);
	}

}

