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
	Button* btn7 = Manage(new Button("FileSelector"));
	events()->connect(btn7->clicked(), this, &StudioContext::OnOpenFileSelector);

	group3->AddWidget(btn6);
	group3->AddWidget(btn7);

	vlayout->AddWidget(group3);

	Block * group4 = Manage(new Block(Horizontal));

	Button* btn8 = Manage(new Button("Modal Dialog"));
	events()->connect(btn8->clicked(), this, &StudioContext::OnOpenModalDialog);

	group4->AddWidget(btn8);

	vlayout->AddWidget(group4);

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
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);
	dialog->AddWidget(textureview);
	AddFrame(dialog);
}

void StudioContext::OnOpenModalDialog()
{
	Dialog * dialog = Manage(new Dialog(true));
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);
	AddFrame(dialog);
}

void StudioContext::OnOpenFileSelector()
{
	FileSelector * fs = Manage(new FileSelector);
	fs->Resize(800, 600);
	fs->MoveTo((size().width() - fs->size().width()) / 2, (size().height() - fs->size().height()) / 2);

	AddFrame(fs);
}

void StudioContext::OnResize(const BI::Size& size)
{
	pop_->MoveTo(size.width() - pop_->size().width(), pop_->position().y());
}

void StudioContext::OnOpenDialogForButton()
{

}
