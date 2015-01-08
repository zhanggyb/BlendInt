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

#ifndef _BLENDINT_OPENGL_GLVERTEXARRAYS_HPP_
#define _BLENDINT_OPENGL_GLVERTEXARRAYS_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <string.h>
#include <BlendInt/Core/Object.hpp>

namespace BlendInt {

	template <int SIZE = 1>
	class GLVertexArrays: public Object
	{
	public:

		GLVertexArrays ()
			: Object()
		{
			memset(ids_, 0, SIZE);
		}

		virtual ~GLVertexArrays ()
		{
			glDeleteVertexArrays (SIZE, ids_);
		}

		inline void generate ()
		{
			if(ids_[0] != 0) clear();
			glGenVertexArrays(SIZE, ids_);
		}

		inline void clear ()
		{
			glDeleteBuffers(SIZE, ids_);
			memset(ids_, 0, SIZE);
		}

		inline GLuint id (int index = 0) const
		{return ids_[index];}

		inline void bind (int index = 0) const
		{
			glBindVertexArray (ids_[index]);
		}

		static inline void reset ()
		{
			glBindVertexArray (0);
		}
		
	private:

		GLuint ids_[SIZE];
	};

}

#endif /* _BLENDINT_OPENGL_GLVERTEXARRAYS_HPP_ */
