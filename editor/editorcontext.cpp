/*
 * GLFWContext.cpp
 */

#include "editorcontext.hpp"

#include <core/image.hpp>

#include <font/fc-config.hpp>
#include <font/fc-pattern.hpp>

#include <gui/widget.hpp>
#include <gui/radio-button.hpp>
#include <gui/check-button.hpp>
#include <gui/frame.hpp>
#include <gui/frame-splitter.hpp>
#include <gui/filebutton.hpp>
#include <gui/color-wheel.hpp>
#include <gui/viewport2d.hpp>
#include <gui/viewport.hpp>
#include <gui/colorselector.hpp>

#include <blendint/stock/shaders.hpp>
#include <gui/dialog.hpp>

#include <gui/frame.hpp>
#include <gui/toolbox.hpp>
#include <gui/block.hpp>
#include <gui/close-button.hpp>
#include <gui/folder-list.hpp>
#include <gui/node.hpp>
#include <gui/menu-button.hpp>

using namespace BI;

EditorContext::EditorContext(int width, int height, const char* name)
: BI::Window(width, height, name)
{
	InitializeGLFWDemoContext ();
}

EditorContext::~EditorContext ()
{
}

void EditorContext::InitializeGLFWDemoContext ()
{
	Dialog* dlg = new Dialog(String("NodeView Debug"));
    dlg->Resize(800, 600);
    dlg->MoveTo(200, 150);
	AddFrame(dlg);

	NodeView* nodeview = new NodeView;
	nodeview->MoveTo(10, 10);
	nodeview->Resize(500, 400);

	Node* node = new Node;
	nodeview->AddNode(node);
	node->MoveTo(10, 10);

	dlg->AddWidget(nodeview);

}
