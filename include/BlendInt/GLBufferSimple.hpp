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
#define _BLENDINT_GLBUFFERSIMPLE_HPP_

#include <BlendInt/AbstractGLBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief A simple GLBuffer with 1 property for all buffers
	 *
	 * Usage:
	 * 	-# Define or new a object: e.g. GLBufferSimple buffer; // or GLBufferSimple* buffer = new GLBufferSimple;
	 * 	-# Generate buffers: buffer.Generate (1);
	 * 	-# Set the property of generated buffer(s): buffer.SetProperty();
	 * 	-# Upload data to this buffer: buffer.Upload();
	 * 	-# Bind the buffer to draw something: buffer.Bind();
	 * 	-# drawing code here
	 * 	-# Unbind the buffer: buffer.Unbind();
	 *
	 * @ingroup opengl
	 */
	class GLBufferSimple: public AbstractGLBuffer
	{
	public:

		GLBufferSimple ();

		virtual ~GLBufferSimple ();

		virtual void SetProperty (int vertices, int unit_size, GLenum target, GLenum usage);

		virtual void Bind ();

		virtual void Unbind ();

		virtual void Upload (const GLvoid* data);

		virtual int Vertices ();

		virtual int UnitSize ();

		virtual GLenum Target ();

		virtual GLenum Usage ();

		inline void set_vertices (int vertices)
		{
			m_property.vertices = vertices;
		}

		inline void set_unit_size (int size)
		{
			m_property.unit_size = size;
		}

		inline void set_target (GLenum target)
		{
			m_property.target = target;
		}

		inline void set_usage (GLenum usage)
		{
			m_property.usage = usage;
		}

	private:

		GLBufferProperty m_property;

	};

}

#endif /* _BLENDINT_GLBUFFERSIMPLE_HPP_ */
