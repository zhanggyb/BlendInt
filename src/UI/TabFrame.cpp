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

#include <BlendInt/Core/String.hpp>
#include <BlendInt/UI/TabFrame.hpp>
#include <BlendInt/UI/Button.hpp>

namespace BlendInt {

	TabFrame::TabFrame()
	: BoxFrame(), m_stack_widget(0), m_button_box(0)
	{
		Init();
	}

	TabFrame::TabFrame(AbstractWidget* parent)
	: BoxFrame(parent), m_stack_widget(0), m_button_box(0)
	{
		Init();
	}

	TabFrame::~TabFrame()
	{

	}

	void TabFrame::Add (const String& title, Widget* widget)
	{
		Button* button = new Button(title);
		m_button_box->Add(button);

		m_stack_widget->Add(widget);

		//events()->connect(button->clicked(), this, &TabFrame::Switch);
	}

//	void TabFrame::Update (int type, const void* data)
//	{
//		switch (type) {
//
//			case FormPosition: {
//				const Point* new_pos = static_cast<const Point*>(data);
//
//				SetPosition(m_body_layout,
//						m_body_layout->position().x() + (new_pos->x() - position().x()),
//						m_body_layout->position().y() + (new_pos->y() - position().y()));
//
//				break;
//			}
//
//			case FormSize: {
//				const Size* new_size = static_cast<const Size*>(data);
//
//				Resize(m_body_layout, *new_size);
//
//				break;
//			}
//
//			default:
//				break;
//		}
//	}


	void TabFrame::Switch()
	{
		std::cout << "Button clicked" << std::endl;
	}

	void TabFrame::Init ()
	{
		m_stack_widget = new StackedWidget;
		m_button_box = new ButtonBox;

		VerticalLayout* body = new VerticalLayout;
		body->Add(m_stack_widget);
		body->Add(m_button_box);

		set_size(400 + margin().left() + margin().right(), 300 + margin().top() + margin().bottom());
		//body->Resize(size());
		//body->SetPosition(position().x() + margin().left(), position().y() + margin().bottom());

		//LockGeometry(body, true);
		SetLayout(body);
	}

}
