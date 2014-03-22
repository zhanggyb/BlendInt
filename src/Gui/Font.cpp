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

#include <string.h>
#include <cwchar>

#include <iostream>

#include <BlendInt/Gui/Font.hpp>

namespace BlendInt {

	Font::Font (const std::string& family, unsigned int size, bool bold, bool italic)
			: family(family), size(size), bold(bold), italic(italic)
	{
		m_cache = FontCache::Create(family, size, 96, bold, italic);
	}

	/*
	Font::Font (const std::string& family, unsigned int size, bool bold, bool italic)
			: family(family), size(size), bold(bold), italic(italic)
	{
	}
	*/

	Font::Font (const char* family, unsigned int size, bool bold, bool italic)
		: family(family), size(size), bold(bold), italic(italic)
	{
		m_cache = FontCache::Create(family, size, 96, bold, italic);
	}

	Font::Font (const Font& orig)
	{
		family = orig.family;
		size = orig.size;
		bold = orig.bold;
		italic = orig.italic;

		m_cache = orig.m_cache;
	}

	Font& Font::operator = (const Font& orig)
	{
		family = orig.family;
		size = orig.size;
		bold = orig.bold;
		italic = orig.italic;

		m_cache = orig.m_cache;

		return *this;
	}

}

