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

#ifndef _BLENDINT_VIEWPORT3D_HPP_
#define _BLENDINT_VIEWPORT3D_HPP_

#include <vector>

#include <BlendInt/Widget.hpp>

#include <BlendInt/Grid.hpp>
#include <BlendInt/FreeCamera.hpp>

using std::vector;

namespace BlendInt {

	/**
	 * @brief A simple 3D viewport
	 *
	 * @ingroup widgets
	 */
	class Viewport3D: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(Viewport3D);

	public:

		Viewport3D();

		Viewport3D(AbstractWidget* parent);

		virtual ~Viewport3D ();

		void Zoom (float factor);

	protected:

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void Render ();

	private:

		virtual void Draw ();

		void DrawCube ();

		void DrawGrid (float width, float height, float small_step, float big_step);

		void InitOnce ();

		vector<AbstractCamera*> m_cameras;

		FreeCamera* m_default_camera;

		Grid* m_grid;

		int m_xold;
		int m_yold;

		float m_rotate_x;
		float m_rotate_y;

		bool m_left_down;

	};

}

#endif /* _BLENDINT_VIEWPORT3D_HPP_ */
