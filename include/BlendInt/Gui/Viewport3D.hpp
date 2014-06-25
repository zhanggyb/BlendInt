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

#include <BlendInt/Gui/AbstractWidget.hpp>

#include <BlendInt/Gui/Grid.hpp>
#include <BlendInt/Gui/Cube.hpp>
#include <BlendInt/Gui/Mesh.hpp>

#include <BlendInt/Gui/NavigationCamera.hpp>

using std::vector;

namespace BlendInt {

	/**
	 * @brief A simple 3D viewport
	 *
	 * @ingroup widgets
	 */
	class Viewport3D: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Viewport3D);

	public:

		Viewport3D();

		virtual ~Viewport3D ();

		void Zoom (float factor);

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual bool UpdateGeometryTest (const GeometryUpdateRequest& request);

		virtual void UpdateGeometry (const GeometryUpdateRequest& request);

		virtual void BroadcastUpdate (const GeometryUpdateRequest& request);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual void Render ();

	private:

		virtual ResponseType Draw (const RedrawEvent& event);

		void InitOnce ();

		vector<RefPtr<AbstractCamera> > m_cameras;

		RefPtr<NavigationCamera> m_default_camera;

		RefPtr<Cube> m_cube;
		RefPtr<Grid> m_grid;

		int m_last_x;
		int m_last_y;

		float m_rX;

		float m_rY;

		MouseButton m_button_down;
	};

}

#endif /* _BLENDINT_VIEWPORT3D_HPP_ */
