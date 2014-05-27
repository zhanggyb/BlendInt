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

#include <cmath>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	ColorWheel::ColorWheel()
	: Widget(),
	  m_vao(0)
	{
		set_size(160, 160);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		std::vector<GLfloat> vertices;

		GenerateWheelVertices(80, &vertices);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();

		m_inner->Bind();
		m_inner->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		m_inner->Reset();

		glBindVertexArray(0);
	}
	
	ColorWheel::~ColorWheel ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	ResponseType ColorWheel::Draw (const RedrawEvent& event)
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

		glm::vec4 color(0.8, 0.2, 0.2, 1.0);

		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_inner.get());

		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}
	
	void ColorWheel::GenerateWheelVertices (int radius, std::vector<GLfloat>* vertices)
	{
		if(!vertices) return;

		vertices->resize(72 * 2 + 2);

		double rad = 0.0;
		float x = 0.f;
		float y = 0.f;

		(*vertices)[0] = 0.f;
		(*vertices)[1] = 0.f;
		for(int i = 0; i < 72; i++) {
			rad = radius * i * 5 * M_PI / 180.0;

			x = cos(rad);
			y = sin(rad);

			(*vertices)[(i + 1) * 2] = x;
			(*vertices)[(i + 1) * 2 + 1] = y;
		}
	}

}
