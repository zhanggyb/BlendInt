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

#ifndef _BLENDINT_GLELEMENTARRAYBUFFER_HPP_
#define _BLENDINT_GLELEMENTARRAYBUFFER_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/Core/Object.hpp>

namespace BlendInt {

	/**
	 * @brief A simple element array buffer with 1 property for all buffers
	 *
	 * @ingroup opengl
	 */
	class GLElementArrayBuffer: public Object
	{
	public:

		GLElementArrayBuffer ();

		virtual ~GLElementArrayBuffer ();

		/**
		 * @brief Generate buffer
		 *
		 * Generate new GL buffers, if there's buffer generated before, this will delete these.
		 */
		void Generate ();

		/**
		 * @brief Delete buffer created
		 */
		void Clear ();

		/**
		 * @brief The buffer id
		 * @return
		 */
		inline GLuint id () const {return m_id;}

		bool IsBbuffer ();

		void Bind ();

		void SetData (GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);

		void SetData (int vertices, size_t size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);

		static void Reset ();

		inline GLenum target ()
		{
			return GL_ELEMENT_ARRAY_BUFFER;
		}

		GLenum GetUsage ();

		GLint GetBufferSize ();

		/**
		 * @brief Get vertex number used in this buffer
		 * @param size Must be 2, 3, how many float variables for one vertex
		 * @return
		 */
		inline GLint vertices ()
		{
			return m_vertices;
		}

	private:

		GLuint m_id;

		GLint m_vertices;

	};

}

#endif /* _BLENDINT_GLELEMENTARRAYBUFFER_HPP_ */
