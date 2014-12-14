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

#ifndef _BLENDINT_GUI_ORTHOCAMERA_HPP_
#define _BLENDINT_GUI_ORTHOCAMERA_HPP_

#include <BlendInt/Gui/AbstractCamera.hpp>

namespace BlendInt {

	class OrthoCamera: public AbstractCamera
	{

	public:

		OrthoCamera();

		virtual ~OrthoCamera ();

		void SetOrtho (float left, float right, float bottom, float top, float near = 100.f, float far = -100.f);

		float near () const	{return near_;}

		float far () const {return far_;}

	protected:

		void set_near (float near)
		{
			near_ = near;
		}

		void set_far (float far)
		{
			far_ = far;
		}

	private:

		float left_;

		float right_;

		float bottom_;

		float top_;

		float near_;

		float far_;

	};

}

#endif /* _BLENDINT_GUI_ORTHOCAMERA_HPP_ */
