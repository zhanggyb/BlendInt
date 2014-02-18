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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>

#include <BlendInt/UI/AbstractWidget.hpp>
#include <BlendInt/ContextManager.hpp>
#include <BlendInt/UI/AbstractLayout.hpp>

namespace BlendInt {

	using namespace std;

	AbstractWidget::AbstractWidget ()
		: AbstractExtraForm(),
		  m_z(0),
		  m_parent(0)
	{
		m_events.reset(new Cpp::ConnectionScope);

		ContextManager::Instance()->SetFocusedWidget(this);
	}

	AbstractWidget::AbstractWidget (AbstractWidget* parent)
		: m_z(0),
			m_parent(0)
	{
		m_events.reset(new Cpp::ConnectionScope);

		if(parent) {
			parent->m_children.insert(this);
			m_parent = parent;
		}

		ContextManager::Instance()->SetFocusedWidget(this);
	}

	AbstractWidget::~AbstractWidget ()
	{
		if(m_flag[WidgetFlagContextHoverList]) {
			ContextManager::Instance()->RemoveWidgetFromHoverDeque(this);
		}

		if(m_flag[WidgetFlagFocus]) {
			ContextManager::Instance()->SetFocusedWidget(0);
		}

		if(m_parent) {
			m_parent->m_children.erase(this);
			m_parent = 0;
		}

		std::set<AbstractWidget*>::iterator it;
		while(m_children.size()) {
			it = m_children.begin();
			(*it)->m_parent = 0;
			if((*it)->ref_count() == 0)
				delete *it;
			m_children.erase(it);
		}

		m_destroyed.fire(this);
	}

	bool AbstractWidget::Register ()
	{
		return ContextManager::Instance()->Register(this);
	}

	bool AbstractWidget::Unregister ()
	{
		return ContextManager::Instance()->Unregister(this);
	}

	bool AbstractWidget::AddChild (AbstractWidget* child)
	{
		if(child->m_parent == this) return true;

		if(child->m_parent) {
			child->m_parent->m_children.erase(child);
		}

		child->m_parent = this;
		m_children.insert(child);

		if(child->layer() != layer()) {
			child->SetLayer(layer());
		}

		return true;
	}

	bool AbstractWidget::SetParent(AbstractWidget* parent)
	{
		if(m_parent == parent) return true;

		if(m_parent) {
			m_parent->m_children.erase(this);
		}

		m_parent = parent;
		m_parent->m_children.insert(this);

		if(layer() != m_parent->layer()) {
			SetLayer(m_parent->layer());
		}

		return true;
	}

	bool AbstractWidget::RemoveChild(AbstractWidget* child)
	{
		if(child->m_parent != this) return false;

		child->m_parent = 0;
		m_children.erase(child);

		return true;
	}

