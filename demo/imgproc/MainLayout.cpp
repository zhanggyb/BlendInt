/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
: VLayout(),
  m_menubar(0),
  m_toolbar(0),
  m_imgview(0)
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

	m_menubar = CreateMenuBar();
	m_toolbar = Manage(new ToolBar);
	m_imgview = Manage(new ImageView);

	m_combo = Manage(new ComboBox);

	HLayout* box = Manage(new HLayout);
	box->SetMargin(0, 0, 0, 0);
	box->SetSpace(-1);
	m_input = Manage(new TextEntry);
	m_tool_open = Manage(new Button("Open"));
	m_input->SetRoundType(RoundTopLeft | RoundBottomLeft);
	m_tool_open->SetRoundType(RoundTopRight | RoundBottomRight);
	box->Append(m_input);
	box->Append(m_tool_open);

	m_toolbar->SetMargin(2, 2, 2, 2);
	m_toolbar->Append(m_menubar);
	m_toolbar->Append(m_combo);
	m_toolbar->Append(box);

	//ToolBox* tbox = Manage(new ToolBox);
	//Expander* expander = CreateExpander();
	//tbox->Append(expander);

	Splitter* splitter = Manage(new Splitter);

	splitter->Append(m_imgview);
	//splitter->Append(tbox);

	Append(splitter);
	Append(m_toolbar);

	events()->connect(m_tool_open->clicked(), this, &MainLayout::OnOpenClick);
}

void MainLayout::OnOpenClick()
{
	m_imgview->Open(ConvertFromString(m_input->text()).c_str());
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}

BI::MenuBar* MainLayout::CreateMenuBar()
{
	using namespace BI;

	MenuBar* menubar = Manage(new MenuBar);

	RefPtr<Menu> file_menu(new Menu);

    file_menu->SetRoundType(RoundBottomLeft | RoundBottomRight);
    file_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddAction("MenuItem2", "Ctrl + 1");
    file_menu->AddAction("MenuItem3", "Ctrl + 1");
    file_menu->AddAction("MenuItem4", "Ctrl + 1");
    file_menu->AddAction("MenuItem5");

	menubar->Append(String("File"), file_menu);

	return menubar;
}

BI::Expander* MainLayout::CreateExpander()
{
	BI::Expander* expander = Manage(new BI::Expander("Tools"));

	BI::Button* btn1 = Manage(new BI::Button("Blur"));
	BI::Button* btn2 = Manage(new BI::Button("Help"));

	BI::VLayout* vbox = Manage(new BI::VLayout);
	vbox->Append(btn1);
	vbox->Append(btn2);

	expander->Setup(vbox);

	return expander;

}
