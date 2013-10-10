/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <string.h>
#include <cwchar>

#include <iostream>

#include <BILO/Font.hpp>

namespace BILO {

	Font::Font (const String& family, unsigned int size, bool bold, bool italic)
			: family(family), size(size), bold(bold), italic(italic)
	{
	}

	/*
	Font::Font (const std::string& family, unsigned int size, bool bold, bool italic)
			: family(family), size(size), bold(bold), italic(italic)
	{
	}
	*/

	Font::Font (const wchar_t* family, unsigned int size, bool bold,
	        bool italic)
		: family(family), size(size), bold(bold), italic(italic)
	{
	}

	Font::Font (const char* family, unsigned int size, bool bold, bool italic)
		: family(family), size(size), bold(bold), italic(italic)
	{
	}

	Font::Font (const Font& orig)
	{
		family = orig.family;
		size = orig.size;
		bold = orig.bold;
		italic = orig.italic;
	}

	Font& Font::operator = (const Font& orig)
	{
		family = orig.family;
		size = orig.size;
		bold = orig.bold;
		italic = orig.italic;

		return *this;
	}

	bool operator < (const Font& src, const Font& dist)
	{
		if(src.family < dist.family) {
			return true;
		} else if(src.family > dist.family) {
			return false;
		}

		if(src.size < dist.size) {
			return true;
		} else if(src.size > dist.size) {
			return false;
		}

		if(src.bold < dist.bold) {
			return true;
		} else if(src.bold > dist.bold) {
			return false;
		}

		if(src.italic < dist.italic) {
			return true;
		} else if (src.italic > dist.italic) {
			return false;
		}

		/*
		std::cout << src.family << " " << src.size << " " << (src.bold ? "bold":"normal") << " " << (src.italic? "italic":"normal")
				<< " < "
				<< dist.family << " " << dist.size << " " << (dist.bold ? "bold":"normal") << " " << (dist.italic? "italic":"normal")
				<< std::endl;
		*/
		return false;
	}

	bool operator == (const Font& src, const Font& dist)
	{
		if (wcscmp(src.family.c_str(), dist.family.c_str()) == 0) {
			return true;
		}

		return (src.family == dist.family &&
				src.size == dist.size &&
				src.bold == dist.bold &&
				src.italic == dist.italic);
	}

}

