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

// vec3, vec4, ivec4, mat4
#include <glm/glm.hpp>
// translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Grid.hpp>

namespace BlendInt {

	Grid::Grid ()
	: m_size(10), m_step(10)
	{
	}

	void Grid::Render (const glm::mat4& MVP)
	{
		if(program()) {
			program()->Activate();
			glUniform4fv(program()->GetUniformLocation("MVP"), 1, glm::value_ptr(MVP));
		}

		if(vertices()) {
			vertices()->bind();
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_INT, 0, BUFFER_OFFSET(0));
			glDrawArrays(GL_LINES, 0, vertices()->vertices());
			glDisableClientState(GL_VERTEX_ARRAY);
			vertices()->unbind();
		}

		if(program()) {
			program()->Deactivate();
		}
	}

	Grid::~Grid ()
	{
	}

	void Grid::InitOnce()
	{
		GLArrayBuffer* buffer = new GLArrayBuffer;
		GLint vertices[11][3];

		for(int i = 0; i < 11; i++) 	{
			vertices[i][0] = i - 5;
			vertices[i][1] = i - 5;
			vertices[i][2] = 0;
		}

		buffer->Generate();
		buffer->bind();
		buffer->set_data(11, sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
		buffer->unbind();

		SetVertexBuffer(buffer);
	}

}
