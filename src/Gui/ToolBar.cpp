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
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ToolBar::ToolBar ()
	: AbstractContainer(),
	  vao_(0),
	  space_(4),
	  move_status_(false),
	  last_x_(0),
	  start_x_(0)
	{
		set_size(200, 32);
		set_margin(4, 4, 4, 4);	// the same as MenuBar

		InitializeToolBar();
	}

	ToolBar::~ToolBar ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ToolBar::PushBack (AbstractWidget* widget)
	{
		int x = GetLastPosition();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().vsum();

		if(PushBackSubWidget(widget)) {

			Size prefer = widget->GetPreferredSize();

			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandY()) {

				ResizeSubWidget(widget, prefer.width(), h);

			} else {

				if(widget->size().height() > h) {
					ResizeSubWidget(widget, prefer.width(), h);
				} else {
					ResizeSubWidget(widget, prefer.width(), widget->size().height());
					SetSubWidgetPosition(widget, x,
									y + (h - widget->size().height()) / 2);
				}

			}
		}
	}

	void ToolBar::PushBack (const RefPtr<Action>& action)
	{
		ToolButton* button = Manage(new ToolButton);
		int x = GetLastPosition();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().top() - margin().bottom();

		if(PushBackSubWidget(button)) {

			Size prefer = button->GetPreferredSize();

			SetSubWidgetPosition(button, x, y);

			if(button->IsExpandY()) {

				ResizeSubWidget(button, prefer.width(), h);

			} else {

				if(button->size().height() > h) {
					ResizeSubWidget(button, prefer.width(), h);
				} else {
					ResizeSubWidget(button, prefer.width(), button->size().height());
					SetSubWidgetPosition(button, x,
									y + (h - button->size().height()) / 2);
				}

			}
		}
	}

	Size ToolBar::GetPreferredSize () const
	{
		Size preferred_size;

		if(first() == 0) {

			Font font;	// Get default font height
			preferred_size.set_width(200);

			int max_font_height = font.GetHeight();

			preferred_size.set_height(max_font_height + margin().vsum());

		} else {

			Size tmp_size;
			preferred_size.set_width(-space_);

			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					preferred_size.add_width(tmp_size.width() + space_);
					preferred_size.set_height(std::max(preferred_size.height(), tmp_size.height()));
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool ToolBar::IsExpandX() const
	{
		return true;
	}

	bool ToolBar::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	void ToolBar::PerformMarginUpdate (const Margin& request)
	{
		int x = position().x() + request.left();
		if (first()) {
			x = first()->position().x();
		}

		int y = position().y() + request.bottom();
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		FillSubWidgets(x, y, w, h, space_);
	}

	void ToolBar::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position (*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void ToolBar::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			int x = position().x() + margin().left();
			if (first()) {
				x = first()->position().x();
			}

			int y = position().y() + margin().bottom();
			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			FillSubWidgets(x, y, w, h, space_);

			set_size(*request.size());

		} else if (request.target()->container() == this) {
			// if a sub widget changed its size, re-align all
			FillSubWidgets(position(), size(), margin(), space_);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType ToolBar::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		program->SetUniform3f(Shaders::instance->triangle_uniform_position(),
		        (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		program->SetUniform1i(Shaders::instance->triangle_uniform_antialias(),
		        0);
		program->SetVertexAttrib4f(Shaders::instance->triangle_attrib_color(),
		        0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->reset();

		return Ignore;
	}

	ResponseType ToolBar::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType ToolBar::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::ContextMenuPressEvent (
					const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::ContextMenuReleaseEvent (
					const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::MousePressEvent (const MouseEvent& event)
	{
		if(event.button() == MouseButtonMiddle) {
			if(first()) {
				move_status_ = true;
				start_x_ = event.position().x();
				last_x_ = first()->position().x();
			}
		}

		return Ignore;
	}

	ResponseType ToolBar::MouseReleaseEvent (const MouseEvent& event)
	{
		if(move_status_) {
			move_status_ = false;
			Refresh();
		}
		return Ignore;
	}

	ResponseType ToolBar::MouseMoveEvent (const MouseEvent& event)
	{
		if(move_status_ && first()) {
			int xmin = first()->position().x();
			int direction = event.position().x() - start_x_;
			int offset = last_x_ + event.position().x() - start_x_;
			int width = size().width() - margin().left() - margin().right();
			int left = position().x() + margin().left();
			int right = position().x() + size().width() - margin().right();

			int xmax = xmin - space_;
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				xmax += p->size().width() + space_;
			}

			DBG_PRINT_MSG("direction: %d, xmin: %d, xmax: %d, offset: %d, left: %d, right: %d", direction, xmin, xmax, offset, left, right);

			if((xmax - xmin) > width) {

				if(direction < 0) {	// left

					if(xmax > right) {
						int x = last_x_;
						for(AbstractWidget* p = first(); p; p = p->next())
						{
							SetSubWidgetPosition(p, x + event.position().x() - start_x_, p->position().y());
							x += p->size().width() + space_;
						}
					}

				} else if (direction > 0) {

					if(xmin < left) {
						int x = last_x_;
						for(AbstractWidget* p = first(); p; p = p->next())
						{
							SetSubWidgetPosition(p, x + event.position().x() - start_x_, p->position().y());
							x += p->size().width() + space_;
						}
					}

				}

			} else if((xmax - xmin) < width) {

				if(direction < 0) { // left
					if(xmin > left) {
						int x = last_x_;
						for(AbstractWidget* p = first(); p; p = p->next())
						{
							SetSubWidgetPosition(p, x + event.position().x() - start_x_, p->position().y());
							x += p->size().width() + space_;
						}
					}
				} else if (direction > 0) {	// right
					if(xmax < right) {
						int x = last_x_;
						for(AbstractWidget* p = first(); p; p = p->next())
						{
							SetSubWidgetPosition(p, x + event.position().x() - start_x_, p->position().y());
							x += p->size().width() + space_;
						}
					}
				}
			}

			Refresh();
		}

		return Ignore;
	}

	void ToolBar::InitializeToolBar ()
	{
		glGenVertexArrays(1, &vao_);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);

		glBindVertexArray(vao_);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}
	
	void ToolBar::RealignSubWidgets (const Size& size, const Margin& margin,
			int space)
	{
		int x = position().x();
		int y = position().y();
		int h = size.height() - margin.top() - margin.bottom();

		if (h < 0) {
			DBG_PRINT_MSG(
					"Error: the geometry for sub widget is not valid, height: %d",
					h);
		}

		x += margin.left();
		y += margin.bottom();
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			SetSubWidgetPosition(p, x, y);
			ResizeSubWidget(p, p->size().width(), h);
			x += p->size().width() + space;
		}
	}
	
	void ToolBar::FillSubWidgets (const Point& out_pos, const Size& out_size,
			const Margin& margin, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgets(x, y, width, height, space);
	}
	
	void ToolBar::FillSubWidgets (int x, int y, int width, int height,
			int space)
	{
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			SetSubWidgetPosition(p, x, y);

			if (p->IsExpandY()) {
				ResizeSubWidget(p, p->size().width(), height);
			} else {

				if (p->size().height() > height) {
					ResizeSubWidget(p, p->size().width(), height);
				} else {
					SetSubWidgetPosition(p, x,
							y + (height - p->size().height()) / 2);
				}

			}

			x += p->size().width() + space;
		}
	}

	int ToolBar::GetLastPosition () const
	{
		int x = position().x() + margin().left();

		if (last()) {
			x = last()->position().x();
			x += last()->size().width() + space_;
		}

		return x;
	}

}
