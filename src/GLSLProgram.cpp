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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <BlendInt/GLSLProgram.hpp>

namespace BlendInt {

	GLSLProgram::GLSLProgram ()
			: m_id(0)
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

		if(m_id) return true;
		else return false;
	}

	void GLSLProgram::AttachShaderPair(const char* vertex_shader, const char* fragment_shader)
	{
		if(m_shaders.size() > 0) {
			std::vector<GLSLShader*>::iterator it;
			for (it = m_shaders.begin(); it != m_shaders.end(); it++) {
				delete *it;
				*it = NULL;
			}
			m_shaders.clear();
		}

		AttachShader(vertex_shader, GL_VERTEX_SHADER);
		AttachShader(fragment_shader, GL_FRAGMENT_SHADER);
	}

	void GLSLProgram::AttachShaderPair(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		if(m_shaders.size() > 0) {
			std::vector<GLSLShader*>::iterator it;
			for (it = m_shaders.begin(); it != m_shaders.end(); it++) {
				delete *it;
				*it = NULL;
			}
			m_shaders.clear();
		}

		AttachShader(vertex_shader, GL_VERTEX_SHADER);
		AttachShader(fragment_shader, GL_FRAGMENT_SHADER);
	}

	void GLSLProgram::AttachShader (const std::string& filename, GLenum type)
	{
		if (glIsProgram(m_id)) {
			GLSLShader* shader = new GLSLShader;
			shader->Load(filename, type);
			m_shaders.push_back(shader);
			glAttachShader(m_id, shader->id());
		}
	}

	void GLSLProgram::AttachShader (const char* buf, GLenum type)
	{
		if (glIsProgram(m_id)) {
			GLSLShader* shader = new GLSLShader;
			shader->Load(buf, type);
			m_shaders.push_back(shader);
			glAttachShader(m_id, shader->id());
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

	void GLSLProgram::Clear ()
	{
		std::vector<GLSLShader*>::iterator it;
		for (it = m_shaders.begin(); it != m_shaders.end(); it++) {
			delete *it;
			*it = NULL;
		}
		m_shaders.clear();

		if (m_id) {
			if (glIsProgram(m_id))
				glDeleteProgram(m_id);
			m_id = 0;
		}
	}

	GLint GLSLProgram::GetAttributeLocation (const char *name)
	{
		GLint attribute = -1;
		if(glIsProgram(m_id)) {
			attribute = glGetAttribLocation(m_id, name);
		}

		if(attribute == -1)
			fprintf(stderr, "Could not bind attribute %s\n", name);

		return attribute;
	}

	GLint GLSLProgram::GetUniformLocation (const char *name)
	{
		GLint uniform = -1;

		if(glIsProgram(m_id)) {
			uniform = glGetUniformLocation(m_id, name);
		}

		if(uniform == -1)
			fprintf(stderr, "Could not bind uniform %s\n", name);

		return uniform;
	}

	void GLSLProgram::Activate()
	{
		if (glIsProgram(m_id))
			glUseProgram (m_id);
	}

	void GLSLProgram::Deactivate()
	{
		glUseProgram(0);
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

}
