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

#include <GLArrayBufferF.hpp>

namespace BlendInt {

	const float GLArrayBufferF::jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	GLArrayBufferF::GLArrayBufferF()
	: m_id(0), m_num_per_vertex(0)
	{

	}

	GLArrayBufferF::~GLArrayBufferF()
	{
		glDeleteBuffers(1, &m_id);
	}

	void GLArrayBufferF::Generate()
	{
		if(!m_id)
			Clear();

		glGenBuffers(1, &m_id);
	}

	void GLArrayBufferF::Clear()
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}

	bool GLArrayBufferF::IsBbuffer ()
	{
		return glIsBuffer(m_id);
	}

	void GLArrayBufferF::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void GLArrayBufferF::Unbind ()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLArrayBufferF::SetData (GLint num_per_vertex, GLsizeiptr size, const float* data, GLenum usage)
	{
		glBufferData (GL_ARRAY_BUFFER, size, data, usage);
		m_num_per_vertex = num_per_vertex;
	}

	GLenum GLArrayBufferF::GetUsage ()
	{
		GLint usage = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);

		return usage;
	}

	GLint GLArrayBufferF::GetBufferSize ()
	{
		GLint buffer_size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);

		return buffer_size;
	}

	size_t GLArrayBufferF::GetVertices()
	{
		size_t num = 0;
		GLint buffer_size = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
		num = buffer_size / (sizeof(float) * m_num_per_vertex);

		return num;
	}

}
