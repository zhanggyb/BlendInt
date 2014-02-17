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

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Cube.hpp>

#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BlendInt {

	Cube::Cube()
	: AbstractPrimitive(), m_vbo_cube_vertices(0),
	m_vbo_cube_colors(0), m_ibo_cube_elements(0),
	m_attribute_coord3d (0), m_attribute_v_color(0), m_uniform_mvp(0)
	{
		InitOnce();
	}

	void Cube::Render (const glm::mat4& mvp)
	{
		if (program()) {

			program()->Use();

			glUniformMatrix4fv(m_uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

			glEnableVertexAttribArray(m_attribute_coord3d);
			// Describe our vertices array to OpenGL (it can't guess its format automatically)
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cube_vertices);
			glVertexAttribPointer(m_attribute_coord3d, // attribute
			        3,            // number of elements per vertex, here (x,y,z)
			        GL_FLOAT,          // the type of each element
			        GL_FALSE,          // take our values as-is
			        0,                 // no extra data between each position
			        0                  // offset of first element
			        );

			glEnableVertexAttribArray(m_attribute_v_color);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cube_colors);
			glVertexAttribPointer(m_attribute_v_color, // attribute
			        3,            // number of elements per vertex, here (R,G,B)
			        GL_FLOAT,          // the type of each element
			        GL_FALSE,          // take our values as-is
			        0,                 // no extra data between each position
			        0                  // offset of first element
			        );

			/* Push each element in buffer_vertices to the vertex shader */
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_cube_elements);

			int size;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE,
			        &size);
			glDrawElements(GL_TRIANGLES, size / sizeof(GLushort),
			        GL_UNSIGNED_SHORT, 0);

			glDisableVertexAttribArray(m_attribute_v_color);
			glDisableVertexAttribArray(m_attribute_coord3d);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			program()->Reset();

		} else {
			std::cout << "program is not valid" << std::endl;
		}
	}

	int Cube::InitOnce()
	{
		GLfloat cube_vertices[] = {
				// front
				-1.0, -1.0,  1.0,
				1.0, -1.0,  1.0,
				1.0,  1.0,  1.0,
				-1.0,  1.0,  1.0,
				// back
				-1.0, -1.0, -1.0,
				1.0, -1.0, -1.0,
				1.0,  1.0, -1.0,
				-1.0,  1.0, -1.0,
		};

		glGenBuffers(1, &m_vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

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

		SetProgram(ShaderManager::Instance()->primitive_program());
		program()->Use();

		m_attribute_coord3d = program()->GetAttributeLocation("coord3d");
		m_attribute_v_color = program()->GetAttributeLocation("v_color");
		m_uniform_mvp = program()->GetUniformLocation("ModelViewProjectionMatrix");

		program()->Reset();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}

	Cube::~Cube()
	{
		glDeleteBuffers(1, &m_vbo_cube_vertices);
		glDeleteBuffers(1, &m_vbo_cube_colors);
		glDeleteBuffers(1, &m_ibo_cube_elements);
	}

}
