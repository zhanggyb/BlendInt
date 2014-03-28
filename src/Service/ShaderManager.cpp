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
			"layout(location=0) in vec2 Coord2D;"
			"layout(location=1) in vec4 Color;"
			"out vec4 VertexColor;"
			""
			"void main(void) {"
			"	gl_Position = vec4(Coord2D, 0.0, 1.0);"
			"	VertexColor = Color;"
			"}";

	const char* ShaderManager::default_widget_geometry_shader =
			"#version 330\n"
			""
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 24) out;"
			"in vec4 VertexColor[];"
			"uniform bool AA = false;"
			"uniform mat4 MVP;"
			"out vec4 PreFragColor;"
			""
			"const vec2 AA_JITTER[8] = vec2[8]("
			"	vec2(0.468813, -0.481430),"
			"	vec2(-0.155755, -0.352820),"
			"	vec2(0.219306, -0.238501),"
			"	vec2(-0.393286,-0.110949),"
			"	vec2(-0.024699, 0.013908),"
			"	vec2(0.343805, 0.147431),"
			"	vec2(-0.272855, 0.269918),"
			"	vec2(0.095909, 0.388710));"
			""
			"void main()"
			"{"
			"	vec4 vertex;"
			""
			"	if(AA) {"
			"		mat4 trans_matrix = mat4(1.0);"
			"		vec4 col_calib;"
			"		for(int jit = 0; jit < 8; jit++) {"
			"			trans_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
			"			for(int n = 0; n < gl_in.length(); n++)"
			"			{"
			"				vertex = MVP * trans_matrix * gl_in[n].gl_Position;"
			"				col_calib = VertexColor[n];"
			"				col_calib.a = col_calib.a/8;"
			"				PreFragColor = col_calib;"
			"				gl_Position = vertex;"
			"				EmitVertex();"
			"			}"
			"			EndPrimitive();"
			"		}"
			"		return;"
			"	} else {"
			"		for(int n = 0; n < gl_in.length(); n++) {"
			"			vertex = MVP * gl_in[n].gl_Position;"
			"			PreFragColor = VertexColor[n];"
			"			gl_Position = vertex;"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"		return;"
			"	}"
			""
			"}";

	const char* ShaderManager::default_widget_fragment_shader =
			"#version 330\n"
			""
			"in vec4 PreFragColor;"
			"uniform bool AA = false;"
			"uniform int Gamma = 0;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	vec4 color_calib = vec4(0.0);"
			"	if(AA) {"
			"		color_calib = vec4(vec3(min(max(-1.0, Gamma/255.0/8.0), 1.0)), 0.0);"
			"	} else {"
			"		color_calib = vec4(vec3(min(max(-1.0, Gamma/255.0), 1.0)), 0.0);"
			"	}"
			"	FragmentColor = PreFragColor + color_calib;"
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

		m_text_program->AttachShader(text_vertex_shader, GL_VERTEX_SHADER);
		m_text_program->AttachShader(text_fragment_shader, GL_FRAGMENT_SHADER);
		if(!m_text_program->Link()) {
			DBG_PRINT_MSG("Fail to link the text program: %d", m_text_program->id());
			return false;
		}

		m_primitive_program->AttachShader(primitive_vertex_shader, GL_VERTEX_SHADER);
		m_primitive_program->AttachShader(primitive_fragment_shader, GL_FRAGMENT_SHADER);
		if(!m_primitive_program->Link()) {
			DBG_PRINT_MSG("Fail to link the primitive program: %d", m_primitive_program->id());
			return false;
		}

		m_default_widget_program->AttachShader(default_widget_vertex_shader, GL_VERTEX_SHADER);
		m_default_widget_program->AttachShader(default_widget_geometry_shader, GL_GEOMETRY_SHADER);
		m_default_widget_program->AttachShader(default_widget_fragment_shader, GL_FRAGMENT_SHADER);
		if(!m_default_widget_program->Link()) {
			DBG_PRINT_MSG("Fail to link the widget program: %d", m_default_widget_program->id());
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
