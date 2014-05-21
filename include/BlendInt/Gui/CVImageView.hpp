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

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/CheckerBoard.hpp>
#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class CVImageView: public Widget
	{
	public:

		CVImageView ();

		virtual ~CVImageView ();

		void Open (const char* filename);

		void Load (const cv::Mat& image);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

	private:

		void InitOnce ();

		GLuint m_vao;

		RefPtr<GLTexture2D> m_texture;
		RefPtr<GLSLProgram> m_program;
		RefPtr<GLArrayBuffer> m_vbo;
		RefPtr<GLArrayBuffer> m_tbo;	// texture coords

		RefPtr<CheckerBoard> m_checkerboard;

		cv::Mat m_image;

		static const char* vertex_shader;
		static const char* fragment_shader;
	};

}

#endif	// __USE_OPENCV__

#endif /* _BLENDINT_GUI_CVIMAGEVIEW_HPP_ */
