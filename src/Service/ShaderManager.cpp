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

#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

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
			""
			"attribute vec3 coord3d;"
			"attribute vec3 v_color;"
			"uniform mat4 ModelViewProjectionMatrix;"
			"varying vec3 f_color;"
			""
			"void main(void) {"
			"	gl_Position = ModelViewProjectionMatrix * vec4(coord3d, 1.0);"
			"	f_color = v_color;"
			"}";

	const char* ShaderManager::primitive_fragment_shader =
			"#version 120\n"
			""
			"varying vec3 f_color;"
			""
			"void main(void) {"
			"	gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);"
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

		m_uniform_mvp = m_primitive_program->GetUniformLocation("ModelViewProjectionMatrix");
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
		if(m_primitive_program == program) return true;

		return false;
	}
}
