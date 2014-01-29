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

#include <BlendInt/Cube.hpp>

namespace BlendInt {

	Cube::Cube()
	: AbstractPrimitive(), m_vb(0), m_cb(0), m_ib(0)
	{

	}

	void Cube::Render(const glm::mat4& mvp)
	{
		if(program()) {
			program()->Activate();
		}



		if(program()) {
			program()->Deactivate();
		}
	}

	void Cube::InitOnce()
	{
		GLfloat cube_vertices[] = {
		        // front
		        -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
		        // back
		        -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
		        -1.0, };

		m_vb = new GLArrayBuffer;
		Retain(m_vb);

		m_vb->Generate();
		m_vb->bind();
		m_vb->set_data(8, sizeof(GLfloat) * 3, cube_vertices);
		m_vb->unbind();

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

		m_cb = new GLArrayBuffer;
		Retain(m_cb);

		m_cb->Generate();
		m_cb->bind();
		m_cb->set_data(8, sizeof(GLfloat) * 3, cube_colors);
		m_cb->unbind();

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

		m_ib = new GLElementArrayBuffer;
		Retain(m_ib);

		m_ib->Generate();
		m_ib->bind();
		m_ib->set_data(12, sizeof(GLushort) * 3, cube_elements);
		m_ib->unbind();
	}

	Cube::~Cube()
	{
		Destroy(m_vb);
		Destroy(m_cb);
		Destroy(m_ib);
	}

}
