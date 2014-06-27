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
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Gui/ProgressBar.hpp>

namespace BlendInt {

	ProgressBar::ProgressBar(Orientation orientation)
	: AbstractWidget(),
	  m_orientation(orientation)
	{
		if(orientation == Horizontal) {
			set_size(200, 6);
		} else {
			set_size(6, 200);
		}

		glGenVertexArrays(2, m_vao);
	}

	ProgressBar::~ProgressBar ()
	{
		glDeleteVertexArrays(2, m_vao);
	}
	
	bool ProgressBar::IsExpandX () const
	{
		if(m_orientation == Horizontal) {
			return true;
		} else {
			return false;
		}
	}
	
	bool ProgressBar::IsExpandY () const
	{
		if(m_orientation == Vertical) {
			return true;
		} else {
			return false;
		}
	}
	
	Size ProgressBar::GetPreferredSize () const
	{
		Size prefer;

		return prefer;
	}

	bool ProgressBar::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		return true;
	}

	void ProgressBar::UpdateGeometry (const GeometryUpdateRequest& request)
	{
	}

	void ProgressBar::BroadcastUpdate (const GeometryUpdateRequest& request)
	{
	}

	ResponseType ProgressBar::Draw(const RedrawEvent& event)
	{

		return Accept;
	}

	ResponseType ProgressBar::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ProgressBar::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ProgressBar::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ProgressBar::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

}
