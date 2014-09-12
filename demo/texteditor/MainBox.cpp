/*
 * MainBox.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: zhanggyb
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Stock/Icons.hpp>

#include "MainBox.hpp"

MainBox::MainBox ()
	: m_menubar(0), m_toolbar(0)
{
	InitOnce();
}

MainBox::~MainBox ()
{

}

void MainBox::InitOnce ()
{
	using namespace BI;

	set_margin(0, 0, 0, 0);
	set_space(1);

	m_menubar = Manage(new BI::MenuBar);
	m_toolbar = Manage(new BI::ToolBar);

    RefPtr<Menu> file_menu(new Menu);
    DBG_SET_NAME(file_menu, "Menu");

    /*
    file_menu->SetRoundType(RoundBottomLeft | RoundBottomRight);
    file_menu->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem2", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem3", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem4", "Ctrl + 1");
    file_menu->AddActionItem("MenuItem5");
    */

	m_menubar->Append(String("File"), file_menu);
	// m_menubar->AddMenuButton("Edit");

	m_toolbar->Append(m_open);

	Append(m_menubar);
	Append(m_toolbar);

	events()->connect(m_open->clicked(), this, &MainBox::OnOpenClick);
}

void MainBox::OnOpenClick()
{
}

void MainBox::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}
