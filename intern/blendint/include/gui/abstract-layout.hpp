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

#include <gui/abstract-widget.hpp>

namespace BlendInt {

	class AbstractLayout: public AbstractWidget
	{
	public:

		AbstractLayout ();

		AbstractLayout (int width, int height);

		AbstractLayout (int width, int height, const Margin& margin);

		virtual ~AbstractLayout ();

		virtual bool AddWidget (AbstractWidget* widget) = 0;

		virtual bool InsertWidget (int index, AbstractWidget* widget) = 0;

		virtual bool InsertWidget (int row, int column, AbstractWidget* widget) = 0;

		virtual void Adjust () = 0;

		const Margin& margin () const {return margin_;}

		void SetMargin (const Margin& margin);

	protected:

		virtual Response Draw (AbstractWindow* context);

		virtual void PerformMarginUpdate (const Margin& margin);

		inline void set_margin (const Margin& margin)
		{
			margin_ = margin;
		}

	private:

		Margin margin_;

	};

}
