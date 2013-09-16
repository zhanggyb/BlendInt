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

#include <BIL/Image.hpp>
#include <OpenImageIO/imageio.h>

OIIO_NAMESPACE_USING

namespace BIL {

	Image::Image ()
	: width_(0), height_(0), channels_(0)
	{

	}

	Image::~Image()
	{

	}

	bool Image::read (const String& filename)
	{
		ImageInput *in = ImageInput::open (ConvertFromString(filename));

		if (! in)
			return false;

		const ImageSpec &spec = in->spec();

		width_ = spec.width;
		height_ = spec.height;
		channels_ = spec.nchannels;
		m_pixels.resize (width_ * height_ * channels_);
		in->read_image (TypeDesc::UINT8, &m_pixels[0]);
		in->close ();

		delete in;

		return true;
	}

}
