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

#include <BlendInt/Gui/BinLayout.hpp>

#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/FileBrowser.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Expander.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to browse and select local directories/files.
	 */
	class FileSelector: public BinLayout
	{
		DISALLOW_COPY_AND_ASSIGN(FileSelector);

	public:

		FileSelector ();

		virtual ~FileSelector ();

		const String& file_selected () const
		{
			return browser_->file_selected();
		}

		Cpp::EventRef<> opened () {return opened_;}

		Cpp::EventRef<> canceled () {return canceled_;}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void InitializeFileSelector ();

		VLayout* CreateBrowserAreaOnce ();

		HLayout* CreateToolButtonsOnce ();

		//ToolBox* CreateSideBarOnce ();

		Expander* CreateSystemDevicesOnce ();

		Expander* CreateSystemBookmarksOnce ();

		void OnFileSelect ();

		GLuint vao_;
		RefPtr<GLArrayBuffer> inner_;

		TextEntry* path_entry_;
		TextEntry* file_entry_;

		Button* open_;
		Button* cancel_;

		FileBrowser* browser_;

		Cpp::Event<> opened_;

		Cpp::Event<> canceled_;
	};

}

#endif /* _BLENDINT_GUI_FILESELECTOR_HPP_ */
