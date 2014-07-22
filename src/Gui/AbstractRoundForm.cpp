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

#include <BlendInt/Gui/AbstractRoundForm.hpp>

namespace BlendInt {

	AbstractRoundForm::AbstractRoundForm()
	: AbstractForm(),
	  m_round_type(RoundNone),
	  m_radius(5)
	{

	}

	AbstractRoundForm::~AbstractRoundForm()
	{

	}

	void AbstractRoundForm::SetRoundType(int type)
	{
		if(m_round_type == type) return;

		UpdateGeometry (UpdateRequest(FormRoundType, &type));
		set_round_type(type);
	}

	void AbstractRoundForm::SetRadius(float rad)
	{
		if(m_radius == rad) return;

		UpdateGeometry(UpdateRequest(FormRoundRadius, &rad));
		set_radius(rad);
	}

	int AbstractRoundForm::GetOutlineVertices (int round_type)
	{
		round_type = round_type & RoundAll;
		int count = 0;

		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return 4 - count + count * WIDGET_CURVE_RESOLU;
	}

}

