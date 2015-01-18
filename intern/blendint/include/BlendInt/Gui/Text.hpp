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

#include <BlendInt/Gui/AbstractForm.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	class Text: public AbstractForm
	{
	public:

		Text (const String& text);

		virtual ~Text ();

	protected:

		virtual void PerformSizeUpdate (const Size& size);

		virtual void Draw (float x, float y, short gamma = 0) const;

	private:

		unsigned int str_len_;

		GLuint vao_;

		GLBuffer<> vbo_;

		RefPtr<GLTexture2D> texture_;

		String text_;

	};

}