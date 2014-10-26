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

#ifndef _BLENDINT_GUI_STATICPANEL_HPP_
#define _BLENDINT_GUI_STATICPANEL_HPP_

#include <BlendInt/Gui/BinLayout.hpp>

#include <BlendInt/Gui/ImagePlane2D.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief A Frame container for demo only
	 */
	class StaticPanel: public BinLayout
	{
		DISALLOW_COPY_AND_ASSIGN(StaticPanel);

	public:

		StaticPanel ();

		virtual ~StaticPanel ();

		void RenderToFile (const std::string& filename);

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void InitializeFramePanel ();

		void RenderToBuffer (Profile& profile);

		GLuint vao_;
        
        GLBuffer<> inner_;

		ImagePlane2D tex_buffer_;

	};

}

#endif /* _BLENDINT_GUI_STATICPANEL_HPP_ */
