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
		tool.Setup (size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(4, vaos_);
		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		tool.SetInnerBufferData(inner_.get());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->Generate();
		outer_->Bind();
		tool.SetOuterBufferData(outer_.get());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[2]);
		area_.reset(new GLArrayBuffer);
		area_->Generate();
		area_->Bind();

		GLfloat verts1[] = {
				0.f, 0.f,
				350.f, 0.f,
				0.f, 250.f,
				350.f, 250.f
		};

		area_->SetData(sizeof(verts1), verts1);
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[3]);
		rect_.reset(new GLArrayBuffer);
		rect_->Generate();
		rect_->Bind();

		GLfloat verts2[] = {
				0.f, 0.f,
				200.f, 0.f,
				0.f, 200.f,
				200.f, 200.f
		};

		rect_->SetData(sizeof(verts2), verts2);
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	NodeView::~NodeView ()
	{
		glDeleteVertexArrays(4, vaos_);
	}

	void NodeView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), DefaultBorderWidth(),
			        round_type(), round_radius());
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void NodeView::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

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
			tool.Setup(size(), DefaultBorderWidth(),
			        round_type(), *request.round_radius());
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_round_radius(*request.round_radius());
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType NodeView::Draw(const Profile& profile)
	{
		RefPtr<GLSLProgram> program =
						Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
				Theme::instance->regular().inner.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		// Stencil test
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glClearStencil(0);	// default is 0
		glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
		//glDepthMask(GL_FALSE);
		glStencilFunc(GL_NEVER, 1, 0xFF);	// GL_NEVER: always fails
		glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)

		// draw stencil pattern
		//glStencilMask(0xFF);

		//glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 1.f, 1.f,
		//		1.f, 1.f);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);	// GL_NEVER: always fails
		glStencilOp(GL_INCR, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x() + 25, (float) position().y() + 25, 0.f);
		glBindVertexArray(vaos_[2]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//glDepthMask(GL_TRUE);

		//glStencilMask(0x00);

		// draw where stencil's value is 0
		//glStencilFunc(GL_EQUAL, 0, 0xFF);
		/* (nothing to draw) */

		// draw only where stencil's value is 1
		glStencilFunc(GL_EQUAL, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);
		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x() + 300, (float) position().y() + 150, 0.f);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 1.f, 0.2f, 0.2f, 0.9f);
		glBindVertexArray(vaos_[3]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisable(GL_STENCIL_TEST);

		// Stencil test end

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.f, 0.f, 1.f, 1.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);

		program->Reset();

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
