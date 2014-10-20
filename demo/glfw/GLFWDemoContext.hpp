/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <GLFW/glfw3.h>

#include <vector>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Clock.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Gui/Panel.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/MenuItemBin.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/Separator.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Gui/TabButton.hpp>
#include <BlendInt/Gui/TabHeader.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/ColorButton.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/HBlockLayout.hpp>
#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/StaticPanel.hpp>

#include <BlendInt/Gui/Panel.hpp>
#include <BlendInt/Gui/Decoration.hpp>
#include <BlendInt/Gui/NodeView.hpp>
#include <BlendInt/Gui/ProgressBar.hpp>
#include <BlendInt/Gui/Workspace.hpp>
#include <BlendInt/Gui/ListView.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/StaticPanel.hpp>

namespace BI=BlendInt;

class GLFWDemoContext: public BI::Context
{
public:

	GLFWDemoContext ();

	virtual ~GLFWDemoContext ();

private:

	void InitializeGLFWDemoContext ();
};

#endif /* GLFWCONTEXT_HPP_ */
