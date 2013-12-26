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

#ifndef _BLENDINT_GLBUFFER_HPP_
#define _BLENDINT_GLBUFFER_HPP_

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

	struct GLBufferProperty
	{
		GLBufferProperty ()
		: vertices(0), unit_size(0), target(0), usage(0)
		{}

		/** Vertex number -- how many vertices are used in this buffer */
		int vertices;

		/** size of one unit vertex */
		int unit_size;

		/** The target buffer object, it's used for glBindBuffer() and glBufferData() */
		GLenum target;

		/** Usage: */
		GLenum usage;
	};

	/**
	 * @brief A simple warpper of OpenGL buffer
	 *
	 * Usage:
	 * 	-# Define or new a GLBuffer object: e.g. GLBuffer buffer; // or GLBuffer* buffer = new GLBuffer;
	 * 	-# Generate buffers: buffer.Generate (1);
	 * 	-# Set the property of generated buffer(s): buffer.set_property();
	 * 	-# Upload data to this buffer: buffer.Upload();
	 * 	-# Bind the buffer to draw something: buffer.bind();
	 * 	-# drawing code here
	 * 	-# Unbind the buffer: buffer.unbind();
	 *
	 * @ingroup opengl
	 */
	class GLBuffer
	{
	public:

		GLBuffer ();

		virtual ~GLBuffer ();

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

		inline int vertices ()
		{
			return m_properties[m_index].vertices;
		}

		inline int vertices (size_t index)
		{
			return m_properties[index].vertices;
		}

		inline void set_vertices (int vertices)
		{
			m_properties[m_index].vertices = vertices;
		}

		inline void set_vertices (size_t index, int vertices)
		{
			m_properties[index].vertices = vertices;
		}

		inline int unit_size ()
		{
			return m_properties[m_index].unit_size;
		}

		inline int unit_size (size_t index)
		{
			return m_properties[index].unit_size;
		}

		inline void set_unit_size (int size)
		{
			m_properties[m_index].unit_size = size;
		}

		inline void set_unit_size (size_t index, int size)
		{
			m_properties[index].unit_size = size;
		}

		inline GLenum target ()
		{
			return m_properties[m_index].target;
		}

		inline GLenum target (size_t index)
		{
			return m_properties[index].target;
		}

		inline void set_target (GLenum target)
		{
			m_properties[m_index].target = target;
		}

		inline void set_target (size_t index, GLenum target)
		{
			m_properties[index].target = target;
		}

		inline GLenum usage ()
		{
			return m_properties[m_index].usage;
		}

		inline GLenum usage (size_t index)
		{
			return m_properties[index].usage;
		}

		inline void set_usage (GLenum usage)
		{
			m_properties[m_index].usage = usage;
		}

		inline void set_usage (size_t index, GLenum usage)
		{
			m_properties[index].usage = usage;
		}

		inline void set_property (int vertices, int unit_size, GLenum target, GLenum usage)
		{
			m_properties[m_index].vertices = vertices;
			m_properties[m_index].unit_size = unit_size;
			m_properties[m_index].target = target;
			m_properties[m_index].usage = usage;
		}

		inline void set_property (size_t index, int vertices, int unit_size, GLenum target, GLenum usage)
		{
			m_properties[index].vertices = vertices;
			m_properties[index].unit_size = unit_size;
			m_properties[index].target = target;
			m_properties[index].usage = usage;
		}

		/**
		 * @brief bind current buffer
		 * @return
		 */
		inline void bind ()
		{
			glBindBuffer(m_properties[m_index].target, m_ids[m_index]);
		}

		inline void bind (size_t index)
		{
			glBindBuffer(m_properties[index].target, m_ids[m_index]);
		}

		/**
		 * @brief unbind the buffer id in current index
		 */
		inline void unbind ()
		{
			glBindBuffer(m_properties[m_index].target, 0);
		}

		inline void unbind (size_t index)
		{
			glBindBuffer(m_properties[index].target, 0);
		}

		/**
		 * @brief return the current index in the buffer vector
		 * @return
		 */
		inline size_t index () const {return m_index;}

		inline size_t size () const {return m_ids.size();}

		bool is_buffer ();

		bool is_buffer (size_t index);

		/**
		 * @brief creates and initializes a buffer object's data store
		 * @param target
		 * @param size
		 * @param data
		 * @param usage
		 */
		inline void upload (const GLvoid* data)
		{
			glBufferData (m_properties[m_index].target,
					m_properties[m_index].unit_size * m_properties[m_index].vertices,
					data,
					m_properties[m_index].usage);
		}

		inline void upload (size_t index, const GLvoid* data)
		{
			glBufferData (m_properties[index].target,
					m_properties[index].unit_size * m_properties[index].vertices,
					data,
					m_properties[index].usage);
		}

		virtual void Destroy ();

		virtual void Destroy (size_t index);

		virtual void Clear ();

	private:

		size_t m_index;

		std::vector<GLuint> m_ids;

		std::vector<GLBufferProperty> m_properties;
	};
}

#endif /* _BIL_GLBUFFER_HPP_ */
