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

#include <BlendInt/Gui/Section.hpp>

namespace BlendInt {

	Section::Section ()
	: AbstractContainer(),
	  m_focused_widget(0),
	  m_last_hover_widget(0)
	{
		set_size(800, 600);
	}

	Section::~Section ()
	{
		AbstractWidget* widget = 0;

		for(std::set<AbstractWidget*>::iterator it = m_set.begin();
				it != m_set.end();
				it++)
		{
			widget = *it;

			// if widget focused
			// if widget hover
			if(widget->managed() && (widget->count() == 0)) {
				delete widget;
			}
		}

		m_set.clear();
	}

	void Section::Insert (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return;
		}

		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
						widget->name().c_str(),
						name().c_str());
				return;
			} else {
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}
		}

		m_set.insert(widget);

		// set shadow
		if(widget->drop_shadow()) {
			if(!widget->m_shadow) {
				widget->m_shadow.reset(new Shadow);
			}

			widget->m_shadow->Resize(widget->size());
		}

		SetContainer(widget, this);
		events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);
	}

	void Section::Remove (AbstractWidget* widget)
	{
	}

	void Section::SetFocusedWidget (AbstractWidget* widget)
	{
		if(m_focused_widget == widget) {
			return;
		}

		if (m_focused_widget) {
			m_focused_widget->set_focus(false);
			m_focused_widget->FocusEvent(false);
		}

		m_focused_widget = widget;
		if (m_focused_widget) {
			m_focused_widget->set_focus(true);
			m_focused_widget->FocusEvent(true);
		}
	}

	void Section::UpdateContainer (const ContainerUpdateRequest& request)
	{
		ReportContainerUpdate(request);
	}

	bool Section::UpdateGeometryTest (
	        const GeometryUpdateRequest& request)
	{
		return true;
	}

	void Section::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					set_size(*size_p);
					break;
				}

				case WidgetPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());
					set_position(*pos_p);
					break;
				}

				default:
					break;
			}
		}

		ReportGeometryUpdate(request);
	}

	void Section::BroadcastUpdate (const GeometryUpdateRequest& request)
	{
	}

	ResponseType Section::Draw (const RedrawEvent& event)
	{
		for(std::set<AbstractWidget*>::iterator it = m_set.begin(); it != m_set.end(); it++)
		{
			DispatchDrawEvent(*it, event);
		}

		return Accept;
	}

	ResponseType Section::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Section::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Section::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Section::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Section::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Section::MousePressEvent (const MouseEvent& event)
	{
		ResponseType response = Ignore;
		AbstractWidget* widget = 0;

		AbstractWidget* original_focused_widget = m_focused_widget;	// mouse press event may change the focused widget
		bool focus_widget_changed = false;

		// check the widget on which the cursor hover

		if(m_last_hover_widget) {

			if(m_last_hover_widget->IsHover(event.position())) {

				DBG_PRINT_MSG("last widget %s is under cursor", m_last_hover_widget->name().c_str());
				UpdateHoverWidget(event);

			} else {

				m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

				while (m_last_hover_widget->container()) {

					if(m_last_hover_widget->container() == this) {	// FIXME: the widget may be mvoed to another context
						DBG_PRINT_MSG("last widget %s is not under cursor and container is context", m_last_hover_widget->name().c_str());
						m_last_hover_widget = 0;
						break;
					} else {
						DBG_PRINT_MSG("last widget %s is not under cursor but container is", m_last_hover_widget->name().c_str());
						m_last_hover_widget = m_last_hover_widget->container();

						if(m_last_hover_widget->IsHover(event.position())) {
							//events()->connect(m_last_hover_widget->destroyed(), this, &Section::OnHoverWidgetDestroyed);
							break;
						} else {
							m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
						}

					}

				}

				if(m_last_hover_widget)
					UpdateHoverWidget(event);

			}

		} else {
			for(std::set<AbstractWidget*>::iterator it = m_set.begin();
					it != m_set.end();
					it++)
			{
				if((*it)->Contain(event.position())) {

					DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					m_last_hover_widget = *it;
					events()->connect(m_last_hover_widget->destroyed(), this, &Section::OnHoverWidgetDestroyed);

					UpdateHoverWidget(event);
					break;
				}
			}
		}

		if(m_last_hover_widget) {
			DBG_PRINT_MSG("last hover widget: %s", m_last_hover_widget->name().c_str());
		}

		if(m_last_hover_widget) {

			response = m_last_hover_widget->MousePressEvent(event);

			if(original_focused_widget != m_focused_widget) {
				focus_widget_changed = true;
			}

			if(response == Accept || response == AcceptAndBreak) {
				widget = m_last_hover_widget;
			} else {

				AbstractWidget* parent = m_last_hover_widget->container();
				while (parent && parent != this) {

					response = parent->MousePressEvent(event);
					if(response == Accept || response == AcceptAndBreak) {
						widget = parent;
						break;
					}

					parent = m_last_hover_widget->container();

				}

			}

		}

		if(focus_widget_changed) {

			/*
			if(original_focused_widget) {
				original_focused_widget->set_focus(false);
				original_focused_widget->FocusEvent(false);
			}
			*/

		} else {

			m_focused_widget = original_focused_widget;
			SetFocusedWidget(widget);

		}

		if(m_focused_widget) {
			DBG_PRINT_MSG("focus widget: %s", m_focused_widget->name().c_str());
		}

		if(widget != 0)
			return Accept;

		return Accept;
	}

	ResponseType Section::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType response;

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseReleaseEvent(event);

			// Check the event status
			if(response == Accept) {

			}
		}

		return Accept;
	}

	ResponseType Section::MouseMoveEvent (const MouseEvent& event)
	{
		ResponseType response;

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseMoveEvent(event);

			if(response == AcceptAndBreak)
				return Accept;
			// check the event status
		}

		// check the widget on which the cursor hover

		if(m_last_hover_widget) {

			if(m_last_hover_widget->IsHover(event.position())) {

				//DBG_PRINT_MSG("last widget %s is under cursor", m_last_hover_widget->name().c_str());
				UpdateHoverWidget(event);

			} else {

				m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

				while (m_last_hover_widget->container()) {

					if(m_last_hover_widget->container() == this) {	// FIXME: the widget may be mvoed to another context
						//DBG_PRINT_MSG("last widget %s is not under cursor and container is context", m_last_hover_widget->name().c_str());
						m_last_hover_widget = 0;
						break;
					} else {
						//DBG_PRINT_MSG("last widget %s is not under cursor but container is", m_last_hover_widget->name().c_str());
						m_last_hover_widget = m_last_hover_widget->container();

						if(m_last_hover_widget->IsHover(event.position())) {
							events()->connect(m_last_hover_widget->destroyed(), this, &Section::OnHoverWidgetDestroyed);
							break;
						} else {
							m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
						}

					}

				}

				if(m_last_hover_widget)
					UpdateHoverWidget(event);

			}

		} else {
			for(std::set<AbstractWidget*>::iterator it = m_set.begin();
					it != m_set.end();
					it++)
			{
				if((*it)->Contain(event.position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					m_last_hover_widget = *it;
					events()->connect(m_last_hover_widget->destroyed(), this, &Section::OnHoverWidgetDestroyed);

					UpdateHoverWidget(event);
					break;
				}
			}
		}


		if(m_last_hover_widget) {
			response = m_last_hover_widget->MouseMoveEvent(event);
			return response;
		}

		return Ignore;
	}

	bool Section::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Warning: %s", "widget pointer is 0");
			return false;
		}

		assert(widget->container() == this);

		std::set<AbstractWidget*>::iterator it = std::find(m_set.begin(), m_set.end(), widget);

		if(it != m_set.end()) {
			widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyed);
			m_set.erase(it);
			SetContainer(widget, 0);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
					widget->name().c_str(),
					name().c_str());
			return false;
		}

		if(widget->hover()) {
			// Unset hover flag
		}

		if(widget->focused()) {
			// TODO: unset focused widget
		}

		CheckSubWidgetRemovedInContainer(widget);

		return true;
	}

	IteratorPtr Section::CreateIterator (const DeviceEvent& event)
	{
		IteratorPtr ret;
		return ret;
	}

	void Section::DispatchDrawEvent (AbstractWidget* widget,
	        const RedrawEvent& event)
	{
		if (widget->visiable()) {

			if(widget->drop_shadow() && widget->m_shadow) {
				widget->m_shadow->DrawAt(event.projection_matrix() * event.view_matrix(), widget->position());
			}

			widget->Draw(event);

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(widget);
			if (p) {

				if(p->scissor_test()) {
					m_scissor_status.Push(p->position().x() + p->margin().left(),
							p->position().y() + p->margin().right(),
							p->size().width() - p->margin().left() - p->margin().right(),
							p->size().height() - p->margin().top() - p->margin().bottom());
				}

				IteratorPtr it = p->CreateIterator(event);

				if(m_scissor_status.valid()) {
					m_scissor_status.Enable();

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}

				} else {

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}
				}

				if(p->scissor_test()) {
					m_scissor_status.Pop();
					m_scissor_status.Disable();
				}

			}

		}
	}

	void Section::UpdateHoverWidget (const MouseEvent& event)
	{
		AbstractContainer* p = dynamic_cast<AbstractContainer*>(m_last_hover_widget);

		if (p) {

			IteratorPtr it = p->CreateIterator(event);
			AbstractWidget* widget = 0;
			for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {

				widget = it->GetWidget();
				if(widget->Contain(event.position())) {
					m_last_hover_widget = widget;
					events()->connect(m_last_hover_widget->destroyed(), this, &Section::OnHoverWidgetDestroyed);

					UpdateHoverWidget(event);
					break;
				}
			}

		}
	}

	void Section::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		RemoveSubWidget(widget);

		if(m_set.size() == 0) {

			DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());

			delete this;
		}
	}

	void Section::OnHoverWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("%s", "HERE");
		while (m_last_hover_widget != widget) {
			m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
			m_last_hover_widget = m_last_hover_widget->container();
		}

		m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

		m_last_hover_widget = m_last_hover_widget->container();
		if(m_last_hover_widget == this)
			m_last_hover_widget = 0;

		DBG_PRINT_MSG("%s", "HERE");

	}

}
