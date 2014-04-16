/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>

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

	m_menubar->AddMenu("File");
	m_menubar->AddMenu("Edit");

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
