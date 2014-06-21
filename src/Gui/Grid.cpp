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

#include <math.h>
#include <assert.h>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

// vec3, vec4, ivec4, mat4
#include <glm/glm.hpp>
// translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/Gui/Grid.hpp>

namespace BlendInt {

	const char* Grid::vertex_shader =
			"#version 330\n"
			"layout(location = 0) in vec2 coord2d;"
			"uniform mat4 m_P;"
			"uniform mat4 m_V;"
			"uniform mat4 m_M;"
			""
			"out vec3 f_color;"
			""
			"void main(void) {"
			"	f_color = vec3(0.35, 0.35, 0.35);"
			""
			"	gl_Position = m_P * m_V * m_M * vec4(coord2d, 0.0, 1.0);"
			"}";

	const char* Grid::fragment_shader =
			"#version 330\n"
			"in vec3 f_color;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	FragmentColor = vec4(f_color, 0.6);"
			"}";

	Grid::Grid ()
	: AbstractPrimitive(),
	  m_vao(0),
	  m_size(10),
	  m_step(10),
	  m_vb(0),
	  m_ib(0)
	{
		InitializeGrid();
	}

	void Grid::SetSize (int size)
	{
		m_size = size;
	}

	void Grid::Render (const glm::mat4& projection_matrix, const glm::mat4& view_matrix)
	{
		glBindVertexArray(m_vao);
		m_program->Use();
		m_program->SetUniformMatrix4fv("m_P", 1, GL_FALSE, glm::value_ptr(projection_matrix));
		m_program->SetUniformMatrix4fv("m_V", 1, GL_FALSE, glm::value_ptr(view_matrix));
		m_program->SetUniformMatrix4fv("m_M", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

		/* Draw the grid using the indices to our vertices using our vertex buffer objects */
		glEnableVertexAttribArray(0);

		m_vb->Bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		/* Push each element in buffer_vertices to the vertex shader */
		m_ib->Bind();

		glDrawElements(GL_LINES, 20 * 21 * 4,
		GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(0);

		m_ib->Reset();
		m_vb->Reset();
		m_program->Reset();
		glBindVertexArray(0);
	}

	Grid::~Grid ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Grid::InitializeGrid()
	{
		glGenVertexArrays(1, &m_vao);

		m_program.reset(new GLSLProgram);
		m_program->Create();
		m_program->AttachShaderPair(vertex_shader, fragment_shader);
		m_program->Link();

		glm::vec2 vertices[21][21];
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 21; j++) {
				vertices[i][j].x = (j - 10) / 1.f;
				vertices[i][j].y = (i - 10) / 1.f;
			}
		}

		m_vb.reset(new GLArrayBuffer);
		m_vb->Generate();
		m_vb->Bind();

		m_vb->SetData(21 * 21 * sizeof(glm::vec2), vertices);
		m_vb->Reset();

		// Create an array of indices into the vertex array that traces both horizontal and vertical lines
		GLushort indices[20 * 21 * 4];
		int i = 0;

		for (int y = 0; y < 21; y++) {
			for (int x = 0; x < 20; x++) {
				indices[i++] = y * 21 + x;
				indices[i++] = y * 21 + x + 1;
			}
		}

		for (int x = 0; x < 21; x++) {
			for (int y = 0; y < 20; y++) {
				indices[i++] = y * 21 + x;
				indices[i++] = (y + 1) * 21 + x;
			}
		}

		m_ib.reset(new GLElementArrayBuffer);

		m_ib->Generate();
		m_ib->Bind();
		m_ib->SetData(20 * 21 * 4, sizeof(GLushort), indices);
		m_ib->Reset();
	}

}
