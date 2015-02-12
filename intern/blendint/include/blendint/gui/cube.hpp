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

#ifndef _BLENDINT_CUBE_HPP_
#define _BLENDINT_CUBE_HPP_

#include <gui/abstract-primitive.hpp>

#include <glm/glm.hpp>

namespace BlendInt {

	class Cube: public AbstractPrimitive
	{
	public:

		Cube ();

		virtual ~Cube ();

		virtual void Render (const glm::mat4& projection_matrix,
				const glm::mat4& view_matrix);

	private:

		void InitializeCube ();

		GLuint m_vao;

		GLuint m_vbo_cube_vertices;
		GLuint m_vbo_cube_colors;
		GLuint m_ibo_cube_elements;

		glm::mat4 mvp;
	};

}

#endif /* _BLENDINT_CUBE_HPP_ */
