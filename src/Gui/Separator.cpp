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

#include <BlendInt/Gui/Separator.hpp>

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

	Separator::Separator ()
	: AbstractWidget(),
	  m_expand_x(false),
	  m_expand_y(false),
	  m_widget_attached(0)
	{
		set_size(1, 1);
	}

	Separator::~Separator ()
	{
	}

	void Separator::AttachWidget (AbstractWidget* widget)
	{
		if(m_widget_attached) {
			m_widget_attached->destroyed().disconnectOne(this, &Separator::OnWidgetDestroyed);
		}

		m_widget_attached = widget;
		if(m_widget_attached) {
			events()->connect(m_widget_attached->destroyed(), this, &Separator::OnWidgetDestroyed);

			m_expand_x = m_widget_attached->IsExpandX();
			m_expand_y = m_widget_attached->IsExpandY();
		}
	}

	Size Separator::GetPreferredSize() const
	{
		if(m_widget_attached) {
			return m_widget_attached->GetPreferredSize();
		}

		Size preferred_size(1, 1);

		return preferred_size;
	}

	void Separator::SetExpandX (bool expand)
	{
		if(!m_widget_attached) {
			m_expand_x = expand;
		}
	}

	void Separator::SetExpandY (bool expand)
	{
		if(!m_widget_attached) {
			m_expand_y = expand;
		}
	}

	void Separator::SetExpand (bool expand_x, bool expand_y)
	{
		if(!m_widget_attached) {
			m_expand_x = expand_x;
			m_expand_y = expand_y;
		}
	}

	bool Separator::IsExpandX() const
	{
		return m_expand_x;
	}

	bool Separator::IsExpandY() const
	{
		return m_expand_y;
	}

	void Separator::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			if(m_widget_attached)
				m_widget_attached->SetPosition(*request.position());

		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void Separator::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			if(m_widget_attached)
				m_widget_attached->Resize(*request.size());

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Separator::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType Separator::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Separator::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Separator::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Separator::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Separator::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Separator::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Separator::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Separator::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Separator::OnWidgetDestroyed (AbstractWidget* widget)
	{
		if(m_widget_attached == widget) {
			widget->destroyed().disconnectOne(this, &Separator::OnWidgetDestroyed);
			m_widget_attached = 0;
		}
	}

}
