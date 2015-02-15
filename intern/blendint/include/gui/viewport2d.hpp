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

#ifndef _BLENDINT_GUI_VIEWPORT2D_HPP_
#define _BLENDINT_GUI_VIEWPORT2D_HPP_

#include <gui/abstract-round-widget.hpp>
#include <gui/ortho-camera.hpp>

#include <gui/grid-floor.hpp>

namespace BlendInt {

	/**
	 * @brief A special viewport with a OrthoCamera
	 */
	class Viewport2D: public AbstractRoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Viewport2D);

	public:

		Viewport2D ();

		virtual ~Viewport2D ();

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual Response Draw (AbstractWindow* context);

	private:

		void InitializeViewport2D ();

		GLuint vao_;

		RefPtr<GLArrayBuffer> inner_;

		OrthoCamera camera_;

		GridFloor* gridfloor_;

		static const float camera_factor;
	};

}

#endif /* _BLENDINT_GUI_VIEWPORT2D_HPP_ */
