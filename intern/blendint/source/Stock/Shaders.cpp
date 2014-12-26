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

		const char* Shaders::widget_text_vertex_shader =
			"#version 330\n"
			"layout(location = 0) in vec4 aCoord;"
			"out vec2 uv;"
			""
			//"uniform mat4 u_projection;"	// projection matrix
			//"uniform mat4 u_view;"// view matrix
			"uniform WidgetMatrices {"
			"	mat4 projection;"
			"	mat4 view;"
			"	mat4 model;"
			"};"
			""
			"uniform vec2 uPosition;"// position
      			"uniform float uRotation = 0.f;"// the rotation in degree, only support rotation along Z axis
			""
			"mat4 TranslateMatrix (const in vec2 t)"
			"{"
			"	return mat4(1.0, 0.0, 0.0, 0.0,"
			"				0.0, 1.0, 0.0, 0.0,"
			"				0.0, 0.0, 1.0, 0.0,"
			"				t.x, t.y, 0.0, 1.0);"
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
			//"	mat4 mvp = u_projection * u_view * TranslateMatrix(uPosition) * RotateMatrixAlongZ(uRotation);"
			"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition) * RotateMatrixAlongZ(uRotation);"
			"	gl_Position = mvp * vec4(aCoord.xy, 0.0, 1.0);"
			"	uv = aCoord.zw;"
			"}";

		const char* Shaders::widget_text_fragment_shader =
			"#version 330\n"
			"in vec2 uv;"
			"uniform sampler2D u_tex;"
			"uniform vec4 uColor;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	float alpha = texture(u_tex, uv).r;"// GL 3.2 only support GL_R8 in glTexImage2D internalFormat
			"	FragmentColor = vec4(uColor.rgb, uColor.a * alpha);"
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

		const char* Shaders::widget_triangle_vertex_shader =
			"#version 330\n"
			""
			"layout(location=0) in vec2 aCoord;"
			"layout(location=1) in vec4 aColor;"
			"out vec4 VertexColor;"
			""
			"void main(void) {"
			"	gl_Position = vec4(aCoord, 0.0, 1.0);"
			"	VertexColor = aColor;"
			"}";

		const char* Shaders::widget_triangle_geometry_shader =
				"#version 330\n"
				""
				"layout (triangles) in;"
				"layout (triangle_strip, max_vertices = 24) out;"
				"in vec4 VertexColor[];"
				"out vec4 PreFragColor;"
				//"uniform mat4 u_projection;"	// projection matrix
				//"uniform mat4 u_view;"// view matrix
				""
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"// position
				"uniform float uRotation = 0.f;"// the rotation in degree, only support rotation along Z axis
				"uniform vec2 uScale = vec2(1.f, 1.f);"// the scale factor, only support xy plane
				"uniform bool uAA = false;"
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
				"mat4 ScaleMatrix (const in vec2 s)"
				"{"
				"	return mat4(s.x, 0.0, 0.0, 0.0,"
				"				0.0, s.y, 0.0, 0.0,"
				"				0.0, 0.0, 0.0, 0.0,"
				"				0.0, 0.0, 0.0, 1.0);"
				"}"
				""
				"mat4 TranslateMatrix (const in vec2 t)"
				"{"
				"	return mat4(1.0, 0.0, 0.0, 0.0,"
				"				0.0, 1.0, 0.0, 0.0,"
				"				0.0, 0.0, 1.0, 0.0,"
				"				t.x, t.y, 0.0, 1.0);"
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
				//"	mat4 mvp = u_projection * u_view * TranslateMatrix(uPosition) * RotateMatrixAlongZ(uRotation) * ScaleMatrix(vec3(uScale.xy, 1.f));"
				"	mat4 mvp = projection * view *  model * TranslateMatrix(uPosition) * RotateMatrixAlongZ(uRotation) * ScaleMatrix(uScale);"
				"	vec4 vertex;"
				""
				"	if(uAA) {"
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

		const char* Shaders::widget_triangle_fragment_shader =
				"#version 330\n"
				""
				"in vec4 PreFragColor;"
				"uniform bool uAA = false;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(0.0);"
				"	if(uAA) {"
				"		color_calib = vec4(vec3(clamp(uGamma/255.0/8.0, -1.0, 1.0)), 0.0);"
				"	} else {"
				"		color_calib = vec4(vec3(clamp(uGamma/255.0, -1.0, 1.0)), 0.0);"
				"	}"
				"	FragmentColor = PreFragColor + color_calib;"
				"}";

		// ---------------------------------------------------------------

		const char* Shaders::widget_simple_triangle_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec3 aCoord;"
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"// position
				""
				"out float VertexShade;"
				""
				"mat4 TranslateMatrix (const in vec2 t)"
				"{"
				"	return mat4(1.0, 0.0, 0.0, 0.0,"
				"				0.0, 1.0, 0.0, 0.0,"
				"				0.0, 0.0, 1.0, 0.0,"
				"				t.x, t.y, 0.0, 1.0);"
				"}"
				""
				"void main(void) {"
				"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition);"
				"	gl_Position = mvp * vec4(aCoord.xy, 0.0, 1.0);"
				"	VertexShade = aCoord.z;"
				"}";

		const char* Shaders::widget_simple_triangle_fragment_shader =
		        "#version 330\n"
				""
				"in float VertexShade;"
				"uniform vec4 uColor;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(vec3(clamp(uGamma/255.0, -1.0, 1.0)), 0.0);"
				"	FragmentColor = vec4(VertexShade, VertexShade, VertexShade, 0.f) + color_calib + uColor;"
				"}";

		// ---------------------------------------------------------------

		const char* Shaders::widget_inner_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec3 aCoord;"
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"out float VertexShade;"
				""
				"void main(void) {"
				"	mat4 mvp = projection * view * model;"
				"	gl_Position = mvp * vec4(aCoord.xy, 0.0, 1.0);"
				"	VertexShade = aCoord.z;"
				"}";

		const char* Shaders::widget_inner_fragment_shader =
		        "#version 330\n"
				""
				"in float VertexShade;"
				"uniform vec4 uColor;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(vec3(clamp(uGamma/255.0, -1.0, 1.0)), 0.0);"
				"	FragmentColor = vec4(VertexShade, VertexShade, VertexShade, 0.f) + color_calib + uColor;"
				"}";

		const char* Shaders::widget_split_inner_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec3 aCoord;"
				""
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"out float VertexShade;"
				""
				"void main(void) {"
				"	mat4 mvp = projection * view * model;"
				"	gl_Position = mvp * vec4(aCoord.xy, 0.0, 1.0);"
				"	VertexShade = aCoord.z;"
				"}";

		const char* Shaders::widget_split_inner_fragment_shader =
		        "#version 330\n"
				""
				"in float VertexShade;"
				"uniform vec4 u_color0;"
				"uniform vec4 u_color1;"
				"uniform int uGamma = 0;"
				"uniform float u_parting = 10.f;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(vec3(clamp(uGamma/255.0, -1.0, 1.0)), 0.0);"
				""
				"	if(gl_FragCoord.x <= u_parting) {"
				"		FragmentColor = vec4(VertexShade, VertexShade, VertexShade, 0.f) + color_calib + u_color0;"
				"	} else {"
				"		FragmentColor = vec4(VertexShade, VertexShade, VertexShade, 0.f) + color_calib + u_color1;"
				"	}"
				"}";

		// ---------------------------------------------------------------

		const char* Shaders::widget_outer_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec2 aCoord;"
				""
				"void main(void) {"
				"	gl_Position = vec4(aCoord.xy, 0.0, 1.0);"
				"}";

		const char* Shaders::widget_outer_geometry_shader =
		        "#version 330\n"
				""
				"layout (triangles) in;"
				"layout (triangle_strip, max_vertices = 24) out;"
				""
				//"uniform mat4 u_projection;"	// projection matrix
				//"uniform mat4 u_view;"			// view matrix
				//""
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"// position
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
				"mat4 TranslateMatrix (const in vec2 t)"
				"{"
				"	return mat4(1.0, 0.0, 0.0, 0.0,"
				"				0.0, 1.0, 0.0, 0.0,"
				"				0.0, 0.0, 1.0, 0.0,"
				"				t.x, t.y, 0.0, 1.0);"
				"}"
				""
				"void main()"
				"{"
				"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition);"
				"	vec4 vertex;"
				""
				"	mat4 aa_matrix = mat4(1.0);"
				"	for(int jit = 0; jit < 8; jit++) {"
				"		aa_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
				"		for(int n = 0; n < gl_in.length(); n++)"
				"		{"
				"			vertex = mvp * aa_matrix * gl_in[n].gl_Position;"
				"			gl_Position = vertex;"
				"			EmitVertex();"
				"		}"
				"		EndPrimitive();"
				"	}"
				"	return;"
				""
				"}";

		const char* Shaders::widget_outer_fragment_shader =
		        "#version 330\n"
				""
				"uniform vec4 uColor;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color = uColor;"
				"	color.a = color.a / 8.f;"
				""
				"	FragmentColor = color;"
				"}";

		// ---------------------------------------------------------------

		const char* Shaders::context_vertex_shader = "#version 330\n"
				"layout(location = 0) in vec2 aCoord;"
				"layout(location = 1) in vec2 UVCoord;"
				"uniform mat4 MVP;"
				"out vec2 f_texcoord;"
				""
				"void main(void) {"
				"	gl_Position = MVP * vec4(aCoord, 0.0, 1.0);"
				"	f_texcoord = UVCoord;"
				"}";

		const char* Shaders::context_fragment_shader = "#version 330\n"
				"in vec2 f_texcoord;"
				"uniform sampler2D uTexture;"
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
				 "		ivec2 tex_size = textureSize(uTexture, 0);"
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
				 "			color = texture(uTexture, f_texcoord + offset[i]);"
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
				"		FragmentColor = texture(uTexture, f_texcoord);"
				//"	}"
				"}";

		const char* Shaders::widget_image_vertex_shader =
				"#version 330\n"
				"layout(location = 0) in vec2 aCoord;"
				"layout(location = 1) in vec2 a_uv;"
				"out vec2 f_texcoord;"
				""
				//"uniform mat4 u_projection;"	// projection matrix
        		//"uniform mat4 u_view;"// view matrix
				""
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"// position
        		"uniform float uRotation = 0.f;"// the rotation in degree, only support rotation along Z axis
				""
		        "mat4 TranslateMatrix (const in vec2 t)"
		        "{"
		        "	return mat4(1.0, 0.0, 0.0, 0.0,"
		        "				0.0, 1.0, 0.0, 0.0,"
		        "				0.0, 0.0, 1.0, 0.0,"
		        "				t.x, t.y, 0.0, 1.0);"
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
				//"	mat4 mvp = u_projection * u_view * TranslateMatrix(uPosition) * RotateMatrixAlongZ(uRotation);"
				"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition) * RotateMatrixAlongZ(uRotation);"
				"	gl_Position = mvp * vec4(aCoord, 0.0, 1.0);"
				"	f_texcoord = a_uv;"
				"}";

		const char* Shaders::widget_image_fragment_shader =
		        "#version 330\n"
				"in vec2 f_texcoord;"
				"uniform sampler2D uTexture;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(0.f);"
				"	color_calib = vec4(vec3(clamp(uGamma/255.f, -1.f, 1.f)), 0.f);"
				"	vec4 color = texture(uTexture, f_texcoord);"
				"	FragmentColor = color + color_calib;"
				"}";

		// -------------------------------------------

		const char* Shaders::widget_line_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec3 aCoord;"
				"uniform WidgetMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"out float VertexShade;"
				""
				"void main(void) {"
				"	mat4 mvp = projection * view * model;"
				"	gl_Position = mvp * vec4(aCoord.xy, 0.0, 1.0);"
				"	VertexShade = aCoord.z;"
				"}";

		const char* Shaders::widget_line_fragment_shader =
		        "#version 330\n"
				""
				"in float VertexShade;"
				"uniform vec4 uColor;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(vec3(clamp(uGamma/255.0, -1.0, 1.0)), 0.0);"
				"	FragmentColor = vec4(VertexShade, VertexShade, VertexShade, 0.f) + color_calib + uColor;"
				"}";

		// ---------------------------------------------------------------

		const char* Shaders::frame_inner_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec3 aCoord;"
				"uniform FrameMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"
				"out float VertexShade;"
				""
		        "mat4 TranslateMatrix (const in vec2 t)"
		        "{"
		        "	return mat4(1.0, 0.0, 0.0, 0.0,"
		        "				0.0, 1.0, 0.0, 0.0,"
		        "				0.0, 0.0, 1.0, 0.0,"
		        "				t.x, t.y, 0.0, 1.0);"
		        "}"
		        ""
				"void main(void) {"
				"	vec4 point = model * TranslateMatrix(uPosition) * vec4(aCoord.xy, 0.f, 1.f);"
				"	gl_Position = projection * view * point;"
				"	VertexShade = aCoord.z;"
				"}";

		const char* Shaders::frame_inner_fragment_shader =
		        "#version 330\n"
				""
				"in float VertexShade;"
				"uniform vec4 uColor;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(vec3(clamp(uGamma/255.0, -1.0, 1.0)), 0.0);"
				"	FragmentColor = vec4(VertexShade, VertexShade, VertexShade, 0.f) + color_calib + uColor;"
				"}";

		// -----------------------------------

		const char* Shaders::frame_outer_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec2 aCoord;"
				""
				"void main(void) {"
				"	gl_Position = vec4(aCoord.xy, 0.0, 1.0);"
				"}";

		const char* Shaders::frame_outer_geometry_shader =
		        "#version 330\n"
				""
				"layout (triangles) in;"
				"layout (triangle_strip, max_vertices = 24) out;"
				""
				"uniform FrameMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"// position
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
				"mat4 TranslateMatrix (const in vec2 t)"
				"{"
				"	return mat4(1.0, 0.0, 0.0, 0.0,"
				"				0.0, 1.0, 0.0, 0.0,"
				"				0.0, 0.0, 1.0, 0.0,"
				"				t.x, t.y, 0.0, 1.0);"
				"}"
				""
				"void main()"
				"{"
				"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition);"
				"	vec4 vertex;"
				""
				"	mat4 aa_matrix = mat4(1.0);"
				"	for(int jit = 0; jit < 8; jit++) {"
				"		aa_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
				"		for(int n = 0; n < gl_in.length(); n++)"
				"		{"
				"			vertex = mvp * aa_matrix * gl_in[n].gl_Position;"
				"			gl_Position = vertex;"
				"			EmitVertex();"
				"		}"
				"		EndPrimitive();"
				"	}"
				"	return;"
				""
				"}";

		const char* Shaders::frame_outer_fragment_shader =
		        "#version 330\n"
				""
				"uniform vec4 uColor;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color = uColor;"
				"	color.a = color.a / 8.f;"
				""
				"	FragmentColor = color;"
				"}";

		// -----------------------------------

		const char* Shaders::frame_image_vertex_shader =
				"#version 330\n"
				"layout(location = 0) in vec2 aCoord;"
				"layout(location = 1) in vec2 aUV;"
				"out vec2 fTexcoord;"
				""
				"uniform FrameMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"// position
				""
		        "mat4 TranslateMatrix (const in vec2 t)"
		        "{"
		        "	return mat4(1.0, 0.0, 0.0, 0.0,"
		        "				0.0, 1.0, 0.0, 0.0,"
		        "				0.0, 0.0, 1.0, 0.0,"
		        "				t.x, t.y, 0.0, 1.0);"
		        "}"
		        ""
				"void main(void) {"
				"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition);"
				"	gl_Position = mvp * vec4(aCoord, 0.0, 1.0);"
				"	fTexcoord = aUV;"
				"}";

		const char* Shaders::frame_image_fragment_shader =
		        "#version 330\n"
				"in vec2 fTexcoord;"
				"uniform sampler2D uTexture;"
				"uniform int uGamma = 0;"
				"out vec4 FragmentColor;"
				""
				"void main(void) {"
				"	vec4 color_calib = vec4(0.f);"
				"	color_calib = vec4(vec3(clamp(uGamma/255.f, -1.f, 1.f)), 0.f);"
				"	vec4 color = texture(uTexture, fTexcoord);"
				"	FragmentColor = color + color_calib;"
				"}";

		// -----------------------------------

		const char* Shaders::frame_shadow_vertex_shader =
				"#version 330\n"
				""
				"layout(location=0) in vec3 aCoord;"
				"out float gAlpha;"
				""
				"void main(void) {"
				"	gl_Position = vec4(aCoord.xy, 0.0, 1.0);"
				"	gAlpha = aCoord.z;"
				"}";

		const char* Shaders::frame_shadow_geometry_shader =
		        "#version 330\n"
				""
				"layout (triangles) in;"
				"layout (triangle_strip, max_vertices = 24) out;"
				"in float gAlpha[];"
				"out float fAlpha;"
				""
				"uniform FrameMatrices {"
				"	mat4 projection;"
				"	mat4 view;"
				"	mat4 model;"
				"};"
				""
				"uniform vec2 uPosition;"
				"uniform bool uAA = false;"// position
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
				"mat4 TranslateMatrix (const in vec2 t)"
				"{"
				"	return mat4(1.0, 0.0, 0.0, 0.0,"
				"				0.0, 1.0, 0.0, 0.0,"
				"				0.0, 0.0, 1.0, 0.0,"
				"				t.x, t.y, 0.0, 1.0);"
				"}"
				""
				"void main()"
				"{"
				"	mat4 mvp = projection * view * model * TranslateMatrix(uPosition);"
				"	vec4 vertex;"
				""
				"	if(uAA) {"
				"		mat4 aa_matrix = mat4(1.0);"
				"		for(int jit = 0; jit < 8; jit++) {"
				"			aa_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
				"			for(int n = 0; n < gl_in.length(); n++)"
				"			{"
				"				vertex = mvp * aa_matrix * gl_in[n].gl_Position;"
				"				fAlpha = gAlpha[n] / 8.f;"
				"				gl_Position = vertex;"
				"				EmitVertex();"
				"			}"
				"			EndPrimitive();"
				"		}"
				"		return;"
				"	} else {"
				"		for(int n = 0; n < gl_in.length(); n++) {"
				"			vertex = mvp * gl_in[n].gl_Position;"
				"			fAlpha = gAlpha[n];"
				"			gl_Position = vertex;"
				"			EmitVertex();"
				"		}"
				"		EndPrimitive();"
				"		return;"
				"	}"
				"}";

		const char* Shaders::frame_shadow_fragment_shader =
		        "#version 330\n"
				""
				"in float fAlpha;"
				"out vec4 FragmentColor;"
				"uniform vec2 uPosition;"
				"uniform vec2 uSize;"
				""
				"void main(void) {"
				"	float alpha = fAlpha;"
				""
				"	if(gl_FragCoord.y > (uPosition.y + uSize.y / 2.f)) {"
				"		if((gl_FragCoord.x > uPosition.x) && (gl_FragCoord.x < (uPosition.x + uSize.x))) {"
				"			alpha = 0.f;"
				"		}"
				"	}"
				""
				"	FragmentColor = vec4(vec3(0.05, 0.05, 0.05), 0.8 * alpha);"
				"}";

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
		: widget_matrices_ubo_total_size_(0),
		  widget_matrices_ubo_binding_point_(0),
		  frame_matrices_ubo_total_size_(0),
		  frame_matrices_ubo_binding_point_(1)
		{
			widget_text_program_.reset(new GLSLProgram);
			primitive_program_.reset(new GLSLProgram);
			widget_triangle_program_.reset(new GLSLProgram);
			widget_simple_triangle_program_.reset(new GLSLProgram);
			widget_inner_program_.reset(new GLSLProgram);
			widget_split_inner_program_.reset(new GLSLProgram);
			widget_outer_program_.reset(new GLSLProgram);
			widget_image_program_.reset(new GLSLProgram);
			widget_line_program_.reset(new GLSLProgram);
			frame_inner_program_.reset(new GLSLProgram);
			frame_outer_program_.reset(new GLSLProgram);
			frame_image_program_.reset(new GLSLProgram);
			frame_shadow_program_.reset(new GLSLProgram);
		}

		Shaders::~Shaders ()
		{
		}

		void Shaders::GetWidgetProjectionMatrix(glm::mat4& matrix)
		{
			/*
			ui_matrix_->bind();
			float* buf_p = (float*)ui_matrix_->map(GL_READ_ONLY);

			memcpy(glm::value_ptr(matrix), buf_p + ui_matrix_offset_[0], ui_matrix_offset_[1] - ui_matrix_offset_[0]);

			ui_matrix_->unmap();
			ui_matrix_->reset();
			*/

			matrix = widget_projection_matrix_;
		}

		void Shaders::SetWidgetProjectionMatrix(const glm::mat4& matrix)
		{
			widget_matrices_ubo_->bind();
			widget_matrices_ubo_->set_sub_data(
					widget_matrices_ubo_offset_[ProjectionIndex],
					widget_matrices_ubo_size_[ProjectionIndex] *
					TypeSize(widget_matrices_ubo_type_[ProjectionIndex]),
					glm::value_ptr(matrix));
			widget_matrices_ubo_->reset();

			widget_projection_matrix_ = matrix;
		}

		void Shaders::PushWidgetProjectionMatrix()
		{
			glm::mat4 matrix;
			GetWidgetProjectionMatrix(matrix);
			widget_projection_matrix_stack.push(matrix);
		}

		void Shaders::PopWidgetProjectionMatrix()
		{
			if(widget_projection_matrix_stack.size()) {
				SetWidgetProjectionMatrix(widget_projection_matrix_stack.top());
				widget_projection_matrix_stack.pop();
			}
		}

		void Shaders::GetWidgetViewMatrix(glm::mat4& matrix)
		{
			// FIXME: the following commented lines does not work, but I don't know why
			// cause the same code in GetUIProjectionMatrix() works
			/*
			ui_matrix_->bind();
			float* buf_p = (float*)ui_matrix_->map(GL_READ_ONLY);

			memcpy(glm::value_ptr(matrix), buf_p + ui_matrix_offset_[2], ui_matrix_offset_[2] - ui_matrix_offset_[1]);

			ui_matrix_->unmap();
			ui_matrix_->reset();
			*/
			matrix = widget_view_matrix_;
		}

		void Shaders::SetWidgetViewMatrix(const glm::mat4& matrix)
		{
			widget_matrices_ubo_->bind();
			widget_matrices_ubo_->set_sub_data(
					widget_matrices_ubo_offset_[ViewIndex],
					widget_matrices_ubo_size_[ViewIndex] *
					TypeSize(widget_matrices_ubo_type_[ViewIndex]),
					glm::value_ptr(matrix));
			widget_matrices_ubo_->reset();

			widget_view_matrix_ = matrix;
		}

		void Shaders::PushWidgetViewMatrix()
		{
			glm::mat4 matrix;
			GetWidgetViewMatrix(matrix);
			widget_view_matrix_stack.push(matrix);
		}

		void Shaders::PopWidgetViewMatrix()
		{
			if(widget_view_matrix_stack.size()) {
				SetWidgetViewMatrix(widget_view_matrix_stack.top());
				widget_view_matrix_stack.pop();
			}
		}

		void Shaders::GetWidgetModelMatrix(glm::mat4& matrix)
		{
			/*
			ui_matrix_->bind();
			float* buf_p = (float*)ui_matrix_->map(GL_READ_ONLY);

			memcpy(glm::value_ptr(matrix), buf_p + ui_matrix_offset_[2], ui_matrix_block_size_ - ui_matrix_offset_[2]);

			ui_matrix_->unmap();
			ui_matrix_->reset();
			*/
			matrix = widget_model_matrix_;
		}

		void Shaders::SetWidgetModelMatrix (const glm::mat4& matrix)
		{
			widget_matrices_ubo_->bind();
			widget_matrices_ubo_->set_sub_data(widget_matrices_ubo_offset_[ModelIndex],
					widget_matrices_ubo_size_[ModelIndex] *
					TypeSize(widget_matrices_ubo_type_[ModelIndex]),
					glm::value_ptr(matrix));
			widget_matrices_ubo_->reset();
			widget_model_matrix_ = matrix;
		}

		void Shaders::PushWidgetModelMatrix()
		{
			glm::mat4 matrix;
			GetWidgetModelMatrix(matrix);
			widget_model_matrix_stack.push(matrix);
		}

		void Shaders::PopWidgetModelMatrix()
		{
			if(widget_model_matrix_stack.size()) {
				SetWidgetModelMatrix(widget_model_matrix_stack.top());
				widget_model_matrix_stack.pop();
			}
		}

		bool Shaders::Setup ()
		{
			if(!SetupWidgetInnerProgram())
				return false;

			if(!SetupWidgetSplitInnerProgram())
				return false;

			if(!SetupWidgetOuterProgram())
				return false;

			if(!SetupWidgetTextProgram())
				return false;

			if(!SetupWidgetTriangleProgram())
				return false;

			if(!SetupWidgetSimpleTriangleProgram())
				return false;

			if(!SetupWidgetImageProgram())
				return false;

			if(!SetupWidgetLineProgram())
				return false;

			if(!SetupPrimitiveProgram())
				return false;

			if(!SetupFrameInnerProgram())
				return false;

			if(!SetupFrameOuterProgram())
				return false;

			if(!SetupFrameImageProgram())
				return false;

			if(!SetupFrameShadowProgram())
				return false;

			// setup uniform block

			const GLchar* names[] = {
				"projection",
				"view",
				"model"
			};
			GLuint block_index = 0;
			GLuint indices[3];
			GLubyte* buf_p = NULL;

			// set default matrix
			glm::mat4 projection = glm::ortho(
					0.f, 800.f,
					0.f, 600.f,
					100.f, -100.f);
			glm::mat4 view = glm::lookAt(
					glm::vec3(0.f, 0.f, 1.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 model(1.f);

			block_index = glGetUniformBlockIndex(widget_inner_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_inner_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			glGetActiveUniformBlockiv(widget_inner_program_->id(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &widget_matrices_ubo_total_size_);
			if(widget_matrices_ubo_total_size_ <= 0) {
				DBG_PRINT_MSG("Error: %s", "matrix block size < 0");
				exit(EXIT_FAILURE);
			}

			buf_p = (GLubyte*)malloc(widget_matrices_ubo_total_size_);

			glGetUniformIndices(widget_inner_program_->id(), 3, names, indices);
			glGetActiveUniformsiv(widget_inner_program_->id(), 3, indices, GL_UNIFORM_OFFSET, widget_matrices_ubo_offset_);
			glGetActiveUniformsiv(widget_inner_program_->id(), 3, indices, GL_UNIFORM_SIZE, widget_matrices_ubo_size_);
			glGetActiveUniformsiv(widget_inner_program_->id(), 3, indices, GL_UNIFORM_TYPE, widget_matrices_ubo_type_);

			memcpy(buf_p + widget_matrices_ubo_offset_[ProjectionIndex],
					glm::value_ptr(projection),
					widget_matrices_ubo_size_[ProjectionIndex] *
					TypeSize(widget_matrices_ubo_type_[ProjectionIndex]));
			memcpy(buf_p + widget_matrices_ubo_offset_[ViewIndex],
					glm::value_ptr(view),
					widget_matrices_ubo_size_[ViewIndex] *
					TypeSize(widget_matrices_ubo_type_[ViewIndex]));
			memcpy(buf_p + widget_matrices_ubo_offset_[ModelIndex],
					glm::value_ptr(model),
					widget_matrices_ubo_size_[ModelIndex] *
					TypeSize(widget_matrices_ubo_type_[ModelIndex]));

			widget_matrices_ubo_.reset(new GLBuffer<UNIFORM_BUFFER>);
			widget_matrices_ubo_->generate();
			widget_matrices_ubo_->bind();
			widget_matrices_ubo_->set_data(widget_matrices_ubo_total_size_, buf_p, GL_DYNAMIC_DRAW);
			widget_matrices_ubo_->reset();

			glBindBufferBase(GL_UNIFORM_BUFFER, widget_matrices_ubo_binding_point_, widget_matrices_ubo_->id());

			free(buf_p);
			buf_p = 0;

			// set uniform block in text program

			//GLint block_size = 0;
			//GLint offset[2];

			block_index = glGetUniformBlockIndex(widget_split_inner_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_split_inner_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			block_index = glGetUniformBlockIndex(widget_outer_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_outer_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			// set uniform block in text program

			block_index = glGetUniformBlockIndex(widget_text_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_text_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			// set uniform block in triangle program

			block_index = glGetUniformBlockIndex(widget_triangle_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_triangle_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			// set uniform block for simple triangle program

			block_index = glGetUniformBlockIndex(widget_simple_triangle_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_simple_triangle_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			// set uniform block in image program

			block_index = glGetUniformBlockIndex(widget_image_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_image_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			// set uniform block in line program

			block_index = glGetUniformBlockIndex(widget_line_program_->id(), "WidgetMatrices");
			glUniformBlockBinding(widget_line_program_->id(), block_index, widget_matrices_ubo_binding_point_);

			// ------------------------------ Frame matrix uniform block

			block_index = glGetUniformBlockIndex(frame_inner_program_->id(), "FrameMatrices");
			glUniformBlockBinding(frame_inner_program_->id(), block_index, frame_matrices_ubo_binding_point_);

			glGetActiveUniformBlockiv(frame_inner_program_->id(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &frame_matrices_ubo_total_size_);
			if(frame_matrices_ubo_total_size_ <= 0) {
				DBG_PRINT_MSG("Error: %s", "matrix block size < 0");
				exit(EXIT_FAILURE);
			}

			buf_p = (GLubyte*)malloc(frame_matrices_ubo_total_size_);

			glGetUniformIndices(frame_inner_program_->id(), 3, names, indices);
			glGetActiveUniformsiv(frame_inner_program_->id(), 3, indices, GL_UNIFORM_OFFSET, frame_matrices_ubo_offset_);
			glGetActiveUniformsiv(frame_inner_program_->id(), 3, indices, GL_UNIFORM_SIZE, frame_matrices_ubo_size_);
			glGetActiveUniformsiv(frame_inner_program_->id(), 3, indices, GL_UNIFORM_TYPE, frame_matrices_ubo_type_);

			memcpy(buf_p + frame_matrices_ubo_offset_[ProjectionIndex],
					glm::value_ptr(projection),
					frame_matrices_ubo_size_[ProjectionIndex] *
					TypeSize(frame_matrices_ubo_type_[ProjectionIndex]));
			memcpy(buf_p + frame_matrices_ubo_offset_[ViewIndex],
					glm::value_ptr(view),
					frame_matrices_ubo_size_[ViewIndex] *
					TypeSize(frame_matrices_ubo_type_[ViewIndex]));
			memcpy(buf_p + frame_matrices_ubo_offset_[ModelIndex],
					glm::value_ptr(model),
					frame_matrices_ubo_size_[ModelIndex] *
					TypeSize(frame_matrices_ubo_type_[ModelIndex]));

			frame_matrices_ubo_.reset(new GLBuffer<UNIFORM_BUFFER>);
			frame_matrices_ubo_->generate();
			frame_matrices_ubo_->bind();
			frame_matrices_ubo_->set_data(frame_matrices_ubo_total_size_, buf_p, GL_DYNAMIC_DRAW);
			frame_matrices_ubo_->reset();

			glBindBufferBase(GL_UNIFORM_BUFFER, frame_matrices_ubo_binding_point_, frame_matrices_ubo_->id());

			free(buf_p);
			buf_p = 0;

			// set uniform block in frame outer program

			block_index = glGetUniformBlockIndex(frame_outer_program_->id(), "FrameMatrices");
			glUniformBlockBinding(frame_outer_program_->id(), block_index, frame_matrices_ubo_binding_point_);

			// set uniform block in frame image program

			block_index = glGetUniformBlockIndex(frame_image_program_->id(), "FrameMatrices");
			glUniformBlockBinding(frame_image_program_->id(), block_index, frame_matrices_ubo_binding_point_);

			// set uniform block in frame shadow program

			block_index = glGetUniformBlockIndex(frame_shadow_program_->id(), "FrameMatrices");
			glUniformBlockBinding(frame_shadow_program_->id(), block_index, frame_matrices_ubo_binding_point_);

			return true;
		}

		bool Shaders::SetupWidgetTextProgram()
		{
			if (!widget_text_program_->Create())
				return false;

			widget_text_program_->AttachShader(widget_text_vertex_shader, GL_VERTEX_SHADER);
			widget_text_program_->AttachShader(widget_text_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!widget_text_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the text program: %d",
				        widget_text_program_->id());
				return false;
			}

			locations_[WIDGET_TEXT_COORD] = widget_text_program_->GetAttributeLocation("aCoord");
			//locations_[TEXT_PROJECTION] = text_program_->GetUniformLocation("u_projection");
			//locations_[TEXT_VIEW] = text_program_->GetUniformLocation("u_view");
			locations_[WIDGET_TEXT_POSITION] = widget_text_program_->GetUniformLocation("uPosition");
			locations_[WIDGET_TEXT_ROTATION] = widget_text_program_->GetUniformLocation("uRotation");
			locations_[WIDGET_TEXT_TEXTURE] = widget_text_program_->GetUniformLocation("u_tex");
			locations_[WIDGET_TEXT_COLOR] = widget_text_program_->GetUniformLocation("uColor");

			/*
			Stock::location[WIDGET_TEXT_COORD] = widget_text_program_->GetAttributeLocation("aCoord");
			//location[TEXT_PROJECTION] = text_program_->GetUniformLocation("u_projection");
			//location[TEXT_VIEW] = text_program_->GetUniformLocation("u_view");
			Stock::location[WIDGET_TEXT_POSITION] = widget_text_program_->GetUniformLocation("uPosition");
			Stock::location[WIDGET_TEXT_ROTATION] = widget_text_program_->GetUniformLocation("uRotation");
			Stock::location[WIDGET_TEXT_TEXTURE] = widget_text_program_->GetUniformLocation("u_tex");
			Stock::location[WIDGET_TEXT_COLOR] = widget_text_program_->GetUniformLocation("uColor");
			*/

			return true;
		}

		bool Shaders::SetupWidgetTriangleProgram()
		{
			if (!widget_triangle_program_->Create()) {
				return false;
			}

			widget_triangle_program_->AttachShader(
			        widget_triangle_vertex_shader, GL_VERTEX_SHADER);
			widget_triangle_program_->AttachShader(
			        widget_triangle_geometry_shader,
			        GL_GEOMETRY_SHADER);
			widget_triangle_program_->AttachShader(
			        widget_triangle_fragment_shader, GL_FRAGMENT_SHADER);
			if (!widget_triangle_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget program: %d",
				        widget_triangle_program_->id());
				return false;
			}

			locations_[WIDGET_TRIANGLE_COORD] = widget_triangle_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_TRIANGLE_COLOR] = widget_triangle_program_->GetAttributeLocation("aColor");
			//locations_[TRIANGLE_PROJECTION] = triangle_program_->GetUniformLocation("u_projection");
			//locations_[TRIANGLE_VIEW] = triangle_program_->GetUniformLocation("u_view");
			locations_[WIDGET_TRIANGLE_POSITION] = widget_triangle_program_->GetUniformLocation("uPosition");
			locations_[WIDGET_TRIANGLE_ROTATION] = widget_triangle_program_->GetUniformLocation("uRotation");
			locations_[WIDGET_TRIANGLE_SCALE] = widget_triangle_program_->GetUniformLocation("uScale");
			locations_[WIDGET_TRIANGLE_ANTI_ALIAS] = widget_triangle_program_->GetUniformLocation("uAA");
			locations_[WIDGET_TRIANGLE_GAMMA] = widget_triangle_program_->GetUniformLocation("uGamma");

			return true;
		}

		bool Shaders::SetupWidgetSimpleTriangleProgram()
		{
			if (!widget_simple_triangle_program_->Create()) {
				return false;
			}

			widget_simple_triangle_program_->AttachShader(
			        widget_simple_triangle_vertex_shader, GL_VERTEX_SHADER);
			widget_simple_triangle_program_->AttachShader(
			        widget_simple_triangle_fragment_shader, GL_FRAGMENT_SHADER);
			if (!widget_simple_triangle_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget simple triangle program: %d",
						widget_simple_triangle_program_->id());
				return false;
			}

			locations_[WIDGET_SIMPLE_TRIANGLE_COORD] = widget_simple_triangle_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_SIMPLE_TRIANGLE_POSITION] = widget_simple_triangle_program_->GetUniformLocation("uPosition");
			locations_[WIDGET_SIMPLE_TRIANGLE_COLOR] = widget_simple_triangle_program_->GetUniformLocation("uColor");
			locations_[WIDGET_SIMPLE_TRIANGLE_GAMMA] = widget_simple_triangle_program_->GetUniformLocation("uGamma");
			//locations_[TRIANGLE_PROJECTION] = triangle_program_->GetUniformLocation("u_projection");
			//locations_[TRIANGLE_VIEW] = triangle_program_->GetUniformLocation("u_view");

			return true;
		}

		bool Shaders::SetupWidgetInnerProgram()
		{
			if (!widget_inner_program_->Create()) {
				return false;
			}

			widget_inner_program_->AttachShader(
			        widget_inner_vertex_shader, GL_VERTEX_SHADER);
			widget_inner_program_->AttachShader(
			        widget_inner_fragment_shader, GL_FRAGMENT_SHADER);
			if (!widget_inner_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget inner program: %d",
						widget_inner_program_->id());
				return false;
			}

			locations_[WIDGET_INNER_COORD] = widget_inner_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_INNER_COLOR] = widget_inner_program_->GetUniformLocation("uColor");
			locations_[WIDGET_INNER_GAMMA] = widget_inner_program_->GetUniformLocation("uGamma");

			return true;
		}

		bool Shaders::SetupWidgetSplitInnerProgram()
		{
			if (!widget_split_inner_program_->Create()) {
				return false;
			}

			widget_split_inner_program_->AttachShader(
			        widget_split_inner_vertex_shader, GL_VERTEX_SHADER);
			widget_split_inner_program_->AttachShader(
			        widget_split_inner_fragment_shader, GL_FRAGMENT_SHADER);
			if (!widget_split_inner_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget split inner program: %d",
						widget_split_inner_program_->id());
				return false;
			}

			locations_[WIDGET_SPLIT_INNER_COORD] = widget_split_inner_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_SPLIT_INNER_COLOR0] = widget_split_inner_program_->GetUniformLocation("u_color0");
			locations_[WIDGET_SPLIT_INNER_COLOR1] = widget_split_inner_program_->GetUniformLocation("u_color1");
			locations_[WIDGET_SPLIT_INNER_PARTING] = widget_split_inner_program_->GetUniformLocation("u_parting");
			locations_[WIDGET_SPLIT_INNER_GAMMA] = widget_split_inner_program_->GetUniformLocation("uGamma");

			return true;
		}

		bool Shaders::SetupWidgetOuterProgram()
		{
			if (!widget_outer_program_->Create()) {
				return false;
			}

			widget_outer_program_->AttachShader(
			        widget_outer_vertex_shader, GL_VERTEX_SHADER);
			widget_outer_program_->AttachShader(
			        widget_outer_geometry_shader,
			        GL_GEOMETRY_SHADER);
			widget_outer_program_->AttachShader(
			        widget_outer_fragment_shader, GL_FRAGMENT_SHADER);
			if (!widget_outer_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget outer program: %d",
						widget_outer_program_->id());
				return false;
			}

			locations_[WIDGET_OUTER_COORD] = widget_outer_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_OUTER_COLOR] = widget_outer_program_->GetUniformLocation("uColor");
			locations_[WIDGET_OUTER_POSITION] = widget_outer_program_->GetUniformLocation("uPosition");

			return true;
		}

		bool Shaders::SetupWidgetImageProgram()
		{
			if (!widget_image_program_->Create()) {
				return false;
			}

			widget_image_program_->AttachShader(widget_image_vertex_shader,
			        GL_VERTEX_SHADER);
			widget_image_program_->AttachShader(widget_image_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!widget_image_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the pixelicon program: %d",
				        widget_image_program_->id());
				return false;
			}

			locations_[WIDGET_IMAGE_COORD] = widget_image_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_IMAGE_UV] = widget_image_program_->GetAttributeLocation("a_uv");
			//locations_[IMAGE_PROJECTION] = image_program_->GetUniformLocation("u_projection");
			//locations_[IMAGE_VIEW] = image_program_->GetUniformLocation("u_view");
			locations_[WIDGET_IMAGE_POSITION] = widget_image_program_->GetUniformLocation("uPosition");
			locations_[WIDGET_IMAGE_ROTATION] = widget_image_program_->GetUniformLocation("uRotation");
			locations_[WIDGET_IMAGE_TEXTURE] = widget_image_program_->GetUniformLocation("uTexture");
			locations_[WIDGET_IMAGE_GAMMA] = widget_image_program_->GetUniformLocation("uGamma");

			return true;
		}

		bool Shaders::SetupWidgetLineProgram()
		{
			if (!widget_line_program_->Create()) {
				return false;
			}

			widget_line_program_->AttachShader(widget_line_vertex_shader,
			        GL_VERTEX_SHADER);
			widget_line_program_->AttachShader(widget_line_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!widget_line_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the widget line program: %d",
						widget_line_program_->id());
				return false;
			}

			locations_[WIDGET_LINE_COORD] = widget_line_program_->GetAttributeLocation("aCoord");
			locations_[WIDGET_LINE_COLOR] = widget_line_program_->GetUniformLocation("uColor");

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

		void Shaders::SetFrameProjectionMatrix(const glm::mat4& matrix)
		{
			frame_matrices_ubo_->bind();
			frame_matrices_ubo_->set_sub_data(
					frame_matrices_ubo_offset_[ProjectionIndex],
					frame_matrices_ubo_size_[ProjectionIndex] *
					TypeSize(frame_matrices_ubo_type_[ProjectionIndex]),
					glm::value_ptr(matrix));
			frame_matrices_ubo_->reset();
		}

		void Shaders::SetFrameViewMatrix(const glm::mat4& matrix)
		{
			frame_matrices_ubo_->bind();
			frame_matrices_ubo_->set_sub_data(
					frame_matrices_ubo_offset_[ViewIndex],
					frame_matrices_ubo_size_[ViewIndex] *
					TypeSize(frame_matrices_ubo_type_[ViewIndex]),
					glm::value_ptr(matrix));
			frame_matrices_ubo_->reset();
		}

		void Shaders::SetFrameModelMatrix(const glm::mat4& matrix)
		{
			frame_matrices_ubo_->bind();
			frame_matrices_ubo_->set_sub_data(
					frame_matrices_ubo_offset_[ModelIndex],
					frame_matrices_ubo_size_[ModelIndex] *
					TypeSize(frame_matrices_ubo_type_[ModelIndex]),
					glm::value_ptr(matrix));
			frame_matrices_ubo_->reset();
		}

		bool Shaders::SetupFrameInnerProgram()
		{
			if(!frame_inner_program_->Create()) {
				return false;
			}

			frame_inner_program_->AttachShader(frame_inner_vertex_shader, GL_VERTEX_SHADER);
			frame_inner_program_->AttachShader(frame_inner_fragment_shader, GL_FRAGMENT_SHADER);

			if(!frame_inner_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the frame inner program: %d", frame_inner_program_->id());
				return false;
			}

			locations_[FRAME_INNER_COORD] = frame_inner_program_->GetAttributeLocation("aCoord");
			locations_[FRAME_INNER_COLOR] = frame_inner_program_->GetUniformLocation("uColor");
			locations_[FRAME_INNER_POSITION] = frame_inner_program_->GetUniformLocation("uPosition");
			locations_[FRAME_INNER_GAMMA] = frame_inner_program_->GetUniformLocation("uGamma");

			return true;
		}

		bool Shaders::SetupFrameOuterProgram()
		{
			if(!frame_outer_program_->Create()) {
				return false;
			}

			frame_outer_program_->AttachShader(frame_outer_vertex_shader, GL_VERTEX_SHADER);
			frame_outer_program_->AttachShader(frame_outer_geometry_shader, GL_GEOMETRY_SHADER);
			frame_outer_program_->AttachShader(frame_outer_fragment_shader, GL_FRAGMENT_SHADER);

			if(!frame_outer_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the frame outer program: %d", frame_outer_program_->id());
				return false;
			}

			locations_[FRAME_OUTER_COORD] = frame_outer_program_->GetAttributeLocation("aCoord");
			locations_[FRAME_OUTER_COLOR] = frame_outer_program_->GetUniformLocation("uColor");
			locations_[FRAME_OUTER_POSITION] = frame_outer_program_->GetUniformLocation("uPosition");

			return true;
		}

		bool Shaders::SetupFrameImageProgram()
		{
			if (!frame_image_program_->Create()) {
				return false;
			}

			frame_image_program_->AttachShader(frame_image_vertex_shader,
			        GL_VERTEX_SHADER);
			frame_image_program_->AttachShader(frame_image_fragment_shader,
			        GL_FRAGMENT_SHADER);
			if (!frame_image_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the frame image program: %d",
						frame_image_program_->id());
				return false;
			}

			locations_[FRAME_IMAGE_COORD] = frame_image_program_->GetAttributeLocation("aCoord");
			locations_[FRAME_IMAGE_UV] = frame_image_program_->GetAttributeLocation("aUV");
			locations_[FRAME_IMAGE_POSITION] = frame_image_program_->GetUniformLocation("uPosition");
			locations_[FRAME_IMAGE_TEXTURE] = frame_image_program_->GetUniformLocation("uTexture");
			locations_[FRAME_IMAGE_GAMMA] = frame_image_program_->GetUniformLocation("uGamma");

			return true;
		}

		bool Shaders::SetupFrameShadowProgram()
		{
			if(!frame_shadow_program_->Create()) {
				return false;
			}

			frame_shadow_program_->AttachShader(frame_shadow_vertex_shader, GL_VERTEX_SHADER);
			frame_shadow_program_->AttachShader(frame_shadow_geometry_shader, GL_GEOMETRY_SHADER);
			frame_shadow_program_->AttachShader(frame_shadow_fragment_shader, GL_FRAGMENT_SHADER);

			if(!frame_shadow_program_->Link()) {
				DBG_PRINT_MSG("Fail to link the frame shadow program: %d", frame_shadow_program_->id());
				return false;
			}

			//locations_[FRAME_SHADOW_COORD] = frame_shadow_program_->GetAttributeLocation("aCoord");
			//locations_[FRAME_SHADOW_COLOR] = frame_shadow_program_->GetUniformLocation("uColor");
			//locations_[FRAME_SHADOW_POSITION] = frame_shadow_program_->GetUniformLocation("uPosition");

			locations_[FRAME_SHADOW_COORD] = frame_shadow_program_->GetAttributeLocation("aCoord");
			locations_[FRAME_SHADOW_POSITION] = frame_shadow_program_->GetUniformLocation("uPosition");
			locations_[FRAME_SHADOW_ANTI_ALIAS] = frame_shadow_program_->GetUniformLocation("uAA");
			locations_[FRAME_SHADOW_SIZE] = frame_shadow_program_->GetUniformLocation("uSize");

			return true;
		}

		/* Helper function to convert GLSL types to storage sizes */
		size_t Shaders::TypeSize(GLenum type)
		{
			size_t size;

#define CASE(Enum, Count, Type) \
		case Enum: size = Count * sizeof(Type); break

			switch (type) {
			CASE(GL_FLOAT, 				 1, 	GLfloat);
			CASE(GL_FLOAT_VEC2, 		 2, 	GLfloat);
			CASE(GL_FLOAT_VEC3,			 3, 	GLfloat);
			CASE(GL_FLOAT_VEC4,			 4, 	GLfloat);
			CASE(GL_INT,				 1, 	GLint);
			CASE(GL_INT_VEC2,			 2, 	GLint);
			CASE(GL_INT_VEC3,			 3, 	GLint);
			CASE(GL_INT_VEC4,			 4, 	GLint);
			CASE(GL_UNSIGNED_INT,		 1, 	GLuint);
			CASE(GL_UNSIGNED_INT_VEC2,	 2, 	GLuint);
			CASE(GL_UNSIGNED_INT_VEC3,	 3, 	GLuint);
			CASE(GL_UNSIGNED_INT_VEC4,	 4, 	GLuint);
			CASE(GL_BOOL,				 1, 	GLboolean);
			CASE(GL_BOOL_VEC2,			 2, 	GLboolean);
			CASE(GL_BOOL_VEC3,			 3, 	GLboolean);
			CASE(GL_BOOL_VEC4,			 4, 	GLboolean);
			CASE(GL_FLOAT_MAT2,			 4, 	GLfloat);
			CASE(GL_FLOAT_MAT2x3,		 6, 	GLfloat);
			CASE(GL_FLOAT_MAT2x4,		 8, 	GLfloat);
			CASE(GL_FLOAT_MAT3,			 9, 	GLfloat);
			CASE(GL_FLOAT_MAT3x2,		 6, 	GLfloat);
			CASE(GL_FLOAT_MAT3x4,		12,		GLfloat);
			CASE(GL_FLOAT_MAT4,			16, 	GLfloat);
			CASE(GL_FLOAT_MAT4x2,		 8, 	GLfloat);
			CASE(GL_FLOAT_MAT4x3,		12, 	GLfloat);
#undef CASE

			default:
				fprintf(stderr, "Unknown type:0x%x\n", type);
				exit(EXIT_FAILURE);
				break;

			}

			return size;
		}

	}

}

