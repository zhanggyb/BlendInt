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

#pragma once

// generate makefile with cmake -DENABLE_OPENCV to activate
#ifdef __USE_OPENCV__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <core/timer.hpp>
#include <core/mutex.hpp>

#include <opengl/gl-buffer.hpp>

#include <gui/abstract-scrollable.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	class CVImageView: public AbstractScrollable
	{

	public:

		CVImageView();

		virtual ~CVImageView ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		bool OpenCamera (int n, const Size& resolution = Size(640, 480));

		bool OpenFile (const std::string& filename);

		void Play ();

		void Pause ();

		void Stop ();

		void Release ();

	protected:

		virtual void ProcessImage (cv::Mat& image);

	private:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual Response Draw (AbstractWindow* context);

		virtual void PostDraw (AbstractWindow* context);

		void OnUpdateFrame (Timer* sender);

		/**
		 * @brief Vertex Array Objects
		 *
		 * 0 - for background
		 * 1 - for plane to display image texture
		 */

		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> vbo_;

		GLTexture2D texture_;

		cv::VideoCapture video_stream_;

		cv::Mat image_;

		RefPtr<Timer> timer_;

		Mutex mutex_;

		AbstractWindow* off_screen_context_;

		Size image_size_;

		bool playing_;

		int count;
	};

}

#endif	// __USE_OPENCV__
