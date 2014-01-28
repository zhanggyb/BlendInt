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

#include <BlendInt/Mesh.hpp>

namespace BlendInt {

	Mesh::Mesh ()
	: Object(), m_vertices(0), m_index(0), m_program(0)
	{
	}

	void Mesh::Render (const glm::mat4& MVP)
	{
	}

	void Mesh::SetProgram (GLSLProgram* program)
	{
		if(!program) return;
		if(m_program == program) return;

		if(m_program)
			Destroy(m_program);

		m_program = program;
		Retain(m_program);
	}

	void Mesh::SetVertexBuffer (GLArrayBuffer* vb)
	{
		if(!vb) return;
		if(m_vertices == vb) return;

		if(m_vertices) Destroy(m_vertices);
		m_vertices = vb;
		Retain(m_vertices);
	}

	void Mesh::SetIndexBuffer (GLElementArrayBuffer* ib)
	{
		if(!ib) return;
		if(m_index == ib) return;

		if(m_index) Destroy(m_index);
		m_index = ib;
		Retain(m_index);
	}

	Mesh::~Mesh ()
	{
		Destroy(m_vertices);
		Destroy(m_index);
		Destroy(m_program);
	}

}
