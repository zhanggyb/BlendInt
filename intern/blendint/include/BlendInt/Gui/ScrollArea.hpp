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

#ifndef _BLENDINT_GUI_SCROLLAREA_HPP_
#define _BLENDINT_GUI_SCROLLAREA_HPP_

#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/ScrollBar.hpp>

#include <BlendInt/Gui/TableLayout.hpp>

namespace BlendInt {

	class ScrollArea: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollArea);

	public:

		ScrollArea();

		virtual ~ScrollArea ();

		void SetViewport (AbstractScrollable* scrollable);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const Context* context);

	private:

		enum SubWidgetIndex {
			HScrollBarIndex,
			VScrollBarIndex,
			ScrollViewIndex,
			CornerWidgetIndex
		};

		void InitializeScrollArea ();

		GLuint vao_;

		GLBuffer<> vbo_;

		TableLayout* layout_;
	};
}

#endif /* _BLENDINT_GUI_SCROLLAREA_HPP_ */
