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
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iterator>

#include <BlendInt/GLBufferWrapper.hpp>

namespace BlendInt {

	GLBufferWrapper::GLBufferWrapper ()
	: m_index(0)
	{
	}

	GLBufferWrapper::~GLBufferWrapper ()
	{
		Clear();
	}

	void GLBufferWrapper::Generate (size_t num)
	{
		if(m_ids.size()) Clear();

		m_ids.resize(num);

		glGenBuffers((GLsizei)num, &(m_ids[0]));
	}

	void GLBufferWrapper::Destroy ()
	{
		size_t index_last = m_ids.size() - 1;

		glDeleteBuffers(1, &(m_ids[m_index]));

		std::vector<GLuint>::iterator it = m_ids.begin();
		std::advance(it, m_index);

		m_ids.erase(it);

		if(m_index == index_last)
			m_index--;
	}

	void GLBufferWrapper::Destroy (size_t index)
	{
		size_t index_last = m_ids.size() - 1;

		glDeleteBuffers(1, &(m_ids[index]));

		if(m_index > index) {
			m_index -= 1;
		} else if(m_index == index_last && index == index_last) {
			m_index--;
		}

		std::vector<GLuint>::iterator it = m_ids.begin();
		std::advance(it, index);

		m_ids.erase(it);
	}

	void GLBufferWrapper::Clear ()
	{
		glDeleteBuffers((GLsizei)m_ids.size(), &(m_ids[0]));
		m_ids.clear();
	}

}
