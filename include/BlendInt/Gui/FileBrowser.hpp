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

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/AbstractItemView.hpp>
#include <BlendInt/Gui/FileSystemModel.hpp>

namespace BlendInt {

	/**
	 * @brief A special item view to show files in a directory
	 */
	class FileBrowser: public AbstractItemView
	{
		DISALLOW_COPY_AND_ASSIGN(FileBrowser);

	public:

		enum DisplayMode {
			ListMode,
			IconMode,
			TreeMode
		};

		FileBrowser ();

		virtual ~FileBrowser ();

		bool Load (const std::string& pathname);

		const String& file_selected () const
		{
			return file_selected_;
		}

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual const RefPtr<AbstractItemModel> GetModel () const;

		/**
		 * @brief Set the model used in this item view
		 * @param model A RefPtr to a item model, must be FileSystemModel
		 */
		virtual void SetModel (const RefPtr<AbstractItemModel>& model);

		virtual ModelIndex GetIndexAt (const Point& point) const;

		Cpp::EventRef<> clicked ()
		{
			return clicked_;
		}

	protected:

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void InitializeFileBrowserOnce ();

		void OnHBarSlide (int val);

		void OnVBarSlide (int val);

		GLuint vao_;

		Font font_;

		RefPtr<GLArrayBuffer> inner_;

		String file_selected_;

		RefPtr<FileSystemModel> model_;

		int highlight_index_;

		Cpp::Event<> clicked_;
	};

}

#endif /* _BLENDINT_GUI_FILEBROWSER_HPP_ */
