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

#ifndef _BLENDINT_STOCKICONS_HPP_
#define _BLENDINT_STOCKICONS_HPP_

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>

namespace BlendInt {

	class Interface;

	/**
	 * @brief class for Stock Icons
	 *
	 * MUST initialized in Interface::Initialize()
	 */
	class StockIcons
	{
		DISALLOW_COPY_AND_ASSIGN(StockIcons);

	public:

		static StockIcons* instance;

		RefPtr<VertexIcon> menu () const {return m_menu;}

		RefPtr<VertexIcon> circle () const {return m_circle;}

		RefPtr<VertexIcon> checkbox() const {return m_checkbox;}

		RefPtr<VertexIcon> num () const {return m_num;}

	private:

		friend class Interface;

		static bool Initialize ();

		static void Release ();

		/**
		 * @brief Default constructor
		 */
		StockIcons ();

		/**
		 * @brief private destructor
		 */
		~StockIcons ();

		/**
		 * @brief Create default icons
		 *
		 * Call in constructor
		 */
		void CreateIcons ();

		RefPtr<VertexIcon> m_menu;

		RefPtr<VertexIcon> m_circle;

		RefPtr<VertexIcon> m_checkbox;

		RefPtr<VertexIcon> m_num;
	};

}

#endif /* _BLENDINT_STOCKICONS_HPP_ */
