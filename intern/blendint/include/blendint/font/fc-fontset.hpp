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

#ifndef _BLENDINT_FONT_FCFONTSET_HPP_
#define _BLENDINT_FONT_FCFONTSET_HPP_

#include <fontconfig/fontconfig.h>

#include <core/object.hpp>

#include <font/fc-pattern.hpp>

namespace BlendInt {

	namespace Fc {

		class FontSet: public Object
		{
		public:

			FontSet (::FcFontSet* fs = 0)
			: Object(),
			  fontset_(fs)
			{
				if(fontset_ == 0)
					fontset_ = FcFontSetCreate();
			}

			virtual ~FontSet ()
			{
				FcFontSetDestroy(fontset_);
			}

			inline bool add (const Pattern& font)
			{
				return FcFontSetAdd (fontset_, font.pattern());
			}

			inline void print ()
			{
				FcFontSetPrint(fontset_);
			}

			inline ::FcFontSet* fontset () const
			{
				return fontset_;
			}

		private:

			FontSet (const FontSet& orig);

			FontSet& operator = (const FontSet& orig);

			::FcFontSet* fontset_;
		};

	}

}

#endif /* _BLENDINT_FONT_FCFONTSET_HPP_ */
