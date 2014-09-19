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
 * License along with BlendInt.	 If not, see
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
#include <BlendInt/Gui/Workspace.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Icons.hpp>

#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>

namespace BlendInt {

	using Stock::Shaders;
	using Stock::Icons;

	EdgeButton::EdgeButton(int round_type)
	: AbstractButton()
	{
		set_size(14, 14);
		set_round_type(round_type);

		VertexTool tool;
		tool.GenerateShadedVertices(size(), 1.f, this->round_type(), round_radius());

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	EdgeButton::~EdgeButton()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void EdgeButton::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
				tool.GenerateShadedVertices(*request.size(), DefaultBorderWidth(),
						round_type(), round_radius());
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());
			GLArrayBuffer::reset();

			set_size(*request.size());

			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType EdgeButton::Draw(Profile& profile)
	{
		Shaders::instance->widget_program()->use();

		glm::vec3 pos((GLfloat)position().x(), (GLfloat)position().y(), 0.f);

		glUniform3fv(Shaders::instance->location(Stock::WIDGET_POSITION), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 0);

		if (hover()) {

			glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 15);
			if (is_checked()) {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
				        Theme::instance->radio_button().inner_sel.data());
			} else {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
				        Theme::instance->radio_button().inner.data());
			}

		} else {
			glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 0);
			if (is_checked()) {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
				        Theme::instance->radio_button().inner_sel.data());
			} else {
				glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
				        Theme::instance->radio_button().inner.data());
			}
		}

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 1);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_COLOR), 1,
		        Theme::instance->radio_button().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);

			glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		return Accept;
	}

	// -------------------------------

	EdgeButtonLayer::EdgeButtonLayer()
	: Container()
	{
		set_margin(0, 0, 0, 0);
		InitializeSideButtonLayer();
	}

	EdgeButtonLayer::~EdgeButtonLayer()
	{

	}

	bool EdgeButtonLayer::Contain(const Point& point) const
	{
		bool retval = false;

		for(AbstractWidget* p = first(); p; p = p->next()) {

			if(p->visiable()) {
				retval = p->Contain(point);
			}

			if(retval) break;
		}

		return retval;
	}

	void EdgeButtonLayer::PerformMarginUpdate(const Margin& request)
	{
		AlighButtons(position(), size(), request);
	}

	void EdgeButtonLayer::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			AlighButtons(position(), *request.size(), margin());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void EdgeButtonLayer::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AlighButtons(*request.position(), size(), margin());
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	ResponseType EdgeButtonLayer::Draw(Profile& profile)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::MousePressEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::MouseReleaseEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType EdgeButtonLayer::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	void EdgeButtonLayer::InitializeSideButtonLayer()
	{
		EdgeButton* left = Manage(new EdgeButton(RoundTopRight | RoundBottomRight));
		DBG_SET_NAME(left, "LeftButton");
		EdgeButton* right = Manage(new EdgeButton(RoundTopLeft | RoundBottomLeft));
		DBG_SET_NAME(right, "RightButton");
		EdgeButton* head = Manage(new EdgeButton(RoundTopLeft | RoundTopRight));
		DBG_SET_NAME(head, "HeadButton");

		PushBackSubWidget(left);
		PushBackSubWidget(right);
		PushBackSubWidget(head);

		AlighButtons(position(), size(), margin());
	}

	void EdgeButtonLayer::AlighButtons(const Point& out_pos,
			const Size& out_size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		AlignButtons(x, y, w, h);
	}

	void EdgeButtonLayer::AlignButtons(int x, int y, int w, int h)
	{
		AbstractWidget* p = first();

		SetSubWidgetPosition(p, x, y + h * 9 / 10);
		p = p->next();
		SetSubWidgetPosition(p, x + w - last()->size().width(), y + h * 9 / 10);
		p = p->next();
		SetSubWidgetPosition(p, x + w * 9 / 10, y);
	}

	// -------------------------------

	ViewportLayer::ViewportLayer()
	{
		set_margin(0, 0, 0, 0);
	}

	ViewportLayer::~ViewportLayer()
	{
	}

	bool ViewportLayer::Contain(const Point& point) const
	{
		if(next()) {
			if(next()->Contain(point)) {
				return false;
			} else {
				return VLayout::Contain(point);
			}
		}

		return VLayout::Contain(point);
	}

	// -------------------------------

	Workspace::Workspace()
	: Container(),
	  left_sidebar_(0),
	  right_sidebar_(0),
	  header_(0),
	  viewport_(0),
	  splitter_(0),
	  vao_(0)
	{
		set_size(800, 600);
		set_margin(0, 0, 0, 0);

		InitializeWorkspace();
	}

	Workspace::~Workspace()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Workspace::SetViewport (AbstractWidget* viewport)
	{
		if(viewport_ == viewport) return;

		if(viewport_)
			splitter_->Remove(viewport_);

		splitter_->Append(viewport);
		viewport_ = viewport;

		DBG_PRINT_MSG("viewport size: %d %d", viewport_->size().width(), viewport_->size().height());
	}

	void Workspace::SetLeftSideBar (AbstractWidget* widget)
	{
		if(left_sidebar_ == widget) return;

		if(left_sidebar_)
			splitter_->Remove(left_sidebar_);

		splitter_->Prepend(widget);
		left_sidebar_ = widget;
	}

	void Workspace::SetRightSideBar (AbstractWidget* widget)
	{
		if(right_sidebar_ == widget) return;

		if(right_sidebar_)
			splitter_->Remove(right_sidebar_);

		splitter_->Append(widget);
		right_sidebar_ = widget;
	}

	void Workspace::SetHeader (AbstractWidget* widget)
	{
		if(header_ == widget) return;

		ViewportLayer* v = dynamic_cast<ViewportLayer*>(first());

		if(header_)
			v->Remove(header_);

		v->Append(widget);
		header_ = widget;
	}

	bool Workspace::IsExpandX () const
	{
		return true;
	}

	bool Workspace::IsExpandY () const
	{
		return true;
	}

	Size Workspace::GetPreferredSize () const
	{
		Size prefer;

		if(widget_count() == 0) {
			prefer.reset(500, 400);
		} else {
			Size tmp;
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				tmp = p->GetPreferredSize();
				prefer.set_width(std::max(prefer.width(), tmp.width()));
				prefer.set_height(std::max(prefer.height(), tmp.height()));
			}

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void Workspace::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void Workspace::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			Size inner_size(request.size()->width() - margin().hsum(),
			        request.size()->height() - margin().vsum());

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0.f);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			set_size(*request.size());

			ResizeSubWidgets(size());

		} else if (request.source()->container() == this) {

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Workspace::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		/*
		if (request.source()->container() == this) {
			EnableShadow(request.source());
		}

		if(request.source() != container()) {
			ReportRoundTypeUpdate(request);
		}
		*/
	}

	void Workspace::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		/*
		if (request.source()->container() == this) {
			EnableShadow(request.source());
		}

		if(request.source() != container()) {
			ReportRoundRadiusUpdate(request);
		}
		*/
	}

	ResponseType Workspace::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->reset();

		return Ignore;
	}

	ResponseType Workspace::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Workspace::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Workspace::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Workspace::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Workspace::InitializeWorkspace ()
	{
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0.f);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::TRIANGLE_COORD));
		glVertexAttribPointer (
				Shaders::instance->location (Stock::TRIANGLE_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		splitter_ = Manage(new Splitter);
		DBG_SET_NAME(splitter_, "Splitter");
		splitter_->SetMargin(0, 0, 0, 0);

		ViewportLayer* vlayout = Manage(new ViewportLayer);
		DBG_SET_NAME(vlayout, "VLayout");
		vlayout->SetSpace(1);
		vlayout->Append(splitter_);

		EdgeButtonLayer* btnlayout = Manage(new EdgeButtonLayer);
		DBG_SET_NAME(btnlayout, "SideButton Layer");

		PushBackSubWidget(vlayout);
		PushBackSubWidget(btnlayout);

		ResizeSubWidget(vlayout, size());
		ResizeSubWidget(btnlayout, size());
	}

}
