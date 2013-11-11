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

#ifndef _BLENDINT_ROUNDBOX_HPP_
#define _BLENDINT_ROUNDBOX_HPP_

#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	/**
	 * flags to set which corners will become rounded:
	 *
	 * 1------2
	 * |      		|
	 * 8------4
	 */
	enum RoundType {
		RoundNone = 0,
		RoundTopLeft = (1 << 0),
		RoundTopRight = (1 << 1),
		RoundBottomRight = (1 << 2),
		RoundBottomLeft = (1 << 3),
		/* just for convenience */
		RoundAll = (RoundTopLeft | RoundTopRight
				| RoundBottomRight | RoundBottomLeft),
		UI_RB_ALPHA = RoundAll + 1
	};

	enum RoundBoxPropertyType {
		FormRoundType = AbstrctFormPropertyLast + 1,
		FormRoundRadius,
		AbstractRoundBoxPropertyLast = FormRoundRadius
	};

	class AbstractRoundBox: public AbstractForm
	{
	public:

		AbstractRoundBox();

		virtual ~AbstractRoundBox();

		bool set_round_type (int type);

		int round_type () const {return m_round_type;}

		/**
		 * @brief set the round radius for the corner
		 * @param radius
		 */
		bool set_radius (float radius);

		float radius () const {return m_radius;}

	protected:

		virtual void update (int type, const void* data) = 0;

		virtual void render () = 0;

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

	private:

		int m_round_type;

		float m_radius;
	};

}

#endif /* _BLENDINT_ROUNDBOX_HPP_ */
