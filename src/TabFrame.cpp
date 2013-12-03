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

#include <BlendInt/TabFrame.hpp>
#include <BlendInt/String.hpp>
#include <BlendInt/Button.hpp>

namespace BlendInt {

	TabFrame::TabFrame()
	: Frame(), m_stack_widget(0), m_button_layout(0), m_body_layout(0)
	{
		Init();
	}

	TabFrame::TabFrame(AbstractWidget* parent)
	: Frame(parent), m_stack_widget(0), m_button_layout(0), m_body_layout(0)
	{
		Init();
	}

	TabFrame::~TabFrame()
	{

	}

	void TabFrame::Add (const String& title, Widget* widget)
	{
		Button* button = new Button(title);
		m_button_layout->add(button);

		m_stack_widget->Add(widget);

		//events()->connect(button->clicked(), this, &TabFrame::Switch);
	}

	void TabFrame::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				const Point* new_pos = static_cast<const Point*>(data);

				SetPosition(m_body_layout,
						m_body_layout->position().x() + (new_pos->x() - position().x()),
						m_body_layout->position().y() + (new_pos->y() - position().y()));

				break;
			}

			case FormSize: {
				const Size* new_size = static_cast<const Size*>(data);

				Resize(m_body_layout, *new_size);

				break;
			}

			default:
				break;
		}
	}

	void TabFrame::Render ()
	{
		dispatch_render(m_body_layout);
	}

	void TabFrame::KeyPressEvent (KeyEvent* event)
	{
		dispatch_key_press_event(m_body_layout, event);
	}

	void TabFrame::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void TabFrame::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void TabFrame::MousePressEvent (MouseEvent* event)
	{
		dispatch_mouse_press_event(m_body_layout, event);
	}

	void TabFrame::MouseReleaseEvent (MouseEvent* event)
	{
		dispatch_mouse_release_event(m_body_layout, event);
	}

	void TabFrame::MouseMoveEvent (MouseEvent* event)
	{
		dispatch_mouse_move_event(m_body_layout, event);
	}

	void TabFrame::Switch()
	{
		std::cout << "Button clicked" << std::endl;
	}

	void TabFrame::Init ()
	{
		m_stack_widget = new StackedWidget;
		m_button_layout = new HorizontalLayout;

		m_body_layout = new VerticalLayout;
		m_body_layout->add(m_button_layout);
		m_body_layout->add(m_stack_widget);
		bind(m_body_layout);

		set_size(400 + margin().left() + margin().right(), 300 + margin().top() + margin().bottom());
		m_body_layout->Resize(size());
		m_body_layout->SetPosition(position().x() + margin().left(), position().y() + margin().bottom());

		LockGeometry(m_body_layout, true);
	}

}
