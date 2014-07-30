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

#ifndef _BLENDINT_GUI_FILESYSTEMMODEL_HPP_
#define _BLENDINT_GUI_FILESYSTEMMODEL_HPP_

#include <boost/filesystem.hpp>

#include <BlendInt/Gui/AbstractItemModel.hpp>

namespace BlendInt {

	/**
	 * @brief A data model based on boost::filesystem for the local file system
	 *
	 * This data model contains 6 columns by default:
	 * <Name> - <Type> - <Owner> - <Group> - <Permissions> - <Last write time>
	 *
	 * The default constructor does nothing, use Load() to load and store a file
	 * list in a path.
	 */
	class FileSystemModel: public AbstractItemModel
	{
	public:

		FileSystemModel ();

		virtual ~FileSystemModel ();

		/**
		 * @brief List files in a path
		 * @param pathname The path name
		 * @return
		 * 	- true success
		 * 	- false failure
		 */
		bool Load (const std::string& pathname);

		void Clear ();

		virtual int GetRows (const ModelIndex& parent = ModelIndex()) const;

		virtual int GetColumns (const ModelIndex& parent = ModelIndex()) const;

		virtual bool InsertColumns (int column, int count, const ModelIndex& parent = ModelIndex());

		virtual bool RemoveColumns (int column, int count, const ModelIndex& parent = ModelIndex());

		virtual bool InsertRows (int row, int count, const ModelIndex& parent = ModelIndex());

		virtual bool RemoveRows (int row, int count, const ModelIndex& parent = ModelIndex());

		virtual ModelIndex GetRootIndex () const;

		virtual ModelIndex GetIndex (int row, int column, const ModelIndex& parent = ModelIndex()) const;

#ifdef DEBUG

		void Print ();

		void PrintRow (ModelNode* first);

#endif	// DEBUG

	protected:

		const ModelNode* root () const
		{
			return root_;
		}

	private:

		void InsertColumns (int column, int count, ModelNode* left);

		void DestroyColumnsInRow (int column, int count, ModelNode* node);

		static void DestroyRow (ModelNode* node);

		static void DestroyColumn (ModelNode* node);

		boost::filesystem::path path_;

		int rows_;

		int columns_;

		ModelNode* root_;

		static const int DefaultColumns = 5;

	};

}

#endif /* _BLENDINT_GUI_FILESYSTEMMODEL_HPP_ */
