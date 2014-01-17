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

#ifndef _BLENDINT_CHECKBUTTON_HPP_
#define _BLENDINT_CHECKBUTTON_HPP_

#include <boost/smart_ptr.hpp>

#include <BlendInt/AbstractButton.hpp>
#include <BlendInt/GLArrayBufferF.hpp>

namespace BlendInt {

	class String;

	/**
	 * @brief Toggle Button
	 *
	 * @ingroup widgets
	 */
	class ToggleButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(ToggleButton);

	public:

		ToggleButton ();

		ToggleButton (const String& text);

		ToggleButton (AbstractWidget* parent);

		ToggleButton (const String& text, AbstractWidget* parent);

	protected:

		virtual ~ToggleButton ();

		virtual void Update (int type, const void* data);

		virtual void Draw ();

	private:

		void InitializeOnce ();

		void InitializeOnce (const String& text);

		boost::scoped_ptr<GLArrayBufferF> m_inner_buffer;
		boost::scoped_ptr<GLArrayBufferF> m_outer_buffer;
		boost::scoped_ptr<GLArrayBufferF> m_emboss_buffer;

	};

}

#endif /* _BIL_CHECKBUTTON_HPP_ */
