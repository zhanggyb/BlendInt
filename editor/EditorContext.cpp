/*
 * GLFWContext.cpp
 */

#include "EditorContext.hpp"

#include <BlendInt/Core/Image.hpp>

#include <BlendInt/Font/FcConfig.hpp>
#include <BlendInt/Font/FcPattern.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/CheckButton.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/FileButton.hpp>
#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Gui/Viewport2D.hpp>
#include <BlendInt/Gui/Viewport.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/Dialog.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/CloseButton.hpp>
#include <BlendInt/Gui/FolderList.hpp>
#include <BlendInt/Gui/Node.hpp>
#include <BlendInt/Gui/MenuButton.hpp>

using namespace BI;

EditorContext::EditorContext(GLFWwindow* win)
: BI::Context(),
  window_(win)
{
	InitializeGLFWDemoContext ();
}

EditorContext::~EditorContext ()
{
}

void EditorContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

void EditorContext::InitializeGLFWDemoContext ()
{
	Dialog* dlg = new Dialog(String("NodeView Debug"));
    dlg->Resize(800, 600);
    dlg->MoveTo(200, 150);
	AddFrame(dlg);

	NodeView* nodeview = new NodeView;
	nodeview->MoveTo(100, 100);
	nodeview->Resize(500, 400);

	Node* node = new Node;
	nodeview->AddNode(node);
	node->MoveTo(50, 50);

	dlg->AddWidget(nodeview);

}
