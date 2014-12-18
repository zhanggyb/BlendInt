/*
 * GLFWContext.cpp
 */

#include "StudioContext.hpp"

#include <BlendInt/Gui/Frame.hpp>

using namespace BI;

StudioContext::StudioContext()
: BI::Context(),
  button_(0)
{
	InitializeStudioContext();
}

StudioContext::~StudioContext ()
{

}

void StudioContext::InitializeStudioContext()
{
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

	PopupFrame* pop = Manage(new PopupFrame);
	pop->MoveTo(1020, 400);
	pop->Resize(pop->GetPreferredSize());
	pop->SetLayout(vlayout);

	AddFrame(pop);
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

void StudioContext::OnOpenDialogForButton()
{

}
