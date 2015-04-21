/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_FOLDERLIST_HPP_
#define _BLENDINT_GUI_FOLDERLIST_HPP_

#include <blendint/gui/abstract-round-widget.hpp>
#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/text.hpp>

namespace BlendInt {

	/**
	 * @brief A simple widget to display and edit folders in FileSelector
	 */
	class FolderList: public AbstractRoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(FolderList);

	public:

		FolderList();

		virtual ~FolderList ();

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual Response Draw (AbstractWindow* context);

	private:

		void InitializeFolderListOnce ();

		GLuint vao_[2];
		GLBuffer<ARRAY_BUFFER, 2> vbo_;

		RefPtr<Text> text_;

	};
}


#endif /* _BLENDINT_GUI_FOLDERLIST_HPP_ */
