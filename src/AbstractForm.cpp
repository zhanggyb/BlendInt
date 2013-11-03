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

#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/ContextManager.hpp>
#include <BlendInt/AbstractLayout.hpp>

namespace BlendInt {

	using namespace std;

	AbstractForm::AbstractForm ()
		: m_z(0),
		  m_in_layout(false),
		  m_expand_x(false),
		  m_expand_y(false),
		  m_fire_events(true),
		  m_visible(true)
#ifdef DEBUG
		  ,m_id(0)
#endif

	{
#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (AbstractForm::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif
	}


	AbstractForm::AbstractForm (AbstractForm* parent)
		: m_z(0),
		  m_in_layout(false),
		  m_expand_x(false),
		  m_expand_y(false),
		  m_fire_events(true),
		  m_visible(true)
#ifdef DEBUG
		  , m_id(0)
#endif

	{
		bind_to(parent);

#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (AbstractForm::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif

	}

	AbstractForm::~AbstractForm ()
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
		std::set<AbstractForm*>::iterator it;
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

	bool AbstractForm::bind (AbstractForm* child)
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

	bool AbstractForm::unbind (AbstractForm* child)
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

	void AbstractForm::unbind ()
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

	bool AbstractForm::bind_to (ContextManager *parent)
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

	bool AbstractForm::bind_to (AbstractForm* parent)
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

	bool AbstractForm::is_bound ()
	{
		Parent* parent = &m_parent;
		while (parent->type == ParentForm) {
			parent = &(parent->object.form->m_parent);
		}

		if (parent->type == ParentUnknown) return false;

		// return true if parent type is Context Manager
		return true;
	}

	const Size& AbstractForm::size () const
	{
		return m_size;
	}

	void AbstractForm::resize (unsigned int width, unsigned int height)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_size.equal(width, height)) return;

		if(width < m_minimal_size.width() ||
				height < m_minimal_size.height())
			return;

		m_size.set_width(width);
		m_size.set_height(height);

		update(FormPropertySize);

		fire_property_changed_event(FormPropertySize);
	}

	void AbstractForm::resize (const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if(size.width() < m_minimal_size.width() ||
				size.height() < m_minimal_size.height())
			return;

		if (m_size.equal(size)) return;

		m_size = size;

		update(FormPropertySize);

		fire_property_changed_event(FormPropertySize);
	}

	void AbstractForm::set_preferred_size(const Size& size)
	{
		// check the param first
		if (size.width() < m_minimal_size.width() ||
				size.height() < m_minimal_size.height())
			return;

		if(m_preferred_size.equal(size)) return;

		m_preferred_size = size;

		fire_property_changed_event(FormPropertyPreferredSize);
	}

	void AbstractForm::set_preferred_size(unsigned int width, unsigned int height)
	{
		// check the param first
		if(width < m_minimal_size.width() ||
				height < m_minimal_size.height())
			return;

		if(m_preferred_size.equal(width, height)) return;

		m_preferred_size.set_width(width);
		m_preferred_size.set_height(height);

		fire_property_changed_event(FormPropertyPreferredSize);
	}

	void AbstractForm::set_minimal_size(const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_minimal_size.equal(size)) return;

		if(size.width() > m_preferred_size.width() ||
				size.height() > m_preferred_size.height())
			return;

		m_minimal_size = size;

		fire_property_changed_event(FormPropertyMinimalSize);
	}

	void AbstractForm::set_minimal_size(unsigned int width, unsigned int height)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_minimal_size.equal(width, height)) return;

		if(width > m_preferred_size.width() ||
				height > m_preferred_size.height())
			return;

		m_minimal_size.set_width(width);
		m_minimal_size.set_height(height);

		fire_property_changed_event(FormPropertyMinimalSize);
	}

	void AbstractForm::set_position (int x, int y)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_position.equal(x, y)) return;

		m_position.set_x(x);
		m_position.set_y(y);

		// m_property_changed.fire(FormPropertyPosition);
	}

	void AbstractForm::set_position (const Point& pos)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_position.equal(pos)) return;

		m_position = pos;

		//m_property_changed.fire(FormPropertyPosition);
	}

	void AbstractForm::reset_z (int z)
	{
		if (m_z == z) return;

		AbstractForm* root = 0;
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

	void AbstractForm::set_visible (bool visible)
	{
		m_visible = visible;
	}

	void AbstractForm::show ()
	{
		m_visible = true;
	}

	void AbstractForm::hide ()
	{
		m_visible = false;
	}

	const std::string& AbstractForm::name () const
	{
		return m_name;
	}

	void AbstractForm::set_name (const std::string& name)
	{
		m_name = name;
	}

	bool AbstractForm::contain(const Coord2d& cursor)
	{
		if (cursor.x() < m_position.x() ||
				cursor.y() < m_position.y() ||
				cursor.x() > (m_position.x() + m_size.width()) ||
				cursor.y() > (m_position.y() + m_size.height())) {
			return false;
		}

		return true;
	}

	void AbstractForm::set_z_simple (int z)
	{
		m_z = z;

		std::set<AbstractForm*>::iterator it;
		for (it = m_children.begin(); it != m_children.end(); it++)
		{
			(*it)->set_z_simple (z);
		}

		// TODO: call update()
	}

	void AbstractForm::set_pos_priv (AbstractForm* obj, int x, int y)
	{
		if (obj->m_position.equal(x, y)) return;

		obj->m_position.set_x(x);
		obj->m_position.set_y(y);
	}

	void AbstractForm::set_pos_priv (AbstractForm* obj, const Point& pos)
	{
		if (obj->m_position.equal(pos)) return;

		obj->m_position = pos;
	}

	void AbstractForm::resize_priv (AbstractForm* obj, unsigned int width, unsigned int height)
	{
		if (obj->m_size.equal(width, height)) return;

		if(width < obj->m_minimal_size.width() ||
				height < obj->m_minimal_size.height())
			return;

		obj->m_size.set_width(width);
		obj->m_size.set_height(height);

		obj->update(FormPropertySize);
	}

	void AbstractForm::resize_priv (AbstractForm* obj, const Size& size)
	{
		if (obj->m_size.equal(size)) return;

		if(size.width() < obj->m_minimal_size.width() ||
				size.height() < obj->m_minimal_size.height())
			return;

		obj->m_size = size;

		obj->update(FormPropertySize);
	}

#ifdef DEBUG

	uint64_t AbstractForm::id_last = 1;

	map<uint64_t, AbstractForm*> AbstractForm::obj_map;

	inline bool AbstractForm::register_in_map ()
	{
		AbstractForm::obj_map[m_id] = this;
		return true;
	}

	inline bool AbstractForm::unregister_from_map ()
	{
		AbstractForm::obj_map.erase(m_id);
		return true;
	}

	AbstractForm* AbstractForm::find (uint64_t id)
	{
		AbstractForm *ret = NULL;
		if (AbstractForm::obj_map.count(id) == 1)
			ret = AbstractForm::obj_map[id];

		return ret;
	}

	void AbstractForm::print()
	{
		map<uint64_t, AbstractForm*>::iterator it;
		std::cerr << "Print objects: "<< std::endl;
		for(it = obj_map.begin(); it != obj_map.end(); it++)
		{
			std::cerr << it->second->m_name << " ";
		}
		std::cerr << std::endl;

	}

#endif

} /* namespace BlendInt */
