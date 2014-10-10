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

#include <stack>

#include <glm/glm.hpp>

#include <BlendInt/Core/RefPtr.hpp>

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BlendInt {

	class Context;

	namespace Stock {

		enum LocationType {

			// Triangles
			TRIANGLE_COORD,
			TRIANGLE_COLOR,
			//TRIANGLE_PROJECTION,
			//TRIANGLE_VIEW,
			TRIANGLE_POSITION,
			TRIANGLE_ROTATION,
			TRIANGLE_SCALE,
			TRIANGLE_ANTI_ALIAS,
			TRIANGLE_GAMMA,

			WIDGET_COORD,
			WIDGET_COLOR,
			//WIDGET_PROJECTION,
			//WIDGET_VIEW,
			WIDGET_POSITION,
			WIDGET_ROTATION,
			WIDGET_SCALE,
			WIDGET_ANTI_ALIAS,
			WIDGET_GAMMA,

			WIDGET_INNER_COORD,
			WIDGET_INNER_COLOR,
			WIDGET_INNER_GAMMA,

			WIDGET_SPLIT_INNER_COORD,
			WIDGET_SPLIT_INNER_COLOR0,	// The left or bottom color
			WIDGET_SPLIT_INNER_COLOR1,	// The right or top color
			WIDGET_SPLIT_INNER_PARTING,
			WIDGET_SPLIT_INNER_GAMMA,

			WIDGET_OUTER_COORD,
			WIDGET_OUTER_COLOR,
			WIDGET_OUTER_POSITION,	// vec2 of outline or emboss vertices

			// Text
			WIDGET_TEXT_COORD,
			//TEXT_PROJECTION,
			//TEXT_VIEW,
			WIDGET_TEXT_POSITION,
			WIDGET_TEXT_ROTATION,
			WIDGET_TEXT_TEXTURE,
			WIDGET_TEXT_COLOR,

			// Image
			WIDGET_IMAGE_COORD,
			WIDGET_IMAGE_UV,
			//IMAGE_PROJECTION,
			//IMAGE_VIEW,
			WIDGET_IMAGE_POSITION,
			WIDGET_IMAGE_ROTATION,
			WIDGET_IMAGE_TEXTURE,
			WIDGET_IMAGE_GAMMA,

			PRIMITIVE_COORD,
			PRIMITIVE_COLOR,
			PRIMITIVE_PROJECTION,
			PRIMITIVE_VIEW,
			PRIMITIVE_MODEL,

			FRAME_INNER_COORD,
			FRAME_INNER_COLOR,
			FRAME_INNER_GAMMA,

			LocationLast
		};

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

			const RefPtr<GLSLProgram>& widget_text_program () const
			{
				return widget_text_program_;
			}

			const RefPtr<GLSLProgram>& primitive_program () const
			{
				return primitive_program_;
			}

			const RefPtr<GLSLProgram>& triangle_program () const
			{
				return triangle_program_;
			}

			const RefPtr<GLSLProgram>& widget_program () const
			{
				return widget_program_;
			}

			const RefPtr<GLSLProgram>& widget_inner_program () const
			{
				return widget_inner_program_;
			}

			const RefPtr<GLSLProgram>& widget_split_inner_program () const
			{
				return widget_split_inner_program_;
			}

			const RefPtr<GLSLProgram>& widget_outer_program () const
			{
				return widget_outer_program_;
			}

			const RefPtr<GLSLProgram>& widget_image_program () const
			{
				return widget_image_program_;
			}

			const glm::mat4& ui_projection_matrix () const
			{
				return widget_projection_matrix_;
			}

			const glm::mat4& ui_view_matrix () const
			{
				return widget_view_matrix_;
			}

			const glm::mat4& ui_model_matrix () const
			{
				return widget_model_matrix_;
			}

			void GetWidgetProjectionMatrix (glm::mat4& matrix);

			void SetWidgetProjectionMatrix (const glm::mat4& matrix);

			void PushWidgetProjectionMatrix ();

			void PopWidgetProjectionMatrix ();

			void GetWidgetViewMatrix (glm::mat4& matrix);

			void SetWidgetViewMatrix (const glm::mat4& matrix);

			void PushWidgetViewMatrix ();

			void PopWidgetViewMatrix ();

			void SetWidgetModelMatrix (const glm::mat4& matrix);

			void GetWidgetModelMatrix (glm::mat4& matrix);

			void PushWidgetModelMatrix ();

			void PopWidgetModelMatrix ();

			inline GLint location (LocationType index) const
			{
				return locations_[index];
			}

		private:

			friend class BlendInt::Context;

			static bool Initialize ();

			static void Release ();

			Shaders ();

			~Shaders ();

			bool Setup ();

			bool SetupWidgetProgram ();

			bool SetupWidgetInnerProgram ();

			bool SetupWidgetSplitInnerProgram ();

			bool SetupWidgetOuterProgram ();

			bool SetupTextProgram ();

			bool SetupTriangleProgram ();

			bool SetupImageProgram ();

			bool SetupPrimitiveProgram ();

			RefPtr<GLSLProgram> widget_text_program_;

			RefPtr<GLSLProgram> primitive_program_;

			RefPtr<GLSLProgram> triangle_program_;

			RefPtr<GLSLProgram> widget_program_;

			RefPtr<GLSLProgram> widget_inner_program_;

			RefPtr<GLSLProgram> widget_split_inner_program_;

			RefPtr<GLSLProgram> widget_outer_program_;

			RefPtr<GLSLProgram> widget_image_program_;

			GLint locations_[LocationLast];

			RefPtr<GLBuffer<UNIFORM_BUFFER> > widget_matrix_;

			// the offset of uniform block in shaders
			GLint widget_matrix_offset_[3];

			GLint widget_matrix_block_size_;
			
			GLuint widget_matrix_binding_point_;

			glm::mat4 widget_projection_matrix_;

			glm::mat4 widget_view_matrix_;

			glm::mat4 widget_model_matrix_;

			std::stack<glm::mat4> widget_projection_matrix_stack;

			std::stack<glm::mat4> widget_view_matrix_stack;

			std::stack<glm::mat4> widget_model_matrix_stack;

			static const char* widget_text_vertex_shader;

			static const char* widget_text_fragment_shader;

			static const char* primitive_vertex_shader;

			static const char* primitive_fragment_shader;

			static const char* triangle_vertex_shader;

			static const char* triangle_geometry_shader;

			static const char* triangle_fragment_shader;

			static const char* widget_vertex_shader;

			static const char* widget_geometry_shader;

			static const char* widget_fragment_shader;

			static const char* widget_inner_vertex_shader;

			static const char* widget_inner_geometry_shader;

			static const char* widget_inner_fragment_shader;

			static const char* widget_split_inner_vertex_shader;

			static const char* widget_split_inner_fragment_shader;

			static const char* widget_outer_vertex_shader;

			static const char* widget_outer_geometry_shader;

			static const char* widget_outer_fragment_shader;

			static const char* context_vertex_shader;

			static const char* context_fragment_shader;

			static const char* widget_image_vertex_shader;

			static const char* widget_image_fragment_shader;

			static const char* frame_inner_vertex_shader;

			static const char* frame_inner_fragment_shader;
		};

	}

}

#endif /* _BIL_SHADERMANAGER_HPP_ */
