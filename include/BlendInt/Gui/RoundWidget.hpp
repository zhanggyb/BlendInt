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

#ifndef _BLENDINT_ROUNDWIDGET_HPP_
#define _BLENDINT_ROUNDWIDGET_HPP_

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RoundShapeBase.hpp>

namespace BlendInt {

	/**
	 * @brief Widget with round corner
	 */
	class RoundWidget: public Widget, public RoundShapeBase
	{
		DISALLOW_COPY_AND_ASSIGN(RoundWidget);

	public:

		RoundWidget ();

		RoundWidget (AbstractWidget* parent);

		virtual ~RoundWidget();

		/**
		 * @brief set round type
		 * @param type
		 *
		 * Hide the same function in RoundBoxBase to call Update
		 */
		void SetRoundType (int type);

		/**
		 * @brief set round radius
		 * @param radius
		 * Hide the same function in RoundBoxBase to call Update
		 */
		void SetRadius (float radius);

	protected:

		virtual void Draw ();

		/**
		 * @brief calculate vertices for round box edge with no shaded color
		 * @param[in] size the size to calculate edges
		 * @param[in] border border width
		 * @param[out] inner_v
		 * @param[out] outer_v
		 * @return how many vertices are used in the output array
		 */
		inline VerticesSum generate_vertices (const Size* size, float border, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2])
		{
			return generate_round_vertices(size, border, round_type(), radius(), inner_v, outer_v);
		}

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
		inline VerticesSum generate_vertices (const Size* size,
				float border,
				const WidgetTheme* theme,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2])
		{
			return generate_round_vertices (size,
					border,
					round_type(),
					radius(),
					theme,
					shadedir,
					inner,
					outer);
		}

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
		inline VerticesSum generate_vertices (const Size* size,
				float border,
				const Color& color,
				short shadetop,
				short shadedown,
				Orientation shadedir,
				float inner[WIDGET_SIZE_MAX][6],
				float outer[WIDGET_SIZE_MAX][2])
		{
			return generate_round_vertices (size,
					border,
					round_type(),
					radius(),
					color,
					shadetop,
					shadedown,
					shadedir,
					inner,
					outer);
		}

	};
}


#endif /* _BLENDINT_ROUNDWIDGET_HPP_ */
