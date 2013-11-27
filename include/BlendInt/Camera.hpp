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

#ifndef _BLENDINT_CAMERA_HPP_
#define _BLENDINT_CAMERA_HPP_

#include <BlendInt/Coord.hpp>

namespace BlendInt {

	/**
	 * @brief The Camera class used in Viewport3D
	 */
	class Camera
	{
	public:

		Camera ();

		~Camera ();

		void set_position (const Coord3f& pos)
		{
			m_position = pos;
		}

		void set_position (float x, float y, float z)
		{
			m_position.set_x(x);
			m_position.set_y(y);
			m_position.set_z(z);
		}

		void set_target (const Coord3f& target)
		{
			m_target = target;
		}

		void set_target (float x, float y, float z)
		{
			m_target.set_x(x);
			m_target.set_y(y);
			m_target.set_z(z);
		}

		void set_direction (const Coord3f& direction)
		{
			m_direction = direction;
		}

		void set_direction (float x, float y, float z)
		{
			m_direction.set_x(x);
			m_direction.set_y(y);
			m_direction.set_z(z);
		}

		const Coord3f& position () const {return m_position;}

		const Coord3f& target() const {return m_target;}

		const Coord3f& direction() const {return m_direction;}

	private:

		Coord3f m_position;
		Coord3f m_target;
		Coord3f m_direction;
	};
}

#endif /* _BLENDINT_CAMERA_HPP_ */
