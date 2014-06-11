/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Stock/StockItems.hpp>
#include <BlendInt/Gui/HBox.hpp>
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

	m_menubar = Manage(new MenuBar);
	m_toolbar = Manage(new ToolBar);
	m_imgview = Manage(new ImageView);
	m_area = Manage(new ScrollArea);

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

    RefPtr<Menu> file_menu(new Menu);

    file_menu->SetRoundCornerType(RoundBottomLeft | RoundBottomRight);
    file_menu->AddAction(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddAction("MenuItem2", "Ctrl + 1");
    file_menu->AddAction("MenuItem3", "Ctrl + 1");
    file_menu->AddAction("MenuItem4", "Ctrl + 1");
    file_menu->AddAction("MenuItem5");

	m_menubar->AddMenu(String("File"), file_menu);
	
	//m_toolbar->Resize(m_toolbar->size().width(), 24);
	m_toolbar->SetMargin(2, 2, 2, 2);
	m_toolbar->PushBack(m_menubar);
	m_toolbar->PushBack(m_combo);
	m_toolbar->PushBack(box);

	m_area->SetViewport(m_imgview);

	ToolBox* tbox = Manage(new ToolBox);
	Button* btn1 = Manage(new Button("Blur"));
	Button* btn2 = Manage(new Button("Help"));
	tbox->PushBack(btn1);
	tbox->PushBack(btn2);

	Splitter* splitter = Manage(new Splitter);
	splitter->SetMargin(0, 0, 0, 0);

	splitter->PushBack(m_area);
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
