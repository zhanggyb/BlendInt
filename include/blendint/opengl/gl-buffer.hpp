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

#pragma once

#include <blendint/core/object.hpp>
#include <blendint/opengl/opengl.hpp>

#include <string.h>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace BlendInt {

	enum BufferType {

		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		//ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
		COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
		DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
		//DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
		//SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM_BUFFER =  GL_UNIFORM_BUFFER

	};

	template<int TARGET = GL_ARRAY_BUFFER, int SIZE = 1>
	class GLBuffer: public Object
	{
	public:

		GLBuffer ()
		: Object()
		{
			memset(ids_, 0, SIZE);
		}

		virtual ~GLBuffer ()
		{
			glDeleteBuffers(SIZE, ids_);
		}

		inline void generate ()
		{
			if(ids_[0] != 0) clear();
			glGenBuffers(SIZE, ids_);
		}

		inline void clear ()
		{
			glDeleteBuffers(SIZE, ids_);
			memset(ids_, 0, SIZE);
		}

		inline GLuint id (int index = 0) const {return ids_[index];}

		inline bool is_buffer (int index = 0) const
		{
			return glIsBuffer(ids_[index]);
		}

		inline void bind (int index = 0) const
		{
			glBindBuffer (TARGET, ids_[index]);
		}

		static inline void reset ()
		{
			glBindBuffer(TARGET, 0);
		}

		inline void set_data (GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW)
		{
			glBufferData (TARGET, size, data, usage);
		}

		inline void set_sub_data (GLintptr offset, GLsizeiptr size, const GLvoid* data)
		{
			glBufferSubData(TARGET, offset, size, data);
		}

		inline GLvoid* map (GLenum access = GL_READ_ONLY) const
		{
			return glMapBuffer(TARGET, access);
		}

		inline bool unmap () const
		{
			return glUnmapBuffer(TARGET);
		}

		inline GLenum target () const
		{
			return TARGET;
		}

		inline int size () const
		{
			return SIZE;
		}

		inline GLenum get_usage () const
		{
			GLint usage = 0;
			glGetBufferParameteriv(TARGET, GL_BUFFER_USAGE, &usage);
			return usage;
		}

		inline GLint get_buffer_size () const
		{
			GLint buffer_size = 0;
			glGetBufferParameteriv(TARGET, GL_BUFFER_SIZE, &buffer_size);
			return buffer_size;
		}

	private:

		GLuint ids_[SIZE];
	};

}
