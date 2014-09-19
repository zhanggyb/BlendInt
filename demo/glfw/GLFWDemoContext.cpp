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
	vp->SetPosition(200, 100);
	vp->SetPosition(200, 200);

	AddViewport(vp);

	Button* widget = Manage(new Button("Hello World!"));

	vp->AddWidget(widget);

	Point pos = widget->GetGlobalPosition ();

	DBG_PRINT_MSG("widget position: %d, %d", pos.x(), pos.y());

	DBG_PRINT_MSG("viewport: %d, %d, %d, %d", vp->position().x(), vp->position().y(), vp->size().width(), vp->size().height());

	vp->SetPosition(300, 200);

//	Viewport* vp2 = Manage(new Viewport);
//
//	vp->SetPosition(350, 300);
//
//	AddViewport(vp2);

	ViewportSplitterHandle* vp_sp = Manage(new ViewportSplitterHandle);

	Button* btn2 = Manage(new Button("SplitterHandle"));

	vp_sp->SetHandleWidget(btn2);
	vp_sp->Resize(vp_sp->GetPreferredSize());

	AddViewport(vp_sp);

	/*
	Workspace* ws = Manage(new Workspace);
	DBG_SET_NAME(ws, "Workspace");
	ws->SetPosition(200, 100);

	Append(ws);

	Viewport3D* view = Manage(new Viewport3D);
	ws->SetViewport(view);

	ToolBar* tb = Manage(new ToolBar);
	ws->SetHeader(tb);

	ToolBox* box1 = Manage(new ToolBox);
	ws->SetLeftSideBar(box1);

	ToolBox* box2 = Manage(new ToolBox);
	ws->SetRightSideBar(box2);

	StaticPanel* panel = Manage(new StaticPanel);
	DBG_SET_NAME(panel, "Static Panel");
	panel->SetPosition(1020, 200);
	panel->SetSpace(0);

	Decoration* dec = Manage(new Decoration);
	DBG_SET_NAME(dec, "Decoration");
	dec->SetRoundType(RoundTopLeft | RoundTopRight);

	ColorSelector* cs = Manage(new ColorSelector);
	DBG_SET_NAME(cs, "ColorSelector");
	cs->SetRoundType(RoundBottomLeft | RoundBottomRight);

	panel->SetDecoration(dec);
	panel->SetContent(cs);

	panel->Resize(panel->GetPreferredSize());

	Append(panel);
	*/

	/*
    Tab* tab = Manage (new Tab);
    tab->SetPosition(100, 100);

    Button* btn1 = Manage (new Button("Button1"));
    btn1->SetRoundType(RoundNone);

    Button* btn2 = Manage (new Button("Button2"));
    btn2->SetRoundType(RoundNone);

    tab->Add(String("Title1"), btn1);
    tab->Resize(400, 300);

    tab->Add(String("Title2"), btn2);

    PushBack(tab);
    */

}
