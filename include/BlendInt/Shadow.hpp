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

#ifndef _BLENDINT_SHADOW_HPP_
#define _BLENDINT_SHADOW_HPP_

#include <BlendInt/AbstractRoundForm.hpp>
#include <BlendInt/Types.hpp>
#include <BlendInt/GLBufferMultiple.hpp>

namespace BlendInt {

	enum ShadowDirection {
		ShadowNone = 0x0,
		ShadowLeft = (0x1 << 0),
		ShadowRight = (0x1 << 1),
		ShadowTop = (0x1 << 2),
		ShadowBottom = (0x1 << 3),
		ShadowAll = (ShadowLeft | ShadowRight | ShadowTop | ShadowBottom)
	};

	enum ShadowPropertyType {
		ShadowBlurRadius = FormPropertyLast + 1
	};

	/**
	 * @brief Shadow form
	 */
	class Shadow: public AbstractRoundForm
	{
		DISALLOW_COPY_AND_ASSIGN(Shadow);

	public:

		Shadow ();

		virtual ~Shadow ();

		void set_blur_radius (float blur_rad);

		float blur_radius () const {return m_blur_rad;}

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

	private:

		/**
		 * @brief generate shadow vertices
		 * @param[in] size
		 * @param[in] rad
		 * @param[in] step
		 * @param[out] vert
		 * @return
		 */
		int generate_shadow_vertices (const Size* size,
				float rad,
				float step,
				float vert[WIDGET_SIZE_MAX][2]);

		/**
		 * @brief draw shadow without GL Buffer
		 * @param radin
		 */
		void draw (const float radin);

		int m_offset_x;

		int m_offset_y;

		/**
		 * @brief The direction of the projection
		 *
		 * The direction of the projection
		 */
//		int m_direction;

		/**
		 * @brief the blur radius
		 */
		float m_blur_rad;

		GLBufferMultiple m_gl_buffer;
	};

}

#endif /* _BLENDINT_SHADOW_HPP_ */
