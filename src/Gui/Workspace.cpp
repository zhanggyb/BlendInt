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

#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Workspace::Workspace()
	: AbstractContainer(),
	  left_sidebar_(0),
	  right_sidebar_(0),
	  header_(0),
	  viewport_(0),
	  splitter_(0),
	  vao_(0)
	{
		set_size(800, 600);
		set_drop_shadow(true);
		set_margin(0, 0, 0, 0);

		InitializeWorkspace();
	}

	Workspace::~Workspace()
	{
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

		if(request.source() != container()) {
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

			EnableShadow(request.source());
		}

		if(request.source() != container()) {
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
		program->Use();

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
		splitter_->PushBack(viewport_);
//		splitter_->PushBack(right_sidebar_);

		PushBackSubWidget(splitter_);
		PushBackSubWidget(header_);

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

			if(first() == header_) {

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

	}

}

