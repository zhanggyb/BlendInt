/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Service/StockItems.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
	: m_view3d(0), m_button(0)
{
	set_margin(4, 4, 4, 4);
	set_space(4);

	InitOnce();
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	using namespace BI;

	m_view3d = Manage(new BI::Viewport3D);
	m_view3d->set_name("Viewport3D");
	m_view3d->Resize(630, 445);
	m_view3d->SetPosition(5, 30);

	m_button = Manage(new BI::Button);
	m_button->set_name("Button");
	m_button->SetPosition(545, 5);

	Add(m_view3d);
	Add(m_button);
}
