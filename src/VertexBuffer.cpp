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

#include <BlendInt/VertexBuffer.hpp>
#include <BlendInt/ShaderManager.hpp>

namespace BlendInt {

	VertexBuffer::VertexBuffer()
	: m_program(0)
	{
		m_buffer.reset(new GLArrayBufferMultiple);
	}

	VertexBuffer::~VertexBuffer()
	{
		DeleteProgram();
	}

	void VertexBuffer::Generate (size_t num)
	{
		m_buffer->Generate(num);
	}

	void VertexBuffer::Upload (int vertices, int unit_size, GLenum target, GLenum usage, const GLvoid* data, size_t index)
	{
		m_buffer->select(index);
		m_buffer->SetProperty(vertices, unit_size, target, usage);
		m_buffer->Bind();
		m_buffer->Upload(data);
		m_buffer->Unbind();
	}

	void VertexBuffer::SetProgram(GLSLProgram* program)
	{
		if(m_program == program) return;

		DeleteProgram();

		m_program = program;
	}

	void VertexBuffer::Draw()
	{

	}

	void VertexBuffer::DeleteProgram()
	{
		if(ShaderManager::Instance()->Find(m_program)) {
			m_program = 0;
			return;
		}

		if(m_program) {
			delete m_program;
			m_program = 0;
		}
	}

}
