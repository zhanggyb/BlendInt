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

		inline bool is_buffer ()
		{
			return glIsBuffer(m_id);
		}

		inline void set_property (int vertices, int unit_size)
		{
			m_vertices = vertices;
			m_unit_size = unit_size;
		}

		void Bind ();

		void Unbind ();

		void SetData (GLsizeiptr size, const GLvoid* data, GLenum usage);

		int vertices () {return m_vertices;}

		inline int unit_size () {return m_unit_size;}

		inline GLenum target ()
		{
			return GL_ARRAY_BUFFER;
		}

		GLenum GetUsage ();

		GLint GetBufferSize ();

		inline void set_vertices (int vertices)
		{
			m_vertices = vertices;
		}

		inline void set_unit_size (int size)
		{
			m_unit_size = size;
		}

	private:

		GLuint m_id;

		/** Vertex number -- how many vertices are used in this buffer */
		int m_vertices;

		/** size of one unit vertex */
		int m_unit_size;
	};

}

#endif /* _BLENDINT_GLARRAYBUFFER_HPP_ */
