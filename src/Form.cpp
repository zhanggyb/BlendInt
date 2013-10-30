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

#include <BlendInt/Form.hpp>

namespace BlendInt {

	Form::Form()
	: AbstractForm()
	{

	}

	Form::Form(AbstractForm* parent)
	: AbstractForm(parent)
	{

	}

	Form::~Form()
	{

	}

	void Form::press_key (KeyEvent* event)
	{
	}

	void Form::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Form::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Form::press_mouse (MouseEvent* event)
	{
	}

	void Form::release_mouse (MouseEvent* event)
	{
	}

	void Form::move_mouse (MouseEvent* event)
	{
	}

	void Form::update (int property_type)
	{

	}

	void Form::render()
	{

	}

}
