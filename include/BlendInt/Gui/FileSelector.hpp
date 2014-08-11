/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_FILESELECTOR_HPP_
#define _BLENDINT_GUI_FILESELECTOR_HPP_

#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/FileBrowser.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Expander.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to browse and select local directories/files.
	 */
	class FileSelector: public Frame
	{
		DISALLOW_COPY_AND_ASSIGN(FileSelector);

	public:

		FileSelector ();

		virtual ~FileSelector ();

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const Profile& profile);

	private:

		void InitializeFileSelector ();

		VBox* CreateBrowserAreaOnce ();

		ToolBar* CreateToolBarOnce ();

		ToolBox* CreateSideBarOnce ();

		Expander* CreateSystemPartOnce ();

		Expander* CreateSystemBookmarksOnce ();

		void OnFileSelect ();

		void OnCancel ();

		GLuint vao_;
		RefPtr<GLArrayBuffer> inner_;

		TextEntry* path_entry_;
		TextEntry* file_entry_;

		Button* open_;
		Button* cancel_;

		FileBrowser* browser_;
	};

}

#endif /* _BLENDINT_GUI_FILESELECTOR_HPP_ */
