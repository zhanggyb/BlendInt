/*
 * GLFWContext.cpp
 */

#include "StudioContext.hpp"

#include <BlendInt/Gui/Frame.hpp>

using namespace BI;

StudioContext::StudioContext()
: BI::Context(),
  button_(0),
  pop_(0)
{
	InitializeStudioContext();

	events()->connect(resized(), this, &StudioContext::OnResize);
}

StudioContext::~StudioContext ()
{

}

void StudioContext::InitializeStudioContext()
{
	Panel* panel1 = CreateButtonsForWidgets();
	panel1->Resize(240, 320);
	panel1->MoveTo(20, 20);

	pop_ = Manage(new PopupFrame);
	pop_->Resize(280, 360);
	pop_->MoveTo(size().width() - pop_->size().width(), 400);

	pop_->AddWidget(panel1);

	AddFrame(pop_);
}

Panel* StudioContext::CreateButtonsForWidgets()
{
	Panel* panel = Manage(new Panel);

	VLayout* vlayout = Manage(new VLayout);

	Block * group1 = Manage(new Block(Horizontal));

	Button* btn1 = Manage(new Button("Button"));
	Button* btn2 = Manage(new Button("ToggleButton"));
	Button* btn3 = Manage(new Button("RadioButton"));

	group1->AddWidget(btn1);
	group1->AddWidget(btn2);
	group1->AddWidget(btn3);

	vlayout->AddWidget(group1);

	Block * group2 = Manage(new Block(Horizontal));
	
	Button* btn4 = Manage(new Button("TextEntry"));
	Button* btn5 = Manage(new Button("NumericalSlider"));

	group2->AddWidget(btn4);
	group2->AddWidget(btn5);

	vlayout->AddWidget(group2);

	Block * group3 = Manage(new Block(Horizontal));

	Button* btn6 = Manage(new Button("TextureView"));
	events()->connect(btn6->clicked(), this, &StudioContext::OnOpenDialogForTextureView);

	group3->AddWidget(btn6);

	vlayout->AddWidget(group3);

	panel->SetLayout(vlayout);

	return panel;
}

void StudioContext::OnSaveTextureToFile()
{
}

void StudioContext::OnOpenDialogForTextureView()
{
	Dialog* dialog = Manage(new Dialog);
	TextureView* textureview = Manage(new TextureView);
	textureview->MoveTo(50, 50);
	dialog->Resize(textureview->size().width() + 100, textureview->size().height() + 100);

	dialog->AddWidget(textureview);
	AddFrame(dialog);
}

void StudioContext::OnResize(const BI::Size& size)
{
	pop_->MoveTo(size.width() - pop_->size().width(), 400);
}

void StudioContext::OnOpenDialogForButton()
{

}
