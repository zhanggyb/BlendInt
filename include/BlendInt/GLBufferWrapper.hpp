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

#ifndef _BLENDINT_GLBUFFERSIMPLE_HPP_
#define _BLENDINT_GLBUFFERWRAPPER_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <vector>

namespace BlendInt {

	/**
	 * @brief A simple warpper of OpenGL buffer
	 *
	 * Usage:
	 * 	-# Define or new a GLBufferWrapper object: e.g. GLBufferWrapper buffer; // or GLBufferWrapper* buffer = new GLBufferWrapper;
	 * 	-# Generate buffers: buffer.Generate (1);
	 * 	-# Use OpenGL API to bind the buffer: glBindBuffer(target, buffer.id());
	 * 	-# Upload data to this buffer: glBufferData(target, size, data, usage);
	 * 	-# drawing code here
	 * 	-# Unbind the buffer: glBindBuffer(target, 0);
	 *
	 * @ingroup opengl
	 */
	class GLBufferWrapper
	{
	public:

		/**
		 * @brief Default constructor
		 */
		GLBufferWrapper();

		/**
		 * @brief Destructor
		 */
		virtual ~GLBufferWrapper();

		/**
		 * @brief create buffers with the given number
		 * @param num how many buffers need to be generate, default is 1
		 *
		 * This function search the key in the built-in map, if the key was not
		 * used, create a new gl buffer. And set the current buffer to the one
		 * defined by key, so the next functions (bind(), upload(), etc.) use this
		 * buffer.
		 */
		virtual void Generate (size_t num = 1);

		virtual void Destroy ();

		virtual void Destroy (size_t index);

		virtual void Clear ();

		/**
		 * @brief set buffer found by key to the current buffer for other operations
		 * @param index the unique keyword in buffer map
		 *
		 * This function raises an invalid_argument exception if it's not found in buffer
		 * map
		 *
		 * @ingroup functions_throw_exception
		 */
		inline void select (size_t index)
		{
			m_index = index;
		}

		inline size_t index () const {return m_index;}

		inline size_t size () const {return m_ids.size();}

		inline GLuint id () const {return m_ids[m_index];}

		inline GLuint id (size_t index) const {return m_ids[index];}

	private:

		size_t m_index;

		std::vector<GLuint> m_ids;
	};

}

#endif /* _BLENDINT_GLBUFFERSIMPLE_HPP_ */
