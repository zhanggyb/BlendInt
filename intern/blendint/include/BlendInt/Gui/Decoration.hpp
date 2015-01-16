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

#ifndef _BLENDINT_GUI_DECORATION_HPP_
#define _BLENDINT_GUI_DECORATION_HPP_

#include <Cpp/Events.hpp>

#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/AbstractDecoration.hpp>

#include <BlendInt/Gui/CloseButton.hpp>
#include <BlendInt/Gui/Label.hpp>

namespace BlendInt {

	/**
	 * @brief A special widget used as a title/decoration of a dialog
	 */
	class Decoration:  public AbstractDecoration
	{
		DISALLOW_COPY_AND_ASSIGN(Decoration);

	public:

		Decoration ();

		virtual ~Decoration ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual ResponseType Draw (const Context* context);

	private:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		void OnCloseButtonClicked (AbstractButton* button);

		/**
		 * space between tool buttons
		 */
		int space_;

		CloseButton* close_button_;

	};

}


#endif /* _BLENDINT_GUI_DECORATION_HPP_ */
