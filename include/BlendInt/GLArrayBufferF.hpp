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

#ifndef _BLENDINT_GLARRAYBUFFERF_HPP_
#define _BLENDINT_GLARRAYBUFFERF_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <stddef.h>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace BlendInt {

	/**
	 * @brief Array buffer for 2D vertices in float
	 *
	 * The N in template is the number to describe how many float variable for a vertex
	 *
	 * for example:
	 * 	- for 2D vertex array, only 2 variables (x ,y) for a vertex axis, in this case N is 2
	 * 	- for 3D vertex array, 3 variables (x, y, z) are needed, N is 3
	 * 	- for 2D vertex array with color, there're 6 variables for one vertex: (x, y) and (r, g, b, a), in this case N is 6
	 */
	template<size_t N>
	class GLArrayBufferF
	{
	public:

		/**
		 * @brief Default constructor
		 */
		GLArrayBufferF ();

		/**
		 * @brief Destructor
		 */
		~GLArrayBufferF ();

		/**
		 * @brief Generate buffer
		 *
		 * Generate new GL buffers, if there's buffer generated before, this will delete these.
		 */
		void Generate ();

		/**
		 * @brief Delete buffer created
		 */
		void Clear ();

		/**
		 * @brief The buffer id
		 * @return
		 */
		inline GLuint id () const {return m_id;}

		bool IsBbuffer ();

		void Bind ();

		static void Unbind ();

		void SetData (GLsizeiptr size, const float* data, GLenum usage = GL_STATIC_DRAW);

		inline GLenum target ()
		{
			return GL_ARRAY_BUFFER;
		}

		GLenum GetUsage ();

		GLint GetBufferSize ();

		size_t GetVertices ();

		void Draw (GLenum mode, GLint size = 2, GLsizei stride = 0, GLsizei offset = 0);

		void DrawAntiAlias (GLenum mode, GLint size = 2, GLsizei stride = 0, GLsizei offset = 0);

		void DrawWithColor (GLenum mode, GLint size = 2, GLsizei stride = N, GLsizei offset = 0);

	private:

		GLuint m_id;

		static const float jit[WIDGET_AA_JITTER][2];
	};

	template<size_t N>
	const float GLArrayBufferF<N>::jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	template<size_t N>
	GLArrayBufferF<N>::GLArrayBufferF()
	: m_id(0)
	{

	}

	template<size_t N>
	GLArrayBufferF<N>::~GLArrayBufferF()
	{
		glDeleteBuffers(1, &m_id);
	}

	template<size_t N>
	void GLArrayBufferF<N>::Generate()
	{
		if(!m_id)
			Clear();

		glGenBuffers(1, &m_id);
	}

	template<size_t N>
	void GLArrayBufferF<N>::Clear()
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}

	template<size_t N>
	bool GLArrayBufferF<N>::IsBbuffer ()
	{
		return glIsBuffer(m_id);
	}

	template<size_t N>
	void GLArrayBufferF<N>::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	template<size_t N>
	void GLArrayBufferF<N>::Unbind ()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template<size_t N>
	void GLArrayBufferF<N>::SetData (GLsizeiptr size, const float* data, GLenum usage)
	{
		glBufferData (GL_ARRAY_BUFFER, size, data, usage);
	}

	template<size_t N>
	GLenum GLArrayBufferF<N>::GetUsage ()
	{
		GLint usage = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);

		return usage;
	}

	template<size_t N>
	GLint GLArrayBufferF<N>::GetBufferSize ()
	{
		GLint buffer_size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);

		return buffer_size;
	}

	template<size_t N>
	size_t GLArrayBufferF<N>::GetVertices()
	{
		size_t num = 0;
		GLint buffer_size = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
		num = buffer_size / (sizeof(float) * N);

		return num;
	}

	template<size_t N>
	void GLArrayBufferF<N>::Draw (GLenum mode, GLint size, GLsizei stride, GLsizei offset)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(size, GL_FLOAT, stride * sizeof(GLfloat), BUFFER_OFFSET(offset * sizeof(GLfloat)));
		glDrawArrays(mode, 0, GetVertices());
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	template<size_t N>
	void GLArrayBufferF<N>::DrawAntiAlias(GLenum mode, GLint size, GLsizei stride, GLsizei offset)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(size, GL_FLOAT, stride * sizeof(GLfloat), BUFFER_OFFSET(offset * sizeof (GLfloat)));
			glDrawArrays(mode, 0, GetVertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	template<size_t N>
	void GLArrayBufferF<N>::DrawWithColor(GLenum mode, GLint size, GLsizei stride, GLsizei offset)
	{
		if((size + offset) > N) {
			return;
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(size, GL_FLOAT, sizeof(GLfloat) * N, BUFFER_OFFSET(offset * sizeof(GLfloat)));
		glColorPointer(N - size, GL_FLOAT, sizeof(GLfloat) * N, BUFFER_OFFSET((offset + size) * sizeof(GLfloat)));

		glDrawArrays(mode, 0, GetVertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

}

#endif /* _BLENDINT_GLARRAYBUFFERF_HPP_ */
