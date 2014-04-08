/**
 * Main Layout
 */

#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Gui/Widget.hpp>

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

	m_input = Manage(new BI::TextEntry);
	m_open = Manage(new BI::ToolButton);

#ifdef DEBUG
	m_menubar->set_name("MenuBar");
	m_toolbar->set_name("ToolBar");
	m_imgview->set_name("CVImageView");
	m_input->set_name("TextEntry");
	m_open->set_name("ToolButton");
#endif

	m_menubar->AddMenu("File");
	m_menubar->AddMenu("Edit");

	m_toolbar->Add(m_input);
	m_toolbar->Add(m_open);

	Add(m_menubar);
	Add(m_toolbar);

	Add(m_imgview);
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	DBG_PRINT_MSG("get size: %u, %u", context->size().width(), context->size().height());
	DBG_PRINT_MSG("get size: %u, %u", BI::ContextManager::instance->size().width(), BI::ContextManager::instance->size().height());
	Resize(BI::ContextManager::instance->size());
}
