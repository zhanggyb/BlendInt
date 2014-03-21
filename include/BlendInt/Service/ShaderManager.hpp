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
	 * @pre-defined shaders
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

		const GLint& uniform_mvp ()
		{
			return m_uniform_mvp;
		}

	private:

		static bool Initialize ();

		static void Release ();

		ShaderManager ();

		~ShaderManager ();

		bool Setup ();

		RefPtr<GLSLProgram> m_text_program;

		GLint m_uniform_mvp;

		RefPtr<GLSLProgram> m_primitive_program;

		RefPtr<GLSLProgram> m_default_widget_program;

		RefPtr<GLSLProgram> m_default_form_program;

		static const char* text_vertex_shader;

		static const char* text_fragment_shader;

		static const char* primitive_vertex_shader;

		static const char* primitive_fragment_shader;

		static const char* default_widget_vertex_shader;

		static const char* default_widget_fragment_shader;

		static const char* default_form_vertex_shader;

		static const char* default_form_fragment_shader;

	};
}

#endif /* _BIL_SHADERMANAGER_HPP_ */
