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
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	AbstractRoundForm::AbstractRoundForm()
	: AbstractForm(),
	  round_type_(RoundNone),
	  radius_(5.f)
	{

	}

	AbstractRoundForm::~AbstractRoundForm()
	{

	}

	void AbstractRoundForm::SetRoundType(int type)
	{
		if(round_type_ == (type & RoundAll)) return;

		PerformRoundTypeUpdate(type);
	}

	void AbstractRoundForm::SetRadius(float rad)
	{
		if(radius_ == rad) return;

		PerformRoundRadiusUpdate(rad);
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

	void AbstractRoundForm::GenerateRoundedVertices(std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		GenerateVertices(size(),
				default_border_width() * Context::theme->pixel(),
				round_type_,
				radius_,
				inner,
				outer);
	}

	void AbstractRoundForm::GenerateRoundedVertices(Orientation shadedir,
			short shadetop, short shadedown, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		GenerateVertices(size(),
				default_border_width() * Context::theme->pixel(),
				round_type_,
				radius_,
				shadedir,
				shadetop,
				shadedown,
				inner,
				outer);
	}


	int AbstractRoundForm::GetOutlineVertexCount (int round_type)
	{
		round_type = round_type & RoundAll;
		int count = 0;

		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return (4 - count) + count * WIDGET_CURVE_RESOLU;
	}

}
