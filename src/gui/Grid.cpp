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

// vec3, vec4, ivec4, mat4
#include <glm/glm.hpp>
// translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Grid.hpp>

namespace BlendInt {

	Grid::Grid ()
	: AbstractPrimitive(), m_size(10), m_step(10), m_vb(0)
	{
		InitOnce();
	}

	void Grid::SetSize (int size)
	{
		m_size = size;

		Update ();
	}

	void Grid::Update()
	{
		int points_one_line = 2 * m_size + 1;

		int* p = new int[points_one_line * points_one_line * 3];

		for(int i = 0; i < points_one_line; i++)
		{
			for(int j = 0; j < points_one_line; j++)
			{
				*(p + i * points_one_line + j) = j;
				*(p + i * points_one_line + j + 1) = j;
				*(p + i * points_one_line + j + 2) = j;
			}
		}

		delete p;
	}

	void Grid::Render (const glm::mat4& mvp)
	{
		if(program()) {
			program()->Use();
			glUniformMatrix4fv(program()->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		}

		m_vb->bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_INT, 0, BUFFER_OFFSET(0));
		glDrawArrays(GL_LINES, 0, m_vb->vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		m_vb->Reset();

		if(program()) {
			program()->Reset();
		}
	}

	Grid::~Grid ()
	{
		Destroy(m_vb);
	}

	void Grid::InitOnce()
	{
		m_vb = new GLArrayBuffer;
		Retain(m_vb);

		Update ();
	}

}
