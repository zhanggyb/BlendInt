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

	SpaceArea::SpaceArea ()
	: AbstractWidget(),
	  m_expand_x(false),
	  m_expand_y(false),
	  m_widget_attached(0)
	{
		set_size(1, 1);
	}

	SpaceArea::~SpaceArea ()
	{
	}

	void SpaceArea::AttachWidget (AbstractWidget* widget)
	{
		if(m_widget_attached) {
			m_widget_attached->destroyed().disconnectOne(this, &SpaceArea::OnWidgetDestroyed);
		}

		m_widget_attached = widget;
		if(m_widget_attached) {
			events()->connect(m_widget_attached->destroyed(), this, &SpaceArea::OnWidgetDestroyed);

			m_expand_x = m_widget_attached->IsExpandX();
			m_expand_y = m_widget_attached->IsExpandY();
		}
	}

	Size SpaceArea::GetPreferredSize() const
	{
		if(m_widget_attached) {
			return m_widget_attached->GetPreferredSize();
		}

		Size preferred_size(1, 1);

		return preferred_size;
	}

	void SpaceArea::SetExpandX (bool expand)
	{
		if(!m_widget_attached) {
			m_expand_x = expand;
		}
	}

	void SpaceArea::SetExpandY (bool expand)
	{
		if(!m_widget_attached) {
			m_expand_y = expand;
		}
	}

	void SpaceArea::SetExpand (bool expand_x, bool expand_y)
	{
		if(!m_widget_attached) {
			m_expand_x = expand_x;
			m_expand_y = expand_y;
		}
	}

	bool SpaceArea::IsExpandX() const
	{
		return m_expand_x;
	}

	bool SpaceArea::IsExpandY() const
	{
		return m_expand_y;
	}

	void SpaceArea::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(m_widget_attached) {

			switch(request.type()) {	// don't care the source

				case WidgetPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());
					m_widget_attached->SetPosition(*pos_p);
					break;
				}

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					m_widget_attached->Resize(*size_p);
					break;
				}

				default:
					break;
			}

		}
	}
	
	ResponseType SpaceArea::Draw (const RedrawEvent& event)
	{
		return Ignore;
	}

	bool SpaceArea::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		return true;
	}

	void SpaceArea::BroadcastUpdate (const WidgetUpdateRequest& request)
	{
	}

	ResponseType SpaceArea::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType SpaceArea::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType SpaceArea::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType SpaceArea::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType SpaceArea::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType SpaceArea::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType SpaceArea::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType SpaceArea::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void SpaceArea::OnWidgetDestroyed (AbstractWidget* widget)
	{
		if(m_widget_attached == widget) {
			widget->destroyed().disconnectOne(this, &SpaceArea::OnWidgetDestroyed);
			m_widget_attached = 0;
		}
	}

}
