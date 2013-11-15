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

#ifndef _BLENDINT_ROUNDBOXBASE_HPP_
#define _BLENDINT_ROUNDBOXBASE_HPP_

#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/Types.hpp>
#include <BlendInt/Color.hpp>
#include <BlendInt/Theme.hpp>

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

	/**
	 * Structure used in calulating vertex buffer for inner and outline
	 *
	 * @note don't use nested class for SWIG later
	 */
	struct VerticesSum {
		VerticesSum ()
		: total(0), half(0)
		{ }

		int total;	/**< total number of vertices for widget */
		int half;	/**< halfway vertices number */
	};

	class RoundBoxBase
	{
	public:

		RoundBoxBase();

		~RoundBoxBase();

		void set_round_type (int type)
		{
			m_round_type = type;
		}

		int round_type () const {return m_round_type;}

		/**
		 * @brief set the round radius for the corner
		 * @param radius
		 */
		void set_radius (float radius)
		{
			m_radius = radius;
		}

		float radius () const {return m_radius;}

	protected:

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

		static const float jit[WIDGET_AA_JITTER][2];

		VerticesSum generate_vertices (const Size* size,
				float border,
				int round_type,
				float radius,
				float inner_v[WIDGET_SIZE_MAX][2],
				float outer_v[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief calculate vertices for round box edge with no shaded color
		 * @param[in] size the size to calculate edges
		 * @param[in] border border width
		 * @param[out] inner_v
		 * @param[out] outer_v
		 * @return how many vertices are used in the output array
		 */
		VerticesSum generate_vertices (const Size* size, float border, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief calculate vertices for round box edges
		 * @param size
		 * @param border
		 * @param theme
		 * @param shadedir shade direction
		 * @param inner
		 * @param outer
		 * @return
		 */
		VerticesSum generate_vertices (const Size* size,
				float border,
				const WidgetTheme* theme,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief generate vertices array for round box inner and edges
		 * @param[in] size the size to calculate position and shade uv
		 * @param[in] border
		 * @param[in] shadetop the top shade, defined in theme
		 * @param[in] shadedown the bottom shade, defined in theme
		 * @param[in] shadedir true if shade with horizontal direction
		 * @param[out] inner inner vertices with position and color information
		 * @param[out] outer vertices for outline
		 * @return
		 */
		VerticesSum generate_vertices (const Size* size,
				float border,
				const Color& color,
				short shadetop,
				short shadedown,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2]);

	private:

		int m_round_type;

		float m_radius;
	};

}

#endif /* _BLENDINT_ROUNDBOXBASE_HPP_ */
