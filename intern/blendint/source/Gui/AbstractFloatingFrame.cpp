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

#include <BlendInt/Gui/AbstractFloatingFrame.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	AbstractFloatingFrame::AbstractFloatingFrame()
	: AbstractFrame(),
	  round_type_(0),
	  round_radius_(5.f)
	{
	}

	AbstractFloatingFrame::~AbstractFloatingFrame()
	{
	}

	AbstractView* AbstractFloatingFrame::GetFocusedView () const
	{
		return nullptr;
	}

	void AbstractFloatingFrame::SetRoundType (int type)
	{
		if(round_type_ == (type & 0x0F)) return;

		PerformRoundTypeUpdate(type);
	}

	void AbstractFloatingFrame::SetRoundRadius(float radius)
	{
		if(round_radius_ == radius) return;

		PerformRoundRadiusUpdate(radius);
	}

	void AbstractFloatingFrame::GenerateRoundedVertices(
			std::vector<GLfloat>* inner, std::vector<GLfloat>* outer)
	{
		GenerateVertices(size(),
				default_border_width() * Context::theme->pixel(),
				round_type(),
				round_radius_,
				inner,
				outer);
	}

	void AbstractFloatingFrame::GenerateRoundedVertices(
			Orientation shadedir, short shadetop, short shadedown,
			std::vector<GLfloat>* inner, std::vector<GLfloat>* outer)
	{
		GenerateVertices(size(),
				default_border_width() * Context::theme->pixel(),
				round_type(),
				round_radius_,
				shadedir,
				shadetop,
				shadedown,
				inner,
				outer);
	}

	void AbstractFloatingFrame::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);
	}

	void AbstractFloatingFrame::PerformRoundRadiusUpdate(float radius)
	{
		round_radius_ = radius;
	}

}
