/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_GLBUFFER_HPP_
#define _BIL_GLBUFFER_HPP_

#include <vector>

namespace BILO {

	/**
	 * @brief C++ warpper of OpenGL buffer
	 */
	class GLBuffer
	{
	public:

		GLBuffer ();

		~GLBuffer ();

		void generate (size_t size = 1);

		bool reset (size_t index);

		/**
		 * @brief bind current buffer
		 * @return
		 */
		void bind (GLenum target);

		bool bind (size_t index, GLenum target);

		void rebind ();

		void rebind (size_t index);

		/**
		 * @brief unbind the buffer id in current index
		 */
		void unbind ();

		void unbind (size_t index);

		void set_index (size_t index);

		/**
		 * @brief return the current index in the buffer vector
		 * @return
		 */
		const size_t index () const {return m_index;}

		size_t size () const {return m_buffers.size();}

		bool is_buffer ();

		bool is_buffer (size_t index);

		const GLuint& operator [] (size_t index) const
		{
			return m_buffers[index];
		}

		/**
		 * @brief creates and initializes a buffer object's data store
		 * @param target
		 * @param size
		 * @param data
		 * @param usage
		 */
		void upload (GLsizeiptr size, const GLvoid* data, GLenum usage);

		void upload (size_t index, GLsizeiptr size, const GLvoid* data, GLenum usage);

		void destroy ();

		bool destroy (size_t index);

		void clear ();

	private:

		/**
		 * @buffer index
		 *
		 * -1 : initial status
		 */
		int m_index;

		std::vector<GLuint> m_buffers;

		std::vector<int> m_buffer_sizes;

		std::vector<GLenum> m_buffer_targets;

		std::vector<GLenum> m_buffer_usages;
	};
}

#endif /* _BIL_GLBUFFER_HPP_ */
