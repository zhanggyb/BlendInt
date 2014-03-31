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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <BlendInt/OpenGL/ScissorStatus.hpp>

namespace BlendInt {

	ScissorStatus::ScissorStatus ()
	{
	}

	ScissorStatus::~ScissorStatus ()
	{
	}

	void ScissorStatus::Push (const Rect& area)
	{
		m_area_stack.push(area);
	}

	void ScissorStatus::Pop ()
	{
		m_area_stack.pop();
	}

	void ScissorStatus::Push (int x, int y, unsigned int w,
	        unsigned int h)
	{
		m_area_stack.push(Rect(x, y, w, h));
	}

	void ScissorStatus::Enable ()
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_area_stack.top().x(),
				m_area_stack.top().y(),
				m_area_stack.top().width(),
				m_area_stack.top().height());
	}

	void ScissorStatus::Disable()
	{
		if(m_area_stack.empty())
			glDisable(GL_SCISSOR_TEST);
	}

}

