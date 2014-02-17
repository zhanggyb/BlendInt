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

#include <BlendInt/AbstractPrimitive.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLElementArrayBuffer.hpp>

namespace BlendInt {

	class Mesh: public AbstractPrimitive
	{
	public:

		Mesh ();

		bool LoadObj (const char* filename);

		virtual void Render (const glm::mat4& mvp);



	protected:

		virtual ~Mesh();

	private:

		void InitOnce ();

		GLArrayBuffer* m_vb;

		GLElementArrayBuffer* m_ib;

		std::vector<glm::vec4> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<GLushort> m_elements;
	};

}

#endif /* _BLENDINT_MESH_HPP_ */
