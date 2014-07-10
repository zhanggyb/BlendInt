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

#ifndef _BLENDINT_STOCK_SHADERS_HPP_
#define _BLENDINT_STOCK_SHADERS_HPP_

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BlendInt {

	class Interface;

	namespace Stock {

		/**
		 * @brief A class which provide pre-defined shaders
		 *
		 * The ShaderManager class works as a service in BlendInt and
		 * provide simple pre-defined shaders for widgets.
		 */
		class Shaders
		{
		public:

			friend class BlendInt::Interface;

			static Shaders* instance;

			RefPtr<GLSLProgram> default_text_program () const
			{
				return m_default_text_program;
			}

			RefPtr<GLSLProgram> primitive_program () const
			{
				return m_primitive_program;
			}

			RefPtr<GLSLProgram> default_triangle_program () const
			{
				return m_default_triangle_program;
			}

			RefPtr<GLSLProgram> default_line_program () const
			{
				return m_default_line_program;
			}

			RefPtr<GLSLProgram> default_context_program () const
			{
				return m_default_context_program;
			}

			RefPtr<GLSLProgram> default_image_program () const
			{
				return m_default_image_program;
			}

		private:

			static bool Initialize ();

			static void Release ();

			Shaders ();

			~Shaders ();

			bool Setup ();

			RefPtr<GLSLProgram> m_default_text_program;

			RefPtr<GLSLProgram> m_primitive_program;

			RefPtr<GLSLProgram> m_default_triangle_program;

			RefPtr<GLSLProgram> m_default_line_program;

			RefPtr<GLSLProgram> m_default_context_program;

			RefPtr<GLSLProgram> m_default_image_program;

			static const char* text_vertex_shader;

			static const char* text_fragment_shader;

			static const char* primitive_vertex_shader;

			static const char* primitive_fragment_shader;

			static const char* default_widget_vertex_shader;

			static const char* default_widget_triangle_geometry_shader;

			static const char* default_widget_line_geometry_shader;

			static const char* default_widget_fragment_shader;

			static const char* default_context_vertex_shader;

			static const char* default_context_fragment_shader;

			static const char* default_image_vertex_shader;

			static const char* default_image_fragment_shader;
		};

	}

}

#endif /* _BIL_SHADERMANAGER_HPP_ */
