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
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ScrollArea::ScrollArea ()
	: Widget()
	{
		set_size(360, 240);

		InitializeScrollArea();
	}

	ScrollArea::~ScrollArea ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ScrollArea::SetViewport (AbstractView* widget)
	{
		if (!widget)
			return;

		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));
		view->Setup(widget);

		int width = size().width();
		int height = size().height();

		if (widget->size().width() <= width) {
			GetWidgetAt(HScrollBarIndex)->SetVisible(false);
		} else {
			GetWidgetAt(HScrollBarIndex)->SetVisible(true);
		}

		if (widget->size().height() <= height) {
			GetWidgetAt(VScrollBarIndex)->SetVisible(false);
		} else {
			GetWidgetAt(VScrollBarIndex)->SetVisible(true);
		}

		AdjustGeometries(position().x(),
						position().y(), width, height);
	}

	void ScrollArea::CentralizeViewport () const
	{
		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));
		view->CentralizeViewport();
	}

	bool ScrollArea::IsExpandX() const
	{
		return true;
	}

	bool ScrollArea::IsExpandY() const
	{
		return true;
	}

	void ScrollArea::PerformMarginUpdate(const Margin& request)
	{

	}

	ResponseType ScrollArea::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), (float) position().x(), (float) position().y());
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->reset();
		return Ignore;
	}

	void ScrollArea::InitializeScrollArea ()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0.f);

		glBindVertexArray(m_vao);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		ScrollBar* hbar = Manage(new ScrollBar(Horizontal));
		ScrollBar* vbar = Manage(new ScrollBar(Vertical));
		ScrollView* view = Manage(new ScrollView);

		PushBackSubView(hbar);	// HScrollBarIndex
		PushBackSubView(vbar);	// VScrollBarIndex
		PushBackSubView(view);	// ScrollViewIndex

		hbar->SetSliderPercentage(100);
		vbar->SetSliderPercentage(100);
		//hbar->SetVisible(false);
		//vbar->SetVisible(false);

		int x = position().x();
		int y = position().y();
		int w = size().width();
		int h = size().height();
		int bh = hbar->size().height();
		int rw = vbar->size().width();

		MoveSubViewTo(hbar, x, y);
		MoveSubViewTo(vbar, x + w - rw, y + bh);
		MoveSubViewTo(view, x, y + bh);

		ResizeSubView (hbar, w - rw, bh);
		ResizeSubView (vbar, rw, h - bh);
		ResizeSubView (view, w - rw, h - bh);

		events()->connect(hbar->slider_moved(), this, &ScrollArea::OnHorizontalScroll);
		events()->connect(vbar->slider_moved(), this, &ScrollArea::OnVerticalScroll);
	}
	
	void ScrollArea::AdjustGeometries (const Point& out_pos, const Size& out_size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		AdjustGeometries(x, y, w, h);
	}

	void ScrollArea::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();
			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	void ScrollArea::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());

			ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));
			AbstractView* widget = view->viewport();

			int width = request.size()->width();
			int height = request.size()->height();

			if(widget) {
				if (widget->size().width() <= width) {
					GetWidgetAt(HScrollBarIndex)->SetVisible(false);
				} else {
					GetWidgetAt(HScrollBarIndex)->SetVisible(true);
				}

				if (widget->size().height() <= height) {
					GetWidgetAt(VScrollBarIndex)->SetVisible(false);
				} else {
					GetWidgetAt(VScrollBarIndex)->SetVisible(true);
				}
			}

			AdjustGeometries(position().x(),
					position().y(), width, height);
			set_size(*request.size());
		}

		ReportSizeUpdate(request);
	}

	void ScrollArea::AdjustGeometries (int x, int y, int width, int height)
	{
		int bh = 0;	// bottom height of the hbar
		int rw = 0;	// right width of the vbar

		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));
		ScrollBar* hbar = dynamic_cast<ScrollBar*>(GetWidgetAt(HScrollBarIndex));
		ScrollBar* vbar = dynamic_cast<ScrollBar*>(GetWidgetAt(VScrollBarIndex));

		if(hbar->visiable()) {
			bh = hbar->size().height();
		}

		if(vbar->visiable()) {
			rw = vbar->size().width();
		}

		MoveSubViewTo(view, x, y + bh);
		ResizeSubView (view, width - rw, height - bh);

		AbstractView* widget = view->viewport();

		if(hbar->visiable()) {
			MoveSubViewTo(hbar, x, y);
			ResizeSubView (hbar, width - rw, bh);

			if(widget) {
				int percent = view->GetHPercentage();
				hbar->SetMaximum(widget->size().width());
				hbar->SetMinimum(view->size().width());
				hbar->SetSliderPercentage(percent);
			}
		}

		if(vbar->visiable()) {
			MoveSubViewTo(vbar, x + width - rw, y + bh);
			ResizeSubView (vbar, rw, height - bh);

			if(widget) {
				int percent = view->GetVPercentage();
				vbar->SetMaximum(widget->size().height());
				vbar->SetMinimum(view->size().height());
				vbar->SetSliderPercentage(percent);
			}
		}
	}

	void ScrollArea::OnHorizontalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));

		AbstractView* p = view->viewport();

		if (p) {
			view->SetReletivePosition(view->size().width() - value,
			        p->position().y() - view->position().y());
		}
	}

	void ScrollArea::OnVerticalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));

		AbstractView* p = view->viewport();
		if (p) {
			view->SetReletivePosition(
			        p->position().x() - view->position().x(),
			        value - p->size().height());
		}
	}

}
