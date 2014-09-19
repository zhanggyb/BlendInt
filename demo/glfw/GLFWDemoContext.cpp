/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/CheckButton.hpp>
#include <BlendInt/Gui/Viewport.hpp>
#include <BlendInt/Gui/ViewportSplitter.hpp>

GLFWDemoContext::GLFWDemoContext()
: BI::Context()
{
	Initialize ();
}

GLFWDemoContext::~GLFWDemoContext ()
{
}

void GLFWDemoContext::Initialize ()
{
	using namespace BI;

	Viewport* vp = Manage(new Viewport);
	Button* btn = Manage(new Button("Button"));
	vp->AddWidget(btn);

	vp->SetPosition(400, 300);

	AddViewport(vp);

	/*
	Viewport* vp1 = Manage(new Viewport);
	DBG_SET_NAME(vp1, "Viewport1");
	Viewport* vp2 = Manage(new Viewport);
	DBG_SET_NAME(vp2, "Viewport2");

	ViewportSplitter* vs = Manage(new ViewportSplitter);
	vs->Resize(600, 400);
	DBG_SET_NAME(vs, "Viewport Splitter");

	vs->AppendViewport(vp1);
	vs->AppendViewport(vp2);

	AddViewport(vs);

	Button* btn1 = Manage(new Button("Hello1"));
	vp1->AddWidget(btn1);

	AbstractViewport* view = AbstractViewport::GetViewport(btn1);

	DBG_PRINT_MSG("viewport: %s", view->name().c_str());

	Button* btn2 = Manage(new Button("Hello2"));
	vp2->AddWidget(btn2);
	*/
}
