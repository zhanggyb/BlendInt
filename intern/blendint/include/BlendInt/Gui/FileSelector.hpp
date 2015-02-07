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

#pragma once

#include <BlendInt/Gui/LinearLayout.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/FileBrowser.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/AbstractDialog.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to browse and select local directories/files.
	 */
	class FileSelector: public AbstractDialog
	{
		DISALLOW_COPY_AND_ASSIGN(FileSelector);

	public:

		FileSelector ();

		virtual ~FileSelector ();

		const String& file_selected () const
		{
			return browser_->file_selected();
		}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual ResponseType Draw (AbstractWindow* context);

	private:

		LinearLayout* CreateButtons ();

		LinearLayout* CreateBrowserAreaOnce ();

		//ToolBox* CreateSideBarOnce ();

		Expander* CreateSystemDevicesOnce ();

		Expander* CreateSystemBookmarksOnce ();

		void OnFileSelect ();

		void OnClose (AbstractButton* sender);

		void OnOpenParent (AbstractButton* sender);

		void OnGoBackward (AbstractButton* sender);

		void OnGoForward (AbstractButton* sender);

		void OnReload (AbstractButton* sender);

		void OnOpen (AbstractButton* sender);

        GLuint vao_[2];

        GLBuffer<ARRAY_BUFFER, 2> vbo_;

		RefPtr<FrameShadow> shadow_;

		TextEntry* path_entry_;
		TextEntry* file_entry_;

		FileBrowser* browser_;

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

	};

}
