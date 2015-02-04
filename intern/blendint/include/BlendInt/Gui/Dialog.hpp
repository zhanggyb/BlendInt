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

#pragma once

#include <BlendInt/Core/String.hpp>

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/AbstractDialog.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>
#include <BlendInt/Gui/LinearLayout.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

namespace BlendInt {

	class Dialog: public AbstractDialog
	{

	public:

		Dialog (const String& title, int flags = 0);

		virtual ~Dialog();

		bool AddWidget (AbstractWidget* widget);

		bool InsertWidget (int index, AbstractWidget* widget);

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual ResponseType Draw (AbstractWindow* context);

	private:

		void OnCloseButtonClicked (AbstractButton* sender);

		void OnApplyButtonClicked (AbstractButton* sender);

		void OnOKButtonClicked (AbstractButton* sender);

		// 0: inner
		// 1: outer
		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> vbo_;

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

		RefPtr<FrameShadow> shadow_;

		LinearLayout* content_layout_;

		LinearLayout* main_layout_;
	};

}
