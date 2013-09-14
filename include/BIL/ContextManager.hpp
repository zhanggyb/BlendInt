/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_CONTEXTMANAGER_HPP_
#define _BIL_CONTEXTMANAGER_HPP_

#include <BIL/Drawable.hpp>
#include <map>
#include <list>

namespace BIL {

	/**
	 * @brief class to hold and manage drawable objects for render
	 */
	class ContextManager
	{
	public:

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

	private:

		struct ContextLayer {
			int layer;
			std::list<Drawable*> list;
		};

		struct ContextIndex {
			int layer_index;
			int list_index;
		};

		ContextManager ();

		~ContextManager ();

		void update_map (int layer_index1, int layer_index2, int list_index);

		std::list<ContextLayer> m_layers;
		
		std::map<Drawable*, ContextIndex> m_index;

		static ContextManager* context_manager;
		
	};

}

#endif	// _BIL_CONTEXTMANAGER_HPP_
