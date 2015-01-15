/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "HPEContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ImageViewport.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/ComboBox.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/RadioButton.hpp>

using namespace BlendInt;

HPEContext::HPEContext()
: BI::Context(),
  viewport_3d_(0)
{
	FrameSplitter* vsplitter = new FrameSplitter(Vertical);

	FrameSplitter* splitter = new FrameSplitter;

	Workspace* tools = CreateToolsOnce();
	Workspace* workspace = CreateWorkspaceOnce();

	splitter->AddFrame(workspace);
	splitter->AddFrame(tools, PreferredWidth);

	ToolBox* bar = CreateToolBarOnce();

	vsplitter->AddFrame(bar);
	vsplitter->AddFrame(splitter, ExpandY);

	AddFrame(vsplitter);

	events()->connect(resized(), vsplitter, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));
}

HPEContext::~HPEContext ()
{

}

void HPEContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

ToolBox* HPEContext::CreateToolBarOnce()
{
	ToolBox* bar = new ToolBox(Horizontal);

	ComboBox* combo = new ComboBox;
	combo->Resize(48, combo->size().height());

	MenuButton* btn1 = new MenuButton("File");
	MenuButton* btn2 = new MenuButton("Edit");
	MenuButton* btn3 = new MenuButton("View");

	bar->AddWidget(combo);
	bar->AddWidget(btn1);
	bar->AddWidget(btn2);
	bar->AddWidget(btn3);

	bar->Resize(bar->GetPreferredSize());

	return bar;
}

Workspace* HPEContext::CreateWorkspaceOnce()
{
	Workspace* workspace = new Workspace;
	
	ToolBox* header = new ToolBox(Horizontal);

	ComboBox* combo = new ComboBox;

	Block* block1 = new Block(Horizontal);

	Button* btn = new Button("Button1");
	block1->AddWidget(btn);

	btn = new Button("Button2");
	block1->AddWidget(btn);

	header->AddWidget(combo);
	header->AddWidget(block1);

	header->Resize(header->GetPreferredSize());

	workspace->SetHeader(header);
	return workspace;
}

Workspace* HPEContext::CreateToolsOnce()
{
	Workspace* workspace = new Workspace;

	ToolBox* header = CreateRadios();

	ToolBox* tools = new ToolBox(Vertical);

	Expander* expander = new Expander("Resolution");

	NumericalSlider* ns1 = new NumericalSlider;
	NumericalSlider* ns2 = new NumericalSlider;

	Block* vblock = new Block(Vertical);
	vblock->AddWidget(ns1);
	vblock->AddWidget(ns2);

	expander->Setup(vblock);
	expander->Resize(expander->GetPreferredSize());

	Panel* btn_panel = CreateButtons();

	tools->AddWidget(expander);
	tools->AddWidget(btn_panel);

	workspace->SetHeader(header, false);
	workspace->SetViewport(tools);

	return workspace;
}

ToolBox* HPEContext::CreateRadios()
{
	ToolBox* radio_tool = new ToolBox(Horizontal);

	radio_group_.reset(new ButtonGroup);

	ComboBox* combo = new ComboBox;

	Block* hblock = new Block(Horizontal);

	RadioButton* radio1 = new RadioButton(Context::icons->icon_16x16(Icons::SCENE));
	RadioButton* radio2 = new RadioButton(Context::icons->icon_16x16(Icons::SCENE_DATA));
	RadioButton* radio3 = new RadioButton(Context::icons->icon_16x16(Icons::SURFACE_NSURFACE));
	RadioButton* radio4 = new RadioButton(Context::icons->icon_16x16(Icons::SURFACE_NCIRCLE));
	RadioButton* radio5 = new RadioButton(Context::icons->icon_16x16(Icons::SURFACE_NCURVE));

	radio_group_->AddButton(radio1);
	radio_group_->AddButton(radio2);
	radio_group_->AddButton(radio3);
	radio_group_->AddButton(radio4);
	radio_group_->AddButton(radio5);

	radio1->SetChecked(true);

	hblock->AddWidget(radio1);
	hblock->AddWidget(radio2);
	hblock->AddWidget(radio3);
	hblock->AddWidget(radio4);
	hblock->AddWidget(radio5);

	radio_tool->AddWidget(combo);
	radio_tool->AddWidget(hblock);

	radio_tool->Resize(radio_tool->GetPreferredSize());

	return radio_tool;
}

Panel* HPEContext::CreateButtons()
{
	Panel* panel = new Panel;
	panel->SetRoundType(RoundAll);

	Block* hblock1 = new Block(Horizontal);

	ComboBox* camera_no = new ComboBox;
	ToggleButton* btn1 = new ToggleButton("Open Camera");

	hblock1->AddWidget(camera_no);
	hblock1->AddWidget(btn1);

	VLayout* layout = new VLayout;

	Block* hblock2 = new Block(Horizontal);

	Button* play = new Button("Play");
	Button* pause = new Button("Pause");
	Button* stop = new Button("Stop");

	hblock2->AddWidget(play);
	hblock2->AddWidget(pause);
	hblock2->AddWidget(stop);

	layout->AddWidget(hblock1);
	layout->AddWidget(hblock2);

	panel->SetLayout(layout);
	panel->Resize(layout->GetPreferredSize());

	return panel;
}
