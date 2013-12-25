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

#ifndef _BLENDINT_MENUBIN_HPP_
#define _BLENDINT_MENUBIN_HPP_

#include <boost/smart_ptr.hpp>

#include <BlendInt/RoundWidget.hpp>
#include <BlendInt/Menu.hpp>
#include <BlendInt/String.hpp>
#include <BlendInt/GLBuffer.hpp>

namespace BlendInt {

	class MenuBin: public RoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(MenuBin);

	public:

		MenuBin ();

		MenuBin (AbstractWidget* parent);

		virtual ~MenuBin();

		void SetTitle (const String& title);

		void AddMenuItem (const String& text);

		static int DefaultMenuItemHeight;

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void MouseMoveEvent(MouseEvent* event);

	private:

		boost::scoped_ptr<Menu> m_menu;
		boost::scoped_ptr<GLBuffer> m_buffer;
	};

}

#endif /* _BLENDINT_MENUBIN_HPP_ */
