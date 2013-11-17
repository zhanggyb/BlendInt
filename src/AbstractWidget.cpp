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

#include <GL/glew.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>

#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/ContextManager.hpp>
#include <BlendInt/AbstractLayout.hpp>

namespace BlendInt {

	using namespace std;

	AbstractWidget::AbstractWidget ()
		: AbstractExtraForm(),
		  m_z(0),
		  m_lock(false),
		  m_fire_events(true)
#ifdef DEBUG
		  ,m_id(0)
#endif

	{
#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (AbstractWidget::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif

		m_events.reset(new Cpp::ConnectionScope);
	}

	AbstractWidget::AbstractWidget (AbstractWidget* parent)
		: AbstractExtraForm(),
			m_z(0),
		  m_lock(false),
		  m_fire_events(true)
#ifdef DEBUG
		  , m_id(0)
#endif

	{
		bind_to(parent);

#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (AbstractWidget::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif

		m_events.reset(new Cpp::ConnectionScope);
	}

	AbstractWidget::~AbstractWidget ()
	{
		if(m_parent.object.nameless) {
			if(m_parent.type == ParentContextManager) {
				ContextManager::instance()->unbind(this);
			}
			if(m_parent.type == ParentForm) {
				m_parent.object.form->m_children.erase(this);
			}
		}

		// delete all child objects in list
		std::set<AbstractWidget*>::iterator it;
		for(it = m_children.begin(); it != m_children.end(); it++)
		{
			// MUST set the m_parent to avoid double set::erase in child's destruction
			(*it)->m_parent.type = ParentUnknown;
			(*it)->m_parent.object.nameless = 0;
			delete *it;
		}

		m_children.clear();

#ifdef DEBUG
		unregister_from_map();
#endif
	}

	bool AbstractWidget::bind (AbstractWidget* child)
	{
		if (!child) return false;
		if (child == this) return false;	// cannot bind self

		if (child->m_z != m_z) {
			std::cerr << "Cannot bind a child in different layer" << std::endl;
			// TODO throw an exception
			return false;
		}

		if (child->m_parent.object.nameless) {
			if (child->m_parent.type == ParentContextManager) {
				ContextManager::instance()->unbind(child);
			}
			if (child->m_parent.type == ParentForm) {
				if(child->m_parent.object.form == this) return true;
				child->m_parent.object.form->m_children.erase(child);
			}
		}
		child->m_parent.type = ParentForm;
		child->m_parent.object.form = this;

		m_children.insert(child);

		return true;
	}

	bool AbstractWidget::unbind (AbstractWidget* child)
	{
		if(!child) return false;
		if(child == this) return false;

		if(!m_children.count(child))
			return false;

		child->m_parent.type = ParentUnknown;
		child->m_parent.object.nameless = 0;

		m_children.erase(child);

		return true;
	}

	void AbstractWidget::unbind ()
	{
		if (m_parent.object.nameless) {
			if (m_parent.type == ParentContextManager) {
				ContextManager::instance()->unbind(this);
			}
			if (m_parent.type == ParentForm) {
				m_parent.object.form->m_children.erase(this);
			}
		}

		m_parent.type = ParentUnknown;
		m_parent.object.nameless = 0;
	}

	bool AbstractWidget::bind_to (ContextManager *parent)
	{
		if(!parent) return false;

		if (m_parent.object.nameless) {
			if (m_parent.type == ParentForm) {
				m_parent.object.form->m_children.erase(this);
				m_parent.type = ParentUnknown;
				m_parent.object.nameless = 0;
			}
		}

		parent->bind(this);

		return true;
	}

	bool AbstractWidget::bind_to (AbstractWidget* parent)
	{
		if(!parent) return false;
		if(parent == this) return false;	// cannot bind_to self

		if(parent->m_z != m_z) {
			std::cerr << "Cannot bind to a parent in different layer" << std::endl;
			// TODO: throw an exception
			return false;
		}

		if (m_parent.object.nameless) {
			if (m_parent.type == ParentContextManager) {
				m_parent.object.context->unbind(this);
			}
			if (m_parent.type == ParentForm) {
				if (m_parent.object.form == parent) return true;

				m_parent.object.form->m_children.erase(this);
			}
		}

		parent->m_children.insert (this);
		m_parent.type = ParentForm;
		m_parent.object.form = parent;

		return true;
	}

	bool AbstractWidget::is_bound ()
	{
		Parent* parent = &m_parent;
		while (parent->type == ParentForm) {
			parent = &(parent->object.form->m_parent);
		}

		if (parent->type == ParentUnknown) return false;

		// return true if parent type is Context Manager
		return true;
	}

