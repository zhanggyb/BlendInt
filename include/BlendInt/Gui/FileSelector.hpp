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

#include <BlendInt/Gui/VBox.hpp>
#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/FileBrowser.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>

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

		virtual void UpdateGeometry (const GeometryUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

	private:

		void InitializeFileSelector ();

		GLuint m_vao;
		RefPtr<GLArrayBuffer> m_inner;

		VBox* m_layout;

		TextEntry* m_path_entry;
		TextEntry* m_file_entry;

		Button* m_open;
		Button* m_cancel;

		FileBrowser* m_list;

	};

}

#endif /* _BLENDINT_GUI_FILESELECTOR_HPP_ */
