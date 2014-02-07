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
#include <stdlib.h>
#include <stdio.h>

#include <BlendInt/ShaderManager.hpp>

namespace BlendInt {

	/**
	 * Store all the file's contents in memory, useful to pass shaders
	 * source code to OpenGL
	 */
	char* file_read(const char* filename)
	{
	  FILE* in = fopen(filename, "rb");
	  if (in == NULL) return NULL;

	  int res_size = BUFSIZ;
	  char* res = (char*)malloc(res_size);
	  int nb_read_total = 0;

	  while (!feof(in) && !ferror(in)) {
	    if (nb_read_total + BUFSIZ > res_size) {
	      if (res_size > 10*1024*1024) break;
	      res_size = res_size * 2;
	      res = (char*)realloc(res, res_size);
	    }
	    char* p_res = res + nb_read_total;
	    nb_read_total += fread(p_res, 1, BUFSIZ, in);
	  }

	  fclose(in);
	  res = (char*)realloc(res, nb_read_total + 1);
	  res[nb_read_total] = '\0';
	  return res;
	}

	/**
	 * Display compilation errors from the OpenGL shader compiler
	 */
	void print_log(GLuint object)
	{
	  GLint log_length = 0;
	  if (glIsShader(object))
	    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	  else if (glIsProgram(object))
	    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	  else {
	    fprintf(stderr, "printlog: Not a shader or a program\n");
	    return;
	  }

	  char* log = (char*)malloc(log_length);

	  if (glIsShader(object))
	    glGetShaderInfoLog(object, log_length, NULL, log);
	  else if (glIsProgram(object))
	    glGetProgramInfoLog(object, log_length, NULL, log);

	  fprintf(stderr, "%s", log);
	  free(log);
	}

	/**
	 * Compile the shader from file 'filename', with error handling
	 */
	GLuint create_shader(const char* filename, GLenum type)
	{
	  const GLchar* source = file_read(filename);
	  if (source == NULL) {
	    fprintf(stderr, "Error opening %s: ", filename); perror("");
	    return 0;
	  }
	  GLuint res = glCreateShader(type);
	  const GLchar* sources[] = {
	    // Define GLSL version
	#ifdef GL_ES_VERSION_2_0
	    "#version 100\n"
	#else
	    "#version 120\n"
	#endif
	    ,
	    // GLES2 precision specifiers
	#ifdef GL_ES_VERSION_2_0
	    // Define default float precision for fragment shaders:
	    (type == GL_FRAGMENT_SHADER) ?
	    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
	    "precision highp float;           \n"
	    "#else                            \n"
	    "precision mediump float;         \n"
	    "#endif                           \n"
	    : ""
	    // Note: OpenGL ES automatically defines this:
	    // #define GL_ES
	#else
	    // Ignore GLES 2 precision specifiers:
	    "#define lowp   \n"
	    "#define mediump\n"
	    "#define highp  \n"
	#endif
	    ,
	    source };
	  glShaderSource(res, 3, sources, NULL);
	  free((void*)source);

	  glCompileShader(res);
	  GLint compile_ok = GL_FALSE;
	  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	  if (compile_ok == GL_FALSE) {
	    fprintf(stderr, "%s:", filename);
	    print_log(res);
	    glDeleteShader(res);
	    return 0;
	  }

	  return res;
	}

	GLuint create_program(const char *vertexfile, const char *fragmentfile) {
		GLuint program = glCreateProgram();
		GLuint shader;

		if(vertexfile) {
			shader = create_shader(vertexfile, GL_VERTEX_SHADER);
			if(!shader)
				return 0;
			glAttachShader(program, shader);
		}

		if(fragmentfile) {
			shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
			if(!shader)
				return 0;
			glAttachShader(program, shader);
		}

		glLinkProgram(program);
		GLint link_ok = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			fprintf(stderr, "glLinkProgram:");
			print_log(program);
			glDeleteProgram(program);
			return 0;
		}

