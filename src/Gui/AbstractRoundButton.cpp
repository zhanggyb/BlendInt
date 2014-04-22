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

#include <BlendInt/Gui/AbstractRoundButton.hpp>

namespace BlendInt {

	AbstractRoundButton::AbstractRoundButton ()
	: AbstractButton()
	{
	}

	AbstractRoundButton::~AbstractRoundButton ()
	{
	}

	void AbstractRoundButton::SetRoundType (int type)
	{
		if(round_type() == type) return;

		UpdateRequest request(Predefined, FormRoundType, &type);

		if(UpdateTest(request)) {
			Update(request);
			set_round_type(type);
			fire_property_changed_event(FormRoundType);
		}
	}

	void AbstractRoundButton::SetRadius (float rad)
	{
		if(radius() == rad) return;

		UpdateRequest request(Predefined, FormRoundRadius, &rad);

		if(UpdateTest(request)) {
			Update(request);
			set_radius(rad);
			fire_property_changed_event(FormRoundRadius);
		}
	}

}
