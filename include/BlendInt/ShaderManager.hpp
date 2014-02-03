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

	char* file_read (const char* filename);
	void print_log (GLuint object);
	GLuint create_shader (const char* filename, GLenum type);
	GLuint create_program (const char* vertexfile, const char *fragmentfile);
	GLuint create_gs_program (const char* vertexfile, const char *geometryfile,
	        const char *fragmentfile, GLint input, GLint output,
	        GLint vertices);
	GLint get_attrib (GLuint program, const char *name);
	GLint get_uniform (GLuint program, const char *name);

	class Interface;

	/**
	 * @pre-defined shaders
	 */
	class ShaderManager
	{
	public:

		friend class Interface;

		static ShaderManager* Instance ();

		GLSLProgram* text_program () const
		{
			return m_text_program;
		}

		GLSLProgram* primitive_program () const
		{
			return m_primitive_program;
		}

		bool Find (const GLSLProgram* program);

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

		const GLint& uniform_mvp ()
		{
			return m_uniform_mvp;
		}

		const GLuint& text_vbo ()
		{
			return text_vbo_;
		}

	private:

		static bool Initialize ();

		static void Release ();

		ShaderManager ();

		~ShaderManager ();

		bool Setup ();

		GLint text_attribute_coord_;
		GLint text_uniform_tex_;
		GLint text_uniform_color_;
		GLuint text_vbo_;

		GLSLProgram* m_text_program;

		GLint m_uniform_mvp;

		GLSLProgram* m_primitive_program;

		static const char* text_vertex_shader;

		static const char* text_fragment_shader;

		static const char* primitive_vertex_shader;

		static const char* primitive_fragment_shader;

		static ShaderManager* shader_manager;

	};
}

#endif /* _BIL_SHADERMANAGER_HPP_ */
