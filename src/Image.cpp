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

#include <BlendInt/Image.hpp>

#ifndef __APPLE__
#include <OpenImageIO/imageio.h>
#endif

#ifndef __APPLE__
OIIO_NAMESPACE_USING
#endif

namespace BlendInt {

	Image::Image ()
	: m_width(0), m_height(0), m_channels(0)
	{

	}

	Image::~Image()
	{

	}

	bool Image::read (const String& filename)
	{
#ifndef __APPLE__
		ImageInput *in = ImageInput::open (ConvertFromString(filename));

		if (! in)
			return false;

		const ImageSpec &spec = in->spec();

		m_width = spec.width;
		m_height = spec.height;
		m_channels = spec.nchannels;
		m_pixels.resize (m_width * m_height * m_channels);
		in->read_image (TypeDesc::UINT8, &m_pixels[0]);
		in->close ();

		delete in;
#endif

		return true;
	}

}
