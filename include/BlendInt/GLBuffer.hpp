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

#include <vector>

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
#endif  // __UNIX__

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace BlendInt {

	/**
	 * @brief A simple warpper of OpenGL buffer
	 *
	 * @ingroup opengl
	 */
	class GLBuffer
	{
	public:

		GLBuffer ();

		~GLBuffer ();

		/**
		 * @brief create buffers with the given number
		 * @param num how many buffers need to be generate, default is 1
		 *
		 * This function search the key in the built-in map, if the key was not
		 * used, create a new gl buffer. And set the current buffer to the one
		 * defined by key, so the next functions (bind(), upload(), etc.) use this
		 * buffer.
		 */
		void generate (size_t num = 1);

		/**
		 * @brief set buffer found by key to the current buffer for other operations
		 * @param index the unique keyword in buffer map
		 *
		 * This function raises an invalid_argument exception if it's not found in buffer
		 * map
		 *
		 * @ingroup functions_throw_exception
		 */
		void select (size_t index)
		{
			m_index = index;
		}

		int vertices ()
		{
			return m_properties[m_index].vertices;
		}

		int vertices (size_t index)
		{
			return m_properties[index].vertices;
		}

		void set_vertices (int vertices);

		void set_vertices (size_t index, int vertices);

		int unit_size ()
		{
			return m_properties[m_index].unit_size;
		}

		int unit_size (size_t index)
		{
			return m_properties[index].unit_size;
		}

		void set_unit_size (int size);

		void set_unit_size (size_t index, int size);

		GLenum target ()
		{
			return m_properties[m_index].target;
		}

		GLenum target (size_t index)
		{
			return m_properties[index].target;
		}

		void set_target (GLenum target);

		void set_target (size_t index, GLenum target);

		GLenum usage ()
		{
			return m_properties[m_index].usage;
		}

		GLenum usage (size_t index)
		{
			return m_properties[index].usage;
		}

		void set_usage (GLenum usage);

		void set_usage (size_t index, GLenum usage);

		void set_property (int vertices, int unit_size, GLenum target, GLenum usage);

		void set_property (size_t index, int vertices, int unit_size, GLenum target, GLenum usage);

		/**
		 * @brief bind current buffer
		 * @return
		 */
		void bind ();

		void bind (size_t index);

		/**
		 * @brief unbind the buffer id in current index
		 */
		void unbind ();

		void unbind (size_t index);

		/**
		 * @brief return the current index in the buffer vector
		 * @return
		 */
		int index () const {return m_index;}

		size_t size () const {return m_ids.size();}

		bool is_buffer ();

		bool is_buffer (size_t index);

		/**
		 * @brief creates and initializes a buffer object's data store
		 * @param target
		 * @param size
		 * @param data
		 * @param usage
		 */
		void upload (const GLvoid* data);

		void upload (size_t index, const GLvoid* data);

		void destroy ();

		void destroy (size_t index);

		void clear ();

	private:

		struct Property
		{
			Property ()
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

		size_t m_index;

		std::vector<GLuint> m_ids;

		std::vector<Property> m_properties;
	};
}

#endif /* _BIL_GLBUFFER_HPP_ */
