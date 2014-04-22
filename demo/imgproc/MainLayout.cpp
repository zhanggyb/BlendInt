/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Service/StockItems.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
	: m_menubar(0), m_toolbar(0), m_imgview(0)
{
	InitOnce();
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	using namespace BI;
	set_margin(0, 0, 0, 0);

	m_menubar = Manage(new BI::MenuBar);
	m_toolbar = Manage(new BI::ToolBar);
	m_imgview = Manage(new BI::CVImageView);
	m_area = Manage(new BI::ScrollArea);

	m_input = Manage(new BI::TextEntry);
	m_combo = Manage(new BI::ComboBox);
	m_open = Manage(new BI::ToolButton);

#ifdef DEBUG
	m_menubar->set_name("MenuBar");
	m_toolbar->set_name("ToolBar");
	m_imgview->set_name("CVImageView");
	m_area->set_name("ScrollArea");
	m_input->set_name("TextEntry");
	m_combo->set_name("ComboBox");
	m_open->set_name("ToolButton");
#endif

    RefPtr<Menu> file_menu(new Menu);
    file_menu->set_name("Menu");

    file_menu->SetRoundType(RoundBottomLeft | RoundBottomRight);
    file_menu->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem2", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem3", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem4", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem5");

	m_menubar->AddMenuButton("File", file_menu);
	//m_menubar->AddMenuButton("Edit");

	m_toolbar->Resize(m_toolbar->size().width(), 24);
	m_toolbar->SetPreferredSize(m_toolbar->preferred_size().width(), 24);
	m_toolbar->SetMargin(2, 2, 2, 2);
	m_toolbar->Add(m_input);
	m_toolbar->Add(m_combo);
	m_toolbar->Add(m_open);

	m_area->SetViewport(m_imgview);

	Add(m_menubar);
	Add(m_area);
	Add(m_toolbar);

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
