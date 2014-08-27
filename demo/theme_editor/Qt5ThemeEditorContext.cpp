/*
 * Qt5Context.cpp
 */

#include "Qt5ThemeEditorContext.hpp"
#include <QtGui/QCursor>

#include <BlendInt/Gui/ColorSelector.hpp>

Qt5ThemeEditorContext::Qt5ThemeEditorContext (QWindow* window)
: BI::Context (), window_(window)
{

	Initialize();

}

Qt5ThemeEditorContext::~Qt5ThemeEditorContext ()
{
}

void Qt5ThemeEditorContext::SetCursor(int cursor_type)
{
	if(window_) {
		window_->setCursor(QCursor(static_cast<Qt::CursorShape>(cursor_type)));
	}
}

void Qt5ThemeEditorContext::Initialize()
{
	using namespace BI;

	Panel* panel1 = CreateButtonPanel();
	panel1->SetPosition(900, 580);
	panel1->Resize(panel1->GetPreferredSize());

	Panel* panel2 = CreateColorPanel();
	panel2->SetPosition(900, 100);
	panel2->Resize(panel2->GetPreferredSize());

	Section* section = PushBack(panel1);
	section->PushBack(panel2);
}

BI::Panel* Qt5ThemeEditorContext::CreateButtonPanel ()
{
	using namespace BI;

	Panel* panel = Manage(new Panel);

	VBlockLayout* block = Manage(new VBlockLayout);

	Button* btn1 = Manage(new Button("Button"));
	Button* btn2 = Manage(new Button("Button"));
	Button* btn3 = Manage(new Button("Button"));
	Button* btn4 = Manage(new Button("Button"));

	block->PushBack(btn1);
	block->PushBack(btn2);
	block->PushBack(btn3);
	block->PushBack(btn4);

	panel->Setup(block);

	return panel;
}

BI::Panel* Qt5ThemeEditorContext::CreateColorPanel ()
{
	using namespace BI;

	Panel* panel = Manage(new Panel);

	ColorSelector* cs = Manage(new ColorSelector);

	panel->Setup(cs);

	return panel;
}
