/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_CHECKBUTTON_HPP_
#define _BIL_CHECKBUTTON_HPP_

#include <BIL/AbstractButton.hpp>
#include <BIL/Types.hpp>

namespace BIL {

	class ToggleButton: public AbstractButton
	{
	public:

		explicit ToggleButton (Widget* parent = NULL);

		ToggleButton (const String& text, Widget* parent = NULL);

		// Button (const Icon& icon, const wstring& text, Widget* parent = NULL);

		virtual ~ToggleButton ();

	protected:

		virtual void Update ();

		virtual void Render ();

	private:

		void DrawButton (WidgetVertexes* vertexes);

	private:
		ToggleButton (const ToggleButton& orig);
		ToggleButton& operator = (const ToggleButton& orig);
	};

}

#endif /* _BIL_CHECKBUTTON_HPP_ */