		return program;
	}

	#ifdef GL_GEOMETRY_SHADER
	GLuint create_gs_program(const char *vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint vertices) {
		GLuint program = glCreateProgram();
		GLuint shader;

		if(vertexfile) {
			shader = create_shader(vertexfile, GL_VERTEX_SHADER);
			if(!shader)
				return 0;
			glAttachShader(program, shader);
		}

		if(geometryfile) {
			shader = create_shader(geometryfile, GL_GEOMETRY_SHADER);
			if(!shader)
				return 0;
			glAttachShader(program, shader);

			glProgramParameteri(program, GL_GEOMETRY_INPUT_TYPE_EXT, input);
			glProgramParameteri(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, output);
			glProgramParameteri(program, GL_GEOMETRY_VERTICES_OUT_EXT, vertices);
		}

		if(fragmentfile) {
			shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
			if(!shader)
				return 0;
			glAttachShader(program, shader);
		}

		glLinkProgram(program);
		GLint link_ok = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			fprintf(stderr, "glLinkProgram:");
			print_log(program);
			glDeleteProgram(program);
			return 0;
		}

		return program;
	}
	#else
	GLuint create_gs_program(const char *vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint vertices) {
		fprintf(stderr, "Missing support for geometry shaders.\n");
		return 0;
	}
	#endif

	GLint get_attrib(GLuint program, const char *name) {
		GLint attribute = glGetAttribLocation(program, name);
		if(attribute == -1)
			fprintf(stderr, "Could not bind attribute %s\n", name);
		return attribute;
	}

	GLint get_uniform(GLuint program, const char *name) {
		GLint uniform = glGetUniformLocation(program, name);
		if(uniform == -1)
			fprintf(stderr, "Could not bind uniform %s\n", name);
		return uniform;
	}

	const char* ShaderManager::text_vertex_shader =
			"#version 120\n"
			"attribute vec4 coord;"
			"varying vec2 texpos;"
			""
			"void main(void) {"
			"  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord.xy, 0, 1);"
			"  texpos = coord.zw;"
			"}";

	const char* ShaderManager::text_fragment_shader =
			"#version 120\n"
			"varying vec2 texpos;"
			"uniform sampler2D tex;"
			"uniform vec4 color;"
			""
			"void main(void) {"
			"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;"
			"}";

	const char* ShaderManager::primitive_vertex_shader =
			"#version 120\n"
			"uniform mat4 MVP;"
			""
			"void main(void) {"
			"  gl_Position = MVP * gl_Vertex;"
			"}";

	const char* ShaderManager::primitive_fragment_shader =
			"#version 120\n"
			""
			"void main(void) {"
			"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
			"}";

	ShaderManager* ShaderManager::shader_manager = 0;

	bool ShaderManager::Initialize()
	{
		if(!shader_manager) {
			shader_manager = new ShaderManager;
		}

		return shader_manager->Setup();
	}

	void ShaderManager::Release()
	{
		if(shader_manager) {
			delete shader_manager;
			shader_manager = 0;
		}
	}

	ShaderManager* ShaderManager::Instance()
	{
		if (!shader_manager) {
			std::cerr << "The Shader Manager is not initialized successfully! Exit" << std::endl;
			exit (EXIT_FAILURE);
		}

		return shader_manager;
	}

	ShaderManager::ShaderManager()
	: text_attribute_coord_(-1),
	text_uniform_tex_(-1),
	text_uniform_color_(-1),
	text_vbo_(0),
	m_text_program(0),
	m_uniform_mvp(0),
	m_primitive_program(0)
	{
		m_text_program = new GLSLProgram;
#ifdef DEBUG
		m_text_program->set_name("Text GLSLProgram");
#endif
		Object::Retain(m_text_program);

		m_primitive_program = new GLSLProgram;
#ifdef DEBUG
		m_primitive_program->set_name("Primitive GLSLProgram");
#endif
		Object::Retain(m_primitive_program);
	}

	ShaderManager::~ShaderManager()
	{
		Object::Destroy(m_text_program);
		Object::Destroy(m_primitive_program);

		glDeleteBuffers(1, &text_vbo_);
	}

	bool ShaderManager::Setup ()
	{
		if(!m_text_program->Create())
			return false;

		if(!m_primitive_program->Create()) {
			return false;
		}

		m_text_program->AttachShaderPair(text_vertex_shader, text_fragment_shader);
		if(!m_text_program->Link()) {
			return false;
		}

		m_primitive_program->AttachShaderPair(primitive_vertex_shader, primitive_fragment_shader);
		if(!m_primitive_program->Link()) {
			return false;
		}

		text_attribute_coord_ = m_text_program->GetAttributeLocation("coord");
		text_uniform_tex_ = m_text_program->GetUniformLocation("tex");
		text_uniform_color_ = m_text_program->GetUniformLocation("color");
		if(text_attribute_coord_ == -1 || text_uniform_tex_ == -1 || text_uniform_color_ == -1) {
			std::cerr << "Error: cannot get attributes and uniforms" << std::endl;
			return false;
		}

		m_uniform_mvp = m_primitive_program->GetUniformLocation("MVP");
		if(m_uniform_mvp == -1) {
			return false;
		}

		// Create the vertex buffer object
		if(glIsBuffer(text_vbo_)) {
			glDeleteBuffers(1, &text_vbo_);
		}

		glGenBuffers(1, &text_vbo_);
		/*
		if(!glIsBuffer(text_vbo_)) {
			std::cerr << "Error: cannot generate buffer object for text" << std::endl;
			return false;
		}
		*/

		return true;
	}

	bool ShaderManager::Find(const GLSLProgram* program)
	{
		if(m_text_program == program) return true;

		return false;
	}
}
