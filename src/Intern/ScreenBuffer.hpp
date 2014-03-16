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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_SCREENBUFFER_HPP_
#define _BLENDINT_SCREENBUFFER_HPP_

#include <vector>

#include <glm/mat4x4.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Size.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BlendInt {

	class Interface;
	class ContextManager;

	struct WidgetsLayerBuffer {
		int layer;
		GLTexture2D* texture_buffer;
	};

	/**
	 * @brief A special class used in Interface only to work as a screen buffer
	 *
	 */
	class ScreenBuffer: public Object
	{
	private:

		friend class Interface;
		friend class ContextManager;

		void Render (const glm::mat4& mvp, GLTexture2D* texture);

		ScreenBuffer ();

		~ScreenBuffer ();

		void InitOnce ();

		void Resize (float width, float height, float depth = 0.0);

		void SaveToFile (const char* filename);

		RefPtr<GLSLProgram> m_program;

		RefPtr<GLArrayBuffer> m_vbo;

		GLuint m_vao;

		GLint uniform_texture;
		GLint attribute_coord3d;
		GLint attribute_texcoord;

		std::vector<WidgetsLayerBuffer> m_widgets_layer_buffers;

		static unsigned int max_widgets_layer_buffer_size;

		static const char* vertex_shader;
		static const char* fragment_shader;

	};

}

#endif /* _BLENDINT_SCREENBUFFER_HPP_ */
