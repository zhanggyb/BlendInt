/**
 * Custom frame to do a off-screen render
 */

#ifndef _STUDIO_FRAME_HPP_
#define _STUDIO_FRAME_HPP_

#include <BlendInt/Gui/BinLayout.hpp>
#include <BlendInt/Gui/ImagePlane2D.hpp>

namespace BI=BlendInt;

class StudioFrame: public BI::BinLayout
{
public:

	StudioFrame ();

	virtual ~StudioFrame ();

	void RenderToFile (const std::string& filename);

protected:

	virtual void PerformSizeUpdate (const BI::SizeUpdateRequest& request);

	virtual BI::ResponseType Draw (BI::Profile& profile);

private:

	void RenderToBuffer ();

	BI::ImagePlane2D tex_buffer_;

};

#endif
