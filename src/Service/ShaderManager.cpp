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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#include <BlendInt/Types.hpp>
#endif

#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

#ifdef __OPENGL_CORE_330__
	const char* ShaderManager::text_vertex_shader =
			"#version 330\n"
			"layout(location = 0) in vec4 coord;"
			"uniform mat4 MVP;"
			"out vec2 texpos;"
			""
			"void main(void) {"
			"  gl_Position = MVP * vec4(coord.xy, 0.0, 1.0);"
			"  texpos = coord.zw;"
			"}";

	const char* ShaderManager::text_fragment_shader =
			"#version 330\n"
			"in vec2 texpos;"
			"uniform sampler2D tex;"
			"uniform vec4 color;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	float alpha = texture(tex, texpos).r;"	// GL 3.2 only support GL_R8 in glTexImage2D internalFormat
			"	FragmentColor = vec4(color.rgb, color.a * alpha);"
			"}";

	const char* ShaderManager::primitive_vertex_shader =
			"#version 330\n"
			""
			"layout(location = 0) in vec3 coord3d;"
			"layout(location = 1) in vec3 v_color;"
			"uniform mat4 MVP;"
			"out vec3 f_color;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(coord3d, 1.0);"
			"	f_color = v_color;"
			"}";

	const char* ShaderManager::primitive_fragment_shader =
			"#version 330\n"
			""
			"in vec3 f_color;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	FragmentColor = vec4(f_color, 1.0);"
			"}";

	const char* ShaderManager::default_widget_vertex_shader =
			"#version 330\n"
			""
			"layout(location=0) in vec2 xy;"
			"layout(location=1) in vec4 color;"
			"in float z;"
			"uniform mat4 MVP;"
			"out vec4 f_color;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(xy, z, 1.0);"
			"	f_color = color;"
			"}";

	const char* ShaderManager::default_widget_fragment_shader =
			"#version 330\n"
			""
			"in vec4 f_color;"
			"uniform int gamma = 0;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	vec4 color_calib = vec4(vec3(min(max(-1.0, gamma/255.0), 1.0)), 0.0);"
			"	FragmentColor = f_color + color_calib;"
			"}";

	const char* ShaderManager::default_form_vertex_shader =
			"#version 330\n"
			""
			"layout(location = 0) in vec2 xy;"
			"layout(location = 1) in vec4 color;"
			"uniform mat4 MVP;"
			"out vec4 f_color;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(xy, 0.0, 1.0);"
			"	f_color = color;"
			"}";

	const char* ShaderManager::default_form_fragment_shader =
			"#version 330\n"
			""
			"in vec4 f_color;"
			"uniform int gamma = 0;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	vec4 color_calib = vec4(vec3(min(max(-1.0, gamma/255.0), 1.0)), 0.0);"
			"	FragmentColor = f_color + color_calib;"
			"}";

#else	// Legacy opengl

	const char* ShaderManager::text_vertex_shader =
			"#version 120\n"
			"attribute vec4 coord;"
			"uniform mat4 MVP;"
			"varying vec2 texpos;"
			""
			"void main(void) {"
			"  gl_Position = MVP * vec4(coord.xy, 0.0, 1.0);"
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
			"	gl_FragColor = vec4(f_color, 1.0);"
			"}";

	const char* ShaderManager::default_widget_vertex_shader =
			"#version 120\n"
			""
			"attribute vec2 xy;"
			"attribute float z;"
			"attribute vec4 color;"
			"uniform mat4 MVP;"
			"varying vec4 f_color;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(xy, z, 1.0);"
			"	f_color = color;"
			"}";

	const char* ShaderManager::default_widget_fragment_shader =
			"#version 120\n"
			""
			"varying vec4 f_color;"
			""
			"void main(void) {"
			"	gl_FragColor = f_color;"
			"}";

	const char* ShaderManager::default_form_vertex_shader =
			"#version 120\n"
			""
			"attribute vec2 xy;"
			"attribute vec4 color;"
			"uniform mat4 MVP;"
			"varying vec4 f_color;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(xy, 0.0, 1.0);"
			"	f_color = color;"
			"}";

	const char* ShaderManager::default_form_fragment_shader =
			"#version 120\n"
			""
			"varying vec4 f_color;"
			""
			"void main(void) {"
			"	gl_FragColor = f_color;"
			"}";

#endif

	ShaderManager* ShaderManager::instance = 0;

	bool ShaderManager::Initialize()
	{
		bool ret = false;

		if(!instance) {
			instance = new ShaderManager;

			if(instance) {
				ret = instance->Setup();
			} else {
				ret = false;
			}
		}

		return ret;
	}

	void ShaderManager::Release()
	{
		if(instance) {
			delete instance;
			instance = 0;
		}
	}

	ShaderManager::ShaderManager()
	: m_uniform_mvp(0)
	{
		m_text_program.reset(new GLSLProgram);
#ifdef DEBUG
		m_text_program->set_name("Text GLSLProgram");
#endif

		m_primitive_program.reset(new GLSLProgram);
#ifdef DEBUG
		m_primitive_program->set_name("Primitive GLSLProgram");
#endif

		m_default_widget_program.reset(new GLSLProgram);
#ifdef DEBUG
		m_default_widget_program->set_name("Widget GLSLProgram");
#endif

		m_default_form_program.reset(new GLSLProgram);
#ifdef DEBUG
		m_default_form_program->set_name("Form GLSLProgram");
#endif

	}

	ShaderManager::~ShaderManager()
	{
	}

	bool ShaderManager::Setup ()
	{
		if(!m_text_program->Create())
			return false;

		if(!m_primitive_program->Create()) {
			return false;
		}

		if(!m_default_widget_program->Create()) {
			return false;
		}

		if(!m_default_form_program->Create()) {
			return false;
		}

		m_text_program->AttachShaderPair(text_vertex_shader, text_fragment_shader);
		if(!m_text_program->Link()) {
			DBG_PRINT_MSG("Fail to link the text program: %d", m_text_program->id());
			return false;
		}

		m_primitive_program->AttachShaderPair(primitive_vertex_shader, primitive_fragment_shader);
		if(!m_primitive_program->Link()) {
			DBG_PRINT_MSG("Fail to link the primitive program: %d", m_primitive_program->id());
			return false;
		}

		m_default_widget_program->AttachShaderPair(default_widget_vertex_shader, default_widget_fragment_shader);
		if(!m_default_widget_program->Link()) {
			DBG_PRINT_MSG("Fail to link the widget program: %d", m_default_widget_program->id());
			return false;
		}

		m_default_form_program->AttachShaderPair(default_form_vertex_shader, default_form_fragment_shader);
		if(!m_default_form_program->Link()) {
			DBG_PRINT_MSG("Fail to link the default form program: %d", m_default_form_program->id());
			return false;
		}

		m_uniform_mvp = m_primitive_program->GetUniformLocation("MVP");
		if(m_uniform_mvp == -1) {
			return false;
		}

		return true;
	}

//	bool ShaderManager::Find(const GLSLProgram* program)
//	{
//		if(m_text_program == program) return true;
//		if(m_primitive_program == program) return true;
//
//		return false;
//	}
}
