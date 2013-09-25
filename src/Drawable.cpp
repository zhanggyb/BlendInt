/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>

#include <BILO/Drawable.hpp>
#include <BILO/ContextManager.hpp>

namespace BILO {

	using namespace std;

	Drawable::Drawable ()
		: m_z(0),
		  m_roundcorner (RoundCornerNone), m_corner_radius(0.0), m_visible(true)
#ifdef DEBUG
		  ,m_id(0)
#endif

	{
#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (Drawable::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif
	}


	Drawable::Drawable (Drawable* parent)
		: m_z(0),
		  m_roundcorner (RoundCornerNone), m_corner_radius(0.0), m_visible(true)
#ifdef DEBUG
		  , m_id(0)
#endif

	{
		bind_to(parent);

#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (Drawable::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif

	}

	Drawable::~Drawable ()
	{
		if(m_parent.object.nameless) {
			if(m_parent.type == ParentContextManager) {
				ContextManager::instance()->unbind(this);
			}
			if(m_parent.type == ParentDrawable) {
				m_parent.object.drawable->m_children.erase(this);
			}
		}

		// delete all child objects in list
		std::set<Drawable*>::iterator it;
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

	bool Drawable::bind (Drawable* child)
	{
		if (!child) return false;
		if (child == this) return false;	// cannot bind self

		if (child->m_z != m_z) {
			std::cerr << "Cannot bind a child in different layer" << std::endl;
			return false;
		}

		if (child->m_parent.object.nameless) {
			if (child->m_parent.type == ParentContextManager) {
				ContextManager::instance()->unbind(child);
			}
			if (child->m_parent.type == ParentDrawable) {
				if(child->m_parent.object.drawable == this) return true;
				child->m_parent.object.drawable->m_children.erase(child);
			}
		}
		child->m_parent.type = ParentDrawable;
		child->m_parent.object.drawable = this;

		m_children.insert(child);

		return true;
	}

	bool Drawable::unbind (Drawable* child)
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

	void Drawable::unbind ()
	{
		if (m_parent.object.nameless) {
			if (m_parent.type == ParentContextManager) {
				ContextManager::instance()->unbind(this);
			}
			if (m_parent.type == ParentDrawable) {
				m_parent.object.drawable->m_children.erase(this);
			}
		}

		m_parent.type = ParentUnknown;
		m_parent.object.nameless = 0;
	}

	bool Drawable::bind_to (ContextManager *parent)
	{
		if(!parent) return false;

		if (m_parent.object.nameless) {
			if (m_parent.type == ParentDrawable) {
				m_parent.object.drawable->m_children.erase(this);
				m_parent.type = ParentUnknown;
				m_parent.object.nameless = 0;
			}
		}

		parent->bind(this);

		return true;
	}

	bool Drawable::bind_to (Drawable* parent)
	{
		if(!parent) return false;
		if(parent == this) return false;	// cannot bind_to self

		if(parent->m_z != m_z) {
			std::cerr << "Cannot bind to a parent in different layer" << std::endl;
			return false;
		}

		if (m_parent.object.nameless) {
			if (m_parent.type == ParentContextManager) {
				m_parent.object.context->unbind(this);
			}
			if (m_parent.type == ParentDrawable) {
				if (m_parent.object.drawable == parent) return true;

				m_parent.object.drawable->m_children.erase(this);
			}
		}

		parent->m_children.insert (this);
		m_parent.type = ParentDrawable;
		m_parent.object.drawable = parent;

		return true;
	}

	bool Drawable::is_bound ()
	{
		Parent* parent = &m_parent;
		while (parent->type == ParentDrawable) {
			parent = &(parent->object.drawable->m_parent);
		}

		if (parent->type == ParentUnknown) return false;

		// return true if parent type is Context Manager
		return true;
	}

	const Size& Drawable::size () const
	{
		return size_;
	}

	void Drawable::resize (int w, int h)
	{
		if (size_.equal(w, h)) return;
		size_.set_width(w);
		size_.set_height(h);
		update(WidgetPropertySize);
	}

	void Drawable::resize (const Size& size)
	{
		if (size_.equal(size)) return;

		size_ = size;
		update(WidgetPropertySize);
	}

	const Point& Drawable::pos () const
	{
		return pos_;
	}

	void Drawable::set_pos (int x, int y)
	{
		if (pos_.equal(x, y)) return;

		pos_.set_x(x);
		pos_.set_y(y);
		update(WidgetPropertyPosition);
	}

	void Drawable::set_pos (const Point& pos)
	{
		if (pos_.equal(pos)) return;

		pos_ = pos;
		update(WidgetPropertyPosition);
	}

	void Drawable::reset_z (int z)
	{
		if (m_z == z) return;

		Drawable* root = 0;
		Parent* parent = &m_parent;
		while (parent->type == ParentDrawable) {
			root = parent->object.drawable;
			parent = &(parent->object.drawable->m_parent);
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

		update (WidgetPropertyLayer);
	}

	const Margin& Drawable::margin () const
	{
		return margin_;
	}

	void Drawable::set_margin (int left, int right, int top, int bottom)
	{
		margin_.set_left(left);
		margin_.set_right(right);
		margin_.set_top(top);
		margin_.set_bottom(bottom);

		update (WidgetPropertyMargin);
	}

	void Drawable::set_margin (const Margin& margin)
	{
		margin_ = margin;
		update (WidgetPropertyMargin);
	}

	const Padding& Drawable::padding () const
	{
		return padding_;
	}

	void Drawable::set_padding (const Padding& padding)
	{
		padding_ = padding;

		update(WidgetPropertyPadding);
	}

	void Drawable::set_padding (int l, int r, int t, int b)
	{
		padding_.set_left(l);
		padding_.set_right(r);
		padding_.set_top(t);
		padding_.set_bottom(b);

		update(WidgetPropertyPadding);
	}

	void Drawable::set_roundcorner (RoundCornerType type)
	{
		if (m_roundcorner == type) return;

		m_roundcorner = type;
		update(WidgetPropertyRoundCorner);
	}

	void Drawable::set_corner_radius (float radius)
	{
		if (m_corner_radius == radius) return;

		m_corner_radius = radius;
		update(WidgetPropertyRoundCorner);
	}

	inline RoundCornerType Drawable::roundcorner () const
	{
		return m_roundcorner;
	}

	inline bool Drawable::visible () const
	{
		return m_visible;
	}

	void Drawable::set_visible (bool visible)
	{
		m_visible = visible;

		update (WidgetPropertyVisibility);
	}

	inline void Drawable::show ()
	{
		m_visible = true;

		update (WidgetPropertyVisibility);
	}

	inline void Drawable::hide ()
	{
		m_visible = false;

		update (WidgetPropertyVisibility);
	}

	const std::string& Drawable::name () const
	{
		return m_name;
	}

	void Drawable::set_name (const std::string& name)
	{
		m_name = name;
	}

	bool Drawable::contain(const Coord2d& cursor)
	{
		if (cursor.x() < pos_.x() ||
				cursor.y() < pos_.y() ||
				cursor.x() > (pos_.x() + size_.width()) ||
				cursor.y() > (pos_.y() + size_.height())) {
			return false;
		}

		return true;
	}

	void Drawable::set_z_simple (int z)
	{
		m_z = z;

		std::set<Drawable*>::iterator it;
		for (it = m_children.begin(); it != m_children.end(); it++)
		{
			(*it)->set_z_simple (z);
		}

		// TODO: call update()
	}

#ifdef DEBUG

	uint64_t Drawable::id_last = 1;

	map<uint64_t, Drawable*> Drawable::obj_map;

	inline bool Drawable::register_in_map ()
	{
		Drawable::obj_map[m_id] = this;
		return true;
	}

	inline bool Drawable::unregister_from_map ()
	{
		Drawable::obj_map.erase(m_id);
		return true;
	}

	Drawable* Drawable::find (uint64_t id)
	{
		Drawable *ret = NULL;
		if (Drawable::obj_map.count(id) == 1)
			ret = Drawable::obj_map[id];

		return ret;
	}

	void Drawable::print()
	{
		map<uint64_t, Drawable*>::iterator it;
		std::cerr << "Print objects: "<< std::endl;
		for(it = obj_map.begin(); it != obj_map.end(); it++)
		{
			std::cerr << it->second->m_name << " ";
		}
		std::cerr << std::endl;

	}

#endif

} /* namespace BILO */
