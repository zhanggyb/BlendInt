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

#include "EditorContext.hpp"
#include <BlendInt/Gui/Window.hpp>

#include <BlendInt/Gui/AbstractDialog.hpp>
#include <BlendInt/Gui/MessageBox.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/Dialog.hpp>
#include <BlendInt/Gui/TextureView.hpp>
#include <BlendInt/Gui/Clock.hpp>

int main (int argc, char* argv[])
{
	using namespace BlendInt;

	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	DBG_PRINT_MSG("Dialog size: %ld", sizeof(AbstractDialog));

	if(Window::Initialize()) {

		Window win(1280, 800, "UI Editor");

		Dialog* dlg = new Dialog("Demo", Dialog::DialogButtonOK);
		TextureView* texview = new TextureView;
		texview->OpenFile("test.jpg");
		dlg->AddWidget(texview);
		win.AddFrame(dlg);
		dlg->Resize(dlg->GetPreferredSize());

		FileSelector* fs = new FileSelector;
		win.AddFrame(fs);

		win.Exec();

		Window::Terminate();
	}

	return 0;
}
