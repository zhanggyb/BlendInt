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

#include <BlendInt/Gui/AbstractResizableForm.hpp>

namespace BlendInt {

	void AbstractResizableForm::Resize (unsigned int width, unsigned int height)
	{
		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);

		if(Update(UpdateRequest(Predefined, FormSize, &new_size))) {
			set_size(width, height);
		}
	}

	void AbstractResizableForm::Resize (const Size& size)
	{
		if(AbstractResizableForm::size() == size) return;

		if(Update(UpdateRequest(Predefined, FormSize, &size))) {
			set_size(size);
		}
	}

}
