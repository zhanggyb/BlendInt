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

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt
{

	ContextLayerExt::ContextLayerExt ()
			: refresh(true), widgets(0), buffer(0)
	{
	}

	ContextLayerExt::~ContextLayerExt ()
	{
		if (buffer) {
			DBG_PRINT_MSG("%s", "Delete texture buffer in context layer");
			delete buffer;
		}

		if (widgets) {
			DBG_PRINT_MSG("%s", "Delete widget set in context layer");
			assert(widgets->size() == 0);
			widgets->clear();
			delete widgets;
		}
	}

	Context::Context ()
	{
	}

	Context::~Context ()
	{
	}

	bool Context::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {

			return true;
		} else {

			return false;
		}
	}

	ResponseType Context::Draw (const RedrawEvent& event)
	{
		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Context::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Context::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	bool Context::AddSubWidget (AbstractWidgetExt* widget)
	{
		return true;
	}

	bool Context::RemoveSubWidget (AbstractWidgetExt* widget)
	{
		return true;
	}

	AbstractWidgetIterator* Context::First (const DeviceEvent& event)
	{
		// A Context object only interacts with Interface
		return 0;
	}

	bool Context::End (const DeviceEvent& event, AbstractWidgetIterator* iter)
	{
		// A Context object only interacts with Interface
		return false;
	}

}
