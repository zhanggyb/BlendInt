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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/LinearLayout.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/FolderList.hpp>

#include <BlendInt/Gui/CloseButton.hpp>

#include <BlendInt/Gui/Separator.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	FileSelectorDecoration::FileSelectorDecoration ()
	: AbstractDecoration()
	{
		LinearLayout* hlayout = Manage(new LinearLayout);
		DBG_SET_NAME(hlayout, "FileSelectorDecorationLayout");

		// create close button
		CloseButton* close_button = Manage(new CloseButton);

		// directory control group
		Block* block1 = Manage(new Block);

		Button* btn_back = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::BACK)));
		Button* btn_forward = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::FORWARD)));
		Button* btn_up = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::FILE_PARENT)));
		Button* btn_reload = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::FILE_REFRESH)));

		block1->AddWidget(btn_back);
		block1->AddWidget(btn_forward);
		block1->AddWidget(btn_up);
		block1->AddWidget(btn_reload);

		block1->Resize(block1->GetPreferredSize());

		// create new
		Button* btn_new = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::NEWFOLDER), "Create New Directory"));

		// display mode
		Block* block2 = Manage(new Block);

		Button* btn_short_list = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::SHORTDISPLAY)));
		Button* btn_detail_list = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::LONGDISPLAY)));
		Button* btn_thumbnail = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::IMGDISPLAY)));

		block2->AddWidget(btn_short_list);
		block2->AddWidget(btn_detail_list);
		block2->AddWidget(btn_thumbnail);

		Block* block3 = Manage(new Block);

		Button* btn_sort_alpha = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::SORTALPHA)));
		Button* btn_sort_ext = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::SORTBYEXT)));
		Button* btn_sort_time = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::SORTTIME)));
		Button* btn_sort_size = Manage(new Button(AbstractWindow::icons->icon_16x16(Icons::SORTSIZE)));

		block3->AddWidget(btn_sort_alpha);
		block3->AddWidget(btn_sort_ext);
		block3->AddWidget(btn_sort_time);
		block3->AddWidget(btn_sort_size);

		Button* open = Manage(new Button(String("Open")));
		DBG_SET_NAME(open, "Open Button");

		Separator* separator1 = Manage(new Separator);
		Separator* separator2 = Manage(new Separator);
		Separator* separator3 = Manage(new Separator(true));

		hlayout->AddWidget(close_button);
		hlayout->AddWidget(separator1);
		hlayout->AddWidget(block1);
		hlayout->AddWidget(separator2);
		hlayout->AddWidget(btn_new);
		hlayout->AddWidget(block2);
		hlayout->AddWidget(block3);
		hlayout->AddWidget(separator3);
		hlayout->AddWidget(open);

		hlayout->Resize(hlayout->GetPreferredSize());

		PushBackSubView(hlayout);

		set_size(hlayout->size());

		events()->connect(close_button->clicked(), this, &FileSelectorDecoration::OnCloseButtonClicked);
	}

	FileSelectorDecoration::~FileSelectorDecoration ()
	{
	}

	bool FileSelectorDecoration::IsExpandX () const
	{
		return true;
	}

	bool FileSelectorDecoration::IsExpandY () const
	{
		return false;
	}

	Size FileSelectorDecoration::GetPreferredSize () const
	{
		return first_subview()->GetPreferredSize();
	}

	void FileSelectorDecoration::PerformSizeUpdate (
	        const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			ResizeSubView(first_subview(), size());

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FileSelectorDecoration::OnCloseButtonClicked(AbstractButton* button)
	{
		fire_close_triggered();
	}

	// -----------------------------

	FileSelector::FileSelector ()
	: Dialog(),
	  path_entry_(0),
	  file_entry_(0)
	{
		set_size(500, 400);

		InitializeFileSelector();

		//events()->connect(cancel_->clicked(), this, &FileSelector::OnCancel);
	}

	FileSelector::~FileSelector ()
	{
	}
	
	void FileSelector::InitializeFileSelector ()
	{
		FileSelectorDecoration* dec = Manage(new FileSelectorDecoration);

		//ToolBox* sidebar = CreateSideBarOnce();
		LinearLayout* area = CreateBrowserAreaOnce();

		SetDecoration(dec);
		SetLayout(area);

		std::string pwd =getenv("PWD");
		pwd.append("/");
		path_entry_->SetText(pwd);
		browser_->Load(getenv("PWD"));

		events()->connect(dec->close_triggered(), this, &FileSelector::OnCloseButtonClicked);

		events()->connect(browser_->clicked(), this, &FileSelector::OnFileSelect);
		//events()->connect(open_->clicked(), &opened_, &Cpp::Event<>::fire);
		//events()->connect(cancel_->clicked(), &canceled_, &Cpp::Event<>::fire);
	}

	void FileSelector::OnFileSelect ()
	{
		file_entry_->SetText(browser_->file_selected());
	}

	void FileSelector::OnCloseButtonClicked()
	{
		delete this;
	}

	LinearLayout* FileSelector::CreateBrowserAreaOnce()
	{
		LinearLayout* vbox = Manage(new LinearLayout(Vertical));
		DBG_SET_NAME(vbox, "VBox in Broser Area");
		vbox->SetSpace(2);

		path_entry_ = Manage(new TextEntry);
		DBG_SET_NAME(path_entry_, "Path Entry");
		path_entry_->SetRoundType(RoundAll);

		file_entry_ = Manage(new TextEntry);
		DBG_SET_NAME(file_entry_, "File Entry");
		file_entry_->SetRoundType(RoundAll);

		browser_ = Manage(new FileBrowser);
		DBG_SET_NAME(browser_, "FileBrowser");

		vbox->AddWidget(path_entry_);
		vbox->AddWidget(file_entry_);
		vbox->AddWidget(browser_);

		return vbox;
	}

	/*
	ToolBox* FileSelector::CreateSideBarOnce ()
	{
		ToolBox* toolbox = Manage(new ToolBox);
		DBG_SET_NAME(toolbox, "SideBar");
		toolbox->SetMargin(2, 2, 2, 2);

		Expander* exp1 = CreateSystemDevicesOnce();
		Expander* exp2 = CreateSystemBookmarksOnce();

		toolbox->Append(exp1);
		toolbox->Append(exp2);

		return toolbox;
	}
	*/

	Expander* FileSelector::CreateSystemDevicesOnce ()
	{
		Expander* expander = Manage(new Expander("System"));
		DBG_SET_NAME(expander, "System Expander");

		FolderList* system_folders = Manage(new FolderList);
		DBG_SET_NAME(system_folders, "System Folders");

		expander->Setup(system_folders);

		return expander;
	}

	Expander* FileSelector::CreateSystemBookmarksOnce ()
	{
		Expander* expander = Manage(new Expander("System Bookmarks"));
		DBG_SET_NAME(expander, "System Bookmarks Expander");

		FolderList* system_bookmark = Manage(new FolderList);
		DBG_SET_NAME(system_bookmark, "System Bookmarks");

		expander->Setup(system_bookmark);

		return expander;
	}

}
