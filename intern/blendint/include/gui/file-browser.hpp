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

#include <string>
#include <deque>

#include <opengl/gl-buffer.hpp>

#include <gui/font.hpp>
#include <gui/abstract-item-view.hpp>
#include <gui/filesystem-model.hpp>

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

		/**
		 * @brief Load and list all files in given pathname
		 */
		bool Open (const std::string& pathname);

		/**
		 * @brief Load and list the parent folder
		 */
		bool OpenParent ();

		/**
		 * @brief Go backward
		 */
		bool GoBackward ();

		/**
		 * @brief Go forward
		 */
		bool GoForward ();

		const String& file_selected () const
		{
			return file_selected_;
		}

		const std::string& pathname () const
		{
			return pathname_;
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

		Cpp::EventRef<> selected ()
		{
			return selected_;
		}

	protected:

		virtual Response Draw (AbstractWindow* context);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual Response PerformMousePress (AbstractWindow* context);

	private:

		void InitializeFileBrowserOnce ();

		GLuint vaos_[2];

		Font font_;

		/**
		 * 0 - for inner
		 * 1 - for row
		 */
		GLBuffer<ARRAY_BUFFER, 2> buffer_;

		String file_selected_;

		std::string pathname_;

		std::deque<std::string> history_;

		size_t history_index_;

		RefPtr<FileSystemModel> model_;

		int highlight_index_;

		Cpp::Event<> selected_;
	};

}
