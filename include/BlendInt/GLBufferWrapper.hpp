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

#ifndef _BLENDINT_GLBUFFERWRAPPER_HPP_
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

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

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
		 * @param[in] num how many buffers need to be generated, default is 1
		 *
		 * Generate new GL buffers. And set the current index to 0.
		 *
		 * If there're buffers generated before, call Clear() to delete these.
		 */
		virtual void Generate (size_t num = 1);

		/**
		 * @brief Destroy the current buffer
		 */
		virtual void Destroy ();

		/**
		 * @brief Destroy the buffer at given index
		 * @param index The index in the vector where stores buffer ids
		 */
		virtual void Destroy (size_t index);

		/**
		 * @brief Delete all buffers created
		 */
		virtual void Clear ();

		/**
		 * @brief Set the current buffer id
		 * @param index The index in buffer vector
		 */
		inline void select (size_t index)
		{
			m_index = index;
		}

		inline size_t index () const {return m_index;}

		inline size_t size () const {return m_ids.size();}

		inline GLuint id () const {return m_ids[m_index];}

		inline GLuint id (size_t index) const {return m_ids[index];}

		bool IsBuffer ();

		bool IsBuffer (size_t index);

	protected:

		std::vector<GLuint>& ids() {return m_ids;}

		inline void set_index (size_t index)
		{
			m_index = index;
		}

	private:

		size_t m_index;

		std::vector<GLuint> m_ids;
	};

}

#endif /* _BLENDINT_GLBUFFERSIMPLE_HPP_ */
