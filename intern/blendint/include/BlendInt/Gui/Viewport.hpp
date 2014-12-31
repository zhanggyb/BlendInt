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

#ifndef _BLENDINT_GUI_VIEWPORT_HPP_
#define _BLENDINT_GUI_VIEWPORT_HPP_

#include <BlendInt/Gui/GridFloor.hpp>
#include <BlendInt/Gui/PerspectiveCamera.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/Frame.hpp>

namespace BlendInt {

	class Viewport: public Frame
	{
		DISALLOW_COPY_AND_ASSIGN(Viewport);

	public:

		Viewport ();

		virtual ~Viewport ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize() const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType KeyPressEvent (const Context* context);

		virtual void MouseHoverInEvent (const Context* context);

		virtual void MouseHoverOutEvent (const Context* context);

		virtual ResponseType MousePressEvent (const Context* context);

		virtual ResponseType MouseReleaseEvent (const Context* context);

		virtual ResponseType MouseMoveEvent (const Context* context);

		virtual ResponseType DispatchHoverEvent (const Context* context);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

	private:

		void InitializeViewport ();

		GLuint vao_;

		GLBuffer<> buffer_;

		RefPtr<GridFloor> gridfloor_;

		RefPtr<PerspectiveCamera> default_camera_;

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

	};

}


#endif /* _BLENDINT_GUI_VIEWPORT_HPP_ */
