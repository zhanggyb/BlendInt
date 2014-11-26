/*
 * A context works in GLFW
 */

#ifndef _MARKERBASEDARCONTEXT_HPP_
#define _MARKERBASEDARCONTEXT_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/CVImageViewport.hpp>
#include <BlendInt/Core/Timer.hpp>

namespace BI=BlendInt;

class MarkerBasedARContext: public BI::Context
{
public:

	explicit MarkerBasedARContext (GLFWwindow* window);

	virtual ~MarkerBasedARContext ();

	virtual void SynchronizeWindow ();

	virtual void MakeGLContextCurrent ();

protected:

private:

	enum Status {
		VideoPlay,
		VideoPause,
		VideoStop
	};

	BI::ToolBox* CreateToolBoxOnce ();

	BI::ToolBox* CreateToolBarOnce ();

	bool OpenCamera (int n, const BI::Size& resolution = BI::Size(640, 480));

	void OnToggleCamera (bool toggled);

	void OnPlay ();

	void OnPause ();

	void OnStop();

	void OnTimeout (BI::Timer* t);

	BI::CVImageViewport* viewport_;

	Status status_;

	cv::VideoCapture video_stream_;

	cv::Mat frame_;

	BI::RefPtr<BI::Timer> timer_;

	GLFWwindow* window_;

};

#endif /* _MARKERBASEDARCONTEXT_HPP_ */
