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

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opengl/glsl-program.hpp>

#include <gui/cube.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Cube::Cube ()
	: AbstractPrimitive(),
	  m_vao(0),
	  m_vbo_cube_vertices(0),
	  m_vbo_cube_colors(0),
	  m_ibo_cube_elements(0)
	{
		InitializeCube();
	}

	Cube::~Cube()
	{
		glDeleteBuffers(1, &m_vbo_cube_vertices);
		glDeleteBuffers(1, &m_vbo_cube_colors);
		glDeleteBuffers(1, &m_ibo_cube_elements);
		glDeleteVertexArrays(1, &m_vao);
	}

	void Cube::Render (const glm::mat4& projection_matrix, const glm::mat4& view_matrix)
	{
		RefPtr<GLSLProgram> program = AbstractWindow::shaders->primitive_program();

		program->use();
		program->SetUniformMatrix4fv("m_P", 1, GL_FALSE, glm::value_ptr(projection_matrix));
		program->SetUniformMatrix4fv("m_V", 1, GL_FALSE, glm::value_ptr(view_matrix));
		program->SetUniformMatrix4fv("m_M", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

		/* Push each element in buffer_vertices to the vertex shader */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_cube_elements);

		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		program->reset();
	}

	void Cube::InitializeCube()
	{
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		GLfloat cube_vertices[] = {
				// front
				-1.0, -1.0,  1.0, 1.0,
				1.0, -1.0,  1.0, 1.0,
				1.0,  1.0,  1.0, 1.0,
				-1.0,  1.0,  1.0, 1.0,
				// back
				-1.0, -1.0, -1.0, 1.0,
				1.0, -1.0, -1.0, 1.0,
				1.0,  1.0, -1.0, 1.0,
				-1.0,  1.0, -1.0, 1.0
		};

		glGenBuffers(1, &m_vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		GLfloat cube_colors[] = {
				// front colors
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0,
				1.0, 1.0, 1.0,
				// back colors
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0,
				1.0, 1.0, 1.0,
		};

		glGenBuffers(1, &m_vbo_cube_colors);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cube_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		GLushort cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// top
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// bottom
				4, 0, 3,
				3, 7, 4,
				// left
				4, 5, 1,
				1, 0, 4,
				// right
				3, 2, 6,
				6, 7, 3,
		};

		glGenBuffers(1, &m_ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
