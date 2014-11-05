/*
 * GLFWContext.cpp
 */

#include "StudioContext.hpp"

#include <BlendInt/Gui/StaticPanel.hpp>

#include <BlendInt/Gui/Frame.hpp>

using namespace BI;

StudioContext::StudioContext()
: BI::Context(),
  frame_(0),
  button_(0)
{
	Initialize ();
}

StudioContext::~StudioContext ()
{

}

void StudioContext::Initialize ()
{
	frame_ = Manage(new StudioFrame);
	frame_->SetPosition(100, 100);
	frame_->Resize(800, 600);

	// Setup a widget
	FileSelector* fs = Manage(new FileSelector);
	DBG_SET_NAME(fs, "FileSelector");
	fs->SetRoundType(RoundAll);
	
	frame_->Setup(fs);

	Frame* screen1 = Manage(new Frame);
	screen1->Resize(frame_->size());
	screen1->SetPosition(100, 100);
	screen1->AddWidget(frame_);
	AddFrame(screen1);

	StaticPanel* panel = Manage(new StaticPanel);
	button_ = Manage(new Button("Save Texture"));
	panel->Setup(button_);
	panel->SetPosition(1100, 600);
	panel->Resize(panel->GetPreferredSize());

	Frame* screen2 = Manage(new Frame);
	screen2->Resize(panel->size());
	screen2->SetPosition(1100, 600);
	screen2->AddWidget(panel);
	AddFrame(screen2);

	events()->connect(button_->clicked(), this, &StudioContext::OnSaveTextureToFile);

	Button* btn = Manage(new Button);
	btn->SetPosition(20, 750);
	btn->Resize(100, 40);

	Frame* screen3 = Manage(new Frame);
	screen3->Resize(btn->size());
	screen3->SetPosition(20, 750);
	screen3->AddWidget(btn);
	AddFrame(screen3);
}

void StudioContext::OnSaveTextureToFile()
{
	std::string filename;
	
	if(frame_->first_child()->name().empty()) {
		filename = "Widget.png";
	} else {
		filename = frame_->first_child()->name() + ".png";
	}
		
	frame_->RenderToFile(filename);
}
