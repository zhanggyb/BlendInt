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

  CVImageView ();

  virtual ~CVImageView ();

  virtual bool IsExpandX () const;

  virtual bool IsExpandY () const;

  virtual Size GetPreferredSize () const;

  bool OpenCamera (int n, int fps, const Size& resolution = Size(640, 480));

  bool OpenImageFile (const std::string& filename);

  bool OpenVideoFile (const std::string& filename, int fps);

  /**
   * @brief Play the video stream
   */
  void Play ();

  /**
   * @brief Pause the video stream
   *
   * If the video stream is read from camera, this is the same as Stop()
   */
  void Pause ();

  /**
   * @brief Stop the video stream
   *
   * If the video stream is read from a file, call Play() after will read frames from beginning.
   * If the video stream is read from camera, this is the same as Pause()
   */
  void Stop ();

  /**
   * @brief Release all OpenCV data
   */
  void Release ();

protected:

  virtual void ProcessImage (cv::Mat& image);

  void DrawTexture ();

private:

  enum CVImageFlagMasks
  {

    /**
     * Use this bit to check image or stream
     * 0 - image
     * 1 - video
     */
    DisplayModeMask = 0x1 << 0,

    /**
     * if image or video opened
     */
    StreamingMask = 0x1 << 1,

    /**
     * Use this bit to check video device
     * 0 - file
     * 1 - camera
     */
    DeviceTypeMask = 0x1 << 2,

    /**
     * Use this bit to check playback status
     * 1 - play
     */
    VideoPlayMask = 0x1 << 3,

    VideoPauseMask = 0x1 << 4,

    VideoStopMask = 0x1 << 5,

    PlaybackMask = VideoPlayMask | VideoPauseMask | VideoStopMask

  };

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

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

  /**
   * @brief status of current image or video stream
   *
   * Bit:
   *    0 - if image is loaded
   *    1 - if current use image or video stream
   *    2 - if video from file or camera
   *    3 - if video is playing
   *    4 - if video is paused
   *    5 - if video is stop
   */
  int flags_;
};

}

#endif  // __USE_OPENCV__
