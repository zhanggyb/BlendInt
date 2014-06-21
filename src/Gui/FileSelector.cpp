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

		InitializeFileBrowser();
	}

	FileSelector::~FileSelector ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	void FileSelector::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
									static_cast<const Size*>(request.data());
					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());
					m_inner->Reset();

					break;
				}

				default:
					break;
			}

			Frame::UpdateGeometry(request);
		}
	}

	ResponseType FileSelector::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.0f);
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		program->Reset();

		return AcceptAndContinue;
	}

	void FileSelector::InitializeFileBrowser ()
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

		ScrollArea* area = Manage(new ScrollArea);

		m_list = Manage(new FileBrowser);
		area->SetViewport(m_list);

		m_layout->PushBack(dir_layout);
		m_layout->PushBack(file_layout);
		m_layout->PushBack(area);

		Setup(m_layout);
	}

}
