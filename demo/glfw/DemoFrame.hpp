/*
 * DemoFrame.hpp
 *
 *  Created on: 2013年11月16日
 *      Author: zhanggyb
 */

#ifndef DEMOFRAME_HPP_
#define DEMOFRAME_HPP_

#include <BlendInt/UI/Frame.hpp>
#include <BlendInt/UI/HorizontalLayout.hpp>
#include <BlendInt/UI/Button.hpp>

#include <BlendInt/Timer.hpp>

using namespace BlendInt;

class DemoFrame: public Frame
{
public:

	DemoFrame ();

	virtual ~DemoFrame();

	void SetWidget (AbstractWidget* widget);

	AbstractWidget* widget () const {return m_widget;}

	void FullWindow (unsigned int width, unsigned int height);

protected:

	virtual void Update (int type, const void* data);

	virtual void Render ();

	virtual void KeyPressEvent (KeyEvent* event);

	virtual void ContextMenuPressEvent (ContextMenuEvent* event);

	virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

	virtual void MousePressEvent (MouseEvent* event);

	virtual void MouseReleaseEvent (MouseEvent* event);

	virtual void MouseMoveEvent (MouseEvent* event);

private:

	void start_time ();

	void reset_time ();

	void stop_time ();

	HorizontalLayout* m_hlayout;

	Timer* m_timer;

	AbstractWidget* m_widget;
};

#endif /* DEMOFRAME_HPP_ */
