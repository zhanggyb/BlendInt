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

#ifndef _BLENDINT_VERTEXBUFFER_HPP_
#define _BLENDINT_VERTEXBUFFER_HPP_

#include <boost/smart_ptr.hpp>

#include <BlendInt/GLBufferMultiple.hpp>
#include <BlendInt/GLSLProgram.hpp>

namespace BlendInt {

	/**
	 * @brief C++ wrapper for OpenGL Vertex Buffer Object
	 *
	 * A combination of GLBuffer and GLSLProgram
	 *
	 * @ingroup opengl
	 */
	class VertexBuffer
	{
	public:

		/**
		 * @brief Default constructor
		 */
		VertexBuffer ();

		/**
		 * @brief Destructor
		 */
		virtual ~VertexBuffer ();

		void Generate (size_t num = 1);

		void Upload (int vertices, int unit_size, GLenum target, GLenum usage, const GLvoid* data, size_t index = 0);

		/**
		 * @brief Set GLSL program for drawing
		 * @param program GLSLProgram object, can be allocated by new or from ShaderManager
		 *
		 * @note When this class is destructed, it will delete the program object if it's not
		 * managed by ShaderManager.
		 */
		void SetProgram (GLSLProgram* program);

		virtual void Draw ();

	private:

		void DeleteProgram ();

		GLSLProgram* m_program;

		boost::scoped_ptr<GLBufferMultiple> m_buffer;
	};

}

#endif /* _BLENDINT_VERTEXBUFFER_HPP_ */
