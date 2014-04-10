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

#include <BlendInt/Gui/TabButton.hpp>

#include <BlendInt/Gui/Tab.hpp>

namespace BlendInt {

	Tab::Tab ()
	: m_title_height(12), m_stack(0)
	{
		set_preferred_size(400, 300);
		set_size(400, 300);

		m_stack = Manage(new Stack);
		m_stack->SetMargin(0, 0, 0, 0);
		AppendSubWidget(m_stack);
	}

	Tab::~Tab ()
	{
	}

	void Tab::Add (const String& title, AbstractWidget* widget)
	{
		TabButton* btn = Manage(new TabButton);
		btn->SetText(title);

		AppendSubWidget(btn);
		m_buttons.push_back(btn);

		m_stack->Add(widget);

		AdjustGeometries();
	}

	bool Tab::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);
					return true;
				}

				case FormSize: {
					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType Tab::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType Tab::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Tab::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	void Tab::AdjustGeometries()
	{
		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();

		int btn_y = position().y() + size().height() - margin().top() - m_title_height;

		unsigned int w = size().width() - margin().left() - margin().right();
		unsigned int h = size().height() - margin().top() - margin().bottom();

		unsigned int temp = 0;
		for(std::deque<AbstractButton*>::iterator it = m_buttons.begin(); it != m_buttons.end(); it++)
		{
			SetPosition((*it), x + temp, btn_y);
			Resize((*it), (*it)->size().width(), m_title_height);
			temp += (*it)->size().width();
		}

		SetPosition(m_stack, x, y);
		Resize(m_stack, w, h - m_title_height);
	}

}
