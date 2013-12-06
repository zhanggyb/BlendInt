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
: Frame(), m_hlayout(0), m_timer(0), m_widget(0)
{
	m_hlayout = new HorizontalLayout(AlignHorizontalCenter, this);
	m_hlayout->set_margin(0, 0, 0, 0);

	Label* label = new Label("Click to create: ");
	m_hlayout->Add(label);

	Button* start_button = new Button("Start");
	m_hlayout->Add(start_button);

	Button* reset_button = new Button("Reset Interval");
	m_hlayout->Add(reset_button);

	Button* stop_button = new Button("Stop");
	m_hlayout->Add(stop_button);

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


void DemoFrame::SetWidget (AbstractWidget* widget)
{
	if (widget && (widget != m_widget)) {
		delete m_widget;
		m_widget = widget;

		m_widget->SetPosition(position().x() + margin().left(),
		        position().y() + margin().bottom());
		m_widget->Resize(
		        size().width() - margin().left() - margin().right(),
		        size().height() - margin().top() - margin().bottom());

		bind(m_widget);
	}
}

void DemoFrame::FullWindow(unsigned int width, unsigned int height)
{
	std::cout << "Get FullWindow event: " << width << " " << height << std::endl;
	Resize(width, height);
}

void DemoFrame::Update(int type, const void* data)
{
	switch (type) {

		case FormPosition: {
			if (m_widget) {
				const Point* pos_p = static_cast<const Point*>(data);
				int offset_x = pos_p->x() - position().x();
				int offset_y = pos_p->y() - position().y();
				SetPosition(m_widget, m_widget->position().x() + offset_x,
				        m_widget->position().y() + offset_y);
			}
			break;
		}

		case FormSize: {
			if (m_widget) {
				Size size = *(static_cast<const Size*>(data));
				size.add_width(-(margin().left() + margin().right()));
				size.add_height(-(margin().top() + margin().bottom()));
				m_widget->Resize(size);
			}
			break;
		}

		case FrameMargin: {
			if (m_widget) {
				const Margin* margin_p = static_cast<const Margin*>(data);
				Size new_size(
				        size().width() - margin_p->left()
				                - margin_p->right(),
				        size().height() - margin_p->top()
				                - margin_p->bottom());
				m_widget->Resize(new_size);
			}
			break;
		}

		default:
			Widget::Update(type, data);
			break;
	}
}

void DemoFrame::Render ()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(position().x(), position().y(), z());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG
	glLineWidth(1);
	glEnable(GL_LINE_STIPPLE);

	glColor4f(1.0f, 0.0f, 0.0f, 0.45f);
	glLineStipple(1, 0xAAAA);
	glBegin(GL_LINE_LOOP);
	glVertex2i(0, 0);
	glVertex2i(size().width(), 0);
	glVertex2i(size().width(), size().height());
	glVertex2i(0, size().height());
	glEnd();

	glDisable(GL_LINE_STIPPLE);
#endif

	glDisable(GL_BLEND);

	glPopMatrix();

	if(m_widget)
		dispatch_render(m_widget);
}

void DemoFrame::start_time()
{
	m_timer->Start();
}

void DemoFrame::reset_time()
{
	m_timer->SetInterval(2000);
}

void DemoFrame::KeyPressEvent(KeyEvent* event) {
	if(m_widget)
		dispatch_key_press_event(m_widget, event);
}

void DemoFrame::ContextMenuPressEvent(ContextMenuEvent* event) {
}

void DemoFrame::ContextMenuReleaseEvent(ContextMenuEvent* event) {
}

void DemoFrame::MousePressEvent(MouseEvent* event) {
	if(m_widget)
		dispatch_mouse_press_event(m_widget, event);
}

void DemoFrame::MouseReleaseEvent(MouseEvent* event) {
	if(m_widget)
		dispatch_mouse_release_event(m_widget, event);
}

void DemoFrame::MouseMoveEvent(MouseEvent* event) {
	if(m_widget)
		dispatch_mouse_move_event(m_widget, event);
}

void DemoFrame::stop_time()
{
	std::cout << "Stop timer" << std::endl;

	m_timer->Stop();
}
