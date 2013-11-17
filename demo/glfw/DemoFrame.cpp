/*
 * DemoFrame.cpp
 *
 *  Created on: 2013年11月16日
 *      Author: zhanggyb
 */

#include "DemoFrame.hpp"

#include <BlendInt/Label.hpp>

DemoFrame::DemoFrame()
: Frame(), m_hlayout(0)
{
	m_hlayout = new HorizontalLayout(AlignHorizontalCenter, this);
	m_hlayout->set_margin(0, 0, 0, 0);

	Label* label = new Label("Click to create: ");
	m_hlayout->add(label);

	Button* button = new Button("Button");
	m_hlayout->add(button);

	button = new Button("ScrollView");
	m_hlayout->add(button);

	preset_size(m_hlayout->size().width() + margin().left() + margin().right(),
				m_hlayout->size().height() + margin().top() + margin().bottom());

	set_widget(m_hlayout);

	set_preferred_size(size());
}

DemoFrame::~DemoFrame()
{

}
