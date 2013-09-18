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

#include <GL/glew.h>
#include <BILO/Buffer.hpp>

namespace BILO {

	namespace GL {

		Buffer::Buffer()
		: buffer_(0), size_(0)
		{
		}

		Buffer::Buffer (GLsizei size)
		: buffer_(0), size_(0)
		{
			generate (size);
		}

		Buffer::~Buffer()
		{
			clear();
		}

		void Buffer::generate(GLsizei size)
		{
			if((size_ > 0) && (!buffer_)) {
				glDeleteBuffers (size_, buffer_);
				delete [] buffer_;
				buffer_ = 0;
			}

			size_ = size;
			buffer_ = new GLuint[size_];
			glGenBuffers (size_, buffer_);
		}

		void Buffer::bind (GLenum target, int n)
		{
			if (n < 0 || n >= size_) return;

			if (glIsBuffer(*(buffer_ + n))) {
				glBindBuffer (target, *(buffer_ + n));
			}
		}

		void Buffer::clear ()
		{
			if((size_ > 0) && (!buffer_)) {
				glDeleteBuffers (size_, buffer_);
				delete [] buffer_;
				buffer_ = 0;
				size_ = 0;
			}
		}

		GLuint Buffer::operator [] (int n) const
		{
			if (n < 0 || n >= size_) return 0;

			return *(buffer_ + n);
		}

		bool Buffer::isValid (int n) const
		{
			if (n < 0 || n >= size_) return false;

			return glIsBuffer(*(buffer_ + n));
		}

	}
}
