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

#ifndef _BLENDINT_GUI_VIEWPORTSPLITTER_HPP_
#define _BLENDINT_GUI_VIEWPORTSPLITTER_HPP_

#include <BlendInt/Gui/AbstractViewport.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	class Widget;
	class ViewportSplitter;

	class ViewportSplitterHandle: public AbstractViewport
	{
	public:

		ViewportSplitterHandle (Orientation orientation = Horizontal);

		virtual ~ViewportSplitterHandle ();

		void SetHandleWidget (Widget* widget);

		virtual Size GetPreferredSize () const;

	protected:

		friend class ViewportSplitter;

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		Orientation orientation_;

		GLuint vao_;

		GLBuffer<ARRAY_BUFFER> buffer_;

		AbstractViewport* previous_viewport_;

		AbstractViewport* next_viewport_;
	};

	// -------------------------------

	class ViewportSplitter: public AbstractViewport
	{
	public:

		ViewportSplitter (Orientation orientation = Horizontal);

		virtual ~ViewportSplitter ();



	protected:

		virtual ResponseType Draw (Profile& profile);

	private:

		friend class ViewportSplitterHandle;

		Orientation orientation_;
	};
}

#endif /* _BLENDINT_GUI_VIEWPORTSPLITTER_HPP_ */
