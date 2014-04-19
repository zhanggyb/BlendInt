/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Service/StockItems.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
	: m_menubar(0), m_toolbar(0), m_scene(0)
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

	m_input = Manage(new BI::TextEntry);
	m_open = Manage(new BI::ToolButton);

    m_scene = Manage(new BI::Viewport3D);
#ifdef DEBUG
	m_menubar->set_name("MenuBar");
	m_toolbar->set_name("ToolBar");
	m_input->set_name("TextEntry");
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
	// m_menubar->AddMenuButton("Edit");

	m_toolbar->Add(m_input);
	m_toolbar->Add(m_open);

	Add(m_menubar);
	Add(m_toolbar);

    Add(m_scene);

	events()->connect(m_open->clicked(), this, &MainLayout::OnOpenClick);
}

void MainLayout::OnOpenClick()
{
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}
