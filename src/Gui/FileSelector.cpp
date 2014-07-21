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
	: m_layout(0), m_path_entry(0)
	{
		set_size(500, 400);
		set_margin(2, 2, 2, 2);
		set_drop_shadow(true);

		InitializeFileSelector();
	}

	FileSelector::~FileSelector ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	void FileSelector::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_inner->Reset();
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

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		program->Reset();

		return Ignore;
	}

	void FileSelector::InitializeFileSelector ()
	{
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);
		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		m_inner->Reset();

		// create sub widgets

		m_layout = Manage(new VBox);
		m_layout->SetMargin(2, 2, 2, 2);
		m_layout->SetSpace(4);

		m_path_entry = Manage(new TextEntry);
		m_path_entry->SetRoundCornerType(RoundAll);

		m_open = Manage(new Button(String("Open")));

		HBox* dir_layout = Manage(new HBox);
		dir_layout->SetMargin(0, 0, 0, 0);
		dir_layout->PushBack(m_path_entry);
		dir_layout->PushBack(m_open);

		m_file_entry = Manage(new TextEntry);
		m_file_entry->SetRoundCornerType(RoundAll);
		m_cancel = Manage(new Button(String("Cancel")));

		HBox* file_layout = Manage(new HBox);
		file_layout->SetMargin(0, 0, 0, 0);
		file_layout->PushBack(m_file_entry);
		file_layout->PushBack(m_cancel);

		m_list = Manage(new FileBrowser);

		m_layout->PushBack(dir_layout);
		m_layout->PushBack(file_layout);
		m_layout->PushBack(m_list);

		Setup(m_layout);

		m_list->Open(getenv("PWD"));
	}

}
