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
	: AbstractVectorContainer(4)
	{
		InitializeScrollArea();
	}

	ScrollArea::~ScrollArea ()
	{
	}

	void ScrollArea::SetViewport (AbstractWidget* widget)
	{
		if (!widget)
			return;

		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));
		view->SetViewport(widget);

		if (widget->size().width() <= size().width()) {
			sub_widget(HScrollBarIndex)->SetVisible(false);
		} else {
			sub_widget(HScrollBarIndex)->SetVisible(true);
		}

		if (widget->size().height() <= size().height()) {
			sub_widget(VScrollBarIndex)->SetVisible(false);
		} else {
			sub_widget(VScrollBarIndex)->SetVisible(true);
		}

		AdjustGeometries(size());
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

	void ScrollArea::UpdateContainer(const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerRefresh: {

				Refresh();
				break;
			}

			default:
				break;
		}

	}

	void ScrollArea::UpdateGeometry (const WidgetUpdateRequest& request)
	{

		switch (request.type()) {

			case WidgetPosition: {
				const Point* pos_p = static_cast<const Point*>(request.data());
				int x = pos_p->x() - position().x();
				int y = pos_p->y() - position().y();
				MoveSubWidgets(x, y);
				break;
			}

			case WidgetSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				AdjustGeometries(*size_p);

				break;
			}

			default:
				break;
		}

	}

	ResponseType ScrollArea::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	void ScrollArea::InitializeScrollArea ()
	{
		set_margin(0, 0, 0, 0);
		set_size(400, 300);

		ScrollView* view = Manage(new ScrollView);
		ScrollBar* hbar = Manage(new ScrollBar(Horizontal));
		ScrollBar* vbar = Manage(new ScrollBar(Vertical));

		SetSubWidget(0, view);
		SetSubWidget(1, hbar);
		SetSubWidget(2, vbar);

		//m_hbar->SetVisible(false);
		//m_vbar->SetVisible(false);

		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();
		unsigned int w = size().width() - margin().left() - margin().right();
		unsigned int h = size().height() - margin().top() - margin().bottom();
		int bh = hbar->size().height();
		int rw = vbar->size().width();

		SetSubWidgetPosition(hbar, x, y);
		SetSubWidgetPosition(vbar, x + w - rw, y + bh);
		SetSubWidgetPosition(view, x, y);

		ResizeSubWidget (hbar, w - rw, hbar->size().height());
		ResizeSubWidget (vbar, vbar->size().width(), h - bh);
		ResizeSubWidget (view, w, h);

		events()->connect(hbar->slider_moved(), this, &ScrollArea::OnHorizontalScroll);
		events()->connect(vbar->slider_moved(), this, &ScrollArea::OnVerticalScroll);
	}
	
	void ScrollArea::AdjustGeometries (const Size& size)
	{
		int x = position().x() + margin().left();
		int y = position().y() + margin().bottom();
		unsigned int w = size.width() - margin().left() - margin().right();
		unsigned int h = size.height() - margin().top() - margin().bottom();
		int bh = 0;
		int rw = 0;

		if(sub_widget(HScrollBarIndex)->visiable()) {
			bh = sub_widget(HScrollBarIndex)->size().height();
		}

		if(sub_widget(VScrollBarIndex)->visiable()) {
			rw = sub_widget(VScrollBarIndex)->size().width();
		}

		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));
		ScrollBar* hbar = dynamic_cast<ScrollBar*>(sub_widget(HScrollBarIndex));
		ScrollBar* vbar = dynamic_cast<ScrollBar*>(sub_widget(VScrollBarIndex));

		SetSubWidgetPosition(view, x, y + bh);
		ResizeSubWidget (view, w - rw, h - bh);

		view->CentralizeViewport();

		if(hbar->visiable()) {
			SetSubWidgetPosition(hbar, x, y);
			ResizeSubWidget (hbar, w - rw, hbar->size().height());
			int percent = view->GetHPercentage();
			hbar->SetMaximum(view->viewport()->size().width());
			hbar->SetMinimum(view->size().width());
			hbar->SetPercentage(percent);
		}

		if(vbar->visiable()) {
			SetSubWidgetPosition(vbar, x + w - rw, y + bh);
			ResizeSubWidget (vbar, vbar->size().width(), h - bh);
			int percent = view->GetVPercentage();
			vbar->SetMaximum(view->viewport()->size().height());
			vbar->SetMinimum(view->size().height());
			vbar->SetPercentage(percent);
		}
	}

	void ScrollArea::OnHorizontalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));

		AbstractWidget* p = view->viewport();
		if (p) {
			view->SetReletivePosition(value - p->size().width(),
			        p->position().y() - view->position().y());
		}
	}

	void ScrollArea::OnVerticalScroll (int value)
	{
		ScrollView* view = dynamic_cast<ScrollView*>(sub_widget(ScrollViewIndex));

		AbstractWidget* p = view->viewport();
		if (p) {
			view->SetReletivePosition(
			        p->position().x() - view->position().x(),
			        value - p->size().height());
		}
	}

}

