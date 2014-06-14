/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/HBox.hpp>
#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
	: m_menubar(0), m_toolbar(0), m_imgview(0)
{
	set_margin(0, 0, 0, 0);
	set_space(1);
	InitOnce();
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	using namespace BI;

	m_menubar = CreateMenuBar();
	m_toolbar = Manage(new ToolBar);
	m_imgview = Manage(new ImageView);

	m_combo = Manage(new ComboBox);

	HBox* box = Manage(new HBox);
	box->SetMargin(0, 0, 0, 0);
	box->SetSpace(-1);
	m_input = Manage(new TextEntry);
	m_open = Manage(new Button("Open"));
	m_input->SetRoundCornerType(RoundTopLeft | RoundBottomLeft);
	m_open->SetRoundCornerType(RoundTopRight | RoundBottomRight);
	box->PushBack(m_input);
	box->PushBack(m_open);

	m_toolbar->SetMargin(2, 2, 2, 2);
	m_toolbar->PushBack(m_menubar);
	m_toolbar->PushBack(m_combo);
	m_toolbar->PushBack(box);

	ToolBox* tbox = Manage(new ToolBox);
	Expander* expander = CreateExpander();
	tbox->PushBack(expander);

	Splitter* splitter = Manage(new Splitter);
	splitter->SetMargin(0, 0, 0, 0);

	splitter->PushBack(m_imgview);
	splitter->PushBack(tbox);

	PushBack(splitter);
	PushBack(m_toolbar);

	events()->connect(m_open->clicked(), this, &MainLayout::OnOpenClick);
}

void MainLayout::OnOpenClick()
{
	m_imgview->Open(ConvertFromString(m_input->text()).c_str());
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}

BI::MenuBar* MainLayout::CreateMenuBar()
{
	using namespace BI;

	MenuBar* menubar = Manage(new MenuBar);

	RefPtr<Menu> file_menu(new Menu);

    file_menu->SetRoundCornerType(RoundBottomLeft | RoundBottomRight);
    file_menu->AddAction(Stock::Icons::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddAction("MenuItem2", "Ctrl + 1");
    file_menu->AddAction("MenuItem3", "Ctrl + 1");
    file_menu->AddAction("MenuItem4", "Ctrl + 1");
    file_menu->AddAction("MenuItem5");

	menubar->AddMenu(String("File"), file_menu);

	return menubar;
}

BI::Expander* MainLayout::CreateExpander()
{
	BI::Expander* expander = Manage(new BI::Expander("Tools"));

	BI::Button* btn1 = Manage(new BI::Button("Blur"));
	BI::Button* btn2 = Manage(new BI::Button("Help"));

	BI::VBox* vbox = Manage(new BI::VBox);
	vbox->PushBack(btn1);
	vbox->PushBack(btn2);

	expander->Setup(vbox);

	return expander;

}
