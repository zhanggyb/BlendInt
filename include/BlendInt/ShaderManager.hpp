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

#include <BlendInt/GLSLProgram.hpp>

namespace BlendInt {

	class Interface;

	/**
	 * @pre-defined shaders
	 */
	class ShaderManager
	{
	public:

		friend class Interface;

		static ShaderManager* instance ();

		const GLSLProgram& text_program ()
		{
			return text_program_;
		}

		const GLint& text_attribute_coord ()
		{
			return text_attribute_coord_;
		}

		const GLint& text_uniform_tex ()
		{
			return text_uniform_tex_;
		}

		const GLint& text_uniform_color ()
		{
			return text_uniform_color_;
		}

		const GLuint& text_vbo ()
		{
			return text_vbo_;
		}

	private:

		static bool initialize ();

		static void release ();

		ShaderManager ();

		~ShaderManager ();

		bool setup ();

		GLint text_attribute_coord_;
		GLint text_uniform_tex_;
		GLint text_uniform_color_;
		GLuint text_vbo_;

		GLSLProgram text_program_;

		static const char* text_vs_shader_;

		static const char* text_fs_shader_;

		static ShaderManager* shader_manager;

	};
}

#endif /* _BIL_SHADERMANAGER_HPP_ */
