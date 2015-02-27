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

#pragma once

#include <opengl/glarraybuffer.hpp>
#include <opengl/glelementarraybuffer.hpp>

#include <gui/abstract-form.hpp>

namespace BlendInt {

	/**
	 * @brief CheckerBoard form
	 *
	 * A form show a gray checkerboard: light: #999999, dark: #666666
	 */
	class ChessBoard: public AbstractForm
	{
	public:

		explicit ChessBoard (size_t cell_size);

		virtual ~ChessBoard ();

		void SetCellSize (size_t size);

		size_t cell_size () const {return m_cell_size;}

		virtual void Draw (float x, float y) const;

		virtual void DrawInRect (const Rect& rect,
				int align,
				uint32_t color = 0xFFFFFFFF,
				short gamma = 0,
				float rotate = 0.f,
				bool scale = false) const;
	protected:

		virtual void PerformSizeUpdate (const Size& size);

		static void GenerateCheckerVertices (const Size& size,
						size_t cell_size,
						std::vector<GLfloat>* vertices,
						std::vector<GLuint>* light_indices,
						std::vector<GLuint>* dark_indices);


		static void GenerateCheckerVertices (size_t num,
						size_t cell_size,
						std::vector<GLfloat>* vertex,
						std::vector<GLuint>* index);

	private:

		void InitOnce ();

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_vbo;
		RefPtr<GLElementArrayBuffer> m_light_ibo;
		RefPtr<GLElementArrayBuffer> m_dark_ibo;

		int light_elements_;

		int dark_elements_;

		size_t m_cell_size;
	};

}
