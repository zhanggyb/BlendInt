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
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/FolderList.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FolderList::FolderList()
	: AbstractWidget()
	{
		set_round_type(RoundAll);
		set_size(240, 160);

		InitializeFolderListOnce();
	}

	FolderList::~FolderList()
	{
		glDeleteVertexArrays(2, vao_);
	}

	Size FolderList::GetPreferredSize () const
	{
		return Size(240, 160);
	}

	ResponseType FolderList::Draw (Profile& profile)
	{
		Shaders::instance->triangle_program()->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (GLfloat)position().x(), (GLfloat)position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR),
				Theme::instance->regular().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 1);
		glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR),
				Theme::instance->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);

			glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		Shaders::instance->triangle_program()->reset();

		return Accept;
	}

	ResponseType FolderList::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType FolderList::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType FolderList::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType FolderList::ContextMenuPressEvent (
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FolderList::ContextMenuReleaseEvent (
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FolderList::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType FolderList::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType FolderList::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void FolderList::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(),
			        round_type(), round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_size(*request.size());

			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void FolderList::PerformRoundTypeUpdate (
			const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_round_type(*request.round_type());

			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void FolderList::PerformRoundRadiusUpdate (
			const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(),
			        round_type(), *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_round_radius(*request.round_radius());

			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	void FolderList::InitializeFolderListOnce ()
	{
		VertexTool tool;
		tool.GenerateVertices (size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
