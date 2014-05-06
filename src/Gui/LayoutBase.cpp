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

#include <BlendInt/Gui/LayoutBase.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	LayoutBase::LayoutBase (AbstractContainer* container)
	: m_container(container)
	{
#ifdef DEBUG
		if(!container) {
			DBG_PRINT_MSG("Critical Error: %s", "no container assigned");
		}
#endif
	}

	LayoutBase::~LayoutBase ()
	{
	}

	void LayoutBase::SetPosition (AbstractWidget* widget, int x,
					int y)
	{
		m_container->SetSubWidgetPosition(widget, x, y);
	}

	void LayoutBase::SetPosition (AbstractWidget* widget,
					const Point& pos)
	{
		m_container->SetSubWidgetPosition(widget, pos);
	}

	void LayoutBase::Resize (AbstractWidget* widget,
					unsigned int width, unsigned int height)
	{
		m_container->ResizeSubWidget(widget, width, height);
	}

	void LayoutBase::Resize (AbstractWidget* widget, const Size& size)
	{
		m_container->ResizeSubWidget(widget, size);
	}

}
