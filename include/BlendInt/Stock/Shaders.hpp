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

	class Interface;
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

			// Text
			TEXT_COORD,
			//TEXT_PROJECTION,
			//TEXT_VIEW,
			TEXT_POSITION,
			TEXT_ROTATION,
			TEXT_TEXTURE,
			TEXT_COLOR,

			// Image
			IMAGE_COORD,
			IMAGE_UV,
			//IMAGE_PROJECTION,
			//IMAGE_VIEW,
			IMAGE_POSITION,
			IMAGE_ROTATION,
			IMAGE_TEXTURE,
			IMAGE_GAMMA,

			PRIMITIVE_COORD,
			PRIMITIVE_COLOR,
			PRIMITIVE_PROJECTION,
			PRIMITIVE_VIEW,
			PRIMITIVE_MODEL,

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

			const RefPtr<GLSLProgram>& text_program () const
			{
				return text_program_;
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

			const RefPtr<GLSLProgram>& image_program () const
			{
				return image_program_;
			}

			const glm::mat4& ui_projection_matrix () const
			{
				return ui_projection_matrix_;
			}

			const glm::mat4& ui_view_matrix () const
			{
				return ui_view_matrix_;
			}

			const glm::mat4& ui_model_matrix () const
			{
				return ui_model_matrix_;
			}

			void GetUIProjectionMatrix (glm::mat4& matrix);

			void SetUIProjectionMatrix (const glm::mat4& matrix);

			void PushUIProjectionMatrix ();

			void PopUIProjectionMatrix ();

			void GetUIViewMatrix (glm::mat4& matrix);

			void SetUIViewMatrix (const glm::mat4& matrix);

			void PushUIViewMatrix ();

			void PopUIViewMatrix ();

			void SetUIModelMatrix (const glm::mat4& matrix);

			void GetUIModelMatrix (glm::mat4& matrix);

			void PushUIModelMatrix ();

			void PopUIModelMatrix ();

			inline GLint location (LocationType index) const
			{
				return locations_[index];
			}

		private:

			friend class BlendInt::Interface;
			friend class BlendInt::Context;

			static bool Initialize ();

			static void Release ();

			Shaders ();

			~Shaders ();

			bool Setup ();

			bool SetupWidgetProgram ();

			bool SetupTextProgram ();

			bool SetupTriangleProgram ();

			bool SetupImageProgram ();

			bool SetupPrimitiveProgram ();

			RefPtr<GLSLProgram> text_program_;

			RefPtr<GLSLProgram> primitive_program_;

			RefPtr<GLSLProgram> triangle_program_;

			RefPtr<GLSLProgram> widget_program_;

			RefPtr<GLSLProgram> image_program_;

			GLint locations_[LocationLast];

			RefPtr<GLBuffer<UNIFORM_BUFFER> > ui_matrix_;

			// the offset of uniform block in shaders
			GLint ui_matrix_offset_[3];

			GLint ui_matrix_block_size_;
			
			GLuint ui_matrix_binding_point_;

			glm::mat4 ui_projection_matrix_;

			glm::mat4 ui_view_matrix_;

			glm::mat4 ui_model_matrix_;

			std::stack<glm::mat4> ui_projection_matrix_stack;

			std::stack<glm::mat4> ui_view_matrix_stack;

			std::stack<glm::mat4> ui_model_matrix_stack;

			static const char* text_vertex_shader;

			static const char* text_fragment_shader;

			static const char* primitive_vertex_shader;

			static const char* primitive_fragment_shader;

			static const char* triangle_vertex_shader;

			static const char* triangle_geometry_shader;

			static const char* triangle_fragment_shader;

			static const char* widget_vertex_shader_ext;

			static const char* widget_geometry_shader_ext;

			static const char* widget_fragment_shader_ext;

			static const char* context_vertex_shader;

			static const char* context_fragment_shader;

			static const char* image_vertex_shader;

			static const char* image_fragment_shader;
		};

	}

}

#endif /* _BIL_SHADERMANAGER_HPP_ */
