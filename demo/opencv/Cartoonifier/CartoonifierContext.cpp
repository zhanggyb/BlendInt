/*
 * GLFWContext.cpp
 */

#include "CartoonifierContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/CVImageView.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/CVVideoView.hpp>

#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/Dialog.hpp>

using namespace BlendInt;

CartoonifierContext::CartoonifierContext()
: BI::Context()
{
	Dialog* dialog = Manage(new Dialog);
	dialog->SetPosition(800, 200);

	Expander* expander = Manage(new Expander("Light"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);
	NumericalSlider* ns3 = Manage(new NumericalSlider);

	VBlockLayout* vblock = Manage(new VBlockLayout);
	vblock->Append(ns1);
	vblock->Append(ns2);
	vblock->Append(ns3);

	expander->Setup(vblock);

	expander->Resize(expander->GetPreferredSize());

	ToolBox* toolbox = Manage(new ToolBox);
	toolbox->Append(expander);

	dialog->AddWidget(toolbox);

	AddFrame(dialog);

	CVVideoView* video = Manage(new CVVideoView);
	Dialog* frame2 = Manage(new Dialog);
	frame2->SetPosition(20, 100);

	frame2->AddWidget(video);

	video->OpenCamera(0);

	AddFrame(frame2);
}

CartoonifierContext::~CartoonifierContext ()
{

}
