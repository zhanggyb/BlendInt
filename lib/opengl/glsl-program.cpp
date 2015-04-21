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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <blendint/core/types.hpp>

#include <blendint/opengl/opengl.hpp>
#include <blendint/opengl/glsl-program.hpp>

namespace BlendInt {

	GLSLProgram::GLSLProgram ()
			: Object(), m_id(0)
	{
	}

	GLSLProgram::~GLSLProgram ()
	{
		Clear();
	}

	bool GLSLProgram::Create ()
	{
		Clear();

		m_id = glCreateProgram();

		if (glIsProgram(m_id))
			return true;
		else
			return false;
	}

	void GLSLProgram::AttachShaderPair(const char* vertex_shader, const char* fragment_shader)
	{
		AttachShader(vertex_shader, GL_VERTEX_SHADER);
		AttachShader(fragment_shader, GL_FRAGMENT_SHADER);
	}

	void GLSLProgram::LoadShaderPairFile(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		LoadShaderFile(vertex_shader, GL_VERTEX_SHADER);
		LoadShaderFile(fragment_shader, GL_FRAGMENT_SHADER);
	}

	void GLSLProgram::LoadShaderFile (const std::string& filename, GLenum type)
	{
		if (glIsProgram(m_id)) {
			GLuint shader = GLSLShader::Create(filename, type);
			glAttachShader(m_id, shader);
		}
	}

	void GLSLProgram::AttachShader (const char* buf, GLenum type)
	{
		if (glIsProgram(m_id)) {
			GLuint shader = GLSLShader::Create(buf, type);
			glAttachShader(m_id, shader);
		}
	}

	void GLSLProgram::AttachShader (const GLSLShader& shader)
	{
		if (glIsProgram(m_id)) {
			glAttachShader(m_id, shader.id());
		}
	}

	bool GLSLProgram::IsValid () const
	{
		return glIsProgram(m_id);
	}

	bool GLSLProgram::Link ()
	{
		GLint link_ok = GL_FALSE;

		if (glIsProgram(m_id)) {
			glLinkProgram(m_id);
			glGetProgramiv(m_id, GL_LINK_STATUS, &link_ok);
			if (!link_ok) {
				std::cerr << "Fail to glLinkProgram" << std::endl;
			}
		}

		return link_ok == GL_TRUE ? true : false;
	}

	bool GLSLProgram::DetachShader(GLuint shader)
	{
		bool ret = false;

		if(glIsProgram(m_id)) {
			glDetachShader(m_id, shader);

			ret = glGetError() == GL_NO_ERROR? true : false;
		}

		return ret;
	}

	bool GLSLProgram::DetachShader(const GLSLShader& shader)
	{
		bool ret = false;

		if(glIsProgram(m_id)) {
			glDetachShader(m_id, shader.id());
			ret = glGetError() == GL_NO_ERROR? true : false;
		}

		return ret;
	}

	void GLSLProgram::Clear ()
	{
		if(glIsProgram(m_id)) {

			GLint num = 0;
			glGetProgramiv(m_id, GL_ATTACHED_SHADERS, &num);

			if(num) {

				GLuint shaders[32];
				GLsizei max = 32;
				GLsizei count = 0;

				glGetAttachedShaders(m_id, max, &count, shaders);

				do {
					for(int i = 0; i < count; i++) {
						glDetachShader(m_id, shaders[i]);
						glDeleteShader(shaders[i]);
					}

					glGetAttachedShaders(m_id, max, &count, shaders);
				} while (count);
			}

			glDeleteProgram(m_id);
		}

		m_id = 0;

		/* old code
		std::vector<GLSLShader*>::iterator it;
		for (it = m_shaders.begin(); it != m_shaders.end(); it++) {
			Object::Destroy(*it);
		}
		m_shaders.clear();

		if (m_id) {
			if (glIsProgram(m_id))
				glDeleteProgram(m_id);
			m_id = 0;
		}
		*/
	}

	GLint GLSLProgram::GetAttributeLocation (const char *name)
	{
		GLint attribute_location = -1;

		attribute_location = glGetAttribLocation(m_id, name);

		if(attribute_location == -1)
			DBG_PRINT_MSG("Cannot bind attribute %s", name);

		return attribute_location;
	}

	GLint GLSLProgram::GetUniformLocation (const char *name)
	{
		GLint uniform_location = -1;

		uniform_location = glGetUniformLocation(m_id, name);

		if(uniform_location == -1)
			DBG_PRINT_MSG("Cannot bind uniform %s", name);

		return uniform_location;
	}

	void GLSLProgram::SetVertexAttrib1f(GLuint index, GLfloat v0)
	{
		glVertexAttrib1f(index, v0);
	}

	bool GLSLProgram::SetVertexAttrib1f(const char* name, GLfloat v0)
	{
		GLint attribute_location = GetAttributeLocation(name);

		if(attribute_location < 0) return false;

		glVertexAttrib1f(attribute_location, v0);

		return true;
	}

