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

#include <BlendInt/Gui/FileSystemModel.hpp>

namespace BlendInt {

	FileSystemModel::FileSystemModel()
	: ListModel()
	{

	}

	FileSystemModel::~FileSystemModel()
	{

	}

	bool FileSystemModel::Load (const std::string& pathname)
	{
		namespace fs = boost::filesystem;
		bool is_path = false;

		m_path = fs::path(pathname);
		fs::file_status status;
		char buf[32];

		ItemDeque* row = 0;

		try {
			if (fs::exists(m_path)) {

				if (fs::is_directory(m_path)) {

					fs::directory_iterator it(m_path);
					fs::directory_iterator it_end;
					while (it != it_end) {

						status = fs::status(it->path());

						row = new ItemDeque;
						snprintf(buf, 32, "%o", status.permissions());
						row->push_back(String(buf));
						row->push_back(it->path().filename().native());
						PushBack(row);

						it++;
					}

					is_path = true;
				}
			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

		return is_path;
	}

}
