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

#ifndef _BLENDINT_GUI_FILEBUTTON_HPP_
#define _BLENDINT_GUI_FILEBUTTON_HPP_

#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/FileSelector.hpp>

#include <BlendInt/Gui/StaticPanel.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief A button used to call FileSelector
	 */
	class FileButton: public AbstractButton
	{

		DISALLOW_COPY_AND_ASSIGN(FileButton);

	public:

		FileButton ();

		virtual ~FileButton ();

		const String& file () const
		{
			return file_;
		}

		Cpp::EventRef<> file_selected() {return file_selected_;}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void InitializeFileButtonOnce ();

		void OnClicked();

		void OnOpened ();

		void OnCanceled ();

		GLuint vao_[2];

		RefPtr<GLArrayBuffer> inner_;
		RefPtr<GLArrayBuffer> outer_;

		StaticPanel* panel_;

		String file_;

		Cpp::Event<> file_selected_;
	};

}

#endif /* _BLENDINT_GUI_FILEBUTTON_HPP_ */
