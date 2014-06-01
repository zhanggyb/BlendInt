/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Dot.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Dot::Dot ()
	: AbstractRoundForm(), m_vao(0)
	{
		set_round_type(RoundAll);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		glBindVertexArray(0);
	}

	Dot::~Dot ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Dot::Resize(unsigned int radius)
	{
		Size dot_size (radius * 2, radius * 2);
		set_round_type(RoundAll);
		set_radius(radius);
		set_size(dot_size);

		GenerateFormBuffer(dot_size, RoundAll, radius, m_inner_buffer.get(), m_outer_buffer.get(), 0);
	}

	void Dot::UpdateGeometry (const UpdateRequest& request)
	{
		switch (request.type()) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				GenerateFormBuffer(*size_p, round_type(), radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);
				break;
			}

			case FormRoundType: {
				const int* round_p =
								static_cast<const int*>(request.data());
				GenerateFormBuffer(size(), *round_p, radius(), m_inner_buffer.get(), m_outer_buffer.get(), 0);
				glBindVertexArray(0);
				break;
			}

			case FormRoundRadius: {
				const float* radius_p =
								static_cast<const float*>(request.data());
				GenerateFormBuffer(size(), round_type(), *radius_p, m_inner_buffer.get(), m_outer_buffer.get(), 0);
				break;
			}

			default:
				break;
		}

	}

	void Dot::Draw (const glm::mat4& mvp)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 1.f, 1.f, 1.f, 1.f);

		glEnableVertexAttribArray(0);

		m_inner_buffer->Bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						m_inner_buffer->GetBufferSize()
										/ (2 * sizeof(GLfloat)));

		program->SetVertexAttrib4fv("Color", Theme::instance->scroll().outline.data());
		program->SetUniform1i("AA", 1);

		m_outer_buffer->Bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
						m_outer_buffer->GetBufferSize()
						/ (2 * sizeof(GLfloat)));
		m_outer_buffer->Reset();

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);
	}

}
