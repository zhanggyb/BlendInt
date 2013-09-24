/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_CHECKBUTTON_HPP_
#define _BILO_CHECKBUTTON_HPP_

#include <BILO/AbstractButton.hpp>
#include <BILO/String.hpp>

namespace BILO {

	class ToggleButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(ToggleButton);

	public:

		ToggleButton (const String& text);

		ToggleButton (Drawable* parent);

		ToggleButton (const String& text, Drawable* parent);

		virtual ~ToggleButton ();

	protected:

		virtual void render ();

	};

}

#endif /* _BIL_CHECKBUTTON_HPP_ */
