/*
 * GLFWContext.cpp
 */

#include "StudioContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Decoration.hpp>
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/Clock.hpp>

using namespace BI;

StudioContext::StudioContext(GLFWwindow* window)
: BI::Context(),
  button_(0),
  pop_(0),
  menubar_(nullptr),
  window_(window)
{
	InitializeStudioContext();

	events()->connect(resized(), this, &StudioContext::OnResize);
}

StudioContext::~StudioContext ()
{

}

void StudioContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

void StudioContext::InitializeStudioContext()
{
	menubar_ = CreateMenuBar();

	AddFrame(menubar_);
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
	events()->connect(btn5->clicked(), this, &StudioContext::OnOpenDialogForNumericalSlider);

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

	Button* btn8 = Manage(new Button("Decoration"));
	events()->connect(btn8->clicked(), this, &StudioContext::OnOpenDialogForDecoration);
	Button* btn9 = Manage(new Button("Modal Dialog"));
	events()->connect(btn9->clicked(), this, &StudioContext::OnOpenModalDialog);

	group4->AddWidget(btn8);
	group4->AddWidget(btn9);

	vlayout->AddWidget(group4);

	Block * group5 = Manage(new Block(Horizontal));

	Button* btn10 = Manage(new Button("ScrollView"));
	events()->connect(btn10->clicked(), this, &StudioContext::OnOpenDialogForScrollView);
	Button* btn11 = Manage(new Button("Blocks"));
	events()->connect(btn11->clicked(), this, &StudioContext::OnOpenDialogForBlocks);

	group5->AddWidget(btn10);
	group5->AddWidget(btn11);

	vlayout->AddWidget(group5);

	panel->SetLayout(vlayout);

	return panel;
}

Panel* StudioContext::CreateButtonsForMenuTest()
{
	Panel* panel = Manage(new Panel);

	VLayout* vlayout = Manage(new VLayout);

	Block * group1 = Manage(new Block(Horizontal));

	Button* btn1 = Manage(new Button("Menu1"));
	Button* btn2 = Manage(new Button("Clock"));
	Button* btn3 = Manage(new Button("Menu3"));

	events()->connect(btn1->clicked(), this, &StudioContext::OnOpenMenu1);
	events()->connect(btn2->clicked(), this, &StudioContext::OnOpenDialogForClock);

	group1->AddWidget(btn1);
	group1->AddWidget(btn2);
	group1->AddWidget(btn3);

	vlayout->AddWidget(group1);

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
	Dialog * dialog = Manage(new Dialog("Hello", true));
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
	menubar_->MoveTo(0, size.height() - 32);
	menubar_->Resize(size.width(), 32);
}

void StudioContext::OnOpenDialogForDecoration()
{
	Dialog * dialog = Manage(new Dialog("Hello World", true));
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);

	Decoration* dec = Manage(new Decoration("Test Decoration"));
	dec->MoveTo(50, 50);
	dec->Resize(500, 24);
	dialog->AddWidget(dec);

	AddFrame(dialog);
}

void StudioContext::OnOpenDialogForScrollView()
{
	Dialog * dialog = Manage(new Dialog("ScrollView", true));
	dialog->Resize(500, 400);
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);

	ScrollView* scroll = Manage(new ScrollView);
	Button* test_btn = Manage(new Button("Test test"));
	scroll->Setup(test_btn);

	scroll->MoveTo(50, 25);
	//scroll->Resize(500, 24);

	dialog->AddWidget(scroll);

	AddFrame(dialog);
}

void StudioContext::OnOpenDialogForButton()
{
}

void StudioContext::OnOpenDialogForNumericalSlider()
{
	Dialog * dialog = Manage(new Dialog("NumericalSlider", true));
	dialog->Resize(500, 400);
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);

	Panel* panel1 = Manage(new Panel);
	panel1->MoveTo(20, 20);

	Panel* panel2 = Manage(new Panel);
	panel2->MoveTo(20, 20);
	panel2->Resize(250, 250);

	NumericalSlider* ns = Manage(new NumericalSlider);
	ns->Resize(150, ns->size().height());
	ns->MoveTo(50, 50);
	ns->SetValue(50.0);

	panel2->AddWidget(ns);
	panel1->AddWidget(panel2);

	dialog->AddWidget(panel1);

	AddFrame(dialog);
}

void StudioContext::OnOpenPanel1 (AbstractButton* btn)
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

void StudioContext::OnOpenDialogForBlocks()
{
	Dialog * dialog = Manage(new Dialog("ScrollView", true));
	dialog->Resize(500, 400);
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);

	Block* main_block = Manage(new Block(Vertical));

	Block* block1 = Manage(new Block(Horizontal));
	Button* btn1 = Manage(new Button("Button1"));
	Button* btn2 = Manage(new Button("Button2"));
	Button* btn3 = Manage(new Button("Button3"));
	block1->AddWidget(btn1);
	block1->AddWidget(btn2);
	block1->AddWidget(btn3);

	Block* block2 = Manage(new Block(Horizontal));
	Button* btn4 = Manage(new Button("Button4"));
	Button* btn5 = Manage(new Button("Button5"));
	Button* btn6 = Manage(new Button("Button6"));
	block2->AddWidget(btn4);
	block2->AddWidget(btn5);
	block2->AddWidget(btn6);

	main_block->AddWidget(block1);
	main_block->AddWidget(block2);
	main_block->Resize(main_block->GetPreferredSize());
	main_block->MoveTo(100, 100);

	DBG_PRINT_MSG("main block size: (%d, %d)", main_block->size().width(), main_block->size().height());
	
	dialog->AddWidget(main_block);

	AddFrame(dialog);
}

void StudioContext::OnOpenDialogForClock()
{
	Dialog * dialog = Manage(new Dialog("Clock"));
	dialog->Resize(500, 400);
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);

	Clock* clock = Manage(new Clock);
	clock->Start();

	dialog->AddWidget(clock);
	clock->MoveTo((dialog->size().width() - clock->size().width()) / 2, (dialog->size().height() - clock->size().height()) / 2);

	AddFrame(dialog);
}

void StudioContext::OnOpenMenu1()
{
	Menu* menu1 = Manage(new Menu);
	menu1->Resize(menu1->GetPreferredSize());
	menu1->MoveTo((size().width() - menu1->size().width()) / 2, (size().height() - menu1->size().height()) / 2);

	menu1->AddAction("MenuItem1");
	menu1->AddButton(Manage(new Button("Test")));

	AddFrame(menu1);
}

void StudioContext::OnOpenPanel2(BI::AbstractButton* btn)
{
	Panel* panel = CreateButtonsForMenuTest();
	panel->Resize(240, 320);
	panel->MoveTo(20, 20);

	pop_ = Manage(new PopupFrame);
	pop_->Resize(280, 360);
	pop_->MoveTo(size().width() - pop_->size().width(), 400);

	pop_->AddWidget(panel);

	AddFrame(pop_);
}

BI::ToolBox* StudioContext::CreateMenuBar()
{
	MenuButton* menubtn1 = Manage(new MenuButton("Widgets"));

	ToolBox* menubar = Manage(new ToolBox(size().width(), 24, Horizontal));
	menubar->MoveTo(0, size().height() - menubar->size().height());

	menubar->AddWidget(menubtn1);

	events()->connect(menubtn1->clicked(), this, &StudioContext::OnOpenPanel1);

	MenuButton* menubtn2 = Manage(new MenuButton("Menus"));
	menubar->AddWidget(menubtn2);

	events()->connect(menubtn2->clicked(), this, &StudioContext::OnOpenPanel2);

	return menubar;
}
