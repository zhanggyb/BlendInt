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
#include <BlendInt/Gui/HBox.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	FileSelector::FileSelector ()
	: layout_(0), path_entry_(0)
	{
		set_size(500, 400);
		set_margin(2, 2, 2, 2);
		set_drop_shadow(true);

		InitializeFileSelector();
	}

	FileSelector::~FileSelector ()
	{
		glDeleteVertexArrays(1, &vao_);
	}
	
	void FileSelector::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			inner_->Reset();
		}

		ReportSizeUpdate(request);
	}

	ResponseType FileSelector::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		program->Reset();

		return Ignore;
	}

	void FileSelector::InitializeFileSelector ()
	{
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);
		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		tool.SetInnerBufferData(inner_.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_->Reset();

		// create sub widgets

		layout_ = Manage(new VBox);
		layout_->SetMargin(2, 2, 2, 2);
		layout_->SetSpace(4);

		path_entry_ = Manage(new TextEntry);
		path_entry_->SetRoundCornerType(RoundAll);

		open_ = Manage(new Button(String("Open")));

		HBox* dir_layout = Manage(new HBox);
		dir_layout->SetMargin(0, 0, 0, 0);
		dir_layout->PushBack(path_entry_);
		dir_layout->PushBack(open_);

		file_entry_ = Manage(new TextEntry);
		file_entry_->SetRoundCornerType(RoundAll);
		cancel_ = Manage(new Button(String("Cancel")));

		HBox* file_layout = Manage(new HBox);
		file_layout->SetMargin(0, 0, 0, 0);
		file_layout->PushBack(file_entry_);
		file_layout->PushBack(cancel_);

		browser_ = Manage(new FileBrowser);

		layout_->PushBack(dir_layout);
		layout_->PushBack(file_layout);
		layout_->PushBack(browser_);

		Setup(layout_);

		std::string pwd =getenv("PWD");
		pwd.append("/");
		path_entry_->SetText(pwd);
		browser_->Load(getenv("PWD"));

		events()->connect(browser_->clicked(), this, &FileSelector::OnFileSelect);
	}

	void FileSelector::OnFileSelect ()
	{
		file_entry_->SetText(browser_->file_selected());
	}

}
