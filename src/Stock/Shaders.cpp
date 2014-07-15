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

#include <BlendInt/Core/Types.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	namespace Stock {

#ifdef __OPENGL_CORE_330__
		const char* Shaders::text_vertex_shader =
				"#version 330\n"
				"layout(location = 0) in vec4 a_coord;"
				"out vec2 uv;"
				""
				"uniform mat4 u_projection;"	// projection matrix
        		"uniform mat4 u_view;"// view matrix
				"uniform vec3 u_position;"// position
        		"uniform float u_rotation = 0.f;"// the rotation in degree, only support rotation along Z axis
				""
		        "mat4 TranslateMatrix (const in vec3 t)"
		        "{"
		        "	return mat4(1.0, 0.0, 0.0, 0.0,"
		        "				0.0, 1.0, 0.0, 0.0,"
		        "				0.0, 0.0, 1.0, 0.0,"
		        "				t.x, t.y, t.z, 1.0);"
		        "}"
		        ""
		        "mat4 RotateMatrix( const in float angle,"
		        "					const in vec3 axis )"
		        "{"
		        "	vec3 n = normalize( axis );"
		        "	float theta = radians( angle );"
		        "	float c = cos( theta );"
		        "	float s = sin( theta );"
		        "	mat3 R;"
		        "	R[0] = n.xyz*n.x*(1.0-c) + vec3(      c,  n.z*s, -n.y*s );"
		        "	R[1] = n.xyz*n.y*(1.0-c) + vec3( -n.z*s,      c,  n.x*s );"
		        "	R[2] = n.xyz*n.z*(1.0-c) + vec3(  n.y*s, -n.x*s,      c );"
		        "	return mat4( R[0],	0.0,"
		        "				 R[1],	0.0,"
		        "				 R[2],	0.0,"
		        "				 0.0, 0.0, 0.0, 1.0 );"
		        "}"
		        ""
		        "mat4 RotateMatrixAlongZ (const in float angle)"
		        "{"
		        "	return RotateMatrix(angle, vec3(0.0, 0.0, 1.0));"
		        "}"
		        ""
				"void main(void) {"
				"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation);"
				"	gl_Position = mvp * vec4(a_coord.xy, 0.0, 1.0);"
				"	uv = a_coord.zw;"
				"}";

		const char* Shaders::text_fragment_shader =
				"#version 330\n"
				"in vec2 uv;"
				"uniform sampler2D u_tex;"
				"uniform vec4 u_color;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	float alpha = texture(u_tex, uv).r;"// GL 3.2 only support GL_R8 in glTexImage2D internalFormat
				"	FragmentColor = vec4(u_color.rgb, u_color.a * alpha);"
				"}";

		const char* Shaders::primitive_vertex_shader = "#version 330\n"
				""
				"layout(location = 0) in vec3 coord3d;"
				"layout(location = 1) in vec3 v_color;"
				"uniform mat4 m_P;"
				"uniform mat4 m_V;"
				"uniform mat4 m_M;"
				""
				"out vec3 f_color;"
				""
				"void main(void) {"
				"	gl_Position = m_P * m_V * m_M * vec4(coord3d, 1.0);"
				"	f_color = v_color;"
				"}";

		const char* Shaders::primitive_fragment_shader =
				"#version 330\n"
				""
				"in vec3 f_color;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	FragmentColor = vec4(f_color, 1.0);"
				"}";

		const char* Shaders::default_widget_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec2 a_coord;"
				"layout(location=1) in vec4 a_color;"
				"out vec4 VertexColor;"
				""
				"void main(void) {"
				"	gl_Position = vec4(a_coord, 0.0, 1.0);"
				"	VertexColor = a_color;"
				"}";

		const char* Shaders::default_widget_triangle_geometry_shader =
		        "#version 330\n"
				        ""
				        "layout (triangles) in;"
				        "layout (triangle_strip, max_vertices = 24) out;"
				        "in vec4 VertexColor[];"
				        "out vec4 PreFragColor;"
						"uniform mat4 u_projection;"	// projection matrix
		        		"uniform mat4 u_view;"// view matrix
						"uniform vec3 u_position;"// position
		        		"uniform float u_rotation = 0.f;"// the rotation in degree, only support rotation along Z axis
		        		"uniform vec2 u_scale = vec2(1.f, 1.f);"// the scale factor, only support xy plane
		        		"uniform bool u_AA = false;"
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
				        "mat4 ScaleMatrix (const in vec3 s)"
				        "{"
				        "	return mat4(s.x, 0.0, 0.0, 0.0,"
				        "				0.0, s.y, 0.0, 0.0,"
				        "				0.0, 0.0, s.z, 0.0,"
				        "				0.0, 0.0, 0.0, 1.0);"
				        "}"
				        ""
				        "mat4 TranslateMatrix (const in vec3 t)"
				        "{"
				        "	return mat4(1.0, 0.0, 0.0, 0.0,"
				        "				0.0, 1.0, 0.0, 0.0,"
				        "				0.0, 0.0, 1.0, 0.0,"
				        "				t.x, t.y, t.z, 1.0);"
				        "}"
				        ""
				        "mat4 RotateMatrix( const in float angle,"
				        "					const in vec3 axis )"
				        "{"
				        "	vec3 n = normalize( axis );"
				        "	float theta = radians( angle );"
				        "	float c = cos( theta );"
				        "	float s = sin( theta );"
				        "	mat3 R;"
				        "	R[0] = n.xyz*n.x*(1.0-c) + vec3(      c,  n.z*s, -n.y*s );"
				        "	R[1] = n.xyz*n.y*(1.0-c) + vec3( -n.z*s,      c,  n.x*s );"
				        "	R[2] = n.xyz*n.z*(1.0-c) + vec3(  n.y*s, -n.x*s,      c );"
				        "	return mat4( R[0],	0.0,"
				        "				 R[1],	0.0,"
				        "				 R[2],	0.0,"
				        "				 0.0, 0.0, 0.0, 1.0 );"
				        "}"
				        ""
				        "mat4 RotateMatrixAlongZ (const in float angle)"
				        "{"
				        "	return RotateMatrix(angle, vec3(0.0, 0.0, 1.0));"
				        "}"
				        ""
				        "void main()"
				        "{"
						"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation) * ScaleMatrix(vec3(u_scale.xy, 1.f));"
				        "	vec4 vertex;"
				        ""
				        "	if(u_AA) {"
				        "		mat4 aa_matrix = mat4(1.0);"
				        "		vec4 col_calib;"
				        "		for(int jit = 0; jit < 8; jit++) {"
				        "			aa_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
				        "			for(int n = 0; n < gl_in.length(); n++)"
				        "			{"
				        "				vertex = mvp * aa_matrix * gl_in[n].gl_Position;"
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
				        "			vertex = mvp * gl_in[n].gl_Position;"
				        "			PreFragColor = VertexColor[n];"
				        "			gl_Position = vertex;"
				        "			EmitVertex();"
				        "		}"
				        "		EndPrimitive();"
				        "		return;"
				        "	}"
				        ""
				        "}";

		const char* Shaders::default_widget_line_geometry_shader =
		        "#version 330\n"
				        ""
				        "layout (lines) in;"
				        "layout (line_strip, max_vertices = 16) out;"
				        "in vec4 VertexColor[];"
				        "out vec4 PreFragColor;"
						"uniform mat4 u_projection;"	// projection matrix
						"uniform mat4 u_view;"			// view matrix
						"uniform vec3 u_position;"		// position
						"uniform float u_rotation = 0.f;"		// the rotation in degree, only support rotation along Z axis
						"uniform vec2 u_scale = vec2(1.f, 1.f);"			// the scale factor, only support xy plane
		        		"uniform bool u_AA = false;"
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
						"mat4 ScaleMatrix (const in vec3 s)"
		        		"{"
		        		"	return mat4(s.x, 0.0, 0.0, 0.0,"
		        		"				0.0, s.y, 0.0, 0.0,"
		        		"				0.0, 0.0, s.z, 0.0,"
		        		"				0.0, 0.0, 0.0, 1.0);"
		        		"}"
		        		""
		        		"mat4 TranslateMatrix (const in vec3 t)"
		        		"{"
		        		"	return mat4(1.0, 0.0, 0.0, 0.0,"
						"				0.0, 1.0, 0.0, 0.0,"
		        		"				0.0, 0.0, 1.0, 0.0,"
		        		"				t.x, t.y, t.z, 1.0);"
		        		"}"
		        		""
						"mat4 RotateMatrix( const in float angle,"
						"					const in vec3 axis )"
						"{"
						"	vec3 n = normalize( axis );"
						"	float theta = radians( angle );"
						"	float c = cos( theta );"
						"	float s = sin( theta );"
						"	mat3 R;"
						"	R[0] = n.xyz*n.x*(1.0-c) + vec3(      c,  n.z*s, -n.y*s );"
						"	R[1] = n.xyz*n.y*(1.0-c) + vec3( -n.z*s,      c,  n.x*s );"
						"	R[2] = n.xyz*n.z*(1.0-c) + vec3(  n.y*s, -n.x*s,      c );"
						"	return mat4( R[0],	0.0,"
						"				 R[1],	0.0,"
						"				 R[2],	0.0,"
						"				 0.0, 0.0, 0.0, 1.0 );"
						"}"
						""
		        		"mat4 RotateMatrixAlongZ (const in float angle)"
		        		"{"
						"	return RotateMatrix(angle, vec3(0.0, 0.0, 1.0));"
		        		"}"
						""
				        "void main()"
				        "{"
		        		"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation) * ScaleMatrix(vec3(u_scale.xy, 1.f));"
						"	vec4 vertex;"
				        ""
				        "	if(u_AA) {"
				        "		mat4 aa_matrix = mat4(1.0);"
				        "		vec4 col_calib;"
				        "		for(int jit = 0; jit < 8; jit++) {"
				        "			aa_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
				        "			for(int n = 0; n < gl_in.length(); n++)"
				        "			{"
				        "				vertex = mvp * aa_matrix * gl_in[n].gl_Position;"
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
				        "			vertex = mvp * gl_in[n].gl_Position;"
				        "			PreFragColor = VertexColor[n];"
				        "			gl_Position = vertex;"
				        "			EmitVertex();"
				        "		}"
				        "		EndPrimitive();"
				        "		return;"
				        "	}"
				        ""
				        "}";

		const char* Shaders::default_widget_fragment_shader =
		        "#version 330\n"
				        ""
				        "in vec4 PreFragColor;"
				        "uniform bool u_AA = false;"
				        "uniform int u_gamma = 0;"
				        "out vec4 FragmentColor;"
				        ""
				        "void main(void) {"
				        "	vec4 color_calib = vec4(0.0);"
				        "	if(u_AA) {"
				        "		color_calib = vec4(vec3(clamp(u_gamma/255.0/8.0, -1.0, 1.0)), 0.0);"
				        "	} else {"
				        "		color_calib = vec4(vec3(clamp(u_gamma/255.0, -1.0, 1.0)), 0.0);"
				        "	}"
				        "	FragmentColor = PreFragColor + color_calib;"
				        "}";

		const char* Shaders::default_context_vertex_shader = "#version 330\n"
				"layout(location = 0) in vec2 a_coord;"
				"layout(location = 1) in vec2 UVCoord;"
				"uniform mat4 MVP;"
				"out vec2 f_texcoord;"
				""
				"void main(void) {"
				"	gl_Position = MVP * vec4(a_coord, 0.0, 1.0);"
				"	f_texcoord = UVCoord;"
				"}";

		const char* Shaders::default_context_fragment_shader = "#version 330\n"
				"in vec2 f_texcoord;"
				"uniform sampler2D TexID;"
				//"uniform bool Blur = false;"
				"out vec4 FragmentColor;"
				//"const float gaussian_kernel[9] = float[9]("
				//"	1., 2., 1.,"
				//"	2., 4., 2.,"
				//"	1., 2., 1."
				//");"
				//"const float radius = 1.0;"
				""
				"void main(void) {"
				""
				/*
				 "	if(Blur) {"
				 ""
				 "		vec4 color;"
				 "		float grey;"
				 "		color = vec4(grey, grey, grey, 0.25f);"
				 ""
				 "		ivec2 tex_size = textureSize(TexID, 0);"
				 "		float s = float(tex_size.s);"
				 "		float t = float(tex_size.t);"
				 "		vec4 sum = vec4(0.0);"
				 "    	vec2 offset[9] = vec2[9]("
				 "			vec2(-1. / s, 1. / t),	vec2(0., 1. / t),	vec2 (1. / s, 1. / t),"
				 "			vec2(-1. / s, 0.), 		vec2(0., 0.), 		vec2 (1. / s, 0.),"
				 "			vec2(-1. / s, -1. / t), vec2(0., -1. / t),	vec2 (1. / s, -1. / t)"
				 "		);"
				 "		vec4 sample[9];"
				 ""
				 "		for(int i = 0; i < 9; i++)"
				 "		{"
				 "			color = texture(TexID, f_texcoord + offset[i]);"
				 "			grey = dot(color.rgb, vec3(0.25, 0.25, 0.25));"
				 "			sample[i] = vec4(grey, grey, grey, color.a * 0.25);"
				 "			sum += sample[i] * gaussian_kernel[i];"
				 "		}"
				 ""
				 "		sum /= 16.0;"
				 "		FragmentColor = sum;"
				 //"FragmentColor = color;"
				 ""
				 "	} else {"
				 */
				"		FragmentColor = texture(TexID, f_texcoord);"
				//"	}"
				"}";

		const char* Shaders::default_image_vertex_shader =
				"#version 330\n"
				"layout(location = 0) in vec2 a_coord;"
				"layout(location = 1) in vec2 a_uv;"
				"out vec2 f_texcoord;"
				""
				"uniform mat4 u_projection;"	// projection matrix
        		"uniform mat4 u_view;"// view matrix
				"uniform vec3 u_position;"// position
        		"uniform float u_rotation = 0.f;"// the rotation in degree, only support rotation along Z axis
				""
		        "mat4 TranslateMatrix (const in vec3 t)"
		        "{"
		        "	return mat4(1.0, 0.0, 0.0, 0.0,"
		        "				0.0, 1.0, 0.0, 0.0,"
		        "				0.0, 0.0, 1.0, 0.0,"
		        "				t.x, t.y, t.z, 1.0);"
		        "}"
		        ""
		        "mat4 RotateMatrix( const in float angle,"
		        "					const in vec3 axis )"
		        "{"
		        "	vec3 n = normalize( axis );"
		        "	float theta = radians( angle );"
		        "	float c = cos( theta );"
		        "	float s = sin( theta );"
		        "	mat3 R;"
		        "	R[0] = n.xyz*n.x*(1.0-c) + vec3(      c,  n.z*s, -n.y*s );"
		        "	R[1] = n.xyz*n.y*(1.0-c) + vec3( -n.z*s,      c,  n.x*s );"
		        "	R[2] = n.xyz*n.z*(1.0-c) + vec3(  n.y*s, -n.x*s,      c );"
		        "	return mat4( R[0],	0.0,"
		        "				 R[1],	0.0,"
		        "				 R[2],	0.0,"
		        "				 0.0, 0.0, 0.0, 1.0 );"
		        "}"
		        ""
		        "mat4 RotateMatrixAlongZ (const in float angle)"
		        "{"
		        "	return RotateMatrix(angle, vec3(0.0, 0.0, 1.0));"
		        "}"
		        ""
				"void main(void) {"
				"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation);"
				"	gl_Position = mvp * vec4(a_coord, 0.0, 1.0);"
				"	f_texcoord = a_uv;"
				"}";

		const char* Shaders::default_image_fragment_shader =
		        "#version 330\n"
				"in vec2 f_texcoord;"
				"uniform sampler2D TexID;"
				"uniform int u_gamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(0.0);"
				"	color_calib = vec4(vec3(clamp(u_gamma/255.0, -1.0, 1.0)), 0.0);"
				"	vec4 color = texture(TexID, f_texcoord);"
				"	FragmentColor = color + color_calib;"
				"}";

