/*
 * DemoFrame.cpp
 *
 *  Created on: 2013年11月16日
 *      Author: zhanggyb
 */

#include "DemoFrame.hpp"

#include <BlendInt/Label.hpp>

DemoFrame::DemoFrame()
: Frame(), m_hlayout(0), m_timer(0)
{
	m_hlayout = new HorizontalLayout(AlignHorizontalCenter, this);
	m_hlayout->set_margin(0, 0, 0, 0);

	Label* label = new Label("Click to create: ");
	m_hlayout->add(label);

	Button* start_button = new Button("Start");
	m_hlayout->add(start_button);

	Button* stop_button = new Button("Stop");
	m_hlayout->add(stop_button);

	set_size(m_hlayout->size().width() + margin().left() + margin().right(),
				m_hlayout->size().height() + margin().top() + margin().bottom());

	set_widget(m_hlayout);

	SetPreferredSize(size());

	m_timer = new Timer;
//	m_timer->Start();

	// connect timer
	events()->connect(start_button->clicked(),this, &DemoFrame::start_time);
	events()->connect(stop_button->clicked(), this, &DemoFrame::stop_time);
}

DemoFrame::~DemoFrame()
{
	if(m_timer)
		delete m_timer;
}

void DemoFrame::start_time()
{
	m_timer->Start();
}

void DemoFrame::stop_time()
{
	m_timer->Stop();
}
