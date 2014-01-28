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

#ifndef _BLENDINT_BUTTON_HPP_
#define _BLENDINT_BUTTON_HPP_

#include <BlendInt/AbstractButton.hpp>
#include <BlendInt/String.hpp>

#include <BlendInt/GLArrayBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief The most common button class
	 *
	 * @ingroup widgets
	 */
	class Button: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(Button);

	public:

		Button ();

		Button (const String& text);

		Button (AbstractWidget* parent);

		Button (const String& text, AbstractWidget* parent);

		// Button (const Icon& icon, const wstring& text, Widget* parent = NULL);

	protected:

		virtual ~Button ();

		virtual void Update (int type, const void* data);

		virtual void Draw ();

	private:

		void InitOnce ();

		void InitOnce (const String& text);

		GLArrayBuffer* m_inner_buffer;
		GLArrayBuffer* m_outer_buffer;
		GLArrayBuffer* m_emboss_buffer;
	};

}

#endif	// _BIL_BUTTON_H_
