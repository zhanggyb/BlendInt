#include "QtGLWindow.hpp"
#include "Qt5Context.hpp"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QMouseEvent>

#include <QtCore/qmath.h>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>

#include <BlendInt/Window/KeyEvent.hpp>
#include <BlendInt/Window/MouseEvent.hpp>

namespace BI = BlendInt;

static BI::KeyEvent kKeyEvent;
static BI::MouseEvent kMouseEvent;

class DemoWindow : public QtGLWindow
{
public:

	DemoWindow();

	virtual ~DemoWindow ();

    void initialize();

    void render();

protected:

    virtual void keyPressEvent (QKeyEvent* ev);

    virtual void keyReleaseEvent (QKeyEvent* ev);

    virtual void resizeEvent (QResizeEvent* ev);

    virtual void mousePressEvent(QMouseEvent* ev);

    virtual void mouseMoveEvent (QMouseEvent* ev);

    virtual void mouseReleaseEvent (QMouseEvent* ev);

private:

    GLuint loadShader(GLenum type, const char *source);

    Qt5Context* m_context;
};

DemoWindow::DemoWindow()
: QtGLWindow(), m_context(0)
{
}

DemoWindow::~DemoWindow()
{
	BI::Interface::Release();
}

int main(int argc, char **argv)
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(16);

    DemoWindow window;
    window.setFormat(format);
    window.resize(1024, 600);
    window.show();

    window.setAnimating(true);

    return app.exec();
}

GLuint DemoWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void DemoWindow::initialize()
{
	/* initialize BlendInt after OpenGL content is created */
	if (!BI::Interface::Initialize()) {
		exit(-1);
	}

	BI::Interface::instance->Resize(1024, 600);

	m_context = Manage (new Qt5Context(this));
	BI::Interface::instance->SetCurrentContext(m_context);
	m_context->Resize(1024, 600);
}

void DemoWindow::render()
{
	BI::Interface::instance->Draw();
}

void DemoWindow::resizeEvent(QResizeEvent* ev)
{
	if(BI::Interface::instance) {
		BI::Interface::instance->Resize(ev->size().width(), ev->size().height());
		ev->accept();
	}
}

void DemoWindow::keyPressEvent(QKeyEvent* ev)
{

}

void DemoWindow::keyReleaseEvent(QKeyEvent* ev)
{

}

void DemoWindow::mouseMoveEvent(QMouseEvent* ev)
{
	kMouseEvent.set_action(BI::MouseMove);
	kMouseEvent.set_button(BI::MouseButtonNone);
	kMouseEvent.set_position(ev->pos().x(), BI::Interface::instance->GetCurrentContextHeight() - ev->pos().y());

	BI::Interface::instance->DispatchMouseEvent(kMouseEvent);
	ev->accept();
}

void DemoWindow::mousePressEvent(QMouseEvent* ev)
{
	BI::MouseAction mouse_action = BI::MousePress;

	BI::MouseButton mouse_button = BI::MouseButtonNone;

	switch(ev->button()) {

		case Qt::LeftButton:
			mouse_button = BI::MouseButtonLeft;
			break;

		case Qt::RightButton:
			mouse_button = BI::MouseButtonRight;
			break;

		case Qt::MiddleButton:
			mouse_button = BI::MouseButtonMiddle;
			break;

		default:
			break;

	}

	kMouseEvent.set_button(mouse_button);
	kMouseEvent.set_action(mouse_action);
	kMouseEvent.set_modifiers(BI::ModifierNone);

	BI::Interface::instance->DispatchMouseEvent(kMouseEvent);
	ev->accept();
}

void DemoWindow::mouseReleaseEvent(QMouseEvent* ev)
{
	BI::MouseAction mouse_action = BI::MouseRelease;

	BI::MouseButton mouse_button = BI::MouseButtonNone;

	switch(ev->button()) {

		case Qt::LeftButton:
			mouse_button = BI::MouseButtonLeft;
			break;

		case Qt::RightButton:
			mouse_button = BI::MouseButtonRight;
			break;

		case Qt::MiddleButton:
			mouse_button = BI::MouseButtonMiddle;
			break;

		default:
			break;

	}

	kMouseEvent.set_button(mouse_button);
	kMouseEvent.set_action(mouse_action);
	kMouseEvent.set_modifiers(BI::ModifierNone);

	BI::Interface::instance->DispatchMouseEvent(kMouseEvent);
	ev->accept();
}
