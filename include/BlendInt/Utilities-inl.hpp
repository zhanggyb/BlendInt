/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_UTILITIES_HPP_
#define _BLENDINT_UTILITIES_HPP_

namespace BlendInt {

	/**
	 * @brief Correct a numeric value in a scope
	 * @param[in] value the value to be corrected
	 * @param[in] min minimum of the scope
	 * @param[in] max maximum of the scope
	 * @return min if value < min, max if value > max,
	 * otherwize return value
	 */
	template <typename T>
	inline T correct_in_scope (T value, T min, T max)
	{
		return ((value) < (min) ?
				(min) : ((value) > (max) ?
						 (max) : (value)));
	}

	inline unsigned char float_to_uchar (float val)
	{
		return (val <= 0.0f) ?
			0 : ((val > (1.0f - 0.5f / 255.0f)) ?
				 255 : static_cast<unsigned char>(255.0f * val + 0.5f));
	}

	/**
	 * @brief Copy elements in arrary from src to dist
	 */
	template <typename T>
	inline void copy_arrary_v2 (const T src[2], T dist[2])
	{
		dist[0] = src[0];
		dist[1] = src[1];
	}

	inline void copy_v2_v2(float r[2], const float a[2])
	{
		r[0] = a[0];
		r[1] = a[1];
	}
}

#endif	// _BIL_UTILITIES_H_
