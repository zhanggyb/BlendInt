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
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <stddef.h>

#include <BlendInt/Types.hpp>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace BlendInt {

	/**
	 * @brief Array buffer for 2D vertices in float
	 *
	 * The N in template is the number to describe how many float variable for a vertex
	 *
	 * for example:
	 * 	- for 2D vertex array, only 2 variables (x ,y) for a vertex axis, in this case N is 2
	 * 	- for 3D vertex array, 3 variables (x, y, z) are needed, N is 3
	 * 	- for 2D vertex array with color, there're 6 variables for one vertex: (x, y) and (r, g, b, a), in this case N is 6
	 */
	class GLArrayBufferF
	{
	public:

		/**
		 * @brief Default constructor
		 */
		GLArrayBufferF ();

		/**
		 * @brief Destructor
		 */
		~GLArrayBufferF ();

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

		static void Unbind ();

		void SetData (GLint num_per_vertex, GLsizeiptr size, const float* data, GLenum usage = GL_STATIC_DRAW);

		inline GLenum target ()
		{
			return GL_ARRAY_BUFFER;
		}

		GLenum GetUsage ();

		GLint GetBufferSize ();

		size_t GetVertices ();

	private:

		GLuint m_id;

		GLint m_num_per_vertex;	// how many variables per vertex

		static const float jit[WIDGET_AA_JITTER][2];
	};


}

#endif /* _BLENDINT_GLARRAYBUFFERF_HPP_ */
