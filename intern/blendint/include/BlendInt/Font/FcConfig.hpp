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

#include <BlendInt/Font/FcPattern.hpp>

namespace BlendInt {

	class FcConfig
	{
	public:

		inline FcConfig ()
		: config_(0)
		{
			config_ = FcConfigCreate();
		}

		inline ~FcConfig ()
		{
			FcConfigDestroy(config_);
		}

		static inline bool substitute (const FcConfig* config, const FcPattern& p, FcMatchKind kind)
		{
			return FcConfigSubstitute(
					config == nullptr ? NULL : config->config_,
							p.pattern(),
							kind
			);
		}

		static inline FcPattern match (const FcConfig* config, const FcPattern& p, FcResult* result)
		{
			::FcPattern* pattern = FcFontMatch (
					config == nullptr ? NULL : config->config_,
					p.pattern(),
					result);

			return FcPattern(pattern);
		}

	private:

		::FcConfig* config_;

	};

}

#endif /* _BLENDINT_FONT_FCCONFIG_HPP_ */
