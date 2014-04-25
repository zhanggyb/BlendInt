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

#ifndef _BLENDINT_PROGRAM_HPP_
#define _BLENDINT_PROGRAM_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/OpenGL/GLSLShader.hpp>

namespace BlendInt {

	/**
	 * @brief Wrapper class for OpenGL Programs
	 *
	 * Usage:
	 * 	-# Define or new a GLSLProgram object, e.g. GLSLProgram foo; // or GLSLProgram* foo = new GLSLProgram;
	 * 	-# Call Create(): foo.Create()
	 * 	-# Attach shaders: foo.AttachShaderPair(vs_shader, fs_shader);
	 * 	-# Call Link(): foo.Link()
	 * 	-# Call Use() for drawing: foo.Use();	// and draw sth
	 * 	-# Call Reset() after drawing: foo.Reset();
	 *
	 * @ingroup opengl
	 */
	class GLSLProgram: public Object
	{
	public:

		/**
		 * @brief Default constructor
		 */
		GLSLProgram ();

		/**
		 * @brief Destructor
		 */
		virtual ~GLSLProgram ();

		/**
		 * @brief Create the GLSL program
		 * @return
		 * 	- true if success
		 * 	- false if fail
		 */
		bool Create ();

		/**
		 * @brief Attach shader
		 * @param[in] buf The shader stored in memory as string
		 * @param[in] type The shader type, must be one of:
		 * 	- GL_VERTEX_SHADER
		 * 	- GL_TESS_CONTROL_SHADER
		 * 	- GL_TESS_EVALUATION_SHADER
		 * 	- GL_GEOMETRY_SHADER
		 * 	- GL_FRAGMENT_SHADER
		 *
		 * The param type is defined in manual of glCreateShader
		 */
		void AttachShader (const char* buf, GLenum type);

		/**
		 * @brief Attach shader
		 * @param[in] filename The path of the shader file
		 * @param[in] type The shader type, must be one of:
		 * 	- GL_VERTEX_SHADER
		 * 	- GL_TESS_CONTROL_SHADER
		 * 	- GL_TESS_EVALUATION_SHADER
		 * 	- GL_GEOMETRY_SHADER
		 * 	- GL_FRAGMENT_SHADER
		 *
		 * The param type is defined in manual of glCreateShader
		 */
		void LoadShaderFile (const std::string& filename, GLenum type);

		void AttachShader (const GLSLShader& shader);

		/**
		 * @brief Attach vertex and fragment shaders
		 * @param vertex_shader The vertex shader stored in memory as string
		 * @param fragment_shader The vertex shader stored in memory as string
		 */
		void AttachShaderPair (const char* vertex_shader,
		        const char* fragment_shader);

		/**
		 * @brief Attach vertex and fragment shaders
		 * @param vertex_shader File name of the vertex shader
		 * @param fragment_shader File name of the fragment shader
		 */
		void LoadShaderPairFile (const std::string& vertex_shader, const std::string& fragment_shader);

		/**
		 * @brief Link the program
		 * @return
		 * 	- true if success
		 * 	- false if fail
		 *
		 * 	@note this function should be called after attaching correct shaders
		 */
		bool Link ();

		bool DetachShader (GLuint shader);

		bool DetachShader (const GLSLShader& shader);

		/**
		 * @brief Get if the program is valid
		 * @return
		 * 	- true if valid
		 * 	- false if not valid
		 */
		bool IsValid () const;

		/**
		 * @brief Clear all shaders created
		 */
		void Clear ();

		/**
		 * @brief Use this program for render
		 */
		void Use ();

		/**
		 * @brief Unuse this program for render
		 */
		static void Reset ();

		/**
		 * @brief Print log
		 */
		void PrintLog ();

		/**
		 * @brief Get the program id
		 * @return GLSL program id
		 */
		GLuint id () const
		{
			return m_id;
		}

		/**
		 * @brief Get attribute location
		 * @param name The attribute name
		 * @return The attribute location
		 */
		GLint GetAttributeLocation (const char *name);

		/**
		 * @brief Get uniform location
		 * @param name The uniform name
		 * @return The uniform location
		 */
		GLint GetUniformLocation (const char *name);

		void SetVertexAttrib1f (GLuint index, GLfloat v0);

		bool SetVertexAttrib1f (const char* name, GLfloat v0);

		void SetVertexAttrib2f (GLuint index, GLfloat v0, GLfloat v1);

		bool SetVertexAttrib2f (const char* name, GLfloat v0, GLfloat v2);

		void SetVertexAttrib3f (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2);

		bool SetVertexAttrib3f (const char* name, GLfloat v0, GLfloat v1, GLfloat v2);

		void SetVertexAttrib4f (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

		bool SetVertexAttrib4f (const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

		void SetVertexAttrib4fv (GLuint index, const GLfloat* v);

		bool SetVertexAttrib4fv (const char* name, const GLfloat* v);

		void SetUniform1i (GLint location, GLint v0);

		bool SetUniform1i (const char* name, GLint v0);

		bool SetUniform1f (const char* name, GLfloat v0);

		bool SetUniform2f (const char* name, GLfloat v0, GLfloat v1);

		bool SetUniform3f (const char* name, GLfloat v0, GLfloat v1, GLfloat v2);

		void SetUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

		bool SetUniform4f (const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

		bool SetUniform4fv (const char* name, GLsizei count, const GLfloat* value);

		void SetUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

		bool SetUniformMatrix4fv (const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);

	private:

		GLuint m_id;
	};

}

#endif /* _BIL_PROGRAM_HPP_ */
