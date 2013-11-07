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

#ifndef _BLENDINT_SHADER_HPP_
#define _BLENDINT_SHADER_HPP_

#include <GL/gl.h>

#include <string>

namespace BlendInt {

	/**
	 * @brief Wrapper class for OpenGL Shader
	 *
	 * @ingroup opengl
	 */
	class GLSLShader
	{
	public:

		/**
		 * @brief Default Constructor
		 */
		GLSLShader ();

		~GLSLShader ();

		/**
		 * @brief Load a vertex or fragment shader sources and compile
		 * @param filename filename to be loaded
		 * @param type Shader Type defined in OpenGL
		 *
		 * Must be one of: Must be one of GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
		 *       GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
		 */
		void load (const std::string& filename, GLenum type);

		void load (const char* buf, GLenum type);

		GLuint id () const
		{
			return id_;
		}

		GLenum type () const;

		bool isValid () const;

		bool isDeleted () const;

		/**
		 * Display compilation errors from the OpenGL shader compiler
		 */
		void print_log ();

		void clear ();

	private:
		/**
		 * @brief Read a fragment or vertex shader from a file
		 * @param filename file to read shader from
		 * @return a newly-allocated text buffer containing code.
		 * This buffer must be freed after usage
		 */
		char* read (const char *filename);

		/**
		 * @brief Compile a shader from a text buffer
		 * @param source code of the shader
		 * @param type type of shader
		 * @return the shader object on the compiled program
		 */
		GLuint compile (const char* source, const GLenum type);

		GLuint id_;
	};

}

#endif /* _BIL_SHADER_HPP_ */
