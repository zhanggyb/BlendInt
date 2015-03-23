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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <opengl/opengl.hpp>

#include <gui/folder-list.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	FolderList::FolderList()
	: AbstractRoundWidget()
	{
		set_round_type(RoundAll);
		set_size(240, 160);

		InitializeFolderListOnce();

		text_.reset(new Text(String(L"Hello World!")));
	}

	FolderList::~FolderList()
	{
		glDeleteVertexArrays(2, vao_);
	}

	Size FolderList::GetPreferredSize () const
	{
		return Size(240, 160);
	}

	Response FolderList::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders()->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED), 0);
		glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
				AbstractWindow::theme()->regular().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

		AbstractWindow::shaders()->widget_outer_program()->use();

		glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
		        0.f, 0.f);
		glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
				AbstractWindow::theme()->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		             outline_vertex_count(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
			        0.f, -1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        emboss_vertex_count(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		text_->Draw(0.f, 0.f);

		return Finish;
	}

	void FolderList::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			GenerateRoundedVertices(&inner_verts, &outer_verts);

			vbo_.bind(0);
			vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.bind(1);
			vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			vbo_.reset();

			RequestRedraw();
		}

		ReportSizeUpdate(request);
	}

	void FolderList::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	void FolderList::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		vbo_.reset();

		RequestRedraw();
	}

	void FolderList::InitializeFolderListOnce ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateRoundedVertices(&inner_verts, &outer_verts);

		vbo_.generate();

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		vbo_.reset();
	}

}
