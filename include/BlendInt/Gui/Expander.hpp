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

#ifndef _BLENDINT_GUI_EXPANDER_HPP_
#define _BLENDINT_GUI_EXPANDER_HPP_

#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/AbstractVectorContainer.hpp>
#include <BlendInt/Gui/AbstractSingleContainer.hpp>

namespace BlendInt {

	class ToggleButton;

	class ExpandButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(ExpandButton);

	public:

		ExpandButton ();

		ExpandButton (const String& text);

		virtual ~ExpandButton ();

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

	private:

		void InitializeExpandButton ();

		void InitializeExpandButton (const String& text);
	};


	// --------------------------

	/**
	 * @brief Expander
	 */
	class Expander: public AbstractVectorContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Expander);

	public:

		Expander ();

		Expander (const String& title);

		virtual ~Expander ();

		bool Setup (AbstractWidget* widget);

		void SetTitle (const String& text);

		const String& GetTitle () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformMarginUpdate (const Margin& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	protected:

		void InitializeExpander ();

		void FillInExpander (const Point& out_pos, const Size& out_size, const Margin& margin);

		void FillInExpander (int x, int y, int width, int height);

		void OnToggled (bool toggle);

		GLuint m_vao;

		int m_frame_height;

		RefPtr<GLArrayBuffer> m_inner;
	};

}

#endif /* _BLENDINT_GUI_EXPANDER_HPP_ */
