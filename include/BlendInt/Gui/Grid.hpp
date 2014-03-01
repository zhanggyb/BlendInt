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

#ifndef _BLENDINT_GRID_HPP_
#define _BLENDINT_GRID_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLElementArrayBuffer.hpp>
#include <BlendInt/Gui/AbstractPrimitive.hpp>

namespace BlendInt {

	class Grid: public AbstractPrimitive
	{
	public:

		Grid ();

		virtual ~Grid ();

		void SetSize (int size);

		void Update ();

		virtual void Render (const glm::mat4& MVP);

	private:

		void InitOnce ();

		int m_size;
		int m_step;

		RefPtr<GLArrayBuffer> m_vb;	// vertex buffer
		RefPtr<GLElementArrayBuffer> m_ib;	// index buffer

		GLint m_attribute_coord2d;
		GLint m_uniform_mvp;

		static const char* vertex_shader;

		static const char* fragment_shader;

	};

}



#endif /* _BLENDINT_GRID_HPP_ */
