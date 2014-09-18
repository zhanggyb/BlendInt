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

#include <BlendInt/Gui/Viewport.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	glm::mat4 Viewport::default_view_matrix = glm::lookAt(
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 0.f, 0.f),
	         	glm::vec3(0.f, 1.f, 0.f));

	Viewport::Viewport ()
	: AbstractContainer(),
	focused_(0),
	top_hovered_(0)
	{
		set_size(500, 400);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);

		VertexTool tool;
		tool.GenerateShadedVertices(size(),
				DefaultBorderWidth(),
				RoundAll,
				round_radius());

		glGenVertexArrays(2, vao_);
		buffers_.generate();

		glBindVertexArray(vao_[0]);
		buffers_.bind(0);
		buffers_.set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffers_.bind(1);
		buffers_.set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::reset();
		glBindVertexArray(0);
	}

	Viewport::~Viewport()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void Viewport::AddWidget (Widget* widget)
	{
		if(PushBackSubWidget(widget)) {
			widget->SetPosition(margin().left(), margin().bottom());
		}
	}

	Viewport* Viewport::GetViewport (AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();
		AbstractWidget* viewport = 0;

		if(container == 0) {
			return dynamic_cast<Viewport*>(widget);
		} else {

			while(container->container()) {
				viewport = container;
				container = container->container();
			}

		}

		return dynamic_cast<Viewport*>(viewport);
	}

	bool Viewport::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Viewport::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		return true;
	}

	bool Viewport::RoundTypeUpdateTest (const RoundTypeUpdateRequest& request)
	{
		return true;
	}

	bool Viewport::RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	void Viewport::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(request.position()->x()  + offset_x());
			float y = static_cast<float>(request.position()->y()  + offset_y());

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

	void Viewport::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			DBG_PRINT_MSG("%s", "here");

			float x = static_cast<float>(position().x() + offset_x());
			float y = static_cast<float>(position().y() + offset_y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());
			VertexTool tool;
			tool.GenerateShadedVertices(size(),
				DefaultBorderWidth(),
				RoundAll,
				round_radius());

			buffers_.bind(0);
			buffers_.set_sub_data(0, tool.inner_size(), tool.inner_data());
			buffers_.bind(1);
			buffers_.set_sub_data(0, tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();
		}

		if(request.source() == this) {
			ReportSizeUpdate (request);
		}
	}

	void Viewport::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{

	}

	void Viewport::PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request)
	{

	}

	ResponseType Viewport::Draw (Profile& profile)
	{
		glViewport(position().x(), position().y(), size().width(), size().height());
		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetUIProjectionMatrix(projection_matrix_);
		Shaders::instance->SetUIModelMatrix(model_matrix_);

		Shaders::instance->widget_program()->use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
				0.f, 0.f, 0.f);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS),
				0);

		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.4f, 0.65f, 0.2f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(RoundAll) + 2);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 1);
		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.f, 0.f, 0.f, 1.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(RoundAll) * 2 + 2);

		glBindVertexArray(0);
		GLSLProgram::reset();

		for(AbstractWidget* p = first(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	ResponseType Viewport::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Viewport::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Viewport::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	bool Viewport::CheckAndUpdateHoverWidget (const MouseEvent& event)
	{
		if (top_hovered_) {

			if (IsHoverThroughExt (top_hovered_->container_,
					event.position ())) {

				if (top_hovered_->Contain (event.position ())) {

					AbstractWidget* orig = top_hovered_;
					UpdateHoverWidgetSubs (event);

					if (orig != top_hovered_) {
						orig->destroyed ().disconnectOne (this,
								&Viewport::OnHoverWidgetDestroyed);
						events ()->connect (top_hovered_->destroyed (),
								this, &Viewport::OnHoverWidgetDestroyed);
					}

				} else {

					top_hovered_->destroyed ().disconnectOne (this,
							&Viewport::OnHoverWidgetDestroyed);
					top_hovered_->set_hover (false);
					top_hovered_->CursorEnterEvent (false);

					// find which contianer contains cursor position
					while (top_hovered_->container ()) {

						if (top_hovered_->container () == this) {	// FIXME: the widget may be mvoed to another context
							top_hovered_ = 0;
							break;
						} else {
							top_hovered_ =
									top_hovered_->container ();

							if (top_hovered_->Contain (
									event.position ())) {
								break;
							}
						}
					}

					if (top_hovered_) {
						UpdateHoverWidgetSubs (event);
						events ()->connect (top_hovered_->destroyed (),
								this, &Viewport::OnHoverWidgetDestroyed);
					}
				}

			} else {

				top_hovered_->destroyed().disconnectOne(this,
				        &Viewport::OnHoverWidgetDestroyed);
				top_hovered_->set_hover(false);
				top_hovered_->CursorEnterEvent(false);

				// find which contianer contains cursor position
				while (top_hovered_->container()) {

					if (top_hovered_->container() == this) {	// FIXME: the widget may be mvoed to another context
						top_hovered_ = 0;
						break;
					} else {
						top_hovered_ = top_hovered_->container();

						if (IsHoverThroughExt(top_hovered_, event.position())) {
							break;
						}
					}
				}

				if (top_hovered_) {
					UpdateHoverWidgetSubs(event);
					events()->connect(top_hovered_->destroyed(), this, &Viewport::OnHoverWidgetDestroyed);
				}

			}

		} else {

			for(AbstractWidget* p = last(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(event.position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					top_hovered_ = p;
					top_hovered_->set_hover(true);
					top_hovered_->CursorEnterEvent(true);

					break;
				}
			}

			if(top_hovered_) {

				UpdateHoverWidgetSubs(event);

				events()->connect(top_hovered_->destroyed(), this,
				        &Viewport::OnHoverWidgetDestroyed);
			}

		}

		return top_hovered_ != 0;
	}

	void Viewport::UpdateHoverWidgetSubs (const MouseEvent& event)
	{
		AbstractContainer* parent = dynamic_cast<AbstractContainer*>(top_hovered_);

		if (parent) {

			for(AbstractWidget* p = parent->last(); p; p = p->previous())
			{
				if(p->visiable() && p->Contain(event.position())) {

					top_hovered_ = p;
					top_hovered_->set_hover(true);
					top_hovered_->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	ResponseType Viewport::DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMousePressEvent(widget->container(), event) == Ignore) {
					return widget->MousePressEvent(event);
				} else {
					return Accept;
				}

			} else {
				return widget->MousePressEvent(event);
			}

		}
	}

	ResponseType Viewport::DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMouseReleaseEvent(widget->container(), event) == Ignore) {
					return widget->MouseReleaseEvent(event);
				} else {
					return Accept;
				}

			} else {
				DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MouseReleaseEvent(event);
			}

		}
	}

	void Viewport::OnHoverWidgetDestroyed (AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(top_hovered_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Viewport::OnHoverWidgetDestroyed);

		top_hovered_ = 0;
	}

	void Viewport::ClearHoverWidgets()
	{
		if(top_hovered_) {

			top_hovered_->destroyed().disconnectOne(this, &Viewport::OnHoverWidgetDestroyed);

			while (top_hovered_ && top_hovered_ != this) {
				top_hovered_->set_hover(false);
				top_hovered_ = top_hovered_->container();
			}

			if(top_hovered_ == this)
				top_hovered_ = 0;

		}
	}

}
