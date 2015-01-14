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

#include <algorithm>

#include <BlendInt/Gui/Stack.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Stack::Stack()
	: Widget(),
	  active_widget_(0)
	{
		set_size(400, 300);
	}

	Stack::~Stack ()
	{
	}

	void Stack::AddWidget (AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {
			int w = size().width();
			int h = size().height();

			ResizeSubView(widget, w, h);
			MoveSubViewTo(widget, 0, 0);

			if(subs_count() == 1) {
				active_widget_ = widget;
				active_widget_->SetVisible(true);
			} else {
				widget->SetVisible(false);
			}
		}
	}

	void Stack::InsertWidget (int index, AbstractWidget* widget)
	{
		if(InsertSubView(index, widget)) {
			int w = size().width();
			int h = size().height();

			ResizeSubView(widget, w, h);
			MoveSubViewTo(widget, 0, 0);

			widget->SetVisible(false);
		}
	}

	void Stack::Remove (AbstractWidget* widget)
	{
		if(RemoveSubView(widget)) {

			if(active_widget_ == widget) {

				if(subs_count() == 0) {
					active_widget_ = 0;
				} else {
					active_widget_ = dynamic_cast<AbstractWidget*>(first_subview());
					active_widget_->SetVisible(true);
				}

			}
		}
	}

	int Stack::GetIndex () const
	{
		int index = 0;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p == active_widget_) {
				break;
			}

			index++;
		}

		if(index >= subs_count()) index = -1;

		return index;
	}

	void Stack::SetIndex (int index)
	{
		int count = subs_count();

		if(index > (count - 1)) return;

		if(count) {

			AbstractView* widget = GetWidgetAt(index);
			if(active_widget_ == widget) {
				return;
			}

			active_widget_->SetVisible(false);
			active_widget_ = dynamic_cast<AbstractWidget*>(widget);
			active_widget_->SetVisible(true);
		}
	}

	bool Stack::IsExpandX () const
	{
		bool ret = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandX()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	bool Stack::IsExpandY () const
	{
		bool ret = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandY()) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	Size Stack::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(first_subview()) {

			prefer.set_width(0);
			prefer.set_height(0);

			Size tmp;
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				tmp = p->GetPreferredSize();
				prefer.set_width(std::max(prefer.width(), tmp.width()));
				prefer.set_height(std::max(prefer.height(), tmp.height()));
			}

		}

		return prefer;
	}

	void Stack::PerformSizeUpdate (
			const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			for (AbstractView* p = first_subview(); p; p = p->next_view()) {
				ResizeSubView(p, size());
			}
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void BlendInt::Stack::HideSubWidget (int index)
	{
		if(subs_count() && index < (subs_count() - 1)) {
			AbstractView* p = GetWidgetAt(index);
			p->SetVisible(false);
		}
	}

}
