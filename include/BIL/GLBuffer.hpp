/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
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

		explicit GLBuffer (unsigned int size = 1);

		~GLBuffer ();

		bool generate (unsigned int size = 1);

		bool reset (size_t index);

		/**
		 * @brief bind current buffer
		 * @return
		 */
		void bind (GLenum target);

		bool bind (size_t index, GLenum target);

		void unbind (GLenum target);

		void set_index (size_t index);

		/**
		 * @brief return the current index in the buffer vector
		 * @return
		 */
		const size_t index () const {return m_index;}

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
		void upload (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

		void destroy ();

		bool destroy (size_t index);

		void clear ();

	private:

		size_t m_index;

		std::vector<GLuint> m_buffers;
	};
}

#endif /* _BIL_GLBUFFER_HPP_ */
