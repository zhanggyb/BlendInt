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

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <string>

#include <BlendInt/Object.hpp>

namespace BlendInt {

	/**
	 * @brief Wrapper class for OpenGL Shader
	 *
	 * @ingroup opengl
	 */
	class GLSLShader: public Object
	{
	public:

		/**
		 * @brief Default Constructor
		 */
		GLSLShader ();

		void Create (GLenum shaderType);

		void SetSource (const char* source);

		bool Compile ();

		/**
		 * @brief Load a vertex or fragment shader sources and compile
		 * @param filename filename to be loaded
		 * @param type Shader Type defined in OpenGL
		 *
		 * Must be one of: GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
		 *       GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
		 */
		static GLuint Load (const std::string& filename, GLenum type);

		static GLuint Load (const char* buf, GLenum type);

		GLuint id () const
		{
			return m_id;
		}

		/**
		 * @brief Get the shader type
		 * @return The shader type defined in OpenGL:
		 * 	- GL_VERTEX_SHADER
		 * 	- GL_GEOMETRY_SHADER
		 * 	- GL_FRAGMENT_SHADER
		 */
		GLenum GetType () const;

		bool IsValid () const;

		/**
		 * @brief Get the last compile operation on shader was successfully
		 * @return
		 * 	- true If successfully compiled
		 * 	- false If not
		 */
		bool IsCompiled () const;

		/**
		 * @brief Check if shader is currently flagged for deletion
		 * @return
		 * 	- true If shader is currently flagged for deletion
		 * 	- false If otherwise
		 */
		bool IsDeleted () const;

		/**
		 * Display compilation errors from the OpenGL shader compiler
		 */
		void PrintLog ();

		void Clear ();

	protected:

		virtual ~GLSLShader ();

		/**
		 * @brief Read a fragment or vertex shader from a file
		 * @param filename file to read shader from
		 * @return a newly-allocated text buffer containing code.
		 * This buffer must be freed after usage
		 */
		static char* Read (const char *filename);

		/**
		 * @brief Compile a shader from a text buffer
		 * @param source code of the shader
		 * @param type type of shader
		 * @return the shader object on the compiled program
		 */
		static GLuint Compile (const char* source, const GLenum shaderType);

	private:

		GLuint m_id;
	};

}

#endif /* _BLENDINT_SHADER_HPP_ */
