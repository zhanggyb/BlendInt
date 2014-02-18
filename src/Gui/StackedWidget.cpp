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

#include <BlendInt/Gui/StackedWidget.hpp>
#include <algorithm>

namespace BlendInt {

	StackedWidget::StackedWidget()
	: Widget(), m_index(0)
	{

	}

	StackedWidget::StackedWidget(AbstractWidget* parent)
	: Widget (parent), m_index(0)
	{

	}

	StackedWidget::~StackedWidget()
	{

	}

	void StackedWidget::Add (Widget* widget)
	{
//		if(subordinates()->count(widget)) return;

		m_stack.push_back(widget);
//		Attach(widget);

		widget->Resize(size());
		widget->SetPosition(position());
	}

	void StackedWidget::Insert (size_t index, Widget* widget)
	{
//		if(subordinates()->count(widget)) return;
		if(index > (m_stack.size() - 1)) return;

		std::vector<Widget*>::iterator it = m_stack.begin();
		std::advance(it, index);

		m_stack.insert(it, widget);
//		Attach(widget);

		widget->Resize(size());
		widget->SetPosition(position());
	}

	void StackedWidget::Remove (Widget* widget)
	{
//		if(! subordinates()->count(widget)) return;

		std::vector<Widget*>::iterator it;
		it = std::find(m_stack.begin(), m_stack.end(), widget);

		if(it != m_stack.end()) {
			m_stack.erase(it);
//			Detach(widget);

			if(m_index > (m_stack.size() - 1)) {
				m_index = m_stack.size() - 1;
			}
		}
	}

	void StackedWidget::SetIndex (size_t index)
	{
		if(index > (m_stack.size() - 1)) return;

		m_index = index;
	}

	Widget* StackedWidget::GetActiveWidget () const
	{
		if(m_stack.size() == 0) return 0;

		return m_stack[m_index];
	}

	Widget* StackedWidget::GetWidget (size_t index)
	{
		if(index > (m_stack.size() - 1)) return 0;

		return m_stack[index];
	}

	void StackedWidget::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				const Point* new_pos = static_cast<const Point*>(data);
				std::vector<Widget*>::iterator it;

				for (it = m_stack.begin(); it != m_stack.end(); it++)
				{
					SetPosition(*it,
							(*it)->position().x() + (new_pos->x() - position().x()),
							(*it)->position().y() + (new_pos->y() - position().y()));
				}
				break;
			}

			case FormSize: {
				const Size* new_size = static_cast<const Size*>(data);
				std::vector<Widget*>::iterator it;

				for (it = m_stack.begin(); it != m_stack.end(); it++)
				{
					Resize((*it), *new_size);
				}

				break;
			}

			default:
				break;
		}
	}

	void StackedWidget::Draw ()
	{
		if(m_stack.size() == 0) return;

		DispatchRender(m_stack[m_index]);
	}

}
