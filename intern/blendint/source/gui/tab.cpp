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

#include <gui/tab-button.hpp>

#include <gui/tab.hpp>
#include <gui/tab-header.hpp>
#include <stock/shaders.hpp>
#include <gui/stack.hpp>

namespace BlendInt {

	Tab::Tab ()
	: AbstractWidget()
	{
		set_size(400, 300);

		TabHeader* header = Manage(new TabHeader);
		DBG_SET_NAME(header, "TabHeader");
		Stack* stack = Manage(new Stack);
		DBG_SET_NAME(stack, "Stack");

		PushBackSubView(header);	// 0
		PushBackSubView(stack);	// 1

		FillSubWidgetsInTab(size());

		events()->connect(header->button_index_toggled(), this, &Tab::OnButtonToggled);
	}

	Tab::~Tab ()
	{
	}

	void Tab::AddWidget (const String& title, AbstractWidget* widget)
	{
		TabButton* btn = Manage(new TabButton(title));
		DBG_SET_NAME(btn, ConvertFromString(title).c_str());

		TabHeader* header = dynamic_cast<TabHeader*>(GetSubViewAt(0));
		Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

		header->AddButton(btn);
		stack->AddWidget(widget);

		if(header->subs_count() == 1) {
			btn->SetChecked(true);
		}
	}

	bool Tab::IsExpandX () const
	{
		if(GetSubViewAt(0)->IsExpandX())
			return true;

		if(GetSubViewAt(1)->IsExpandX())
			return true;

		return false;
	}

	bool Tab::IsEXpandY () const
	{
		if(GetSubViewAt(0)->IsExpandY())
			return true;

		if(GetSubViewAt(1)->IsExpandY())
			return true;

		return false;
	}

	Size Tab::GetPreferredSize () const
	{
		int w = 0;
		int h = 0;

		Size tmp1 = GetSubViewAt(0)->GetPreferredSize();
		Size tmp2 = GetSubViewAt(1)->GetPreferredSize();

		w = std::max(tmp1.width(), tmp2.width());
		h = tmp1.height() + tmp2.height();

		return Size(w, h);
	}

	int Tab::GetIndex() const
	{
		Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

		return stack->GetIndex();
	}

	void Tab::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillSubWidgetsInTab(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Tab::OnButtonToggled (int index, bool toggled)
	{
		Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

		stack->SetIndex(index);
		RequestRedraw();
	}

	void Tab::FillSubWidgetsInTab(const Size& out_size)
	{
		int x = 0;
		int y = 0;
		int w = out_size.width();
		int h = out_size.height();

		FillSubWidgetsInTab(x, y, w, h);
	}

	Response Tab::Draw (AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	void Tab::FillSubWidgetsInTab(int x, int y, int w, int h)
	{
		int header_y = h;

		TabHeader* header = dynamic_cast<TabHeader*>(GetSubViewAt(0));
		Stack* stack = dynamic_cast<Stack*>(GetSubViewAt(1));

		Size header_size = header->GetPreferredSize();

		if(header_size.height() > h) {
			header->SetVisible(false);
			stack->SetVisible(false);

			return;
		} else {
			header->SetVisible(true);
			stack->SetVisible(true);
		}

		header_y = header_y - header_size.height();
		MoveSubViewTo(header, x, header_y);
		ResizeSubView(header, w, header_size.height());

		MoveSubViewTo(stack, x, y);
		ResizeSubView(stack, w, h - header_size.height());
	}

}
