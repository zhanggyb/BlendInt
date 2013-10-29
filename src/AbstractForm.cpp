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
		  m_roundcorner (RoundCornerNone), m_corner_radius(5.0),
		  m_visible(true), m_in_layout(false),
		  m_expand_x(false), m_expand_y(false)
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
		  m_roundcorner (RoundCornerNone), m_corner_radius(5.0),
		  m_visible(true), m_in_layout(false),
		  m_expand_x(false), m_expand_y(false)
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

	void AbstractForm::resize (int w, int h)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_size.equal(w, h)) return;

		Size new_size (w, h);
		if (update(FormPropertySize, &new_size))
			m_size = new_size;
	}

	void AbstractForm::resize (const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_size.equal(size)) return;

		Size new_size(size);
		if (update(FormPropertySize, &new_size)) m_size = new_size;
	}

	void AbstractForm::set_preferred_size(const Size& size)
	{
		// TODO: check the param first

		m_preferred_size = size;
	}

	void AbstractForm::set_preferred_size(int width, int height)
	{
		// TODO: check the param first

		m_preferred_size.set_width(width);
		m_preferred_size.set_height(height);
	}

	void AbstractForm::set_minimal_size(const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_minimal_size.equal(size)) return;

		Size new_min_size(size);
		if (update(FormPropertyMinimalSize, &new_min_size)) m_minimal_size = new_min_size;
	}

	void AbstractForm::set_minimal_size(int w, int h)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_minimal_size.equal(w, h)) return;

		Size new_min_size(w, h);
		if (update(FormPropertyMinimalSize, &new_min_size)) m_minimal_size = new_min_size;
	}

	const Point& AbstractForm::pos () const
	{
		return m_pos;
	}

	void AbstractForm::set_pos (int x, int y)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_pos.equal(x, y)) return;

		Point new_pos(x, y);
		if (update(FormPropertyPosition, &new_pos)) m_pos = new_pos;
	}

	void AbstractForm::set_pos (const Point& pos)
	{
		// If the object is managed by a layout, disallow position setting
		if(m_in_layout) return;

		if (m_pos.equal(pos)) return;

		Point new_pos(pos);
		if (update(FormPropertyPosition, &new_pos)) m_pos = new_pos;
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
	}

	void AbstractForm::set_roundcorner (int type)
	{
		if (m_roundcorner == type) return;

		int new_type = type;
		if(update(FormPropertyRoundCorner, &new_type)) m_roundcorner = new_type;
	}

	void AbstractForm::set_corner_radius (float radius)
	{
		if (m_corner_radius == radius) return;

		if(update(FormPropertyRoundCorner, &radius)) m_corner_radius = radius;
	}

	int AbstractForm::roundcorner () const
	{
		return m_roundcorner;
	}

	bool AbstractForm::visible () const
	{
		return m_visible;
	}

	void AbstractForm::set_visible (bool visible)
	{
		if (update (FormPropertyVisibility, &visible)) m_visible = visible;
	}

	void AbstractForm::show ()
	{
		bool visiable = true;
		if (update (FormPropertyVisibility, &visiable)) m_visible = true;
	}

	void AbstractForm::hide ()
	{
		bool visible = false;

		if (update (FormPropertyVisibility, &visible)) m_visible = false;
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
		if (cursor.x() < m_pos.x() ||
				cursor.y() < m_pos.y() ||
				cursor.x() > (m_pos.x() + m_size.width()) ||
				cursor.y() > (m_pos.y() + m_size.height())) {
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
		if (obj->m_pos.equal(x, y)) return;

		Point new_pos(x, y);
		if (obj->update(FormPropertyPosition, &new_pos)) obj->m_pos = new_pos;
	}

	void AbstractForm::set_pos_priv (AbstractForm* obj, const Point& pos)
	{
		if (obj->m_pos.equal(pos)) return;

		Point new_pos(pos);
		if (obj->update(FormPropertyPosition, &new_pos)) obj->m_pos = new_pos;
	}

	void AbstractForm::resize_priv (AbstractForm* obj, int w, int h)
	{
		if (obj->m_size.equal(w, h)) return;

		Size new_size (w, h);

		if (obj->update(FormPropertySize, &new_size))
			obj->m_size = new_size;
	}

	void AbstractForm::resize_priv (AbstractForm* obj, const Size& size)
	{
		if (obj->m_size.equal(size)) return;

		Size new_size(size);
		if (obj->update(FormPropertySize, &new_size))
			obj->m_size = new_size;
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
