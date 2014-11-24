/*
 * A context works in GLFW
 */

#ifndef _HPECONTEXT_HPP_
#define _HPECONTEXT_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/CVImageViewport.hpp>
#include <BlendInt/Core/Timer.hpp>
#include <BlendInt/Gui/Viewport.hpp>
#include <BlendInt/Gui/Panel.hpp>

namespace BI=BlendInt;

class HPEContext: public BI::Context
{
public:

	HPEContext ();

	virtual ~HPEContext ();

	virtual void SynchronizeWindow ();

protected:

private:

	enum Status {
		VideoPlay,
		VideoPause,
		VideoStop
	};

	BI::ToolBox* CreateToolBoxOnce ();

	BI::ToolBox* CreateToolBarOnce ();

	BI::Panel* CreateButtons ();

	bool OpenCamera (int n, const BI::Size& resolution = BI::Size(640, 480));

	void OnPlay ();

	void OnPause ();

	void OnStop();

	void OnTimeout (BI::Timer* t);

	BI::CVImageViewport* viewport_image_;

	BI::Viewport* viewport_3d_;

	Status status_;

	cv::VideoCapture video_stream_;

	BI::RefPtr<BI::Timer> timer_;
};

#endif /* _HPECONTEXT_HPP_ */
