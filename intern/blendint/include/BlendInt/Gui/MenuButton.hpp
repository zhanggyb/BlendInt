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

#pragma once

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

#include <BlendInt/Gui/Menu.hpp>

namespace BlendInt {

	/**
	 * @brief A special button used in MenuBar only
	 */
	class MenuButton: public AbstractButton
	{
	public:

		MenuButton (const String& text);

		virtual ~MenuButton ();

		void SetMenu (const RefPtr<Menu>& menu);

		RefPtr<Menu> menu () const {return m_menu;}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual ResponseType Draw (const AbstractWindow* context);

	private:

		void InitializeMenuButton ();

		GLuint vao_;

		GLBuffer<> vbo_;

		RefPtr<Menu> m_menu;
	};

} /* namespace BlendInt */
