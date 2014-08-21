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

	ScrollArea::ScrollArea ()
	: AbstractContainer()
	{
		set_margin(2, 2, 2, 2);
		set_size(360, 240);
		set_drop_shadow(true);

		InitializeScrollArea();
	}

	ScrollArea::~ScrollArea ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ScrollArea::SetViewport (AbstractWidget* widget)
	{
		if (!widget)
			return;

		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));
		view->Setup(widget);

		int width = size().width() - margin().hsum();
		int height = size().height() - margin().vsum();

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

		AdjustGeometries(position().x() + margin().left(),
						position().y() + margin().bottom(), width, height);
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

	ResponseType ScrollArea::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ScrollArea::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollArea::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void ScrollArea::PerformMarginUpdate(const Margin& request)
	{

	}

	ResponseType ScrollArea::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->Reset();
		return Ignore;
	}

	void ScrollArea::InitializeScrollArea ()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0.f);

		glBindVertexArray(m_vao);
		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		ScrollBar* hbar = Manage(new ScrollBar(Horizontal));
		ScrollBar* vbar = Manage(new ScrollBar(Vertical));
		ScrollView* view = Manage(new ScrollView);

		PushBackSubWidget(hbar);	// HScrollBarIndex
		PushBackSubWidget(vbar);	// VScrollBarIndex
		PushBackSubWidget(view);	// ScrollViewIndex

		hbar->SetSliderPercentage(100);
		vbar->SetSliderPercentage(100);
		//hbar->SetVisible(false);
		//vbar->SetVisible(false);

		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();
		int w = size().width() - margin().hsum();
		int h = size().height() - margin().vsum();
		int bh = hbar->size().height();
		int rw = vbar->size().width();

		SetSubWidgetPosition(hbar, x, y);
		SetSubWidgetPosition(vbar, x + w - rw, y + bh);
		SetSubWidgetPosition(view, x, y + bh);

		ResizeSubWidget (hbar, w - rw, bh);
		ResizeSubWidget (vbar, rw, h - bh);
		ResizeSubWidget (view, w - rw, h - bh);

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
			tool.Setup(*request.size(), 0, RoundNone, 0);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());

			ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));
			AbstractWidget* widget = view->viewport();

			int width = request.size()->width() - margin().hsum();
			int height = request.size()->height() - margin().vsum();

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

			AdjustGeometries(position().x() + margin().left(),
					position().y() + margin().bottom(), width, height);
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

		SetSubWidgetPosition(view, x, y + bh);
		ResizeSubWidget (view, width - rw, height - bh);

		AbstractWidget* widget = view->viewport();

		if(hbar->visiable()) {
			SetSubWidgetPosition(hbar, x, y);
			ResizeSubWidget (hbar, width - rw, bh);

			if(widget) {
				int percent = view->GetHPercentage();
				hbar->SetMaximum(widget->size().width());
				hbar->SetMinimum(view->size().width());
				hbar->SetSliderPercentage(percent);
			}
		}

		if(vbar->visiable()) {
			SetSubWidgetPosition(vbar, x + width - rw, y + bh);
			ResizeSubWidget (vbar, rw, height - bh);

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

		AbstractWidget* p = view->viewport();

		if (p) {
			view->SetReletivePosition(view->size().width() - value,
			        p->position().y() - view->position().y());
		}
	}

	void ScrollArea::OnVerticalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(GetWidgetAt(ScrollViewIndex));

		AbstractWidget* p = view->viewport();
		if (p) {
			view->SetReletivePosition(
			        p->position().x() - view->position().x(),
			        value - p->size().height());
		}
	}

}
