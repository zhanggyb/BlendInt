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

#ifndef _BLENDINT_GUI_CVVIDEOVIEW_HPP_
#define _BLENDINT_GUI_CVVIDEOVIEW_HPP_

// generate makefile with cmake -DENABLE_OPENCV to activate
#ifdef __USE_OPENCV__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Core/Timer.hpp>
#include <BlendInt/Core/Mutex.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	class CVVideoViewport: public Frame
	{
	public:

		CVVideoViewport();

		virtual ~CVVideoViewport ();

		bool OpenCamera (int n, const Size& resolution = Size(640, 480));

		bool OpenFile (const std::string& filename);

		void Release ();

		void SetFPS (unsigned int fps);

		void Play ();

		void Pause ();

		void Stop ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual ResponseType Draw (AbstractWindow* context);

		virtual void PostDraw (AbstractWindow* context);

		virtual void ProcessImage (cv::Mat& image);

	private:

		enum Status {
			VideoPlay,
			VideoPause,
			VideoStop
		};

		void InitializeCVVideoView ();

		void OnUpdateFrame (Timer* t);

		GLuint vao_;

		Status status_;

		GLBuffer<> frame_plane_;

		GLTexture2D texture_;

		cv::VideoCapture video_stream_;

		cv::Mat frame_;

		// if upload to GPU (texture)
		bool upload_;

		Mutex mutex_;

		RefPtr<Timer> timer_;

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

	};

}

#endif	// __USE_OPENCV__

#endif /* _BLENDINT_GUI_CVVIDEOVIEW_HPP_ */
