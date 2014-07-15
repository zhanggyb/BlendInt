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

#ifndef _BLENDINT_GUI_COLORWHEEL_HPP_
#define _BLENDINT_GUI_COLORWHEEL_HPP_

#include <vector>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/CircularPicker.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	class ColorWheel: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(ColorWheel);

	public:

		ColorWheel ();

		virtual ~ColorWheel ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool UpdateGeometryTest (const GeometryUpdateRequest& request);

		virtual void UpdateGeometry (const GeometryUpdateRequest& request);

		virtual void BroadcastUpdate (const GeometryUpdateRequest& request);

		virtual void ProcessSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void GenerateWheelVertices (int radius,
						std::vector<GLfloat>& inner_vertices,
						std::vector<GLfloat>& outer_vertices);

	private:

		void InitializeColorWheel ();

		GLuint m_vao[2];

		RefPtr<GLArrayBuffer> m_outline;
		RefPtr<GLArrayBuffer> m_inner;

		CircularPicker m_picker;

	};

}

#endif /* _BLENDINT_GUI_COLORWHEEL_HPP_ */
