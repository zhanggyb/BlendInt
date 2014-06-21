/*
 * Unit test for Timer
 */

#include <Common/Window.hpp>

#include <BlendInt/Core/Timer.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/Button.hpp>

// The fixture for testing class Foo.

using namespace BlendInt;

/*
class AnimatingWidget: public Widget
{
public:
	AnimatingWidget();

protected:

    virtual ~AnimatingWidget();

	virtual void Render ();

private:

	void AddAngle ();

	float m_angle;
	Timer* m_timer;
};

AnimatingWidget::AnimatingWidget ()
	: Widget(), m_angle(0.0), m_timer(0)
{
	m_timer = new Timer;

	events()->connect(m_timer->timeout(), this, &AnimatingWidget::AddAngle);

	set_size(500, 400);

	m_timer->SetInterval(5);
	m_timer->Start();
}

AnimatingWidget::~AnimatingWidget()
{
    if(m_timer)
        delete m_timer;
}

void AnimatingWidget::Render ()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(position().x(), position().y(), z());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render code
	glTranslatef(200.f, 150.f, 0.0f);
	glRotatef(m_angle, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-200.f, 0.0f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(200.f, 0.0f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(000.f, 300.f, 0.f);
	glEnd();

	glDisable(GL_BLEND);

	glPopMatrix();
}

void AnimatingWidget::AddAngle ()
{
	m_angle += 0.5f;
}

*/

/*
class TimerTest: public testing::Test
{

protected:

	TimerTest () {

	}

	virtual ~TimerTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(TimerTest, CheckTimeOut)
{
	Timer timer;

	timer.Start ();

	timer.Stop ();
}
*/

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here

	//AnimatingWidget* widget = new AnimatingWidget;

	//widget->SetPosition(200, 200);

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS();
	return 0;
}
