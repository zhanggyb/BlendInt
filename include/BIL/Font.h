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

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <string>

#include <BIL/Tuple.h>

using namespace std;

namespace BIL {

	struct Font;

	// friend function
	extern bool operator < (const Font& src, const Font& dist);
	extern bool operator == (const Font& src, const Font& dist);

	struct Font
	{
		Font (const string& family = string("Sans"), unsigned int size = 10,
		        bool bold = false, bool italic = false);

		Font (const Font& orig);

		Font& operator = (const Font& orig);

		/** the font family, e.g. "Droid Sans" */
		string family;

		/** font size */
		unsigned int size;

		/** whether text is bold */
		bool bold;

		/** whether text is italic */
		bool italic;
	};

} /* namespace BIL */
#endif /* FONT_H_ */
