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

namespace BlendInt {

	TabFrame::TabFrame()
	: Frame(), m_stack_widget(0), m_layout(0), m_vlayout(0)
	{
		Init();
	}

	TabFrame::TabFrame(AbstractWidget* parent)
	: Frame(parent), m_stack_widget(0), m_layout(0), m_vlayout(0)
	{
		Init();
	}

	TabFrame::~TabFrame()
	{

	}

	void TabFrame::Add (const String& title, Widget* widget)
	{
		Button* button = new Button(title);
		m_layout->add(button);

		m_stack_widget->Add(widget);
	}

	void TabFrame::Init ()
	{
		m_stack_widget = new StackedWidget;
		m_layout = new HorizontalLayout;

		m_vlayout = new VerticalLayout;
		m_vlayout->add(m_layout);
		m_vlayout->add(m_stack_widget);
		bind(m_vlayout);

		set_size(400 + margin().left() + margin().right(), 300 + margin().top() + margin().bottom());
		m_vlayout->Resize(size());
		m_vlayout->SetPosition(position().x() + margin().left(), position().y() + margin().bottom());

		LockGeometry(m_vlayout, true);
	}

}
