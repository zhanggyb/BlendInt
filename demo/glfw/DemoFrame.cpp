/*
 * DemoFrame.cpp
 *
 *  Created on: 2013年11月16日
 *      Author: zhanggyb
 */

#include "DemoFrame.hpp"

#include <BlendInt/Label.hpp>
#include <iostream>

DemoFrame::DemoFrame()
: Frame(), m_hlayout(0), m_timer(0)
{
	m_hlayout = new HorizontalLayout(AlignHorizontalCenter, this);
	m_hlayout->set_margin(0, 0, 0, 0);

	Label* label = new Label("Click to create: ");
	m_hlayout->add(label);

	Button* start_button = new Button("Start");
	m_hlayout->add(start_button);

	Button* reset_button = new Button("Reset Interval");
	m_hlayout->add(reset_button);

	Button* stop_button = new Button("Stop");
	m_hlayout->add(stop_button);

	set_size(m_hlayout->size().width() + margin().left() + margin().right(),
				m_hlayout->size().height() + margin().top() + margin().bottom());

	SetWidget(m_hlayout);

	SetPreferredSize(size());

	m_timer = new Timer;

	// connect timer
	events()->connect(start_button->clicked(),this, &DemoFrame::start_time);
	events()->connect(reset_button->clicked(), this, &DemoFrame::reset_time);
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

void DemoFrame::reset_time()
{
	m_timer->SetInterval(2000);
}

void DemoFrame::stop_time()
{
	unsigned int time_left = m_timer->GetTimeLeft();
	std::cout << "time left: (ms)" << time_left << std::endl;

	m_timer->Stop();
}
