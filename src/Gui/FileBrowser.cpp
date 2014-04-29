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

#include <BlendInt/Gui/FileBrowser.hpp>
#include <BlendInt/Gui/HLayout.hpp>

namespace BlendInt {

	FileBrowser::FileBrowser ()
	: m_layout(0), m_path_entry(0)
	{
		set_preferred_size(500, 400);
		set_size(500, 400);
		set_margin(2, 2, 2, 2);

		InitializeFileBrowser();
	}

	FileBrowser::~FileBrowser ()
	{
	}

	void FileBrowser::InitializeFileBrowser ()
	{
		m_layout = Manage(new VLayout);
		m_layout->SetMargin(2, 2, 2, 2);
		m_layout->SetSpace(4);

		m_path_entry = Manage(new TextEntry);
		m_path_entry->SetRoundType(RoundAll);
		//m_path_entry->SetPreferredSize(65536, m_path_entry->preferred_size().height());
		m_open = Manage(new Button);
		m_open->SetExpandX(false);
		//m_open->SetText("Open");

		HLayout* dir_layout = Manage(new HLayout);
		dir_layout->SetMargin(0, 0, 0, 0);
		dir_layout->Add(m_path_entry);
		dir_layout->Add(m_open);

		m_file_entry = Manage(new TextEntry);
		m_file_entry->SetRoundType(RoundAll);
		m_cancel = Manage(new Button);
		m_cancel->SetExpandX(false);
		//m_cancel->SetText("Cancel");

		HLayout* file_layout = Manage(new HLayout);
		file_layout->SetMargin(0, 0, 0, 0);
		file_layout->Add(m_file_entry);
		file_layout->Add(m_cancel);

		ScrollArea* area = Manage(new ScrollArea);

		m_list = Manage(new DirList);
		area->SetViewport(m_list);

		m_layout->Add(dir_layout);
		m_layout->Add(file_layout);
		m_layout->Add(area);

		Add(m_layout);

		int x = position().x() + margin().left();
		int y = position().y() + margin().right();
		unsigned int w = size().width() - horizontal_margins();
		unsigned int h = size().height() - vertical_margins();

		SetSubWidgetPosition(m_layout, x, y);
		ResizeSubWidget(m_layout, w, h);
	}

}