#else	// Legacy opengl

		const char* Shaders::text_vertex_shader =
		"#version 120\n"
		"attribute vec4 coord;"
		"uniform mat4 MVP;"
		"varying vec2 texpos;"
		""
		"void main(void) {"
		"  gl_Position = MVP * vec4(coord.xy, 0.0, 1.0);"
		"  texpos = coord.zw;"
		"}";

		const char* Shaders::text_fragment_shader =
		"#version 120\n"
		"varying vec2 texpos;"
		"uniform sampler2D tex;"
		"uniform vec4 color;"
		""
		"void main(void) {"
		"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;"
		"}";

		const char* Shaders::primitive_vertex_shader =
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

		const char* Shaders::primitive_fragment_shader =
		"#version 120\n"
		""
		"varying vec3 f_color;"
		""
		"void main(void) {"
		"	gl_FragColor = vec4(f_color, 1.0);"
		"}";

		const char* Shaders::default_widget_vertex_shader =
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

		const char* Shaders::default_widget_fragment_shader =
		"#version 120\n"
		""
		"varying vec4 f_color;"
		""
		"void main(void) {"
		"	gl_FragColor = f_color;"
		"}";

		const char* Shaders::default_form_vertex_shader =
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

		const char* Shaders::default_form_fragment_shader =
		"#version 120\n"
		""
		"varying vec4 f_color;"
		""
		"void main(void) {"
		"	gl_FragColor = f_color;"
		"}";

