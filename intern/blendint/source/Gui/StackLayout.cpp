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

#include <BlendInt/Gui/StackLayout.hpp>

namespace BlendInt {


	StackLayout::StackLayout ()
	: AbstractLayout(),
	  active_widget_(0)
	{
	}

	StackLayout::~StackLayout ()
	{
	}

	void StackLayout::AddWidget (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, margin().left(), margin().bottom());

			if(subs_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void StackLayout::InsertWidget (int index, AbstractWidget* widget)
	{
		if(InsertSubWidget(index, widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, margin().left(), margin().bottom());

			widget->SetVisible(false);
		}
	}

	void StackLayout::InsertWidget (int row, int column, AbstractWidget* widget)
	{
		if(PushFrontSubWidget(widget)) {
			int w = size().width() - margin().hsum();
			int h = size().height() - margin().vsum();

			ResizeSubWidget(widget, w, h);
			SetSubWidgetPosition(widget, margin().left(), margin().bottom());

			if(subs_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void StackLayout::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {

			if(active_widget_ == widget) {

				if(subs_count() == 0) {
					active_widget_ = 0;
				} else {
					active_widget_ = dynamic_cast<AbstractWidget*>(first_child());
					active_widget_->SetVisible(true);
				}

			}
		}
	}

	int StackLayout::GetIndex () const
	{
		int index = 0;

		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			if(p == active_widget_) {
				break;
			}

			index++;
		}

		if(index >= subs_count()) index = -1;

		return index;
	}

	void StackLayout::SetIndex (int index)
	{
		int count = subs_count();

		if(index > (count - 1)) return;

		if(count) {

			AbstractInteractiveForm* widget = GetWidgetAt(index);
			if(active_widget_ == widget) {
				return;
			}

			active_widget_->SetVisible(false);
			active_widget_ = dynamic_cast<AbstractWidget*>(widget);
			active_widget_->SetVisible(true);
		}
	}

	bool StackLayout::IsExpandX () const
	{
		bool ret = false;

		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	bool StackLayout::IsExpandY () const
	{
		bool ret = false;

		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	Size StackLayout::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(first_child()) {

			prefer.set_width(0);
			prefer.set_height(0);

			Size tmp;
			for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
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

	void StackLayout::PerformMarginUpdate (const Margin& request)
	{
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		ResizeSubWidgets(w, h);
	}

	void StackLayout::PerformSizeUpdate (
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

	void BlendInt::StackLayout::HideSubWidget (int index)
	{
		if(subs_count() && index < (subs_count() - 1)) {
			AbstractInteractiveForm* p = GetWidgetAt(index);
			p->SetVisible(false);
		}
	}

}
