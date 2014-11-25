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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/FolderList.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/Separator.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

	FileSelector::FileSelector ()
	: BinLayout(),
	  path_entry_(0),
	  file_entry_(0)
	{
		set_size(500, 400);

		InitializeFileSelector();

		//events()->connect(cancel_->clicked(), this, &FileSelector::OnCancel);
	}

	FileSelector::~FileSelector ()
	{
		glDeleteVertexArrays(1, &vao_);
	}
	
	void FileSelector::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 5.f);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			BinLayout::PerformSizeUpdate(request);
			return;
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FileSelector::PerformRoundTypeUpdate (int round_type)
	{
		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone,
				5.f);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		GLArrayBuffer::reset();

		Refresh();
	}

	void FileSelector::PerformRoundRadiusUpdate (float radius)
	{
		VertexTool tool;
		tool.GenerateVertices(size(), 0, round_type(),
				radius);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		GLArrayBuffer::reset();

		Refresh();
	}

	ResponseType FileSelector::Draw (Profile& profile)
	{
		Shaders::instance->widget_triangle_program()->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		GLSLProgram::reset();

		return Ignore;
	}

	void FileSelector::InitializeFileSelector ()
	{
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);
		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0.f);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_->reset();

		// create sub widgets
		VLayout* layout = Manage(new VLayout);
		DBG_SET_NAME(layout, "Main Layout");
		layout->SetMargin(Margin(2, 2, 2, 2));
		layout->SetSpace(0);

		HLayout* toolbar = CreateToolButtonsOnce();
		//ToolBox* sidebar = CreateSideBarOnce();
		VLayout* area = CreateBrowserAreaOnce();

		//Splitter* splitter = Manage(new Splitter);
		//DBG_SET_NAME(splitter, "Splitter");
		//splitter->Append(sidebar);
		//splitter->Append(area);

		layout->AddWidget(toolbar);
		//layout->Append(splitter);
		layout->AddWidget(area);

		AddWidget(layout);

		std::string pwd =getenv("PWD");
		pwd.append("/");
		path_entry_->SetText(pwd);
		browser_->Load(getenv("PWD"));

		events()->connect(browser_->clicked(), this, &FileSelector::OnFileSelect);
		events()->connect(open_->clicked(), &opened_, &Cpp::Event<>::fire);
		events()->connect(cancel_->clicked(), &canceled_, &Cpp::Event<>::fire);
	}

	void FileSelector::OnFileSelect ()
	{
		file_entry_->SetText(browser_->file_selected());
	}

	VLayout* FileSelector::CreateBrowserAreaOnce()
	{
		VLayout* vbox = Manage(new VLayout);
		DBG_SET_NAME(vbox, "VBox in Broser Area");
		vbox->SetMargin(Margin(2, 2, 2, 2));
		vbox->SetSpace(4);

		path_entry_ = Manage(new TextEntry);
		DBG_SET_NAME(path_entry_, "Path Entry");
		path_entry_->SetRoundType(RoundAll);

		open_ = Manage(new Button(String("Open")));
		DBG_SET_NAME(open_, "Open Button");

		HLayout* dir_layout = Manage(new HLayout);
		DBG_SET_NAME(dir_layout, "DIR Layout");

		dir_layout->SetMargin(Margin(0, 0, 0, 0));
		dir_layout->AddWidget(path_entry_);
		dir_layout->AddWidget(open_);

		file_entry_ = Manage(new TextEntry);
		DBG_SET_NAME(file_entry_, "File Entry");

		file_entry_->SetRoundType(RoundAll);
		cancel_ = Manage(new Button(String("Cancel")));
		DBG_SET_NAME(cancel_, "Cancel Button");

		HLayout* file_layout = Manage(new HLayout);
		DBG_SET_NAME(file_layout, "File Layout");

		file_layout->SetMargin(Margin(0, 0, 0, 0));
		file_layout->AddWidget(file_entry_);
		file_layout->AddWidget(cancel_);

		browser_ = Manage(new FileBrowser);
		DBG_SET_NAME(browser_, "FileBrowser");

		vbox->AddWidget(dir_layout);
		vbox->AddWidget(file_layout);
		vbox->AddWidget(browser_);

		return vbox;
	}

	HLayout* FileSelector::CreateToolButtonsOnce()
	{
		HLayout* hlayout = Manage(new HLayout);
		DBG_SET_NAME(hlayout, "ToolBar Layout");

		// directory control group
		Block* block1 = Manage(new Block);

		Button* btn_back = Manage(new Button(Icons::instance->icon_16x16(Stock::BACK)));
		Button* btn_forward = Manage(new Button(Icons::instance->icon_16x16(Stock::FORWARD)));
		Button* btn_up = Manage(new Button(Icons::instance->icon_16x16(Stock::FILE_PARENT)));
		Button* btn_reload = Manage(new Button(Icons::instance->icon_16x16(Stock::FILE_REFRESH)));

		block1->AddWidget(btn_back);
		block1->AddWidget(btn_forward);
		block1->AddWidget(btn_up);
		block1->AddWidget(btn_reload);

		block1->Resize(block1->GetPreferredSize());

		// create new
		Button* btn_new = Manage(new Button(Icons::instance->icon_16x16(Stock::NEWFOLDER), "Create New Directory"));

		// display mode
		Block* block2 = Manage(new Block);

		Button* btn_short_list = Manage(new Button(Icons::instance->icon_16x16(Stock::SHORTDISPLAY)));
		Button* btn_detail_list = Manage(new Button(Icons::instance->icon_16x16(Stock::LONGDISPLAY)));
		Button* btn_thumbnail = Manage(new Button(Icons::instance->icon_16x16(Stock::IMGDISPLAY)));

		block2->AddWidget(btn_short_list);
		block2->AddWidget(btn_detail_list);
		block2->AddWidget(btn_thumbnail);

		Block* block3 = Manage(new Block);

		Button* btn_sort_alpha = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTALPHA)));
		Button* btn_sort_ext = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTBYEXT)));
		Button* btn_sort_time = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTTIME)));
		Button* btn_sort_size = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTSIZE)));

		block3->AddWidget(btn_sort_alpha);
		block3->AddWidget(btn_sort_ext);
		block3->AddWidget(btn_sort_time);
		block3->AddWidget(btn_sort_size);

		Separator* separator1 = Manage(new Separator);
		Separator* separator2 = Manage(new Separator);
		Separator* separator3 = Manage(new Separator(true));

		hlayout->AddWidget(separator1);
		hlayout->AddWidget(block1);
		hlayout->AddWidget(separator2);
		hlayout->AddWidget(btn_new);
		hlayout->AddWidget(block2);
		hlayout->AddWidget(block3);
		hlayout->AddWidget(separator3);

		return hlayout;
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
