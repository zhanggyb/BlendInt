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

#include <BlendInt/Gui/ScrollArea.hpp>

namespace BlendInt {

	ScrollArea::ScrollArea ()
	: AbstractDequeContainer(), m_view(0), m_hbar(0), m_vbar(0)
	{
		InitOnce();
	}

	ScrollArea::~ScrollArea ()
	{
	}

	void ScrollArea::SetViewport (AbstractWidget* widget)
	{
		if (!widget)
			return;

		if (m_view) {

			m_view->SetViewport(widget);

			if (widget->size().width() <= size().width()) {
				m_hbar->SetVisible(false);
			} else {
				m_hbar->SetVisible(true);
			}

			if (widget->size().height() <= size().height()) {
				m_vbar->SetVisible(false);
			} else {
				m_vbar->SetVisible(true);
			}

			AdjustGeometries(size());
		}
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

	void ScrollArea::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch(request.type()) {

				case FormPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());
					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();
					MoveSubWidgets(x, y);
					break;
				}

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					AdjustGeometries(*size_p);

					break;
				}

				case WidgetRefresh: {

					Refresh();
					break;
				}

				default:
					break;
			}

		}
	}

	ResponseType ScrollArea::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	void ScrollArea::InitOnce ()
	{
		set_margin(0, 0, 0, 0);
		set_size(400, 300);

		m_view = Manage(new ScrollView);
		m_hbar = Manage(new ScrollBar(Horizontal));
		m_vbar = Manage(new ScrollBar(Vertical));

		PushBackSubWidget(m_view);
		PushBackSubWidget(m_hbar);
		PushBackSubWidget(m_vbar);

		m_hbar->SetVisible(false);
		m_vbar->SetVisible(false);

		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();
		unsigned int w = size().width() - margin().left() - margin().right();
		unsigned int h = size().height() - margin().top() - margin().bottom();
		int bh = m_hbar->size().height();
		int rw = m_vbar->size().width();

		SetSubWidgetPosition(m_hbar, x, y);
		SetSubWidgetPosition(m_vbar, x + w - rw, y + bh);
		SetSubWidgetPosition(m_view, x, y);

		ResizeSubWidget (m_hbar, w - rw, m_hbar->size().height());
		ResizeSubWidget (m_vbar, m_vbar->size().width(), h - bh);
		ResizeSubWidget (m_view, w, h);

		events()->connect(m_hbar->slider_moved(), this, &ScrollArea::OnHorizontalScroll);
		events()->connect(m_vbar->slider_moved(), this, &ScrollArea::OnVerticalScroll);
	}
	
	void ScrollArea::AdjustGeometries (const Size& size)
	{
		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();
		unsigned int w = size.width() - margin().left() - margin().right();
		unsigned int h = size.height() - margin().top() - margin().bottom();
		int bh = 0;
		int rw = 0;

		if(m_hbar->visiable()) {
			bh = m_hbar->size().height();
		}

		if(m_vbar->visiable()) {
			rw = m_vbar->size().width();
		}

		SetSubWidgetPosition(m_view, x, y + bh);
		ResizeSubWidget (m_view, w - rw, h - bh);
		m_view->CentralizeViewport();

		if(m_hbar->visiable()) {
			SetSubWidgetPosition(m_hbar, x, y);
			ResizeSubWidget (m_hbar, w - rw, m_hbar->size().height());
			int percent = m_view->GetHPercentage();
			m_hbar->SetMaximum(m_view->viewport()->size().width());
			m_hbar->SetMinimum(m_view->size().width());
			m_hbar->SetPercentage(percent);
		}

		if(m_vbar->visiable()) {
			SetSubWidgetPosition(m_vbar, x + w - rw, y + bh);
			ResizeSubWidget (m_vbar, m_vbar->size().width(), h - bh);
			int percent = m_view->GetVPercentage();
			m_vbar->SetMaximum(m_view->viewport()->size().height());
			m_vbar->SetMinimum(m_view->size().height());
			m_vbar->SetPercentage(percent);
		}
	}

	void ScrollArea::OnHorizontalScroll (int value)
	{
		AbstractWidget* p = m_view->viewport();
		if (p) {
			m_view->SetReletivePosition(value - p->size().width(),
			        p->position().y() - m_view->position().y());
		}
	}

	void ScrollArea::OnVerticalScroll (int value)
	{
		AbstractWidget* p = m_view->viewport();
		if (p) {
			m_view->SetReletivePosition(
			        p->position().x() - m_view->position().x(),
			        value - p->size().height());
		}
	}

}

