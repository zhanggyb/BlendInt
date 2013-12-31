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

#ifndef _BLENDINT_GLARRAYBUFFERMULTIPLE_HPP_
#define _BLENDINT_GLARRAYBUFFERMULTIPLE_HPP_

#include <BlendInt/AbstractGLBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief Multiple GLBuffers, each buffer with own property
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
	class GLArrayBufferMultiple: public AbstractGLBuffer
	{
	public:

		GLArrayBufferMultiple();

		virtual ~GLArrayBufferMultiple ();

		virtual void Generate (size_t num = 1);

		virtual void Destroy ();

		virtual void Destroy (size_t index);

		virtual void Clear ();

		virtual void SetProperty (int vertices, int unit_size, GLenum usage);

		void SetProperty (size_t index, int vertices, int unit_size, GLenum usage);

		virtual void Bind ();

		void Bind (size_t index);

		virtual void Unbind ();

		virtual void Upload (const GLvoid* data);

		void Upload (size_t index, const GLvoid* data);

		virtual int Vertices ();

		virtual int UnitSize ();

		virtual GLenum Target ();

		virtual GLenum Usage ();

		inline int vertices (size_t index)
		{
			return m_properties[index].vertices;
		}

		inline void set_vertices (int vertices)
		{
			m_properties[index()].vertices = vertices;
		}

		inline void set_vertices (size_t index, int vertices)
		{
			m_properties[index].vertices = vertices;
		}

		inline int unit_size ()
		{
			return m_properties[index()].unit_size;
		}

		inline void set_unit_size (int size)
		{
			m_properties[index()].unit_size = size;
		}

		inline void set_unit_size (size_t index, int size)
		{
			m_properties[index].unit_size = size;
		}

		inline GLenum usage (size_t index)
		{
			return m_properties[index].usage;
		}

		inline void set_usage (GLenum usage)
		{
			m_properties[index()].usage = usage;
		}

		inline void set_usage (size_t index, GLenum usage)
		{
			m_properties[index].usage = usage;
		}

	private:

		std::vector<GLBufferProperty> m_properties;
	};
}

#endif /* _BLENDINT_GLARRAYBUFFERMULTIPLE_HPP_ */
