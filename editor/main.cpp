/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

/**
 * @brief BlendInt UI Editor
 */

#include "editorcontext.hpp"
#include <gui/window.hpp>

#include <gui/abstract-dialog.hpp>
#include <gui/message-box.hpp>
#include <gui/colorselector.hpp>
#include <gui/file-selector.hpp>
#include <gui/dialog.hpp>
#include <gui/texture-view.hpp>
#include <gui/clock.hpp>
#include <gui/textentry.hpp>
#include <gui/filebutton.hpp>
#include <gui/table-layout.hpp>
#include <gui/scroll-bar.hpp>
#include <gui/scroll-area.hpp>
#include <gui/scroll-view.hpp>
#include <gui/numerical-slider.hpp>
#include <gui/node-view.hpp>
#include <gui/node.hpp>
#include <gui/panel.hpp>
#include <gui/string-list-model.hpp>

int main (int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	DBG_PRINT_MSG("Dialog size: %ld", sizeof(AbstractDialog));

	if(Window::Initialize()) {

		Window win(1280, 800, "UI Editor");

		//Dialog* dlg = new Dialog("Test TableLayout", 0, Dialog::DialogButtonOK);

		Frame* frame = new Frame(new LinearLayout(Vertical));

//		TextureView* b1 = new TextureView;
//		b1->OpenFile("test.jpg");

//		NodeView* b1 = new NodeView;
//
//		Node* node = new Node;
//		b1->AddNode(node);
//
//		node->MoveTo(200, -100);
//
//		ScrollArea* area = new ScrollArea(400, 400, Margin(0, 0, 0, 0), 0);
//		area->SetScrollableWidget(b1);

//		dlg->AddWidget(area);

		Label* label = new Label("ComboBox Test:");

		ComboBox* combo = new ComboBox;

		frame->AddWidget(label);
		frame->AddWidget(combo);

		frame->Resize(frame->GetPreferredSize());
		frame->MoveTo((win.size().width() - frame->size().width()) / 2,
		        (win.size().height() - frame->size().height()) / 2);

		win.AddFrame(frame);

		win.Exec();

		Window::Terminate();
	}

	return 0;
}