	void AbstractWidget::resize (unsigned int width, unsigned int height)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_lock) return;

		if(AbstractExtraForm::resize(width, height)) {
			fire_property_changed_event(FormSize);
		}
	}

	void AbstractWidget::resize (const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_lock) return;

		if(AbstractExtraForm::resize(size)) {
			fire_property_changed_event(FormSize);
		}
	}

	void AbstractWidget::set_position (int x, int y)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_lock) return;

		if(AbstractExtraForm::set_position(x, y)) {
			fire_property_changed_event(FormPosition);
		}
	}

	void AbstractWidget::set_position (const Point& pos)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_lock) return;

		if(AbstractExtraForm::set_position(pos)) {
			fire_property_changed_event(FormPosition);
		}
	}

	void AbstractWidget::set_preferred_size(unsigned int width, unsigned int height)
	{
		if(AbstractExtraForm::set_preferred_size(width, height)) {
			fire_property_changed_event(FormPreferredSize);
		}
	}

	void AbstractWidget::set_preferred_size(const Size& size)
	{
		if(AbstractExtraForm::set_preferred_size(size)) {
			fire_property_changed_event(FormPreferredSize);
		}
	}

	void AbstractWidget::set_minimal_size(unsigned int width, unsigned int height)
	{
		if(AbstractExtraForm::set_minimal_size(width, height)) {
			fire_property_changed_event(FormMinimalSize);
		}
	}

	void AbstractWidget::set_minimal_size(const Size& size)
	{
		if(AbstractExtraForm::set_minimal_size(size)) {
			fire_property_changed_event(FormMinimalSize);
		}
	}

	void AbstractWidget::set_maximal_size(unsigned int width, unsigned int height)
	{
		if(AbstractExtraForm::set_maximal_size(width, height)) {
			fire_property_changed_event(FormMaximalSize);
		}
	}

	void AbstractWidget::set_maximal_size(const Size& size)
	{
		if(AbstractExtraForm::set_maximal_size(size)) {
			fire_property_changed_event(FormMaximalSize);
		}
	}

	void AbstractWidget::reset_z (int z)
	{
		if (m_z == z) return;

		AbstractWidget* root = 0;
		Parent* parent = &m_parent;
		while (parent->type == ParentForm) {
			root = parent->object.form;
			parent = &(parent->object.form->m_parent);
		}

		if (root)
			root->set_z_simple(z);
		else
			set_z_simple(z);

		if(root) {
			if (root->m_parent.type == ParentContextManager) {
					ContextManager::instance()->bind(root);
			}
		} else {
			if (m_parent.type == ParentContextManager) {
					ContextManager::instance()->bind(this);
			}
		}

		// m_property_changed.fire(FormPropertyLayer);
	}

	const std::string& AbstractWidget::name () const
	{
		return m_name;
	}

	void AbstractWidget::set_name (const std::string& name)
	{
		m_name = name;
	}

	bool AbstractWidget::contain(const Coord2d& cursor)
	{
		if (cursor.x() < position().x() ||
				cursor.y() < position().y() ||
				cursor.x() > (position().x() + size().width()) ||
				cursor.y() > (position().y() + size().height())) {
			return false;
		}

		return true;
	}

	void AbstractWidget::set_z_simple (int z)
	{
		m_z = z;

		std::set<AbstractWidget*>::iterator it;
		for (it = m_children.begin(); it != m_children.end(); it++)
		{
			(*it)->set_z_simple (z);
		}

		// TODO: call update()
	}

	void AbstractWidget::dispatch_key_press_event (AbstractWidget* obj,
	        KeyEvent* event)
	{
		obj->press_key(event);
	}

	void AbstractWidget::dispatch_mouse_move_event (AbstractWidget* obj,
	        MouseEvent* event)
	{
		obj->move_mouse(event);
	}

	void AbstractWidget::dispatch_mouse_press_event (AbstractWidget* obj,
	        MouseEvent* event)
	{
		obj->press_mouse(event);
	}

	void AbstractWidget::dispatch_mouse_release_event (AbstractWidget* obj,
	        MouseEvent* event)
	{
		obj->release_mouse(event);
	}

#ifdef DEBUG

	uint64_t AbstractWidget::id_last = 1;

	map<uint64_t, AbstractWidget*> AbstractWidget::obj_map;

	inline bool AbstractWidget::register_in_map ()
	{
		AbstractWidget::obj_map[m_id] = this;
		return true;
	}

	inline bool AbstractWidget::unregister_from_map ()
	{
		AbstractWidget::obj_map.erase(m_id);
		return true;
	}

	AbstractWidget* AbstractWidget::find (uint64_t id)
	{
		AbstractWidget *ret = NULL;
		if (AbstractWidget::obj_map.count(id) == 1)
			ret = AbstractWidget::obj_map[id];

		return ret;
	}

	void AbstractWidget::print()
	{
		map<uint64_t, AbstractWidget*>::iterator it;
		std::cerr << "Print objects: "<< std::endl;
		for(it = obj_map.begin(); it != obj_map.end(); it++)
		{
			std::cerr << it->second->m_name << " ";
		}
		std::cerr << std::endl;

	}

#endif

} /* namespace BlendInt */

