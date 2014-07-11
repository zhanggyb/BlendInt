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

#ifndef _BLENDINT_GUI_FILEBROWSER_HPP_
#define _BLENDINT_GUI_FILEBROWSER_HPP_

#include <string>
#include <boost/filesystem.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/ListView.hpp>

namespace BlendInt {

	/**
	 * @brief A special list view to show files in a directory
	 */
	class FileBrowser: public ListView
	{
		DISALLOW_COPY_AND_ASSIGN(FileBrowser);

	public:

		enum FileBrowserMode {
			FileList,
			FileIcon
		};

		FileBrowser ();

		virtual ~FileBrowser ();

		bool Open (const std::string& pathname);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

	private:

		bool GetHighlightIndex (int y, unsigned int* index);

		void InitializeFileListOnce ();

		GLuint m_vao;

		boost::filesystem::path m_path;

		Font m_font;

		RefPtr<GLArrayBuffer> m_row;

		unsigned int m_index;	// Highlight index
	};

}

#endif /* _BLENDINT_GUI_FILEBROWSER_HPP_ */
