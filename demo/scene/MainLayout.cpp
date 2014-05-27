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
	set_space(1);

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
    DBG_SET_NAME(file_menu, "Menu");

    //file_menu->SetRoundType(RoundBottomLeft | RoundBottomRight);
    file_menu->AddActionItem(StockItems::instance->icon_check(), String("MenuItem1"), String("Ctrl + 1"));
    file_menu->AddActionItem(String("MenuItem2"), String("Ctrl + 1"));
    file_menu->AddActionItem(String("MenuItem3"), String("Ctrl + 1"));
    file_menu->AddActionItem(String("MenuItem4"), String("Ctrl + 1"));
    file_menu->AddActionItem(String("MenuItem5"));

	m_menubar->AddMenu(String("File"), file_menu);
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
