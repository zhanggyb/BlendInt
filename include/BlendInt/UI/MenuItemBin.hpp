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

#ifndef _BLENDINT_MENU_HPP_
#define _BLENDINT_MENU_HPP_

#include <list>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/UI/MenuItem.hpp>

namespace BlendInt {

	/**
	 * @brief A class to store menu items
	 */
	class MenuItemBin
	{
	public:

		friend class MenuItem;

		/**
		 * @brief Default constructor
		 */
		MenuItemBin ();

		/**
		 * @brief Constructor with title
		 * @param[in] title The title of this menu
		 */
		MenuItemBin (const String& title);

		/**
		 * @brief Destructor
		 */
		~MenuItemBin();

		MenuItem* GetMenuItem (size_t index);

		/**
		 * @brief Set the title of this menu
		 * @param[in] title The title of this menu
		 *
		 * This inline function just simply set the title of the menu.
		 */
		inline void set_title (const String& title)
		{
			m_title = title;
		}

		/**
		 * @brief Get the title of this menu
		 * @return The title string
		 */
		inline const String& title () const {return m_title;}

		/**
		 * @brief Get the size of menu items
		 * @return The size of menu items
		 */
		inline size_t size () const {return m_list.size();}

		/**
		 * @brief Get the reference of menu item list
		 * @return Reference of the std::list member
		 */
		inline std::list<MenuItem*>& list () {return m_list;}

		/**
		 * @brief Add a menu item and set the text
		 * @param[in] text The text string shown in the menu item
		 */
		void Add (const String& text);

		/**
		 *
		 */
		void Add (Icon* icon, const String& text);

		void Add (Icon* icon, const String& text, const String& shortcut);

		void Add (MenuItem* item);

		/**
		 * @brief Set the parent MenuItem of this menu
		 * @param[in] item The parent MenuItem
		 */
		void SetParent (MenuItem* item);

		/**
		 * @brief Remove a MenuItem from this menu
		 * @param[in] item The MenuItem object need to be removed
		 *
		 * @note This function will not delete the item removed, do to so, use MenuBin::Delete instead
		 * or delete it manually.
		 *
		 * @sa Delete
		 */
		void Remove (MenuItem* item);

		/**
		 * @brief Remove and delete a MenuItem from this menu
		 * @param[in] item The MenuItem object need to be deleted
		 *
		 * @sa Remove
		 */
		void Delete (MenuItem* item);

#ifdef DEBUG
		void print_menu_items ();
#endif

	private:

		String m_title;

		MenuItem* m_parent;

		std::list<MenuItem*> m_list;
	};

}

#endif /* _BLENDINT_MENU_HPP_ */
