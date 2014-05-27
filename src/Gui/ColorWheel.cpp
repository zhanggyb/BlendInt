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

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		m_inner->Bind();

		glVertexAttribPointer(0, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(0)					 // offset of first element
							  );

		glVertexAttribPointer(1, // attribute
							  4,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(2 * sizeof(GLfloat))					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0, m_inner->GetBufferSize() / (6 * sizeof(GLfloat)));

		m_inner->Reset();

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}
	
	void ColorWheel::GenerateWheelVertices (int radius, std::vector<GLfloat>* vertices)
	{
		if(!vertices) return;

		vertices->resize(72 * 6 + 6 + 6);

		double rad = 0.0;
		float x = 0.f;
		float y = 0.f;

		// 0 1 2 3 4 5
		// x y r g b a

		// the center point
		(*vertices)[0] = 0.f;
		(*vertices)[1] = 0.f;
		(*vertices)[2] = 1.f;
		(*vertices)[3] = 1.f;
		(*vertices)[4] = 1.f;
		(*vertices)[5] = 1.f;

		int i = 0;
		for(int j = -30; j < 330; j = j + 5) {
			rad = j * M_PI / 180.0;

			x = radius * cos(rad);
			y = radius * sin(rad);

			(*vertices)[(i + 1) * 6 + 0] = x;
			(*vertices)[(i + 1) * 6 + 1] = y;

			if(j == -30) {
				(*vertices)[(i + 1) * 6 + 2] = 1.f;
				(*vertices)[(i + 1) * 6 + 3] = 0.f;
				(*vertices)[(i + 1) * 6 + 4] = 1.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j < 30) {
				(*vertices)[(i + 1) * 6 + 2] = (30 - j) / 60.f;
				(*vertices)[(i + 1) * 6 + 3] = 0.f;
				(*vertices)[(i + 1) * 6 + 4] = 1.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j == 30) {
				(*vertices)[(i + 1) * 6 + 2] = 0.f;
				(*vertices)[(i + 1) * 6 + 3] = 0.f;
				(*vertices)[(i + 1) * 6 + 4] = 1.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j < 90) {
				(*vertices)[(i + 1) * 6 + 2] = 0.f;
				(*vertices)[(i + 1) * 6 + 3] = 1.f - (90 - j) / 60.f;
				(*vertices)[(i + 1) * 6 + 4] = 1.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j == 90) {
				(*vertices)[(i + 1) * 6 + 2] = 0.f;
				(*vertices)[(i + 1) * 6 + 3] = 1.f;
				(*vertices)[(i + 1) * 6 + 4] = 1.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j < 150) {
				(*vertices)[(i + 1) * 6 + 2] = 0.f;
				(*vertices)[(i + 1) * 6 + 3] = 1.f;
				(*vertices)[(i + 1) * 6 + 4] = (150 - j) / 60.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j == 150) {
				(*vertices)[(i + 1) * 6 + 2] = 0.f;
				(*vertices)[(i + 1) * 6 + 3] = 1.f;
				(*vertices)[(i + 1) * 6 + 4] = 0.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j < 210) {
				(*vertices)[(i + 1) * 6 + 2] = 1.f - (210 - j) / 60.f;
				(*vertices)[(i + 1) * 6 + 3] = 1.f;
				(*vertices)[(i + 1) * 6 + 4] = 0.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j == 210) {
				(*vertices)[(i + 1) * 6 + 2] = 1.f;
				(*vertices)[(i + 1) * 6 + 3] = 1.f;
				(*vertices)[(i + 1) * 6 + 4] = 0.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j < 270) {
				(*vertices)[(i + 1) * 6 + 2] = 1.f;
				(*vertices)[(i + 1) * 6 + 3] = (270 - j) / 60.f;
				(*vertices)[(i + 1) * 6 + 4] = 0.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else if (j == 270) {
				(*vertices)[(i + 1) * 6 + 2] = 1.f;
				(*vertices)[(i + 1) * 6 + 3] = 0.f;
				(*vertices)[(i + 1) * 6 + 4] = 0.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			} else {
				(*vertices)[(i + 1) * 6 + 2] = 1.f;
				(*vertices)[(i + 1) * 6 + 3] = 0.f;
				(*vertices)[(i + 1) * 6 + 4] = 1.f - (330 - j) / 60.f;
				(*vertices)[(i + 1) * 6 + 5] = 1.f;
			}
			i++;
		}

		rad = 330 * M_PI / 180.0;
		x = radius * cos(rad);
		y = radius * sin(rad);

		(*vertices)[(i + 1) * 6 + 0] = x;
		(*vertices)[(i + 1) * 6 + 1] = y;
		(*vertices)[(i + 1) * 6 + 2] = 1.f;
		(*vertices)[(i + 1) * 6 + 3] = 0.f;
		(*vertices)[(i + 1) * 6 + 4] = 1.f;
		(*vertices)[(i + 1) * 6 + 5] = 1.f;
	}

}
