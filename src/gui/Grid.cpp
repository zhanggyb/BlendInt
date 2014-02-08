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
#include <iostream>

// vec3, vec4, ivec4, mat4
#include <glm/glm.hpp>
// translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Grid.hpp>
#include <BlendInt/GLSLProgram.hpp>

namespace BlendInt {

	const char* Grid::vertex_shader =
			"attribute vec2 coord2d;"
			"uniform mat4 ModelViewProjectionMatrix;"
			""
			"void main(void) {"
			"	gl_Position = ModelViewProjectionMatrix * vec4(coord2d, 0, 1);"
			"}";

	const char* Grid::fragment_shader =
			"void main(void) {"
			"	gl_FragColor = vec4(0.55, 0.55, 0.55, 0.65);"
			"}";

	Grid::Grid ()
	: AbstractPrimitive(), m_size(10), m_step(10), m_vb(0), m_ib(0),
	  m_uniform_mvp(0)
	{
		InitOnce();
	}

	void Grid::SetSize (int size)
	{
		m_size = size;

//		Update ();
	}

	void Grid::Update()
	{
//		int points_one_line = 2 * m_size + 1;
//
//		int* p = new int[points_one_line * points_one_line * 3];
//
//		for(int i = 0; i < points_one_line; i++)
//		{
//			for(int j = 0; j < points_one_line; j++)
//			{
//				*(p + i * points_one_line + j) = j;
//				*(p + i * points_one_line + j + 1) = j;
//				*(p + i * points_one_line + j + 2) = j;
//			}
//		}
//
//		delete p;
	}

	void Grid::Render (const glm::mat4& mvp)
	{
		if(program()) {

			program()->Use();

			glUniformMatrix4fv(m_uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

			/* Draw the grid using the indices to our vertices using our vertex buffer objects */
			glEnableVertexAttribArray(m_attribute_coord2d);

			m_vb->bind();
			glVertexAttribPointer(m_attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

			/* Push each element in buffer_vertices to the vertex shader */
			m_ib->bind();

			glDrawElements(GL_LINES, 20 * 21 * 4,
			        GL_UNSIGNED_SHORT, 0);

			glDisableVertexAttribArray(m_attribute_coord2d);

			m_ib->Reset();
			m_vb->Reset();

			program()->Reset();
		}
	}

	Grid::~Grid ()
	{
		Destroy(m_ib);
		Destroy(m_vb);
	}

	void Grid::InitOnce()
	{
		GLSLProgram* prog = new GLSLProgram;
		prog->Create();
		prog->AttachShaderPair(vertex_shader, fragment_shader);
		prog->Link();

		SetProgram(prog);

		program()->Use();

		m_attribute_coord2d = program()->GetAttributeLocation("coord2d");
		m_uniform_mvp = program()->GetUniformLocation("ModelViewProjectionMatrix");
		assert((m_attribute_coord2d != -1) && (m_uniform_mvp != -1));

		program()->Reset();

		glm::vec2 vertices[21][21];
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 21; j++) {
				vertices[i][j].x = (j - 10) / 1.f;
				vertices[i][j].y = (i - 10) / 1.f;
			}
		}

		m_vb = new GLArrayBuffer;
		Retain(m_vb);
		m_vb->Generate();
		m_vb->bind();

		m_vb->set_data(21 * 21, sizeof(glm::vec2), vertices);
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

		m_ib = new GLElementArrayBuffer;
		Retain(m_ib);

		m_ib->Generate();
		m_ib->bind();
		m_ib->set_data(20 * 21 * 4, sizeof(GLushort), indices);
		m_ib->Reset();

		Update ();
	}

}
