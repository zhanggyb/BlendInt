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

#ifndef _BLENDINT_GUI_IMAGEVIEWPORT_HPP_
#define _BLENDINT_GUI_IMAGEVIEWPORT_HPP_

// generate makefile with cmake -DENABLE_OPENCV to activate
#ifdef __USE_OPENCV__
#include <opencv2/core/core.hpp>
#endif

#include <BlendInt/Core/Color.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/ChessBoard.hpp>
#include <BlendInt/Gui/Frame.hpp>

namespace BlendInt {

	/**
	 * @brief A frame to display opencv image
	 *
	 * This class provides similar functions as ImageView except that it
	 * get image data from an opencv matrix.
	 *
	 * Due to opencv design, this widget cannot display images with alpha channel.
	 */
	class ImageViewport: public Frame
	{
	public:

		ImageViewport ();

		virtual ~ImageViewport ();

		bool OpenFile (const char* filename);

#ifdef __USE_OPENCV__
		bool LoadCVImage (const cv::Mat& image);
#endif

		bool SetTexture (const RefPtr<GLTexture2D>& texture);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

	private:

		void InitializeImageViewport ();

		void AdjustImageArea (const Size& size);

		void AdjustScrollArea (const Size& size);

		/**
		 * @brief Vertex Array Objects
		 *
		 * 0 - for background
		 * 1 - for plane to display image texture
		 */
		GLuint vao_;

		RefPtr<GLTexture2D> texture_;

		GLBuffer<> image_plane_;

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		static Color background_color;
	};

}

//#endif	// __USE_OPENCV__

#endif /* _BLENDINT_GUI_IMAGEVIEWPORT_HPP_ */
