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

#ifndef _BLENDINT_FONT_FCOBJECTSET_HPP_
#define _BLENDINT_FONT_FCOBJECTSET_HPP_

#include <stdarg.h>
#include <fontconfig/fontconfig.h>

#include <blendint/core/object.hpp>

namespace BlendInt {

	namespace Fc {

		class ObjectSet: public Object
		{
		public:

			ObjectSet (::FcObjectSet* os = 0)
			: Object(),
			  object_set_(os)
			{
				if(object_set_ == 0)
					object_set_ = FcObjectSetCreate();
			}

			virtual ~ObjectSet ()
			{
				FcObjectSetDestroy(object_set_);
			}

			inline bool add (const char* object)
			{
				return FcObjectSetAdd (object_set_, object);
			}

			static inline RefPtr<ObjectSet> build (const char *first, ...)
			{
				va_list args;
				va_start(args, first);
				::FcObjectSet* os = FcObjectSetBuild(first, args, NULL);
				va_end(args);

				return RefPtr<ObjectSet>(new ObjectSet(os));
			}

			inline ::FcObjectSet* object_set () const
			{
				return object_set_;
			}

		private:

			ObjectSet (const ObjectSet& orig);

			ObjectSet& operator = (const ObjectSet& orig);

			::FcObjectSet* object_set_;
		};

	}

}


#endif /* _BLENDINT_FONT_FCOBJECTSET_HPP_ */
