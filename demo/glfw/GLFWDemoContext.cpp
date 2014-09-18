/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/CheckButton.hpp>
#include <BlendInt/Gui/Viewport.hpp>

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

	Widget* widget = Manage(new Widget);

	vp->AddWidget(widget);

	Point pos = widget->GetGlobalPosition ();

	DBG_PRINT_MSG("widget position: %d, %d", pos.x(), pos.y());

	DBG_PRINT_MSG("viewport: %d, %d, %d, %d", vp->position().x(), vp->position().y(), vp->size().width(), vp->size().height());

	vp->SetPosition(300, 200);

	Viewport* vp2 = Manage(new Viewport);

	AddViewport(vp2);

	vp2->SetPosition(450, 300);

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
