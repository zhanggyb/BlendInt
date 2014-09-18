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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

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
			//"uniform mat4 u_projection;"	// projection matrix
			//"uniform mat4 u_view;"// view matrix
			"uniform UIMatrix {"
			"	mat4 projection;"
			"	mat4 view;"
			"	mat4 model;"
			"};"
			""
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
			//"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation);"
			"	mat4 mvp = projection * view * model * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation);"
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
			"layout(location = 0) in vec3 coord;"
			"layout(location = 1) in vec4 color;"
			"uniform mat4 P;"
			"uniform mat4 V;"
			"uniform mat4 M;"
			""
			"out vec4 vColor;"
			""
			"void main(void) {"
			"	gl_Position = P * V * M * vec4(coord, 1.0);"
			"	vColor = color;"
			"}";

		const char* Shaders::primitive_fragment_shader =
			"#version 330\n"
			""
			"in vec4 vColor;"
			"out vec4 vFragColor;"
			""
			"void main(void) {"
			"	vFragColor = vColor;"
			"}";

		const char* Shaders::triangle_vertex_shader =
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

		const char* Shaders::triangle_geometry_shader =
		        "#version 330\n"
			""
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 24) out;"
			"in vec4 VertexColor[];"
			"out vec4 PreFragColor;"
			//"uniform mat4 u_projection;"	// projection matrix
		       	//"uniform mat4 u_view;"// view matrix
			""
			"uniform UIMatrix {"
			"	mat4 projection;"
			"	mat4 view;"
			"	mat4 model;"
			"};"
			""
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
						//"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation) * ScaleMatrix(vec3(u_scale.xy, 1.f));"
					"	mat4 mvp = projection * view *  model * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation) * ScaleMatrix(vec3(u_scale.xy, 1.f));"
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

		const char* Shaders::triangle_fragment_shader =
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

		// ---------------------------------------------------------------

		const char* Shaders::widget_vertex_shader_ext =
				"#version 330\n"
				""
				"layout(location=0) in vec3 a_coord;"
				"out float VertexShade;"
				""
				"void main(void) {"
				"	gl_Position = vec4(a_coord.xy, 0.0, 1.0);"
				"	VertexShade = a_coord.z;"
				"}";

		const char* Shaders::widget_geometry_shader_ext =
		        "#version 330\n"
				""
				"layout (triangles) in;"
				"layout (triangle_strip, max_vertices = 24) out;"
				"in float VertexShade[];"
				""
				//"uniform mat4 u_projection;"	// projection matrix
				//"uniform mat4 u_view;"			// view matrix
				//""
				"uniform UIMatrix {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec3 u_position;"// position
				"uniform float u_rotation = 0.f;"// the rotation in degree, only support rotation along Z axis
				"uniform vec2 u_scale = vec2(1.f, 1.f);"// the scale factor, only support xy plane
				"uniform bool u_AA = false;"
				"out float PreFragShade;"
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
				//"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation) * ScaleMatrix(vec3(u_scale.xy, 1.f));"
				"	mat4 mvp = projection * view * model * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation) * ScaleMatrix(vec3(u_scale.xy, 1.f));"
				"	vec4 vertex;"
				""
				"	if(u_AA) {"
				"		mat4 aa_matrix = mat4(1.0);"
				"		for(int jit = 0; jit < 8; jit++) {"
				"			aa_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
				"			for(int n = 0; n < gl_in.length(); n++)"
				"			{"
				"				vertex = mvp * aa_matrix * gl_in[n].gl_Position;"
				"				PreFragShade = VertexShade[n];"
				"				gl_Position = vertex;"
				"				EmitVertex();"
				"			}"
				"			EndPrimitive();"
				"		}"
				"		return;"
				"	} else {"
				"		for(int n = 0; n < gl_in.length(); n++) {"
				"			vertex = mvp * gl_in[n].gl_Position;"
				"			PreFragShade = VertexShade[n];"
				"			gl_Position = vertex;"
				"			EmitVertex();"
				"		}"
				"		EndPrimitive();"
				"		return;"
				"	}"
				""
				"}";

		const char* Shaders::widget_fragment_shader_ext =
		        "#version 330\n"
				""
				"in float PreFragShade;"
				"uniform vec4 u_color;"
				"uniform bool u_AA = false;"
				"uniform int u_gamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color = u_color;"
				"	vec4 color_calib = vec4(0.0);"
				"	if(u_AA) {"
				"		color.a = color.a / 8.f;"
				"		color_calib = vec4(vec3(clamp(u_gamma/255.0/8.0, -1.0, 1.0)), 0.0);"
				"	} else {"
				"		color_calib = vec4(vec3(clamp(u_gamma/255.0, -1.0, 1.0)), 0.0);"
				"	}"
				"	FragmentColor = vec4(PreFragShade, PreFragShade, PreFragShade, 0.f) + color_calib + color;"
				"}";

		// ---------------------------------------------------------------

		const char* Shaders::context_vertex_shader = "#version 330\n"
				"layout(location = 0) in vec2 a_coord;"
				"layout(location = 1) in vec2 UVCoord;"
				"uniform mat4 MVP;"
				"out vec2 f_texcoord;"
				""
				"void main(void) {"
				"	gl_Position = MVP * vec4(a_coord, 0.0, 1.0);"
				"	f_texcoord = UVCoord;"
				"}";

		const char* Shaders::context_fragment_shader = "#version 330\n"
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

		const char* Shaders::image_vertex_shader =
				"#version 330\n"
				"layout(location = 0) in vec2 a_coord;"
				"layout(location = 1) in vec2 a_uv;"
				"out vec2 f_texcoord;"
				""
				//"uniform mat4 u_projection;"	// projection matrix
        		//"uniform mat4 u_view;"// view matrix
				""
				"uniform UIMatrix {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
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
				//"	mat4 mvp = u_projection * u_view * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation);"
				"	mat4 mvp = projection * view * model * TranslateMatrix(u_position) * RotateMatrixAlongZ(u_rotation);"
				"	gl_Position = mvp * vec4(a_coord, 0.0, 1.0);"
				"	f_texcoord = a_uv;"
				"}";

		const char* Shaders::image_fragment_shader =
		        "#version 330\n"
				"in vec2 f_texcoord;"
				"uniform sampler2D TexID;"
				"uniform int u_gamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(0.f);"
				"	color_calib = vec4(vec3(clamp(u_gamma/255.f, -1.f, 1.f)), 0.f);"
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

		const char* Shaders::triangle_vertex_shader =
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

		const char* Shaders::triangle_fragment_shader =
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
		: ui_matrix_block_size_(0),
			ui_matrix_binding_point_(1)
		{
			text_program_.reset(new GLSLProgram);
			primitive_program_.reset(new GLSLProgram);
			triangle_program_.reset(new GLSLProgram);
			widget_program_.reset(new GLSLProgram);
			image_program_.reset(new GLSLProgram);
		}

		Shaders::~Shaders ()
		{
		}

		void Shaders::GetUIProjectionMatrix(glm::mat4& matrix)
		{
			ui_matrix_->bind();
			float* buf_p = (float*)ui_matrix_->map(GL_READ_ONLY);

			memcpy(glm::value_ptr(matrix), buf_p + ui_matrix_offset_[0], ui_matrix_offset_[1] - ui_matrix_offset_[0]);

			ui_matrix_->unmap();
			ui_matrix_->reset();
		}

		void Shaders::GetUIViewMatrix(glm::mat4& matrix)
		{
			ui_matrix_->bind();
			float* buf_p = (float*)ui_matrix_->map(GL_READ_ONLY);

			memcpy(glm::value_ptr(matrix), buf_p + ui_matrix_offset_[2], ui_matrix_offset_[2] - ui_matrix_offset_[1]);

			ui_matrix_->unmap();
			ui_matrix_->reset();
		}

		void Shaders::GetUIModelMatrix(glm::mat4& matrix)
		{
			ui_matrix_->bind();
			float* buf_p = (float*)ui_matrix_->map(GL_READ_ONLY);

			memcpy(glm::value_ptr(matrix), buf_p + ui_matrix_offset_[2], ui_matrix_block_size_ - ui_matrix_offset_[2]);

			ui_matrix_->unmap();
			ui_matrix_->reset();
		}

		void Shaders::SetUIProjectionMatrix(const glm::mat4& matrix)
		{
			ui_matrix_->bind();
			ui_matrix_->set_sub_data(ui_matrix_offset_[0], ui_matrix_offset_[1] - ui_matrix_offset_[0], glm::value_ptr(matrix));
			ui_matrix_->reset();
		}

		void Shaders::SetUIViewMatrix(const glm::mat4& matrix)
		{
			ui_matrix_->bind();
			ui_matrix_->set_sub_data(ui_matrix_offset_[1], ui_matrix_offset_[2]- ui_matrix_offset_[1], glm::value_ptr(matrix));
			ui_matrix_->reset();
		}

		void Shaders::SetUIModelMatrix (const glm::mat4& matrix)
		{
			ui_matrix_->bind();
			ui_matrix_->set_sub_data(ui_matrix_offset_[2], ui_matrix_block_size_- ui_matrix_offset_[2], glm::value_ptr(matrix));
			ui_matrix_->reset();
		}

		bool Shaders::Setup ()
		{
			if(!SetupWidgetProgram())
				return false;

			if(!SetupTextProgram())
				return false;

			if(!SetupTriangleProgram())
				return false;

			if(!SetupImageProgram())
				return false;

			if(!SetupPrimitiveProgram())
				return false;

			// setup uniform block

			const GLchar* names[] = {
				"projection",
				"view",
				"model"
			};

			GLuint block_index = glGetUniformBlockIndex(widget_program_->id(), "UIMatrix");

			glGetActiveUniformBlockiv(widget_program_->id(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &ui_matrix_block_size_);

			GLubyte* buf_p = (GLubyte*)malloc(ui_matrix_block_size_);

			GLuint indices[3];
			glGetUniformIndices(widget_program_->id(), 3, names, indices);
			glGetActiveUniformsiv(widget_program_->id(), 3, indices, GL_UNIFORM_OFFSET, ui_matrix_offset_);

			// set default matrix
			glm::mat4 projection = glm::ortho(0.f, 800.f, 0.f, 600.f, 100.f, -100.f);
			glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 model(1.f);

			if(ui_matrix_block_size_ > 0) {
				memcpy(buf_p + ui_matrix_offset_[0], glm::value_ptr(projection), sizeof(glm::mat4));
				memcpy(buf_p + ui_matrix_offset_[1], glm::value_ptr(view), sizeof(glm::mat4));
				memcpy(buf_p + ui_matrix_offset_[2], glm::value_ptr(model), sizeof(glm::mat4));
			}

			ui_matrix_.reset(new GLBuffer<UNIFORM_BUFFER>);
			ui_matrix_->generate();
			ui_matrix_->bind();
			ui_matrix_->set_data(ui_matrix_block_size_, glm::value_ptr(projection), GL_DYNAMIC_DRAW);
			ui_matrix_->reset();

			glBindBufferBase(GL_UNIFORM_BUFFER, ui_matrix_binding_point_, ui_matrix_->id());
			glUniformBlockBinding(widget_program_->id(), block_index, ui_matrix_binding_point_);

			free(buf_p);
			buf_p = 0;

			// set uniform block in text program

			//GLint block_size = 0;
			//GLint offset[2];

			block_index = glGetUniformBlockIndex(text_program_->id(), "UIMatrix");
			//glGetActiveUniformBlockiv(text_program_->id(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
			//glGetUniformIndices(text_program_->id(), 2, names, indices);
			//glGetActiveUniformsiv(text_program_->id(), 2, indices, GL_UNIFORM_OFFSET, offset);
			glBindBufferBase(GL_UNIFORM_BUFFER, ui_matrix_binding_point_, ui_matrix_->id());
			glUniformBlockBinding(text_program_->id(), block_index, ui_matrix_binding_point_);

			// set uniform block in triangle program

			block_index = glGetUniformBlockIndex(triangle_program_->id(), "UIMatrix");
			//glGetActiveUniformBlockiv(triangle_program_->id(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
			//glGetUniformIndices(triangle_program_->id(), 2, names, indices);
			//glGetActiveUniformsiv(triangle_program_->id(), 2, indices, GL_UNIFORM_OFFSET, offset);
			glBindBufferBase(GL_UNIFORM_BUFFER, ui_matrix_binding_point_, ui_matrix_->id());
			glUniformBlockBinding(triangle_program_->id(), block_index, ui_matrix_binding_point_);

			// set uniform block in image program

			block_index = glGetUniformBlockIndex(image_program_->id(), "UIMatrix");
			//glGetActiveUniformBlockiv(image_program_->id(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
			//glGetUniformIndices(image_program_->id(), 2, names, indices);
			//glGetActiveUniformsiv(image_program_->id(), 2, indices, GL_UNIFORM_OFFSET, offset);
			glBindBufferBase(GL_UNIFORM_BUFFER, ui_matrix_binding_point_, ui_matrix_->id());
			glUniformBlockBinding(image_program_->id(), block_index, ui_matrix_binding_point_);

			return true;
		}

		bool Shaders::SetupTextProgram()
		{
			if (!text_program_->Create())
				return false;

			text_program_->AttachShader(text_vertex_shader, GL_VERTEX_SHADER);
			text_program_->AttachShader(text_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!text_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the text program: %d",
				        text_program_->id());
				return false;
			}

			locations_[TEXT_COORD] = text_program_->GetAttributeLocation("a_coord");
			//locations_[TEXT_PROJECTION] = text_program_->GetUniformLocation("u_projection");
			//locations_[TEXT_VIEW] = text_program_->GetUniformLocation("u_view");
			locations_[TEXT_POSITION] = text_program_->GetUniformLocation("u_position");
			locations_[TEXT_ROTATION] = text_program_->GetUniformLocation("u_rotation");
			locations_[TEXT_TEXTURE] = text_program_->GetUniformLocation("u_tex");
			locations_[TEXT_COLOR] = text_program_->GetUniformLocation("u_color");

			return true;
		}

		bool Shaders::SetupTriangleProgram()
		{
			if (!triangle_program_->Create()) {
				return false;
			}

			triangle_program_->AttachShader(
			        triangle_vertex_shader, GL_VERTEX_SHADER);
			triangle_program_->AttachShader(
			        triangle_geometry_shader,
			        GL_GEOMETRY_SHADER);
			triangle_program_->AttachShader(
			        triangle_fragment_shader, GL_FRAGMENT_SHADER);
			if (!triangle_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget program: %d",
				        triangle_program_->id());
				return false;
			}

			locations_[TRIANGLE_COORD] = triangle_program_->GetAttributeLocation("a_coord");
			locations_[TRIANGLE_COLOR] = triangle_program_->GetAttributeLocation("a_color");
			//locations_[TRIANGLE_PROJECTION] = triangle_program_->GetUniformLocation("u_projection");
			//locations_[TRIANGLE_VIEW] = triangle_program_->GetUniformLocation("u_view");
			locations_[TRIANGLE_POSITION] = triangle_program_->GetUniformLocation("u_position");
			locations_[TRIANGLE_ROTATION] = triangle_program_->GetUniformLocation("u_rotation");
			locations_[TRIANGLE_SCALE] = triangle_program_->GetUniformLocation("u_scale");
			locations_[TRIANGLE_ANTI_ALIAS] = triangle_program_->GetUniformLocation("u_AA");
			locations_[TRIANGLE_GAMMA] = triangle_program_->GetUniformLocation("u_gamma");

			return true;
		}

		bool Shaders::SetupWidgetProgram()
		{
			if (!widget_program_->Create()) {
				return false;
			}

			widget_program_->AttachShader(
			        widget_vertex_shader_ext, GL_VERTEX_SHADER);
			widget_program_->AttachShader(
			        widget_geometry_shader_ext,
			        GL_GEOMETRY_SHADER);
			widget_program_->AttachShader(
			        widget_fragment_shader_ext, GL_FRAGMENT_SHADER);
			if (!widget_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget program: %d",
				        widget_program_->id());
				return false;
			}

			locations_[WIDGET_COORD] = widget_program_->GetAttributeLocation("a_coord");
			locations_[WIDGET_COLOR] = widget_program_->GetUniformLocation("u_color");
			//locations_[WIDGET_PROJECTION] = widget_program_->GetUniformLocation("u_projection");
			//locations_[WIDGET_VIEW] = widget_program_->GetUniformLocation("u_view");
			locations_[WIDGET_POSITION] = widget_program_->GetUniformLocation("u_position");
			locations_[WIDGET_ROTATION] = widget_program_->GetUniformLocation("u_rotation");
			locations_[WIDGET_SCALE] = widget_program_->GetUniformLocation("u_scale");
			locations_[WIDGET_ANTI_ALIAS] = widget_program_->GetUniformLocation("u_AA");
			locations_[WIDGET_GAMMA] = widget_program_->GetUniformLocation("u_gamma");

			return true;
		}

		bool Shaders::SetupImageProgram()
		{
			if (!image_program_->Create()) {
				return false;
			}

			image_program_->AttachShader(image_vertex_shader,
			        GL_VERTEX_SHADER);
			image_program_->AttachShader(image_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!image_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the pixelicon program: %d",
				        image_program_->id());
				return false;
			}

			locations_[IMAGE_COORD] = image_program_->GetAttributeLocation("a_coord");
			locations_[IMAGE_UV] = image_program_->GetAttributeLocation("a_uv");
			//locations_[IMAGE_PROJECTION] = image_program_->GetUniformLocation("u_projection");
			//locations_[IMAGE_VIEW] = image_program_->GetUniformLocation("u_view");
			locations_[IMAGE_POSITION] = image_program_->GetUniformLocation("u_position");
			locations_[IMAGE_ROTATION] = image_program_->GetUniformLocation("u_rotation");
			locations_[IMAGE_TEXTURE] = image_program_->GetUniformLocation("TexID");
			locations_[IMAGE_GAMMA] = image_program_->GetUniformLocation("u_gamma");

			return true;
		}

		bool Shaders::SetupPrimitiveProgram()
		{
			if (!primitive_program_->Create()) {
				return false;
			}

			primitive_program_->AttachShader(primitive_vertex_shader,
			        GL_VERTEX_SHADER);
			primitive_program_->AttachShader(primitive_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!primitive_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the primitive program: %d",
				        primitive_program_->id());
				return false;
			}

			locations_[PRIMITIVE_COORD] = primitive_program_->GetAttributeLocation("coord");
			locations_[PRIMITIVE_COLOR] = primitive_program_->GetAttributeLocation("color");
			locations_[PRIMITIVE_PROJECTION] = primitive_program_->GetUniformLocation("P");
			locations_[PRIMITIVE_VIEW] = primitive_program_->GetUniformLocation("V");
			locations_[PRIMITIVE_MODEL] = primitive_program_->GetUniformLocation("M");

			return true;
		}

	}

}
