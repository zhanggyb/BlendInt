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

#include <BlendInt/GLBufferSimple.hpp>

namespace BlendInt {

	GLBufferSimple::GLBufferSimple()
	: AbstractGLBuffer()
	{

	}

	GLBufferSimple::~GLBufferSimple ()
	{
	}

	void GLBufferSimple::SetProperty (int vertices, int unit_size,
	        GLenum target, GLenum usage)
	{
		m_property.vertices = vertices;
		m_property.unit_size = unit_size;
		m_property.target = target;
		m_property.usage = usage;
	}

	void GLBufferSimple::Bind ()
	{
		//glBindBuffer(m_property.target, ids()[index()]);
	}

	void GLBufferSimple::Unbind ()
	{
		glBindBuffer(m_property.target, 0);
	}

	void GLBufferSimple::Upload (const GLvoid* data)
	{
		glBufferData (m_property.target,
				m_property.unit_size * m_property.vertices,
				data,
				m_property.usage);
	}

	int GLBufferSimple::Vertices ()
	{
		return m_property.vertices;
	}

	int GLBufferSimple::UnitSize ()
	{
		return m_property.unit_size;
	}

	GLenum GLBufferSimple::Target ()
	{
		return m_property.target;
	}

	GLenum GLBufferSimple::Usage ()
	{
		return m_property.usage;
	}

}
