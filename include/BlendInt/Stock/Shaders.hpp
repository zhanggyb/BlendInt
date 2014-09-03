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

			static Shaders* instance;

			const RefPtr<GLSLProgram>& text_program () const
			{
				return m_text_program;
			}

			const RefPtr<GLSLProgram>& primitive_program () const
			{
				return m_primitive_program;
			}

			const RefPtr<GLSLProgram>& triangle_program () const
			{
				return m_triangle_program;
			}

			const RefPtr<GLSLProgram>& triangle_program_ext () const
			{
				return m_triangle_program_ext;
			}

			const RefPtr<GLSLProgram>& line_program () const
			{
				return m_line_program;
			}

			const RefPtr<GLSLProgram>& context_program () const
			{
				return m_context_program;
			}

			const RefPtr<GLSLProgram>& image_program () const
			{
				return m_image_program;
			}

			GLint primitive_attrib_color_v4 () const
			{
				return m_primitive_attrib_color_v4;
			}

			GLint primitive_attrib_coord_v3 () const
			{
				return m_primitive_attrib_coord_v3;
			}

			GLint primitive_uniform_model () const
			{
				return m_primitive_uniform_model;
			}

			GLint primitive_uniform_projection () const
			{
				return m_primitive_uniform_projection;
			}

			GLint primitive_uniform_view () const
			{
				return m_primitive_uniform_view;
			}

			GLint text_attrib_coord () const
			{
				return m_text_attrib_coord;
			}

			GLint text_uniform_color () const
			{
				return m_text_uniform_color;
			}

			GLint text_uniform_position () const
			{
				return m_text_uniform_position;
			}

			GLint text_uniform_projection () const
			{
				return m_text_uniform_projection;
			}

			GLint text_uniform_rotation () const
			{
				return m_text_uniform_rotation;
			}

			GLint text_uniform_texture () const
			{
				return m_text_uniform_texture;
			}

			GLint text_uniform_view () const
			{
				return m_text_uniform_view;
			}

			GLint triangle_uniform_scale () const
			{
				return m_triangle_uniform_scale;
			}

			/**
			 * @brief The vertex attribute location of coord
			 *
			 * Always return 0
			 */
			GLint triangle_attrib_coord () const
			{
				return m_triangle_attrib_coord;
			}

			/**
			 * @brief The vertex attribute location of color
			 *
			 * Always return 1
			 */
			GLint triangle_attrib_color () const
			{
				return m_triangle_attrib_color;
			}

			GLint triangle_uniform_antialias () const
			{
				return m_triangle_uniform_antialias;
			}

			GLint triangle_uniform_gamma () const
			{
				return m_triangle_uniform_gamma;
			}

			GLint triangle_uniform_position () const
			{
				return m_triangle_uniform_position;
			}

			GLint triangle_uniform_projection () const
			{
				return m_triangle_uniform_projection;
			}

			GLint triangle_uniform_rotation () const
			{
				return m_triangle_uniform_rotation;
			}

			GLint triangle_uniform_view () const
			{
				return m_triangle_uniform_view;
			}

			// ----------------------------------------------------

			GLint triangle_uniform_scale_ext () const
			{
				return m_triangle_uniform_scale_ext;
			}

			/**
			 * @brief The vertex attribute location of coord
			 *
			 * Always return 0
			 */
			GLint triangle_attrib_coord_ext () const
			{
				return m_triangle_attrib_coord_ext;
			}

			/**
			 * @brief The vertex attribute location of color
			 *
			 * Always return 1
			 */
			GLint triangle_attrib_shade_ext () const
			{
				return m_triangle_attrib_shade_ext;
			}

			GLint triangle_uniform_color_ext () const
			{
				return m_triangle_uniform_color_ext;
			}

			GLint triangle_uniform_antialias_ext () const
			{
				return m_triangle_uniform_antialias_ext;
			}

			GLint triangle_uniform_gamma_ext () const
			{
				return m_triangle_uniform_gamma_ext;
			}

			GLint triangle_uniform_position_ext () const
			{
				return m_triangle_uniform_position_ext;
			}

			GLint triangle_uniform_projection_ext () const
			{
				return m_triangle_uniform_projection_ext;
			}

			GLint triangle_uniform_rotation_ext () const
			{
				return m_triangle_uniform_rotation_ext;
			}

			GLint triangle_uniform_view_ext () const
			{
				return m_triangle_uniform_view_ext;
			}

			// ----------------------------------------------------

			/**
			 * @brief The vertex attribute location of coord
			 *
			 * Always return 0
			 */
			GLint line_attrib_coord () const
			{
				return m_line_attrib_coord;
			}

			/**
			 * @brief The vertex attribute location of color
			 *
			 * Always return 1
			 */
			GLint line_attrib_color () const
			{
				return m_line_attrib_color;
			}

			GLint line_uniform_antialias () const
			{
				return m_line_uniform_antialias;
			}

			GLint line_uniform_gamma () const
			{
				return m_line_uniform_gamma;
			}

			GLint line_uniform_position () const
			{
				return m_line_uniform_position;
			}

			GLint line_uniform_projection () const
			{
				return m_line_uniform_projection;
			}

			GLint line_uniform_rotation () const
			{
				return m_line_uniform_rotation;
			}

			GLint line_uniform_view () const
			{
				return m_line_uniform_view;
			}

			GLint image_attrib_coord () const
			{
				return m_image_attrib_coord;
			}

			GLint image_attrib_uv () const
			{
				return m_image_attrib_uv;
			}

			GLint image_uniform_gamma () const
			{
				return m_image_uniform_gamma;
			}

			GLint image_uniform_position () const
			{
				return m_image_uniform_position;
			}

			GLint image_uniform_projection () const
			{
				return m_image_uniform_projection;
			}

			GLint image_uniform_rotation () const
			{
				return m_image_uniform_rotation;
			}

			GLint image_uniform_texture () const
			{
				return m_image_uniform_texture;
			}

			GLint image_uniform_view () const
			{
				return m_image_uniform_view;
			}

		private:

			friend class BlendInt::Interface;

			static bool Initialize ();

			static void Release ();

			Shaders ();

			~Shaders ();

			bool Setup ();

			RefPtr<GLSLProgram> m_text_program;

			RefPtr<GLSLProgram> m_primitive_program;

			RefPtr<GLSLProgram> m_triangle_program;

			// ---------------------------------------------

			RefPtr<GLSLProgram> m_triangle_program_ext;

			// ---------------------------------------------

			RefPtr<GLSLProgram> m_line_program;

			RefPtr<GLSLProgram> m_context_program;

			RefPtr<GLSLProgram> m_image_program;

			GLint m_text_attrib_coord;

			GLint m_text_uniform_projection;

			GLint m_text_uniform_view;

			GLint m_text_uniform_position;

			GLint m_text_uniform_rotation;

			GLint m_text_uniform_texture;

			GLint m_text_uniform_color;

			GLint m_primitive_attrib_coord_v3;

			GLint m_primitive_attrib_color_v4;

			GLint m_primitive_uniform_projection;

			GLint m_primitive_uniform_view;

			GLint m_primitive_uniform_model;

			GLint m_triangle_attrib_coord;

			GLint m_triangle_attrib_color;

			GLint m_triangle_uniform_projection;

			GLint m_triangle_uniform_view;

			GLint m_triangle_uniform_position;

			GLint m_triangle_uniform_rotation;

			GLint m_triangle_uniform_scale;

			GLint m_triangle_uniform_antialias;

			GLint m_triangle_uniform_gamma;

			// ---------------------------------------------------

			GLint m_triangle_attrib_coord_ext;

			GLint m_triangle_attrib_shade_ext;

			GLint m_triangle_uniform_color_ext;

			GLint m_triangle_uniform_projection_ext;

			GLint m_triangle_uniform_view_ext;

			GLint m_triangle_uniform_position_ext;

			GLint m_triangle_uniform_rotation_ext;

			GLint m_triangle_uniform_scale_ext;

			GLint m_triangle_uniform_antialias_ext;

			GLint m_triangle_uniform_gamma_ext;

			// ---------------------------------------------------

			GLint m_line_attrib_coord;

			GLint m_line_attrib_color;

			GLint m_line_uniform_projection;

			GLint m_line_uniform_view;

			GLint m_line_uniform_position;

			GLint m_line_uniform_rotation;

			GLint m_line_uniform_scale;

			GLint m_line_uniform_antialias;

			GLint m_line_uniform_gamma;

			GLint m_image_attrib_coord;

			GLint m_image_attrib_uv;

			GLint m_image_uniform_projection;

			GLint m_image_uniform_view;

			GLint m_image_uniform_position;

			GLint m_image_uniform_rotation;

			GLint m_image_uniform_texture;

			GLint m_image_uniform_gamma;

			static const char* text_vertex_shader;

			static const char* text_fragment_shader;

			static const char* primitive_vertex_shader;

			static const char* primitive_fragment_shader;

			static const char* widget_vertex_shader;

			static const char* widget_triangle_geometry_shader;

			static const char* widget_line_geometry_shader;

			static const char* widget_fragment_shader;

			// ----------------------------------------------

			static const char* widget_vertex_shader_ext;

			static const char* widget_triangle_geometry_shader_ext;

			static const char* widget_fragment_shader_ext;

			// ----------------------------------------------

			static const char* context_vertex_shader;

			static const char* context_fragment_shader;

			static const char* image_vertex_shader;

			static const char* image_fragment_shader;
		};

	}

}

#endif /* _BIL_SHADERMANAGER_HPP_ */
