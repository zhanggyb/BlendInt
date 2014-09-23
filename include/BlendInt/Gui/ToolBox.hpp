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

#ifndef _BLENDINT_GUI_TOOLBOX_HPP_
#define _BLENDINT_GUI_TOOLBOX_HPP_

#include <BlendInt/Gui/Container.hpp>

namespace BlendInt {

	class ToolBox: public Container
	{
		DISALLOW_COPY_AND_ASSIGN(ToolBox);

	public:

		ToolBox();

		virtual ~ToolBox();

		void Append (AbstractWidget* widget);

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual void PerformMarginUpdate (const Margin& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void FillSubWidgets (const Size& out_size, const Margin& margin, int space);

		void FillSubWidgets (int x, int y, int width, int height, int space);

		int GetLastPosition () const;

		GLuint vao_;

		int space_;

		RefPtr<GLArrayBuffer> inner_;

	};

}

#endif /* _BLENDINT_GUI_TOOLBOX_HPP_ */
