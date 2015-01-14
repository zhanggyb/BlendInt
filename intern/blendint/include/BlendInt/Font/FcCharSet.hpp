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

#ifndef _BLENDINT_FONT_CHARSET_HPP_
#define _BLENDINT_FONT_CHARSET_HPP_

#include <fontconfig/fontconfig.h>

namespace BlendInt {

	class FcCharSet
	{
	public:

		inline FcCharSet ()
		: charset_(0)
		{
			charset_ = FcCharSetCreate();
		}

		inline ~FcCharSet ()
		{
			FcCharSetDestroy(charset_);
		}

		inline ::FcCharSet* charset () const
		{
			return charset_;
		}

	private:

		friend inline bool operator == (const FcCharSet& a, const FcCharSet& b);

		::FcCharSet* charset_;
	};

	bool operator == (const FcCharSet& a, const FcCharSet& b)
	{
		return FcCharSetEqual(a.charset_, b.charset_);
	}

}

#endif /* _BLENDINT_FONT_CHARSET_HPP_ */
