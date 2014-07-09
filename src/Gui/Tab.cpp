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
#include <BlendInt/Gui/TabButton.hpp>

#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Gui/TabHeader.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Gui/StackPanel.hpp>

namespace BlendInt {

	Tab::Tab ()
	: AbstractVectorContainer(2)
	{
		set_size(400, 300);

		TabHeader* header = Manage(new TabHeader);
		StackPanel* stack = Manage(new StackPanel);
		stack->SetMargin(4, 4, 4, 4);

		SetSubWidget(0, header);
		SetSubWidget(1, stack);

		FillSubWidgetsInTab(size(), margin());

		events()->connect(header->button_index_toggled(), this, &Tab::OnButtonToggled);
	}

	Tab::~Tab ()
	{
	}

	void Tab::Add (const String& title, AbstractWidget* widget)
	{
		TabButton* btn = Manage(new TabButton);
		btn->SetText(title);

		TabHeader* header = dynamic_cast<TabHeader*>(sub_widget(0));
		StackPanel* stack = dynamic_cast<StackPanel*>(sub_widget(1));

		header->PushBack(btn);
		stack->Add(widget);

		if(header->sub_widget_size() == 1) {
			btn->SetChecked(true);
		}
	}

	bool Tab::IsExpandX () const
	{
		if(sub_widget(0)->IsExpandX())
			return true;

		if(sub_widget(1)->IsExpandX())
			return true;

		return false;
	}

	bool Tab::IsEXpandY () const
	{
		if(sub_widget(0)->IsExpandY())
			return true;

		if(sub_widget(1)->IsExpandY())
			return true;

		return false;
	}

	Size Tab::GetPreferredSize () const
	{
		int w = 0;
		int h = 0;

		Size tmp1 = sub_widget(0)->GetPreferredSize();
		Size tmp2 = sub_widget(1)->GetPreferredSize();

		w = std::max(tmp1.width(), tmp2.width());
		h = tmp1.height() + tmp2.height();

		w = w + margin().hsum();
		h = h + margin().vsum();

		return Size(w, h);
	}

	void Tab::UpdateContainer (const ContainerUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {
				const Margin* margin_p = static_cast<const Margin*>(request.data());
				FillSubWidgetsInTab(size(), *margin_p);

				break;
			}

			default: {
				ReportContainerUpdate(request);
				break;
			}
		}

	}

	void Tab::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					set_position(*pos_p);
					MoveSubWidgets(x, y);
					break;
				}

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					FillSubWidgetsInTab(*size_p, margin());
					set_size(*size_p);
					break;
				}

				default:
					break;
			}

		}

		ReportGeometryUpdate(request);
	}

	ResponseType Tab::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType Tab::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Tab::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Tab::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}
	
	void Tab::OnButtonToggled (int index, bool toggled)
	{
		Stack* stack = dynamic_cast<Stack*>(sub_widget(1));

		stack->SetIndex(index);
		Refresh();
	}

	void Tab::FillSubWidgetsInTab(const Size& out_size, const Margin& margin)
	{
		int x = position().x() + margin.left();
		int y = position().y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		FillSubWidgetsInTab(x, y, w, h);
	}

	bool Tab::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		if(request.source() == this) {
			return true;
		} else if (request.source() == container()) {
			return true;
		} else {	// called by sub widget
			return false;
		}
	}

	void Tab::FillSubWidgetsInTab(int x, int y, int w, int h)
	{
		int header_y = position().y() + size().height() - margin().top();

		TabHeader* header = dynamic_cast<TabHeader*>(sub_widget(0));
		StackPanel* stack = dynamic_cast<StackPanel*>(sub_widget(1));

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
		SetSubWidgetPosition(header, x, header_y);
		ResizeSubWidget(header, w, header_size.height());

		SetSubWidgetPosition(stack, x, y);
		ResizeSubWidget(stack, w, h - header_size.height());
	}

}