#endif

		Shaders* Shaders::instance = 0;

		bool Shaders::Initialize ()
		{
			bool ret = false;

			if (!instance) {
				instance = new Shaders;

				if (instance) {
					ret = instance->Setup();
				} else {
					ret = false;
				}
			}

			return ret;
		}

		void Shaders::Release ()
		{
			if (instance) {
				delete instance;
				instance = 0;
			}
		}

		Shaders::Shaders ()
		{
			m_default_text_program.reset(new GLSLProgram);

			m_primitive_program.reset(new GLSLProgram);

			m_default_triangle_program.reset(new GLSLProgram);

			m_default_line_program.reset(new GLSLProgram);

			m_default_context_program.reset(new GLSLProgram);

			m_default_image_program.reset(new GLSLProgram);
		}

		Shaders::~Shaders ()
		{
		}

		bool Shaders::Setup ()
		{
			if (!m_default_text_program->Create())
				return false;

			if (!m_primitive_program->Create()) {
				return false;
			}

			if (!m_default_triangle_program->Create()) {
				return false;
			}

			if (!m_default_line_program->Create()) {
				return false;
			}

			if (!m_default_context_program->Create()) {
				return false;
			}

			if (!m_default_image_program->Create()) {
				return false;
			}

			m_default_text_program->AttachShader(text_vertex_shader, GL_VERTEX_SHADER);
			m_default_text_program->AttachShader(text_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!m_default_text_program->Link()) {
				DBG_PRINT_MSG("Fail to link the text program: %d",
				        m_default_text_program->id());
				return false;
			}

			m_primitive_program->AttachShader(primitive_vertex_shader,
			        GL_VERTEX_SHADER);
			m_primitive_program->AttachShader(primitive_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!m_primitive_program->Link()) {
				DBG_PRINT_MSG("Fail to link the primitive program: %d",
				        m_primitive_program->id());
				return false;
			}

			m_default_triangle_program->AttachShader(
			        default_widget_vertex_shader, GL_VERTEX_SHADER);
			m_default_triangle_program->AttachShader(
			        default_widget_triangle_geometry_shader,
			        GL_GEOMETRY_SHADER);
			m_default_triangle_program->AttachShader(
			        default_widget_fragment_shader, GL_FRAGMENT_SHADER);
			if (!m_default_triangle_program->Link()) {
				DBG_PRINT_MSG("Fail to link the widget program: %d",
				        m_default_triangle_program->id());
				return false;
			}

			m_default_line_program->AttachShader(default_widget_vertex_shader,
			        GL_VERTEX_SHADER);
			m_default_line_program->AttachShader(
			        default_widget_line_geometry_shader, GL_GEOMETRY_SHADER);
			m_default_line_program->AttachShader(default_widget_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!m_default_line_program->Link()) {
				DBG_PRINT_MSG("Fail to link the widget program: %d",
				        m_default_line_program->id());
				return false;
			}

			m_default_context_program->AttachShader(
			        default_context_vertex_shader, GL_VERTEX_SHADER);
			m_default_context_program->AttachShader(
			        default_context_fragment_shader, GL_FRAGMENT_SHADER);
			if (!m_default_context_program->Link()) {
				DBG_PRINT_MSG("Fail to link the context program: %d",
				        m_default_context_program->id());
				return false;
			}

			m_default_image_program->AttachShader(default_image_vertex_shader,
			        GL_VERTEX_SHADER);
			m_default_image_program->AttachShader(default_image_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!m_default_image_program->Link()) {
				DBG_PRINT_MSG("Fail to link the pixelicon program: %d",
				        m_default_image_program->id());
				return false;
			}

			return true;
		}

	}

}

