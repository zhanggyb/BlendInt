/*
 * GLFWContext.cpp
 */

#include "StudioContext.hpp"

#include <BlendInt/Gui/StaticPanel.hpp>

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
	frame_->SetPosition(20, 100);
	frame_->Resize(800, 600);

	// Setup a widget
	FileSelector* fs = Manage(new FileSelector);
	DBG_SET_NAME(fs, "FileSelector");
	fs->SetRoundType(RoundAll);
	
	frame_->Setup(fs);

	PushBack(frame_);

	StaticPanel* panel = Manage(new StaticPanel);
	button_ = Manage(new Button("Save Texture"));
	panel->SetContent(button_);
	panel->SetPosition(1100, 600);
	panel->Resize(panel->GetPreferredSize());

	PushBack(panel);
	events()->connect(button_->clicked(), this, &StudioContext::OnSaveTextureToFile);
}

void StudioContext::OnSaveTextureToFile()
{
	std::string filename;
	
	if(frame_->first()->name().empty()) {
		filename = "Widget.png";
	} else {
		filename = frame_->first()->name() + ".png";
	}
		
	frame_->RenderToFile(filename);
}
