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

#ifndef _BLENDINT_ABSTRACTGLBUFFER_HPP_
#define _BLENDINT_ABSTRACTGLBUFFER_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/GLBufferWrapper.hpp>

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
	 * @brief A OpenGL buffer
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
	class AbstractGLBuffer: public GLBufferWrapper
	{
	public:

		AbstractGLBuffer ();

		virtual ~AbstractGLBuffer ();

		virtual void SetProperty (int vertices, int unit_size, GLenum target, GLenum usage) = 0;

		virtual void Bind () = 0;

		virtual void Unbind () = 0;

		virtual void Upload (const GLvoid* data) = 0;

		virtual int Vertices () = 0;

		virtual int UnitSize () = 0;

		virtual GLenum Target () = 0;

		virtual GLenum Usage () = 0;
	};

}

#endif /* _BIL_GLBUFFER_HPP_ */
