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

#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/Gui/Grid.hpp>

namespace BlendInt {

	const char* Grid::vertex_shader =
			"attribute vec2 coord2d;"
			"uniform mat4 ModelViewProjectionMatrix;"
			// TODO: define grid color
			"varying vec3 f_color;"
			""
			"void main(void) {"
			"		f_color = vec3(0.35, 0.35, 0.35);"
			""
			"	gl_Position = ModelViewProjectionMatrix * vec4(coord2d, 0, 1);"
			"}";

	const char* Grid::fragment_shader =
			"varying vec3 f_color;"
			""
			"void main(void) {"
			"	gl_FragColor = vec4(f_color, 0.6);"
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

		//Update ();
	}

	void Grid::Update()
	{

	}

	void Grid::Render (const glm::mat4& mvp)
	{
		if(program()) {

			program()->Use();

			glUniformMatrix4fv(m_uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

			/* Draw the grid using the indices to our vertices using our vertex buffer objects */
			glEnableVertexAttribArray(m_attribute_coord2d);

			m_vb->Bind();
			glVertexAttribPointer(m_attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

			/* Push each element in buffer_vertices to the vertex shader */
			m_ib->Bind();

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
		RefPtr<GLSLProgram> prog(new GLSLProgram);
		prog->Create();
		prog->AttachShaderPair(vertex_shader, fragment_shader);
		prog->Link();

		set_program(prog);

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
		m_vb->Bind();

		m_vb->SetData(21 * 21, sizeof(glm::vec2), vertices);
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
		m_ib->Bind();
		m_ib->SetData(20 * 21 * 4, sizeof(GLushort), indices);
		m_ib->Reset();

		Update ();
	}

}
