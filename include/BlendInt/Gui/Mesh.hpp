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

#ifndef _BLENDINT_MESH_HPP_
#define _BLENDINT_MESH_HPP_

#include <vector>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLElementArrayBuffer.hpp>

#include <BlendInt/Gui/AbstractPrimitive.hpp>

namespace BlendInt {

	class Mesh: public AbstractPrimitive
	{
	public:

		Mesh ();

		virtual ~Mesh();

		bool Load (const char* filename);

		virtual void Render (const glm::mat4& projection_matrix, const glm::mat4& view_matrix);

		static bool LoadObj (const char* filename, std::vector<glm::vec4>& vertices,
				std::vector<glm::vec3>& normals, std::vector<GLushort>& elements);

	private:

		void InitializeMesh ();

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_vertex_buffer;
		RefPtr<GLArrayBuffer> m_normal_buffer;

		RefPtr<GLElementArrayBuffer> m_index_buffer;

		RefPtr<GLSLProgram> m_program;

		static const char* vertex_shader;
		static const char* fragment_shader;
	};

}

#endif /* _BLENDINT_MESH_HPP_ */
