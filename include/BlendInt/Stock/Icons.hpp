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
#include <BlendInt/Gui/PixelIcon.hpp>

namespace BlendInt {

	class Interface;

	namespace Stock {

		/**
		 * @brief class for Stock Icons
		 *
		 * MUST initialized in Interface::Initialize()
		 */
		class Icons
		{
		DISALLOW_COPY_AND_ASSIGN(Icons);

		public:

			static Icons* instance;

			RefPtr<VertexIcon> icon_menu () const
			{
				return m_icon_menu;
			}

			RefPtr<VertexIcon> icon_circle () const
			{
				return m_icon_circle;
			}

			RefPtr<VertexIcon> icon_check () const
			{
				return m_icon_check;
			}

			RefPtr<VertexIcon> icon_num () const
			{
				return m_icon_num;
			}

		private:

			friend class BlendInt::Interface;

			static bool Initialize ();

			static void Release ();

			/**
			 * @brief Default constructor
			 */
			Icons ();

			/**
			 * @brief private destructor
			 */
			~Icons ();

			/**
			 * @brief Create default icons
			 *
			 * Call in constructor
			 */
			void CreateIcons ();

			RefPtr<VertexIcon> m_icon_menu;

			RefPtr<VertexIcon> m_icon_circle;

			RefPtr<VertexIcon> m_icon_check;

			RefPtr<VertexIcon> m_icon_num;

			RefPtr<PixelIcon> m_icon_file_16x16;
		};

	}

}

#endif /* _BLENDINT_STOCKICONS_HPP_ */
