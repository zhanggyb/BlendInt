/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
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
#include <BlendInt/Gui/NodeView.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	NodeView::NodeView()
	: AbstractWidget()
	{
		set_size (400, 300);
		set_round_type(RoundAll);
		set_round_radius(10.f);
		//set_drop_shadow(true);

		VertexTool tool;
		tool.GenerateVertices (size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(4, vaos_);
		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[2]);
		area_.reset(new GLArrayBuffer);
		area_->generate();
		area_->bind();

		GLfloat verts1[] = {
				0.f, 0.f,
				350.f, 0.f,
				0.f, 250.f,
				350.f, 250.f
		};

		area_->set_data(sizeof(verts1), verts1);
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[3]);
		rect_.reset(new GLArrayBuffer);
		rect_->generate();
		rect_->bind();

		GLfloat verts2[] = {
				0.f, 0.f,
				200.f, 0.f,
				0.f, 200.f,
				200.f, 200.f
		};

		rect_->set_data(sizeof(verts2), verts2);
		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	NodeView::~NodeView ()
	{
		glDeleteVertexArrays(4, vaos_);
	}

	void NodeView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(),
			        round_type(), round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void NodeView::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_round_type(*request.round_type());
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void NodeView::PerformRoundRadiusUpdate (
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

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType NodeView::Draw(Profile& profile)
	{
		RefPtr<GLSLProgram> program =
						Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR),
				Theme::instance->regular().inner.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		profile.BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		profile.EndPushStencil();

		// Stack 2
		profile.BeginPushStencil();	// area stencil

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x() + 25, (float) position().y() + 425, 0.f);
		glBindVertexArray(vaos_[2]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		profile.EndPushStencil();

		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);
		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x() + 300, (float) position().y() + 450, 0.f);
		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 1.f, 0.2f, 0.2f, 0.9f);
		glBindVertexArray(vaos_[3]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		profile.BeginPopStencil();	// pop area stencil
		glBindVertexArray(vaos_[2]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		profile.EndPopStencil();

		profile.BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		profile.EndPopStencil();


		// Stencil test end

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 1);
		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 0.f, 0.f, 1.f, 1.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);

		program->reset();

		return Accept;
	}

	ResponseType NodeView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType NodeView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType NodeView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType NodeView::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType NodeView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType NodeView::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType NodeView::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType NodeView::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

}
