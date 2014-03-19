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

#ifndef _BLENDINT_GLARRAYBUFFERF_HPP_
#define _BLENDINT_GLARRAYBUFFERF_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <stddef.h>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Object.hpp>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace BlendInt {

	/**
	 * @brief Array buffer
	 *
	 * @ingroup opengl
	 */
	class GLArrayBuffer: public Object
	{
	public:

		/**
		 * @brief Default constructor
		 */
		GLArrayBuffer ();

		/**
		 * @brief Destructor
		 */
		virtual ~GLArrayBuffer ();

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

		bool IsBuffer ();

		void Bind ();

		static void Reset ();

		void SetData (GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);

		//void SetData (int vertices, size_t size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);

		void UpdateData (const GLvoid* data, GLsizeiptr size, GLintptr offset = 0);

		GLvoid* Map (GLenum access = GL_READ_ONLY);

		bool Unmap ();

		inline GLenum target ()
		{
			return GL_ARRAY_BUFFER;
		}

		GLenum GetUsage ();

		GLint GetBufferSize ();

	private:

		GLuint m_id;
	};

}

#endif /* _BLENDINT_GLARRAYBUFFERF_HPP_ */
