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

#ifndef _BLENDINT_FONT_PATTERN_HPP_
#define _BLENDINT_FONT_PATTERN_HPP_

#include <fontconfig/fontconfig.h>

#include <blendint/core/object.hpp>

#include <blendint/font/fc-charset.hpp>

namespace BlendInt {

	namespace Fc {

		class Pattern
		{
		public:

			// the p must be created through FcPatternCreate;
			Pattern (::FcPattern* p = 0)
			: pattern_(p)
			{
				if(pattern_ == 0)
					pattern_ = FcPatternCreate();
			}

			Pattern (const Pattern& p)
			: pattern_(0)
			{
				pattern_ = p.pattern_;

				if(pattern_)
					FcPatternReference(pattern_);
			}

			virtual ~Pattern ()
			{
				if(pattern_)
					FcPatternDestroy(pattern_);
			}

			inline void destroy ()
			{
				if(pattern_) {
					FcPatternDestroy(pattern_);
					pattern_ = 0;
				}
			}

			inline Pattern& operator = (const Pattern& orig)
			{
				if(pattern_)
					FcPatternDestroy(pattern_);

				pattern_ = orig.pattern_;

				if(pattern_)
					FcPatternReference(pattern_);

				return *this;
			}

			inline FcChar32 hash () const
			{
				return FcPatternHash (pattern_);
			}
			
			inline bool add (const char* object, FcValue value, bool append = true)
			{
				return FcPatternAdd(pattern_, object, value, append ? FcTrue : FcFalse);
			}

			inline bool add_integer (const char* object, int i)
			{
				return FcPatternAddInteger(pattern_, object, i);
			}

			inline bool add_double (const char* object, double d)
			{
				return FcPatternAddDouble(pattern_, object, d);
			}

			// add string
			inline bool add_string (const char* object, const FcChar8* s)
			{
				return FcPatternAddString(pattern_, object, s);
			}

			inline bool add_matrix (const char* object, const FcMatrix* m)
			{
				return FcPatternAddMatrix(pattern_, object, m);
			}

			inline bool add_charset (const char* object, const CharSet& c)
			{
				return FcPatternAddCharSet(pattern_, object, c.charset());
			}

			inline bool add_bool (const char* object, bool b)
			{
				return FcPatternAddBool(pattern_, object, b ? FcTrue : FcFalse);
			}

			inline bool add_langset (const char *object, const FcLangSet *l)
			{
				return FcPatternAddLangSet (pattern_, object, l);
			}

			/**
			 * @brief Get a value from a pattern
			 *
			 * @note The value returned is not a copy, but rather
			 * refers to the data stored within the pattern
			 * directly. Same as all get functions.
			 */
			inline FcResult get (const char* object, int id, FcValue* v)
			{
				return FcPatternGet(pattern_, object, id, v);
			}

			inline FcResult get_integer (const char* object, int n, int *i)
			{
				return FcPatternGetInteger(pattern_, object, n, i);
			}

			inline FcResult get_double (const char *object, int n, double *d)
			{
				return FcPatternGetDouble(pattern_, object, n, d);
			}

			inline FcResult get_string (const char *object, int n, FcChar8 **s)
			{
				return FcPatternGetString(pattern_, object, n, s);
			}

			inline FcResult get_matrix (const char *object, int n, FcMatrix **s)
			{
				return FcPatternGetMatrix(pattern_, object, n, s);
			}

			inline FcResult get_charset (const char *object, int n, FcCharSet **c)
			{
				return FcPatternGetCharSet(pattern_, object, n, c);
			}

//			inline FcResult get (const char *object, int n, FcBool *b)
//			{
//				return FcPatternGetBool(pattern_, object, n, b);
//			}

//			inline FcResult get (const char *object, int n, FT_Face *f)
//			{
//				return FcPatternGetFTFace(pattern_, object, n, f);
//			}

			inline FcResult get_langset (const char *object, int n, FcLangSet **l)
			{
				return FcPatternGetLangSet(pattern_, object, n, l);
			}

//			inline FcResult get (const char *object, int n, FcRange **r)
//			{
//				return FcPatternGetRange(pattern_, object, n, r);
//			}

			inline bool del (const char* object)
			{
				return FcPatternDel (pattern_, object);
			}

			inline bool remove (const char* object, int id)
			{
				return FcPatternRemove(pattern_, object, id);
			}

			inline void default_substitute ()
			{
				FcDefaultSubstitute(pattern_);
			}

			inline void print () const
			{
				FcPatternPrint(pattern_);
			}

			inline operator bool () const
			{
				return pattern_ != 0;
			}

			::FcPattern* pattern () const {return pattern_;}

			static inline Pattern name_parse (const FcChar8 *name)
			{
				::FcPattern* p = FcNameParse(name);
				return Pattern(p);
			}

			static inline Pattern duplicate (const Pattern& pattern)
			{
				::FcPattern* p = FcPatternDuplicate (pattern.pattern_);

				return Pattern(p);
			}

		private:

			friend inline bool operator == (const Pattern& pa, const Pattern& pb);

			::FcPattern* pattern_;

		};

		inline bool operator == (const Pattern& pa, const Pattern& pb)
		{
			return FcPatternEqual(pa.pattern_, pb.pattern_);
		}

	}

}

#endif /* _BLENDINT_FONT_PATTERN_HPP_ */
