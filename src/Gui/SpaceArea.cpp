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

#include <BlendInt/Gui/SpaceArea.hpp>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

namespace BlendInt {

	SpaceArea::SpaceArea (Orientation orientation)
	: Widget(), m_orientation(orientation)
	{
		set_size(20, 20);

		m_shadow.reset(new Shadow);
		m_shadow->Resize(20, 20);
	}

	SpaceArea::~SpaceArea ()
	{
	}

	void SpaceArea::SetOrientation (Orientation orientation)
	{
		m_orientation = orientation;
	}

	Size SpaceArea::GetPreferredSize() const
	{
		Size preferred_size(1, 1);

		if(m_orientation == Horizontal) {
			preferred_size.set_width(100);
		} else {
			preferred_size.set_height(100);
		}

		return preferred_size;
	}

	bool SpaceArea::IsExpandX() const
	{
		if(m_orientation == Horizontal) {
			return true;
		} else {
			return false;
		}
	}

	bool SpaceArea::IsExpandY() const
	{
		if(m_orientation == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	void SpaceArea::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				m_shadow->Resize(*size_p);
				break;
			}

			default:
				Widget::UpdateGeometry(request);
		}

	}

	ResponseType SpaceArea::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		if(m_shadow)
			m_shadow->Draw(mvp);

		return Ignore;
	}

}
