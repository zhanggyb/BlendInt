/*
 * GLFWContext.cpp
 */

#include "FontViewerContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Decoration.hpp>
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/Clock.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/TabHeader.hpp>

using namespace BI;

FontViewerContext::FontViewerContext(GLFWwindow* window)
: BI::Context(),
  menubar_(nullptr),
  window_(window)
{
	InitializeFontViewerContext();

	events()->connect(resized(), this, &FontViewerContext::OnResize);
}

FontViewerContext::~FontViewerContext ()
{

}

void FontViewerContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

void FontViewerContext::InitializeFontViewerContext()
{
	menubar_ = CreateMenuBar();

	AddFrame(menubar_);
}

void FontViewerContext::OnResize(const BI::Size& size)
{
	Size menubar_size = menubar_->GetPreferredSize();

	menubar_->MoveTo(0, size.height() - menubar_size.height());
	menubar_->Resize(size.width(), menubar_size.height());
}

BI::ToolBox* FontViewerContext::CreateMenuBar()
{
	MenuButton* menubtn1 = Manage(new MenuButton("Open"));

	ToolBox* menubar = Manage(new ToolBox(size().width(), 24, Horizontal));
	menubar->MoveTo(0, size().height() - menubar->size().height());

	menubar->AddWidget(menubtn1);

	events()->connect(menubtn1->clicked(), this, &FontViewerContext::OnOpen);

	MenuButton* menubtn2 = Manage(new MenuButton("Menus"));
	menubar->AddWidget(menubtn2);

	//events()->connect(menubtn2->clicked(), this, &FontViewerContext::OnOpenPanel2);

	return menubar;
}

void FontViewerContext::OnOpen(AbstractButton* sender)
{
	Dialog* dialog = Manage(new Dialog("Font Viewer"));

	TextureView* textureview = Manage(new TextureView);
	textureview->MoveTo(50, 50);

	Font font;

	textureview->Load(font.GetTexture(L'A'));

	VLayout* layout = Manage(new VLayout);
	layout->AddWidget(textureview);

	dialog->SetLayout(layout);

	dialog->Resize(textureview->size().width() + 100, textureview->size().height() + 100);
	dialog->MoveTo((size().width() - dialog->size().width()) / 2, (size().height() - dialog->size().height()) / 2);
//	dialog->AddWidget(textureview);

	AddFrame(dialog);
}
