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

#include <BlendInt/Gui/SingleStackLayout.hpp>

namespace BlendInt {


	SingleStackLayout::SingleStackLayout ()
	: AbstractStackLayout(),
	  active_widget_(0)
	{
	}

	SingleStackLayout::~SingleStackLayout ()
	{
	}

	void SingleStackLayout::Prepend (AbstractWidget* widget)
	{
		if(PushFrontSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			if(widget_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void SingleStackLayout::Append (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			if(widget_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void SingleStackLayout::Insert (int index, AbstractWidget* widget)
	{
		if(InsertSubWidget(index, widget)) {
			int w = size().width() - margin().left() - margin().right();
			int h = size().height() - margin().top() - margin().bottom();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, position().x() + margin().left(), position().y() + margin().bottom());

			widget->SetVisible(false);
		}
	}

	void SingleStackLayout::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {

			if(active_widget_ == widget) {

				if(widget_count() == 0) {
					active_widget_ = 0;
				} else {
					active_widget_ = first();
					active_widget_->SetVisible(true);
				}

			}
		}
	}

	int SingleStackLayout::GetIndex () const
	{
		int index = 0;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p == active_widget_) {
				break;
			}

			index++;
		}

		if(index >= widget_count()) index = -1;

		return index;
	}

	void SingleStackLayout::SetIndex (int index)
	{
		int count = widget_count();

		if(index > (count - 1)) return;

		if(count) {

			AbstractWidget* widget = GetWidgetAt(index);
			if(active_widget_ == widget) {
				return;
			}

			active_widget_->SetVisible(false);
			active_widget_ = widget;
			active_widget_->SetVisible(true);
		}
	}

	bool SingleStackLayout::IsExpandX () const
	{
		bool ret = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	bool SingleStackLayout::IsExpandY () const
	{
		bool ret = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	Size SingleStackLayout::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(first()) {

			prefer.set_width(0);
			prefer.set_height(0);

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

	void SingleStackLayout::PerformMarginUpdate (const Margin& request)
	{
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		ResizeSubWidgets(w, h);
	}

	void SingleStackLayout::PerformPositionUpdate (
			const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void SingleStackLayout::PerformSizeUpdate (
			const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			set_size(*request.size());
			ResizeSubWidgets(w, h);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType SingleStackLayout::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::ContextMenuPressEvent (
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::ContextMenuReleaseEvent (
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::MousePressEvent (
			const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::MouseReleaseEvent (
			const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType SingleStackLayout::MouseMoveEvent (
			const MouseEvent& event)
	{
		return Ignore;
	}

	void BlendInt::SingleStackLayout::HideSubWidget (int index)
	{
		if(widget_count() && index < (widget_count() - 1)) {
			AbstractWidget* p = GetWidgetAt(index);
			p->SetVisible(false);
		}
	}

}
