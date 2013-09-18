/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_CONTEXTMANAGER_HPP_
#define _BIL_CONTEXTMANAGER_HPP_

#include <BILO/Drawable.hpp>
#include <map>
#include <list>

using std::map;
using std::list;

namespace BILO {

	class Interface;

	/**
	 * @brief class to hold and manage drawable objects for render
	 */
	class ContextManager
	{
	public:

		friend class Interface;

		static ContextManager* instance ();
		
		static bool initialize ();

		static void release ();

		/**
		 * @brief add the drawable in the context list
		 * @param obj
		 * @return false if object already exist and at the same layer
		 *
		 * add or update the Drawable object in the context list
		 */
		bool add_drawable (Drawable* obj);

		/**
		 * @breif remove a drawable from the context list
		 * @param obj
		 * @return
		 */
		bool remove_drawable (Drawable* obj);

		int index_size () const {return m_map.size();}

		int layer_size () const {return m_layers.size();}

#ifdef DEBUG

		void print ();

#endif

	private:

		typedef map<int, list<Drawable*>* > LayerType;
		typedef list<Drawable*> ListType;
		typedef map<Drawable*, int> IndexType;

		ContextManager ();

		~ContextManager ();

		map<int, list<Drawable*>* > m_layers;

		map<Drawable*, int> m_map;

		static ContextManager* context_manager;
		
	};

}

#endif	// _BIL_CONTEXTMANAGER_HPP_
