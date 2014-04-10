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

#ifndef _BLENDINT_GUI_BUTTONGROUP_HPP_
#define _BLENDINT_GUI_BUTTONGROUP_HPP_

#include <boost/smart_ptr.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Object.hpp>

#include <BlendInt/Gui/AbstractButton.hpp>

namespace BlendInt {

	class ButtonGroup: public Object
	{
	public:

		ButtonGroup ();

		~ButtonGroup ();

	private:

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		AbstractButton* m_last_active;

		std::deque<AbstractButton*> m_group;
	};

}

#endif /* _BLENDINT_GUI_BUTTONGROUP_HPP_ */
