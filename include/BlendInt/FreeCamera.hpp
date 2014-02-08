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

#ifndef _BLENDINT_FREECAMERA_HPP_
#define _BLENDINT_FREECAMERA_HPP_

#include <glm/vec3.hpp>
#include <BlendInt/AbstractCamera.hpp>

namespace BlendInt {

	class FreeCamera: public AbstractCamera
	{
	public:

		FreeCamera ();

		virtual ~FreeCamera ();

		void Orbit (float x, float y);

		void Pan (float x, float y);

		void Zoom (float fac);

		virtual void Update ();

	private:

		glm::vec3 m_translation;
	};

}

#endif /* _BLENDINT_FREECAMERA_HPP_ */
