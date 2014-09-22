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

#include <BlendInt/Stock/Shaders.hpp>

using BI::Stock::Shaders;

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
	DBG_SET_NAME(vp, "Viewport");

	Frame* f = Manage(new Frame);
	DBG_SET_NAME(f, "Frame");
	Button* btn = Manage(new Button("Button"));
	DBG_SET_NAME(btn, "Button");

	f->Setup(btn);
	f->Resize(200, 100);

	vp->AddContainer(f);

	vp->SetPosition(400, 300);

	f->SetPosition(100, 100);

	AddViewport(vp);

	glm::mat4 model;
	Shaders::instance->GetUIModelMatrix(model);

	Button* btn2 = Manage(new Button("Button2"));
	vp->AddWidget(btn2);
	btn2->SetPosition(200, 300);

	//Point pos = btn->position();
	//DBG_PRINT_MSG("button position: %d, %d", pos.x(), pos.y());

//	Viewport* vp1 = Manage(new Viewport);
//	DBG_SET_NAME(vp1, "Viewport1");
//	Viewport* vp2 = Manage(new Viewport);
//	DBG_SET_NAME(vp2, "Viewport2");
//
//	ViewportSplitter* vs = Manage(new ViewportSplitter);
//	vs->Resize(600, 400);
//	DBG_SET_NAME(vs, "Viewport Splitter");
//
//	vs->AppendViewport(vp1);
//	vs->AppendViewport(vp2);
//
//	AddViewport(vs);
//
//	Button* btn1 = Manage(new Button("Hello1"));
//	vp1->AddWidget(btn1);
//
//	AbstractViewport* view = AbstractViewport::GetViewport(btn1);
//
//	DBG_PRINT_MSG("viewport: %s", view->name().c_str());
//
//	Button* btn2 = Manage(new Button("Hello2"));
//	vp2->AddWidget(btn2);
}
