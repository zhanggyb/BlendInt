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

#ifndef _BLENDINT_ABSTRACTFORM_HPP_
#define _BLENDINT_ABSTRACTFORM_HPP_

#include <BlendInt/Point.hpp>
#include <BlendInt/Size.hpp>

namespace BlendInt {

	enum AbstractFormProperty {
		FormPropertyPosition,
		FormPropertySize,
		AbstrctFormPropertyLast = FormPropertySize
	};

	/**
	 * @brief Abstract form class
	 */
	class AbstractForm
	{
	public:

		AbstractForm();

		virtual ~AbstractForm();

		const Point& position () const
		{
			return m_position;
		}

		void set_position (int x, int y);

		void set_position (const Point& position);

		const Size& size () const
		{
			return m_size;
		}

		void resize (unsigned int width, unsigned int height);

		void resize (const Size& size);

	protected:

		virtual void update (int type, const void* data) = 0;

		virtual void render () = 0;

		static void resize (AbstractForm* form, unsigned int w, unsigned int h);

		static void resize (AbstractForm* form, const Size& s);

		static void set_position (AbstractForm* form, int x, int y);

		static void set_position (AbstractForm* form, const Point& pos);

	private:

		Point m_position;

		Size m_size;

	};

}

#endif /* _BLENDINT_ABSTRACTFORM_HPP_ */
