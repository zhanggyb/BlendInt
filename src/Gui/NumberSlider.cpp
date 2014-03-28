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

#include <BlendInt/Gui/NumberSlider.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	NumberSlider::NumberSlider (Orientation orientation)
	: AbstractSlider(orientation), m_vao(0)
	{
		InitOnce ();
	}

	NumberSlider::~NumberSlider ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	bool NumberSlider::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {
			switch (request.type()) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(
								*size_p,
								round_type(),
								radius(),
								m_inner_buffer.get(),
								m_outer_buffer.get(),
								0);
				glBindVertexArray(0);
				Refresh();
				return true;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   *type_p,
								   radius(),
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);
				glBindVertexArray(0);
				Refresh();
				return true;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(request.data());
				glBindVertexArray(m_vao);
				GenerateFormBuffer(size(),
								   round_type(),
								   *radius_p,
								   m_inner_buffer.get(),
								   m_outer_buffer.get(),
								   0);
				glBindVertexArray(0);
				Refresh();
				return true;
			}

			default:
				return AbstractSlider::Update(request);
			}

		} else {
			return false;
		}
	}
	
	ResponseType NumberSlider::Draw (const RedrawEvent& event)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		ThemeManager* tm = ThemeManager::instance();

		glm::vec4 color;

		color.r = tm->themes()->number_slider.inner_sel.r() / 255.f;
		color.g = tm->themes()->number_slider.inner_sel.g() / 255.f;
		color.b = tm->themes()->number_slider.inner_sel.b() / 255.f;
		color.a = tm->themes()->number_slider.inner_sel.a() / 255.f;

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner_buffer.get());

		color.r = themes()->number_slider.outline.r() / 255.f;
		color.g = themes()->number_slider.outline.g() / 255.f;
		color.b = themes()->number_slider.outline.b() / 255.f;
		color.a = themes()->number_slider.outline.a() / 255.f;
		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
		program->SetUniform1i("AA", 1);

		DrawTriangleStrip(0, m_outer_buffer.get());

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}

	void NumberSlider::InitOnce()
	{
		set_round_type(RoundAll);
		set_expand_x(true);
		set_radius(10.f);
		set_size(90, 20);
		set_preferred_size(90, 20);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		GenerateFormBuffer(
						size(),
						round_type(),
						radius(),
						m_inner_buffer.get(),
						m_outer_buffer.get(),
						0);

		glBindVertexArray(0);
	}
}
