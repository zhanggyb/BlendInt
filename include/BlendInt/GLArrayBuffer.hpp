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

#ifndef _BLENDINT_GLARRAYBUFFER_HPP_
#define _BLENDINT_GLARRAYBUFFER_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <stddef.h>

namespace BlendInt {

	/**
	 * @brief A simple GLBuffer with 1 property for all buffers
	 *
	 * Usage:
	 * 	-# Define or new a object: e.g. GLBufferSimple buffer; // or GLBufferSimple* buffer = new GLBufferSimple;
	 * 	-# Generate buffers: buffer.Generate (1);
	 * 	-# Set the property of generated buffer(s): buffer.SetProperty();
	 * 	-# Upload data to this buffer: buffer.Upload();
	 * 	-# Bind the buffer to draw something: buffer.Bind();
	 * 	-# drawing code here
	 * 	-# Unbind the buffer: buffer.Unbind();
	 *
	 * @ingroup opengl
	 */
	template<typename T, size_t N>
	class GLArrayBuffer
	{
	public:

		GLArrayBuffer ();

		~GLArrayBuffer ();

		/**
		 * @brief create buffers with the given number
		 * @param[in] num how many buffers need to be generated, default is 1
		 *
		 * Generate new GL buffers. And set the current index to 0.
		 *
		 * If there're buffers generated before, call Clear() to delete these.
		 */
		void Generate ();

		/**
		 * @brief Delete all buffers created
		 */
		void Clear ();

		inline GLuint id () const {return m_id;}

		bool IsBbuffer ();

		void Bind ();

		static void Unbind ();

		void SetData (GLsizeiptr size, const T* data, GLenum usage);

		inline GLenum target ()
		{
			return GL_ARRAY_BUFFER;
		}

		GLenum GetUsage ();

		GLint GetBufferSize ();

		size_t GetVertices ();

	private:

		GLuint m_id;

	};

	template<typename T, size_t N>
	GLArrayBuffer<T, N>::GLArrayBuffer()
	: m_id(0)
	{

	}

	template<typename T, size_t N>
	GLArrayBuffer<T, N>::~GLArrayBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	template<typename T, size_t N>
	void GLArrayBuffer<T, N>::Generate()
	{
		if(!m_id)
			Clear();

		glGenBuffers(1, &m_id);
	}

	template<typename T, size_t N>
	void GLArrayBuffer<T, N>::Clear()
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}

	template<typename T, size_t N>
	bool GLArrayBuffer<T, N>::IsBbuffer ()
	{
		return glIsBuffer(m_id);
	}

	template<typename T, size_t N>
	void GLArrayBuffer<T, N>::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	template<typename T, size_t N>
	void GLArrayBuffer<T, N>::Unbind ()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template<typename T, size_t N>
	void GLArrayBuffer<T, N>::SetData (GLsizeiptr size, const T* data, GLenum usage)
	{
		glBufferData (GL_ARRAY_BUFFER, size, data, usage);
	}

	template<typename T, size_t N>
	GLenum GLArrayBuffer<T, N>::GetUsage ()
	{
		GLint usage = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);

		return usage;
	}

	template<typename T, size_t N>
	GLint GLArrayBuffer<T, N>::GetBufferSize ()
	{
		GLint size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &size);

		return size;
	}

	template<typename T, size_t N>
	size_t GLArrayBuffer<T, N>::GetVertices()
	{
		size_t num = 0;
		GLint buffer_size = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &buffer_size);

		num = buffer_size / (sizeof(T) * N);
	}

}

#endif /* _BLENDINT_GLARRAYBUFFER_HPP_ */
