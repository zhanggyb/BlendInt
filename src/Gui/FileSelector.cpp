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
#include <BlendInt/Gui/HBlockLayout.hpp>
#include <BlendInt/Gui/FolderList.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

	FileSelector::FileSelector ()
	: BinLayout(),
	  path_entry_(0),
	  file_entry_(0)
	{
		set_size(500, 400);
		set_margin(2, 2, 2, 2);

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
			tool.GenerateVertices(*request.size(), 0, round_type(), round_radius());
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

	void FileSelector::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), 0, *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			GLArrayBuffer::reset();

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void FileSelector::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), 0, round_type(),
			        *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			GLArrayBuffer::reset();

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType FileSelector::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);
		program->reset();

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
		layout->SetMargin(2, 2, 2, 2);
		layout->SetSpace(0);

		ToolBar* toolbar = CreateToolBarOnce();
		ToolBox* sidebar = CreateSideBarOnce();
		VLayout* area = CreateBrowserAreaOnce();

		Splitter* splitter = Manage(new Splitter);
		DBG_SET_NAME(splitter, "Splitter");
		splitter->SetMargin(0, 0, 0, 0);
		splitter->Append(sidebar);
		splitter->Append(area);

		layout->Append(toolbar);
		layout->Append(splitter);

		Setup(layout);

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
		vbox->SetMargin(2, 2, 2, 2);
		vbox->SetSpace(4);

		path_entry_ = Manage(new TextEntry);
		DBG_SET_NAME(path_entry_, "Path Entry");
		path_entry_->SetRoundType(RoundAll);

		open_ = Manage(new Button(String("Open")));
		DBG_SET_NAME(open_, "Open Button");

		HLayout* dir_layout = Manage(new HLayout);
		DBG_SET_NAME(dir_layout, "DIR Layout");

		dir_layout->SetMargin(0, 0, 0, 0);
		dir_layout->Append(path_entry_);
		dir_layout->Append(open_);

		file_entry_ = Manage(new TextEntry);
		DBG_SET_NAME(file_entry_, "File Entry");

		file_entry_->SetRoundType(RoundAll);
		cancel_ = Manage(new Button(String("Cancel")));
		DBG_SET_NAME(cancel_, "Cancel Button");

		HLayout* file_layout = Manage(new HLayout);
		DBG_SET_NAME(file_layout, "File Layout");

		file_layout->SetMargin(0, 0, 0, 0);
		file_layout->Append(file_entry_);
		file_layout->Append(cancel_);

		browser_ = Manage(new FileBrowser);
		DBG_SET_NAME(browser_, "FileBrowser");

		vbox->Append(dir_layout);
		vbox->Append(file_layout);
		vbox->Append(browser_);

		return vbox;
	}

	ToolBar* FileSelector::CreateToolBarOnce()
	{
		ToolBar* toolbar = Manage(new ToolBar);
		DBG_SET_NAME(toolbar, "ToolBar");
		toolbar->SetMargin(2, 2, 2, 2);

		// directory control group
		HBlockLayout* block1 = Manage(new HBlockLayout);

		Button* btn_back = Manage(new Button(Icons::instance->icon_16x16(Stock::BACK)));
		Button* btn_forward = Manage(new Button(Icons::instance->icon_16x16(Stock::FORWARD)));
		Button* btn_up = Manage(new Button(Icons::instance->icon_16x16(Stock::FILE_PARENT)));
		Button* btn_reload = Manage(new Button(Icons::instance->icon_16x16(Stock::FILE_REFRESH)));

		block1->Append(btn_back);
		block1->Append(btn_forward);
		block1->Append(btn_up);
		block1->Append(btn_reload);

		block1->Resize(block1->GetPreferredSize());

		// create new
		Button* btn_new = Manage(new Button(Icons::instance->icon_16x16(Stock::NEWFOLDER), "Create New Directory"));

		// display mode
		HBlockLayout* block2 = Manage(new HBlockLayout);

		Button* btn_short_list = Manage(new Button(Icons::instance->icon_16x16(Stock::SHORTDISPLAY)));
		Button* btn_detail_list = Manage(new Button(Icons::instance->icon_16x16(Stock::LONGDISPLAY)));
		Button* btn_thumbnail = Manage(new Button(Icons::instance->icon_16x16(Stock::IMGDISPLAY)));

		block2->Append(btn_short_list);
		block2->Append(btn_detail_list);
		block2->Append(btn_thumbnail);

		HBlockLayout* block3 = Manage(new HBlockLayout);

		Button* btn_sort_alpha = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTALPHA)));
		Button* btn_sort_ext = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTBYEXT)));
		Button* btn_sort_time = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTTIME)));
		Button* btn_sort_size = Manage(new Button(Icons::instance->icon_16x16(Stock::SORTSIZE)));

		block3->Append(btn_sort_alpha);
		block3->Append(btn_sort_ext);
		block3->Append(btn_sort_time);
		block3->Append(btn_sort_size);

		toolbar->Append(block1);
		toolbar->Append(btn_new);
		toolbar->Append(block2);
		toolbar->Append(block3);

		return toolbar;
	}

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
