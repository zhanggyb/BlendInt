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

#ifndef _BLENDINT_FONT_FCCONFIG_HPP_
#define _BLENDINT_FONT_FCCONFIG_HPP_

#include <fontconfig/fontconfig.h>

#include <BlendInt/Core/Object.hpp>

#include <BlendInt/Font/FcPattern.hpp>
#include <BlendInt/Font/FcFontSet.hpp>
#include <BlendInt/Font/FcObjectSet.hpp>

namespace BlendInt {

	namespace Fc {

		class Config: public Object
		{
		public:

			Config (::FcConfig* config = 0)
			: Object(),
			  config_(config)
			{
				if(config_ == 0)
					config_ = FcConfigCreate();
			}

			Config (const Config& orig)
			: Object(),
			  config_(0)
			{
				config_ = orig.config_;

				if(config_)
					FcConfigReference(config_);
			}

			virtual ~Config ()
			{
				FcConfigDestroy(config_);
			}

			inline Config& operator = (const Config& orig)
			{
				if(config_)
					FcConfigDestroy(config_);

				config_ = orig.config_;

				if(config_)
					FcConfigReference(config_);

				return *this;
			}

			inline bool set_current ()
			{
				return FcConfigSetCurrent(config_);
			}

			inline bool upto_date ()
			{
				return FcConfigUptoDate (config_);
			}

			inline operator bool () const
			{
				return config_ != 0;
			}

			static inline Config get_current ()
			{
				FcConfig* config = FcConfigGetCurrent();
				return Config(config);
			}

			static inline bool substitute (const Config* config, const Pattern& p, FcMatchKind kind)
			{
				return FcConfigSubstitute(
						config == nullptr ? NULL : config->config_,
								p.pattern(),
								kind
				);
			}

			static inline Pattern match (const Config* config, const Pattern& p, FcResult* result)
			{
				::FcPattern* pattern = FcFontMatch (
						config == nullptr ? NULL : config->config_,
						p.pattern(),
						result);

				return Pattern(pattern);
			}

			static inline RefPtr<FontSet> list (const Config* config, const Pattern& p, const ObjectSet& os)
			{
				::FcFontSet* fs = 0;

				if(config) {
					fs = FcFontList(config->config_, p.pattern(), os.object_set());
				} else {
					fs = FcFontList(NULL, p.pattern(), os.object_set());
				}

				return RefPtr<FontSet>(new FontSet(fs));
			}

		private:

			::FcConfig* config_;

		};

	}

}

#endif /* _BLENDINT_FONT_FCCONFIG_HPP_ */
