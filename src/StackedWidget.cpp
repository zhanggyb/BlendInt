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

#include <BlendInt/StackedWidget.hpp>
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
		if(children().count(widget)) return;

		m_stack.push_back(widget);
		bind(widget);

		widget->Resize(size());
	}

	void StackedWidget::Insert (size_t index, Widget* widget)
	{
		if(children().count(widget)) return;
		if(index > (m_stack.size() - 1)) return;

		std::vector<Widget*>::iterator it = m_stack.begin();
		std::advance(it, index);

		m_stack.insert(it, widget);
		bind(widget);

		widget->Resize(size());
	}

	void StackedWidget::Remove (Widget* widget)
	{
		if(! children().count(widget)) return;

		std::vector<Widget*>::iterator it;
		it = std::find(m_stack.begin(), m_stack.end(), widget);

		if(it != m_stack.end()) {
			m_stack.erase(it);
			unbind(widget);

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

	void StackedWidget::Render ()
	{
		if(m_stack.size() == 0) return;

		dispatch_render(m_stack[m_index]);
	}

}