	void GLSLProgram::SetVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
	{
		glVertexAttrib2f(index, v0, v1);

	}

	bool GLSLProgram::SetVertexAttrib2f(const char* name, GLfloat v0, GLfloat v1)
	{
		GLint attribute_location = GetAttributeLocation(name);

		if(attribute_location < 0) return false;

		glVertexAttrib2f(attribute_location, v0, v1);

		return true;
	}

	void GLSLProgram::SetVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		glVertexAttrib3f(index, v0, v1, v2);
	}

	bool GLSLProgram::SetVertexAttrib3f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		GLint attribute_location = GetAttributeLocation(name);

		if(attribute_location < 0) return false;

		glVertexAttrib3f(attribute_location, v0, v1, v2);

		return true;
	}

	void GLSLProgram::SetVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		glVertexAttrib4f(index, v0, v1, v2, v3);
	}

	bool GLSLProgram::SetVertexAttrib4f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		GLint attribute_location = GetAttributeLocation(name);

		if(attribute_location < 0) return false;

		glVertexAttrib4f(attribute_location, v0, v1, v2, v3);

		return true;
	}

	void GLSLProgram::SetVertexAttrib4fv(GLuint index, const GLfloat* v)
	{
		glVertexAttrib4fv(index, v);
	}

	bool GLSLProgram::SetVertexAttrib4fv(const char* name, const GLfloat* v)
	{
		GLint attribute_location = GetAttributeLocation(name);

		if(attribute_location < 0) return false;

		glVertexAttrib4fv(attribute_location, v);

		return true;
	}

	bool GLSLProgram::SetUniform4fv(const char* name, GLsizei count, const GLfloat* value)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform4fv (uniform_location, count, value);

		return true;
	}

	void GLSLProgram::SetUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix3fv (location, count, transpose, value);
	}

	void GLSLProgram::SetUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix4fv (location, count, transpose, value);
	}

	void GLSLProgram::SetUniform1i(GLint location, GLint v0)
	{
		glUniform1i (location, v0);
	}

	bool GLSLProgram::SetUniform1i(const char* name, GLint v0)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform1i (uniform_location, v0);

		return true;

	}

	bool GLSLProgram::SetUniform1f(const char* name, GLfloat v0)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform1f (uniform_location, v0);

		return true;

	}

	bool GLSLProgram::SetUniform2f(const char* name, GLfloat v0, GLfloat v1)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform2f (uniform_location, v0, v1);

		return true;
	}

	void GLSLProgram::SetUniform3f(GLint location, GLfloat v0, GLfloat v1,
		GLfloat v2)
	{
		glUniform3f (location, v0, v1, v2);
	}

	bool GLSLProgram::SetUniform3f(const char* name, GLfloat v0, GLfloat v1,
		GLfloat v2)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform3f (uniform_location, v0, v1, v2);

		return true;
	}

	bool GLSLProgram::SetUniform3fv(const char* name, GLsizei count, const GLfloat* value)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform3fv (uniform_location, count, value);

		return true;
	}

	void GLSLProgram::SetUniform4f(GLint location, GLfloat v0, GLfloat v1,
		GLfloat v2, GLfloat v3)
	{
		glUniform4f (location, v0, v1, v2, v3);
	}

	bool GLSLProgram::SetUniform4f(const char* name, GLfloat v0, GLfloat v1,
		GLfloat v2, GLfloat v3)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniform4f (uniform_location, v0, v1, v2, v3);
		return true;
	}

	bool GLSLProgram::SetUniformMatrix3fv(const char* name, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniformMatrix3fv (uniform_location, count, transpose, value);

		return true;
	}

	bool GLSLProgram::SetUniformMatrix4fv(const char* name, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		GLint uniform_location = GetUniformLocation(name);

		if(uniform_location < 0) return false;

		glUniformMatrix4fv (uniform_location, count, transpose, value);

		return true;
	}

	void GLSLProgram::PrintLog ()
	{
		GLint log_length = 0;

		if (glIsProgram(m_id))
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_length);
		else {
			fprintf(stderr, "printlog: Not a program\n");
			return;
		}

		char* log = (char*) malloc(log_length);

		if (glIsProgram(m_id))
			glGetProgramInfoLog(m_id, log_length, NULL, log);

		fprintf(stderr, "%s", log);
		free(log);
	}

	bool GLSLProgram::GetUniformfv (const char* name, GLfloat* params)
	{
		GLint location = GetUniformLocation(name);

		if(location < 0) return false;

		glGetUniformfv (m_id, location, params);

		return true;
	}

	void GLSLProgram::GetUniformfv (GLint location, GLfloat* params)
	{
		glGetUniformfv(m_id, location, params);
	}

}
