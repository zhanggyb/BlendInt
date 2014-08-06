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

#ifndef _BLENDINT_GUI_CVIMAGEVIEW_HPP_
#define _BLENDINT_GUI_CVIMAGEVIEW_HPP_

// generate makefile with cmake -DENABLE_OPENCV to activate
#ifdef __USE_OPENCV__

#include <opencv2/core/core.hpp>

#include <BlendInt/Core/Color.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/CheckerBoard.hpp>
#include <BlendInt/Gui/AbstractScrollable.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to display opencv image
	 *
	 * This class provides similar functions as ImageView except that it
	 * get image data from an opencv matrix.
	 *
	 * Due to opencv design, this widget cannot display images with alpha channel.
	 */
	class CVImageView: public AbstractScrollable
	{
	public:

		CVImageView ();

		virtual ~CVImageView ();

		void Open (const char* filename);

		void Load (const cv::Mat& image);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void InitializeCVImageView ();

		void AdjustImageArea (const Size& size);

		/**
		 * @brief Vertex Array Objects
		 *
		 * 0 - for background
		 * 1 - for plane to display image texture
		 */
		GLuint vaos_[2];

		RefPtr<GLTexture2D> texture_;

		RefPtr<GLArrayBuffer> background_;
		RefPtr<GLArrayBuffer> plane_;

		cv::Mat image_;

		static Color background_color;
	};

}

#endif	// __USE_OPENCV__

#endif /* _BLENDINT_GUI_CVIMAGEVIEW_HPP_ */
