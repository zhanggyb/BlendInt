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
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Dialog.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Dialog::Dialog()
	: AbstractFrame(),
	  vao_(0)
	{
		set_size(400, 300);

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);
	}

	Dialog::~Dialog()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Dialog::Setup(Widget* widget)
	{
		if(widget == 0) return;

		if(widget->parent() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(widget->size());

		if(PushBackSubWidget(widget)) {
			widget->SetPosition(0, 0);
		}
	}

	void Dialog::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(request.position()->x()  + offset().x());
			float y = static_cast<float>(request.position()->y()  + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)size().width(),
				y,
				y + (float)size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

			set_position(*request.position());
		}

		if(request.source() == this) {
			ReportPositionUpdate (request);
		}
	}

	void Dialog::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			float x = static_cast<float>(position().x() + offset().x());
			float y = static_cast<float>(position().y() + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			std::vector<GLfloat> inner_verts;
			GenerateVertices(*request.size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			buffer_.bind();
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.reset();

			set_size(*request.size());

			if (subs_count()) {
				assert(subs_count() == 1);
				FillSingleWidget(0, 0, 0, size().width(), size().height());
			}

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Dialog::PreDraw(Profile& profile)
	{
		assign_profile_frame(profile);

		/*
		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);
		*/
	}

	ResponseType Dialog::Draw(Profile& profile)
	{
		Shaders::instance->frame_inner_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(Shaders::instance->location(Stock::FRAME_INNER_GAMMA), 0);

		if(hover()) {
			glUniform4f(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 0.75f, 0.15f, 0.15f, 0.6f);
		} else {
			glUniform4f(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 0.75f, 0.75f, 0.75f, 0.6f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		/*
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}
		*/

		return subs_count() ? Ignore : Accept;
	}

	void Dialog::PostDraw(Profile& profile)
	{
		/*
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());
		*/
	}

	void Dialog::FocusEvent(bool focus)
	{
	}

	void Dialog::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Dialog::MouseHoverOutEvent(const MouseEvent& event)
	{
	}

	ResponseType Dialog::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Dialog::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Dialog::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Dialog::MousePressEvent(const MouseEvent& event)
	{
		set_event_frame(event);

		last_ = position();
		cursor_ = event.position();

		return Accept;
	}

	ResponseType Dialog::MouseReleaseEvent(const MouseEvent& event)
	{
		return Accept;
	}

	void Dialog::DispatchHoverEvent(const MouseEvent& event)
	{
	}

	ResponseType Dialog::MouseMoveEvent(const MouseEvent& event)
	{
		if(pressed_ext()) {

			int ox = event.position().x() - cursor_.x();
			int oy = event.position().y() - cursor_.y();

			set_position(last_.x() + ox, last_.y() + oy);

			Refresh();

			return Accept;

		}

		return Ignore;
	}

}
