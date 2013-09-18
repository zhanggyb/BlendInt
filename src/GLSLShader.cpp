/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include <BILO/GLSLShader.hpp>

namespace BILO {

	GLSLShader::GLSLShader ()
			: id_(0)
	{
	}

	GLSLShader::~GLSLShader ()
	{
		clear();
	}

	GLenum GLSLShader::type () const
	{
		GLint type;
		glGetShaderiv(id_, GL_SHADER_TYPE, &type);

		return type;
	}

	bool GLSLShader::isValid () const
	{
		return glIsShader(id_);
	}

	bool GLSLShader::isDeleted () const
	{
		GLint status;
		glGetShaderiv(id_, GL_DELETE_STATUS, &status);

		return status == GL_TRUE ? true : false;
	}

	void GLSLShader::load (const std::string& filename, GLenum type)
	{
		clear();

		char* buf = read(filename.c_str());

		if (buf) {
			id_ = compile(buf, type);
			free(buf);
		}
	}

	void GLSLShader::load (const char* buf, GLenum type)
	{
		clear();

		if (buf) {
			id_ = compile(buf, type);
		}
	}

	void GLSLShader::clear ()
	{
		if (id_) {
			if (glIsShader(id_)) {
				glDeleteShader(id_);
			}
			id_ = 0;
		}
	}

	void GLSLShader::print_log ()
	{
		GLint log_length = 0;
		if (glIsShader(id_))
			glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_length);
		else {
			fprintf(stderr, "printlog: Not a shader\n");
			return;
		}

		char* log = (char*) malloc(log_length);

		glGetShaderInfoLog(id_, log_length, NULL, log);

		fprintf(stderr, "%s", log);
		free(log);
	}

	char* GLSLShader::read (const char* filename)
	{
		FILE * file;
		char * buffer;
		size_t size;

		file = fopen(filename, "rb");
		if (!file) {
			fprintf( stderr, "Unable to open file \"%s\".\n", filename);
			return NULL;
		}
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer = (char *) malloc((size + 1) * sizeof(char *));
		fread(buffer, sizeof(char), size, file);
		buffer[size] = '\0';
		fclose(file);
		return buffer;
	}

	GLuint GLSLShader::compile (const char* source, const GLenum type)
	{
		GLint compile_status;
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if (compile_status == GL_FALSE) {
			GLchar messages[256];
			glGetShaderInfoLog(shader, sizeof(messages), 0, &messages[0]);
			fprintf( stderr, "%s\n", messages);
			exit( EXIT_FAILURE);
		}
		return shader;
	}

}
