/**
 * Main Layout
 */

#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
	: m_menubar(0), m_toolbar(0), m_imgview(0)
{
	InitOnce();

	events()->connect(BI::ContextManager::instance->property_changed(), this, &MainLayout::OnResize);
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	set_margin(0, 0, 0, 0);

	m_menubar = Manage(new BI::MenuBar);
	m_toolbar = Manage(new BI::ToolBar);
	m_imgview = Manage(new BI::CVImageView);
	m_area = Manage(new BI::ScrollArea);

	m_input = Manage(new BI::TextEntry);
	m_open = Manage(new BI::ToolButton);

#ifdef DEBUG
	m_menubar->set_name("MenuBar");
	m_toolbar->set_name("ToolBar");
	m_imgview->set_name("CVImageView");
	m_area->set_name("ScrollArea");
	m_input->set_name("TextEntry");
	m_open->set_name("ToolButton");
#endif

	m_menubar->AddMenu("File");
	m_menubar->AddMenu("Edit");

	m_toolbar->Add(m_input);
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
	Resize(BI::ContextManager::instance->size());
}
