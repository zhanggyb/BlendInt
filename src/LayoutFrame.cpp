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

#include <BlendInt/LayoutFrame.hpp>

namespace BlendInt {


	LayoutFrame::LayoutFrame ()
	: Frame(), m_layout(0)
	{
	}

	LayoutFrame::LayoutFrame (AbstractWidget* parent)
	: Frame(parent), m_layout(0)
	{
	}

	void LayoutFrame::SetLayout (AbstractLayout* layout)
	{
		if(m_layout) delete m_layout;

		m_layout = layout;

		bind(m_layout);

		unsigned int width = size().width() - margin().left() - margin().right();
		unsigned int height = size().height() - margin().top() - margin().bottom();

		m_layout->Resize (width, height);

		m_layout->SetPosition(position().x() + margin().left(), position().y() + margin().bottom());

		LockGeometry(m_layout, true);
	}

	void LayoutFrame::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				if(m_layout) {
					const Point* new_pos = static_cast<const Point*>(data);

					SetPosition(m_layout,
							m_layout->position().x() + (new_pos->x() - position().x()),
							m_layout->position().y() + (new_pos->y() - position().y()));

				}
				break;
			}

			case FormSize: {
				if(m_layout) {
					const Size* new_size = static_cast<const Size*>(data);
					unsigned int width = new_size->width() - margin().left() - margin().right();
					unsigned int height = new_size->height() - margin().top() - margin().bottom();
					Resize(m_layout, width, height);
				}
				break;
			}

			default:
				break;
		}
	}

	void LayoutFrame::Render ()
	{
		if(m_layout)
			dispatch_render(m_layout);
	}

	void LayoutFrame::KeyPressEvent (KeyEvent* event)
	{
		if(m_layout)
			dispatch_key_press_event(m_layout, event);
	}

	void LayoutFrame::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void LayoutFrame::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void LayoutFrame::MousePressEvent (MouseEvent* event)
	{
		if(m_layout)
			dispatch_mouse_press_event(m_layout, event);
	}

	void LayoutFrame::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_layout)
			dispatch_mouse_release_event(m_layout, event);
	}

	void LayoutFrame::MouseMoveEvent (MouseEvent* event)
	{
		if(m_layout)
			dispatch_mouse_release_event(m_layout, event);
	}

}
