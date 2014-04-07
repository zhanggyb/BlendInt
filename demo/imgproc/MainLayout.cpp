/**
 * Main Layout
 */

#include <BlendInt/Service/ContextManager.hpp>

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
	m_menubar = Manage(new BI::MenuBar);
	m_toolbar = Manage(new BI::ToolBar);
	m_imgview = Manage(new BI::CVImageView);

#ifdef DEBUG
	m_menubar->set_name("MenuBar");
	m_toolbar->set_name("ToolBar");
	m_imgview->set_name("CVImageView");
#endif

	Add(m_menubar);
	Add(m_toolbar);
	Add(m_imgview);
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	Resize(BI::ContextManager::instance->size());
}
