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

#ifndef _BLENDINT_STOCKICON_HPP_
#define _BLENDINT_STOCKICON_HPP_

#include <BlendInt/VertexIcon.hpp>

namespace BlendInt {

	/**
	 * @brief class for Stock Icons
	 *
	 * MUST initialized in Interface::Initialize()
	 */
	class StockIcon
	{
	public:

		static bool Initialize ();

		static void Release ();

		static StockIcon* Instance ();

		VertexIcon* menu () const {return m_menu;}

		VertexIcon* circle () const {return m_circle;}

		VertexIcon* checkbox() const {return m_checkbox;}

		VertexIcon* num () const {return m_num;}

	private:

		/**
		 * @brief Default constructor
		 */
		StockIcon ();

		/**
		 * @brief private destructor
		 */
		~StockIcon ();

		static StockIcon* stock_icon;

		VertexIcon* m_menu;

		VertexIcon* m_circle;

		VertexIcon* m_checkbox;

		VertexIcon* m_num;

		/**
		 * @brief Create default icons
		 *
		 * Call in constructor
		 */
		void CreateIcons ();

		/**
		 * @brief Copy constructor, disabled
		 */
		StockIcon (const StockIcon& orig);

		/**
		 * @brief Assignment operation, disabled
		 */
		StockIcon& operator = (const StockIcon& orig);

	};

}

#endif /* _BLENDINT_STOCKICON_HPP_ */
