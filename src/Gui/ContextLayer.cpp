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

#include <BlendInt/Gui/ContextLayer.hpp>

namespace BlendInt {

	WidgetSet::WidgetSet ()
	: Object()
	{
	}

	WidgetSet::~WidgetSet ()
	{
#ifdef DEBUG

		if(m_widgets.size()) {
			DBG_PRINT_MSG("Warning: %s", "the widget set in context layer is not cleared");
		}

#endif
	}

	WidgetDeque::WidgetDeque ()
	: Object()
	{
	}

	WidgetDeque::~WidgetDeque ()
	{
#ifdef DEBUG

		if(m_widgets.size()) {
			DBG_PRINT_MSG("Warning: %s", "the widget deque in context layer is not cleared");
		}

#endif
	}

	ContextLayerExt::ContextLayerExt ()
	: Object(),
	  m_refresh(true),
	  m_rescan_tail(false)
	{

	}

	ContextLayerExt::~ContextLayerExt ()
	{

	}

	ContextLayerExt& ContextLayerExt::operator = (const ContextLayerExt& orig)
	{
		m_refresh = orig.m_refresh;
		m_widget_set = orig.m_widget_set;
		m_texture_buffer = orig.m_texture_buffer;
		m_rescan_tail = orig.m_rescan_tail;
		m_hover_list = orig.m_hover_list;

		return *this;
	}

}

