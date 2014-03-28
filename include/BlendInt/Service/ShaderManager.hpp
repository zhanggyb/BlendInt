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

#ifndef _BLENDINT_SHADERMANAGER_HPP_
#define _BLENDINT_SHADERMANAGER_HPP_

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BlendInt {

	class Interface;

	/**
	 * @brief A class which provide pre-defined shaders
	 *
	 * The ShaderManager class works as a service in BlendInt and
	 * provide simple pre-defined shaders for widgets. There's only
	 * one instance of this class and can be accessed by:
	 *
	 * @code
	 * #include <BlendInt/Service/ShaderManager.hpp>
	 *
	 * ShaderManager::instance
	 * @endcode
	 *
	 * The instance can not be deleted.
	 *
	 * There're some pre-defined shader which is used in widgets
	 *
	 * - Vertex shader for text:
	 * @code
	 * "#version 330\n"
	 * "layout(location = 0) in vec4 coord;"
	 * "uniform mat4 MVP;"
	 * "out vec2 texpos;"
	 * ""
	 * "void main(void) {"
	 * "  gl_Position = MVP * vec4(coord.xy, 0.0, 1.0);"
	 * "  texpos = coord.zw;"
	 * "}";
	 * @endcode
	 *
	 * - Fragment shader for text:
	 * @code
	 * "#version 330\n"
	 * "in vec2 texpos;"
	 * "uniform sampler2D tex;"
	 * "uniform vec4 color;"
	 * "out vec4 FragmentColor;"
	 * ""
	 * "void main(void) {"
	 * "	float alpha = texture(tex, texpos).r;"	// GL 3.2 only support GL_R8 in glTexImage2D internalFormat
	 * "	FragmentColor = vec4(color.rgb, color.a * alpha);"
	 * "}";
	 * @endcode
	 *
	 * - Vertex shader for primitives in Viewport3D:
	 * @code
	 * "#version 330\n"
	 * ""
	 * "layout(location = 0) in vec3 coord3d;"
	 * "layout(location = 1) in vec3 v_color;"
	 * "uniform mat4 MVP;"
	 * "out vec3 f_color;"
	 * ""
	 * "void main(void) {"
	 * "	gl_Position = MVP * vec4(coord3d, 1.0);"
	 * "	f_color = v_color;"
	 * "}";
	 * @endcode
	 *
	 * - Fragment shader for primitives in Viewport3D:
	 * @code
	 * "#version 330\n"
	 * ""
	 * "in vec3 f_color;"
	 * "out vec4 FragmentColor;"
	 * ""
	 * "void main(void) {"
	 * "	FragmentColor = vec4(f_color, 1.0);"
	 * "}";
	 * @encode
	 *
	 * - Vertex shader for widgets:
	 * @code
	 * "#version 330\n"
	 * ""
	 * "layout(location=0) in vec2 xy;"
	 * "layout(location=1) in vec4 color;"
	 * "in float z;"
	 * "uniform mat4 MVP;"
	 * "out vec4 f_color;"
	 * ""
	 * "void main(void) {"
	 * "	gl_Position = MVP * vec4(xy, z, 1.0);"
	 * "	f_color = color;"
	 * "}";
	 * @endcode
	 *
	 * - Fragment shader for widgets:
	 * @code
	 * "#version 330\n"
	 * ""
	 * "in vec4 f_color;"
	 * "uniform int gamma = 0;"
	 * "out vec4 FragmentColor;"
	 * ""
	 * "void main(void) {"
	 * "	vec4 color_calib = vec4(vec3(min(max(-1.0, gamma/255.0), 1.0)), 0.0);"
	 * "	FragmentColor = f_color + color_calib;"
	 * "}";
	 * @endcode
	 *
	 * - Vertex shader for 2D forms:
	 * @code
	 * "#version 330\n"
	 * ""
	 * "layout(location = 0) in vec2 xy;"
	 * "layout(location = 1) in vec4 color;"
	 * "uniform mat4 MVP;"
	 * "out vec4 f_color;"
	 * ""
	 * "void main(void) {"
	 * "	gl_Position = MVP * vec4(xy, 0.0, 1.0);"
	 * "	f_color = color;"
	 * "}";
	 * @endcode
	 *
	 * - Fragment shader for 2D forms:
	 * @code
	 * "#version 330\n"
	 * ""
	 * "in vec4 f_color;"
	 * "uniform int gamma = 0;"
	 * "out vec4 FragmentColor;"
	 * ""
	 * "void main(void) {"
	 * "	vec4 color_calib = vec4(vec3(min(max(-1.0, gamma/255.0), 1.0)), 0.0);"
	 * "	FragmentColor = f_color + color_calib;"
	 * "}";
	 * @endcode
	 */
	class ShaderManager
	{
	public:

		friend class Interface;

		static ShaderManager* instance;

		RefPtr<GLSLProgram> text_program () const
		{
			return m_text_program;
		}

		RefPtr<GLSLProgram> primitive_program () const
		{
			return m_primitive_program;
		}

		RefPtr<GLSLProgram> default_widget_program () const
		{
			return m_default_widget_program;
		}

		RefPtr<GLSLProgram> default_form_program () const
		{
			return m_default_form_program;
		}

		//bool Find (const GLSLProgram* program);

	private:

		static bool Initialize ();

		static void Release ();

		ShaderManager ();

		~ShaderManager ();

		bool Setup ();

		RefPtr<GLSLProgram> m_text_program;

		RefPtr<GLSLProgram> m_primitive_program;

		RefPtr<GLSLProgram> m_default_widget_program;

		RefPtr<GLSLProgram> m_default_form_program;

		static const char* text_vertex_shader;

		static const char* text_fragment_shader;

		static const char* primitive_vertex_shader;

		static const char* primitive_fragment_shader;

		static const char* default_widget_vertex_shader;

		static const char* default_widget_geometry_shader;

		static const char* default_widget_fragment_shader;

		static const char* default_form_vertex_shader;

		static const char* default_form_fragment_shader;
	};
}

#endif /* _BIL_SHADERMANAGER_HPP_ */
