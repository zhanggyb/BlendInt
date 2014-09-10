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

	SideButton::SideButton(int round_type)
	: AbstractButton()
	{
		set_size(18, 18);
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

	SideButton::~SideButton()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void SideButton::PerformSizeUpdate(const SizeUpdateRequest& request)
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

	ResponseType SideButton::Draw(Profile& profile)
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

	Workspace::Workspace()
	: AbstractContainer(),
	  left_sidebar_(0),
	  right_sidebar_(0),
	  header_(0),
	  viewport_(0),
	  splitter_(0),
	  left_button_(0),
	  right_button_(0),
	  vao_(0)
	{
		set_size(800, 600);
		set_margin(0, 0, 0, 0);

		InitializeWorkspace();
	}

	Workspace::~Workspace()
	{
		if(left_button_) delete left_button_;
		if(right_button_) delete right_button_;

		glDeleteVertexArrays(1, &vao_);
	}

	void Workspace::SetViewport (AbstractWidget* viewport)
	{
	}

	void Workspace::SetLeftSideBar (AbstractWidget* widget)
	{
	}

	void Workspace::SetRightSideBar (AbstractWidget* widget)
	{
	}

	void Workspace::SetHeader (AbstractWidget* widget)
	{
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

			AdjustGeometries(position(), size(), margin());
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

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.208f, 0.208f, 0.208f, 1.0f);

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

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		splitter_ = Manage(new Splitter);
		splitter_->SetMargin(0, 0, 0, 0);
		viewport_ = Manage(new Viewport3D);
//		left_sidebar_ = Manage(new ToolBox);
//		right_sidebar_ = Manage(new ToolBox);
		header_ = Manage(new ToolBar);

//		splitter_->PushBack(left_sidebar_);
		splitter_->Append(viewport_);
//		splitter_->PushBack(right_sidebar_);

		PushBackSubWidget(splitter_);
		PushBackSubWidget(header_);

		left_button_ = Manage(new SideButton(RoundTopRight | RoundBottomRight));
		right_button_ = Manage(new SideButton(RoundTopLeft | RoundBottomLeft));

		PushBackSubWidget(right_button_);
		PushBackSubWidget(left_button_);

		AdjustGeometries(position(), size(), margin());
	}

	void Workspace::AdjustGeometries(const Point& out_pos, const Size& out_size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		AdjustGeometries(x, y, w, h);
	}

	void Workspace::AdjustGeometries (int x, int y, int w, int h)
	{

		if(header_) {

			Size pref = header_->GetPreferredSize();

			if(splitter_->previous() == header_) {

				ResizeSubWidget(header_, w, pref.height());
				SetSubWidgetPosition(header_, x, y + h - pref.height());

				ResizeSubWidget(splitter_, w, h - pref.height() - 1);
				SetSubWidgetPosition(splitter_, x, y);

			} else {

				ResizeSubWidget(splitter_, w, h - pref.height() - 1);
				SetSubWidgetPosition(splitter_, x, y + h - splitter_->size().height());

				ResizeSubWidget(header_, w, pref.height());
				SetSubWidgetPosition(header_, x, y);

			}

		} else {

			ResizeSubWidget(splitter_, w, h);
			SetSubWidgetPosition(splitter_, x, y);
		}

		SetSubWidgetPosition(left_button_, x, y + h * 0.9);
		SetSubWidgetPosition(right_button_, x + w - right_button_->size().width(), y + h * 0.9);
	}

}