	void AbstractWidget::Resize (unsigned int width, unsigned int height)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);

		Update(FormSize, &new_size);

		set_size(width, height);

		fire_property_changed_event(FormSize);
	}

	void AbstractWidget::Resize (const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(AbstractWidget::size() == size) return;

		Update(FormSize, &size);

		set_size(size);

		fire_property_changed_event(FormSize);
	}

	void AbstractWidget::SetPosition (int x, int y)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);
		Update(FormPosition, &new_pos);

		set_position(x, y);

		fire_property_changed_event(FormPosition);
	}

	void AbstractWidget::SetPosition (const Point& pos)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(position() == pos) return;

		Update(FormPosition, &pos);

		set_position(pos);

		fire_property_changed_event(FormPosition);
	}

	void AbstractWidget::SetPreferredSize(unsigned int width, unsigned int height)
	{
		// check the param first
		if (width < minimal_size().width() ||
				height < minimal_size().height() ||
				width > maximal_size().width() ||
				height > maximal_size().height())
			return;

		if(preferred_size().width() == width && preferred_size().height() == height) return;

		Size new_pref_size(width, height);

		Update(FormPreferredSize, &new_pref_size);

		set_preferred_size(width, height);

		fire_property_changed_event(FormPreferredSize);
	}

	void AbstractWidget::SetPreferredSize(const Size& size)
	{
		if (size.width() < minimal_size().width() ||
				size.height() < minimal_size().height()||
				size.width() > maximal_size().width() ||
				size.height() > maximal_size().height())
			return;

		if(preferred_size() == size) return;

		Update(FormPreferredSize, &size);

		set_preferred_size(size);

		fire_property_changed_event(FormPreferredSize);
	}

	void AbstractWidget::SetMinimalSize(unsigned int width, unsigned int height)
	{
		if(width > preferred_size().width() ||
				height > preferred_size().height())
			return;

		if(minimal_size().width() == width && minimal_size().height() == height) return;

		Size new_min_size(width, height);

		Update(FormMinimalSize, &new_min_size);

		set_minimal_size(width, height);

		fire_property_changed_event(FormMinimalSize);
	}

	void AbstractWidget::SetMinimalSize(const Size& size)
	{
		if(size.width() > preferred_size().width() ||
				size.height() > preferred_size().height())
			return;

		if (minimal_size() == size) return;

		Update(FormMinimalSize, &size);

		set_minimal_size(size);

		fire_property_changed_event(FormMinimalSize);
	}

	void AbstractWidget::SetMaximalSize(unsigned int width, unsigned int height)
	{
		if(width < preferred_size().width() ||
				height < preferred_size().height())
			return;

		if(maximal_size().width() == width && maximal_size().height() == height) return;

		Size new_max_size (width, height);

		Update(FormMaximalSize, &new_max_size);

		set_maximal_size(new_max_size);

		fire_property_changed_event(FormMaximalSize);
	}

	void AbstractWidget::SetMaximalSize(const Size& size)
	{
		if(size.width() < preferred_size().width() ||
				size.height() < preferred_size().height())
			return;

		if(maximal_size() == size) return;

		Update(FormMaximalSize, &size);

		set_maximal_size(size);

		fire_property_changed_event(FormMaximalSize);
	}

	void AbstractWidget::SetLayer (int z)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(m_z == z) return;

		if(m_flag[2]) {
			ContextManager::Instance()->Unregister(this);
			Update (WidgetLayer, &z);
			m_z = z;
			ContextManager::Instance()->Register(this);
		} else {
			Update (WidgetLayer, &z);
			m_z = z;
		}

		fire_property_changed_event(WidgetLayer);
	}


	//void AbstractWidget::reset_z (int z)
	//{
		//if (m_z == z) return;

//		AbstractWidget* root = 0;
//		Parent* parent = &m_parent;
//		while (parent->type == ParentForm) {
//			root = parent->object.form;
//			parent = &(parent->object.form->m_parent);
//		}
//
//		if (root)
//			root->set_z_simple(z);
//		else
//			set_z_simple(z);
//
//		if(root) {
//			if (root->m_parent.type == ParentContextManager) {
//					ContextManager::Instance()->Bind(root);
//			}
//		} else {
//			if (m_parent.type == ParentContextManager) {
//					ContextManager::Instance()->Bind(this);
//			}
//		}

		//set_z_simple(z);
		// m_property_changed.fire(FormPropertyLayer);
	//}

	//void AbstractWidget::set_z_simple (int z)
	//{
	//	m_z = z;

//		std::set<AbstractWidget*>::iterator it;
//		for (it = m_children.begin(); it != m_children.end(); it++)
//		{
//			(*it)->set_z_simple (z);
//		}

		// TODO: call Update()
	//}

	void AbstractWidget::SetPosition(AbstractWidget* obj, int x, int y)
	{
		if(obj->position().x() == x && obj->position().y() == y) return;

		Point new_pos (x, y);
		obj->Update(FormPosition, &new_pos);

		obj->set_position(x, y);
	}

	void AbstractWidget::SetPosition(AbstractWidget* obj, const Point& pos)
	{
		if(obj->position() == pos) return;

		obj->Update(FormPosition, &pos);

		obj->set_position(pos);
	}

	void AbstractWidget::Resize (AbstractWidget* obj, unsigned int width, unsigned int height)
	{
		if(obj->size().width() == width && obj->size().height() == height) return;

		Size new_size (width, height);

		obj->Update(FormSize, &new_size);

		obj->set_size(width, height);
	}

	void AbstractWidget::Resize (AbstractWidget* obj, const Size& size)
	{
		if(obj->size() == size) return;

		obj->Update(FormSize, &size);

		obj->set_size(size);
	}

	void AbstractWidget::dispatch_key_press_event (AbstractWidget* obj,
	        KeyEvent* event)
	{
		obj->KeyPressEvent(event);
	}

	void AbstractWidget::dispatch_mouse_move_event (AbstractWidget* obj,
	        MouseEvent* event)
	{
		obj->MouseMoveEvent(event);
	}

	void AbstractWidget::dispatch_mouse_press_event (AbstractWidget* obj,
	        MouseEvent* event)
	{
		obj->MousePressEvent(event);
	}

	void AbstractWidget::dispatch_mouse_release_event (AbstractWidget* obj,
	        MouseEvent* event)
	{
		obj->MouseReleaseEvent(event);
	}

} /* namespace BlendInt */

