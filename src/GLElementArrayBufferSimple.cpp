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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/GLElementArrayBufferSimple.hpp>

namespace BlendInt {

	GLElementArrayBufferSimple::GLElementArrayBufferSimple()
	: AbstractGLBuffer()
	{

	}

	GLElementArrayBufferSimple::~GLElementArrayBufferSimple ()
	{
	}

	void GLElementArrayBufferSimple::SetProperty (int vertices, int unit_size,
	        GLenum usage)
	{
		m_property.vertices = vertices;
		m_property.unit_size = unit_size;
		m_property.usage = usage;
	}

	void GLElementArrayBufferSimple::Bind ()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids()[index()]);
	}

	void GLElementArrayBufferSimple::Unbind ()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLElementArrayBufferSimple::Upload (const GLvoid* data)
	{
		glBufferData (GL_ELEMENT_ARRAY_BUFFER,
				m_property.unit_size * m_property.vertices,
				data,
				m_property.usage);
	}

	int GLElementArrayBufferSimple::Vertices ()
	{
		return m_property.vertices;
	}

	int GLElementArrayBufferSimple::UnitSize ()
	{
		return m_property.unit_size;
	}

	GLenum GLElementArrayBufferSimple::Target ()
	{
		return GL_ELEMENT_ARRAY_BUFFER;
	}

	GLenum GLElementArrayBufferSimple::Usage ()
	{
		return m_property.usage;
	}

}
