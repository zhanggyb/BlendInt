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

//			if(widget->focused()) {
//				assert(widget == m_focused_widget);
//				widget->set_focus(false);
//				m_focused_widget = 0;
//			}

			// disconnect events

			if(widget->hover()) {

				while (m_last_hover_widget && m_last_hover_widget != widget) {
					m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(false);
					m_last_hover_widget = m_last_hover_widget->container();
				}

				if(m_last_hover_widget) {
					assert(m_last_hover_widget == widget);
					m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(false);
				}

			}

			widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyed);

			widget->m_container = 0;

			if(widget->managed() && (widget->count() == 0)) {
				delete widget;
			}
		}

		m_set.clear();
	}

	void Section::Insert (AbstractWidget* widget)
	{
		if (!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return;
		}

		if (widget->container()) {
			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
				        widget->name().c_str(), name().c_str());
				return;
			} else {
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}
		}

		m_set.insert(widget);
		SetContainer(widget, this);

		// set shadow
		EnableShadow(widget);

		events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);
	}

	void Section::Remove (AbstractWidget* widget)
	{
		RemoveSubWidget(widget);

		if(m_set.size() == 0) {

			if(managed() && (count() == 0)) {
				DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}

		}
	}

	bool Section::Contain (const Point& point) const
	{
		return true;
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

		if (request.source()->container() == this) {

			switch (request.type()) {

				case WidgetPosition: {
					//m_layers[request.source()->z()].m_hover_list_valid = false;
					break;
				}

				case WidgetSize: {
					//m_layers[request.source()->z()].m_hover_list_valid = false;

					const Size* size_p =
					        static_cast<const Size*>(request.data());

					if (request.source()->drop_shadow()
							&& request.source()->m_shadow) {
						request.source()->m_shadow->Resize(*size_p);
					}

					break;
				}

				case WidgetRoundCornerType: {
					//m_layers[request.source()->z()].m_hover_list_valid = false;

					const int* type_p =
					        static_cast<const int*>(request.data());

					if (request.source()->drop_shadow()
							&& request.source()->m_shadow) {
						request.source()->m_shadow->SetRoundType(*type_p);
					}

					break;
				}

				case WidgetRoundCornerRadius: {
					//m_layers[request.source()->z()].m_hover_list_valid = false;

					const float* radius_p =
					        static_cast<const float*>(request.data());

					if (request.source()->drop_shadow()
							&& request.source()->m_shadow) {
						request.source()->m_shadow->SetRadius(*radius_p);
					}

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
		CheckAndUpdateHoverWidget(event);

		if(m_last_hover_widget) {

			/*
			{
				DBG_PRINT_MSG("print hover widgets in section %s", name().c_str());
				AbstractWidget* hover_widget = m_last_hover_widget;

				while(hover_widget != this) {
					DBG_PRINT_MSG("\t%s", hover_widget->name().c_str());
					hover_widget = hover_widget->container();
				}
			}
			*/

			return DispatchMousePressEvent(m_last_hover_widget, event);

		} else {
			return Ignore;
		}
	}

	ResponseType Section::MouseReleaseEvent (const MouseEvent& event)
	{
		CheckAndUpdateHoverWidget(event);

		if(m_last_hover_widget) {

			/*
			{
				DBG_PRINT_MSG("print hover widgets in section %s", name().c_str());
				AbstractWidget* hover_widget = m_last_hover_widget;

				while(hover_widget != this) {
					DBG_PRINT_MSG("\t%s", hover_widget->name().c_str());
					hover_widget = hover_widget->container();
				}
			}
			*/

			return DispatchMouseReleaseEvent(m_last_hover_widget, event);

		} else {
			return Ignore;
		}
	}

	ResponseType Section::MouseMoveEvent (const MouseEvent& event)
	{
		CheckAndUpdateHoverWidget(event);

		if(m_last_hover_widget) {
			return m_last_hover_widget->MouseMoveEvent(event);
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

		if(it == m_set.end()) {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
					widget->name().c_str(),
					name().c_str());
			return false;
		}

		widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyed);
		m_set.erase(it);
		SetContainer(widget, 0);

		if(widget->hover()) {

			while (m_last_hover_widget && m_last_hover_widget != widget) {
				m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
				m_last_hover_widget->set_hover(false);
				m_last_hover_widget = m_last_hover_widget->container();
			}

			if(m_last_hover_widget) {
				assert(m_last_hover_widget == widget);
				m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
				m_last_hover_widget->set_hover(false);
				m_last_hover_widget = 0;
			}

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
				widget->m_shadow->Draw(glm::vec3(widget->position().x(), widget->position().y(), 0.f));
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

	bool Section::CheckAndUpdateHoverWidget (const MouseEvent& event)
	{
		if (m_last_hover_widget) {

			if (IsHoverThrough(m_last_hover_widget, event.position())) {
				//DBG_PRINT_MSG("last widget %s is under cursor", m_last_hover_widget->name().c_str());
				UpdateHoverWidgetSubs(event);
			} else {
				m_last_hover_widget->destroyed().disconnectOne(this,
				        &Section::OnHoverWidgetDestroyed);
				m_last_hover_widget->set_hover(false);
				m_last_hover_widget->CursorEnterEvent(false);

				// find which contianer contains cursor position
				while (m_last_hover_widget->container()) {

					if (m_last_hover_widget->container() == this) {	// FIXME: the widget may be mvoed to another context
						//DBG_PRINT_MSG("last widget %s is not under cursor and container is context", m_last_hover_widget->name().c_str());
						m_last_hover_widget = 0;
						break;
					} else {
						//DBG_PRINT_MSG("last widget %s is not under cursor but container is", m_last_hover_widget->name().c_str());
						m_last_hover_widget = m_last_hover_widget->container();

						if (IsHoverThrough(m_last_hover_widget, event.position())) {
							break;
						} else {
							m_last_hover_widget->destroyed().disconnectOne(this,
							        &Section::OnHoverWidgetDestroyed);
							m_last_hover_widget->set_hover(false);
							m_last_hover_widget->CursorEnterEvent(false);
						}
					}
				}

				if (m_last_hover_widget) {
					UpdateHoverWidgetSubs(event);
				}
			}

		} else {
			for (std::set<AbstractWidget*>::iterator it = m_set.begin();
			        it != m_set.end(); it++) {
				if ((*it)->Contain(event.position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					m_last_hover_widget = *it;
					events()->connect(m_last_hover_widget->destroyed(), this,
					        &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(true);
					m_last_hover_widget->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}
		}

		return m_last_hover_widget != 0;
	}

	void Section::UpdateHoverWidgetSubs (const MouseEvent& event)
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
					m_last_hover_widget->set_hover(true);
					m_last_hover_widget->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	void Section::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		RemoveSubWidget(widget);

		if(m_set.size() == 0) {

			if(managed() && (count() == 0)) {
				DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}

		}
	}

	ResponseType Section::DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMousePressEvent(widget->container(), event) == Ignore) {
					//DBG_PRINT_MSG("mouse press in %s and get ignore", widget->name().c_str());
					return widget->MousePressEvent(event);
				} else {
					return Accept;
				}

			} else {
				//DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MousePressEvent(event);
			}

		}
	}

	ResponseType Section::DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMouseReleaseEvent(widget->container(), event) == Ignore) {
					//DBG_PRINT_MSG("mouse press in %s and get ignore", widget->name().c_str());
					return widget->MouseReleaseEvent(event);
				} else {
					return Accept;
				}

			} else {
				DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MouseReleaseEvent(event);
			}

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
