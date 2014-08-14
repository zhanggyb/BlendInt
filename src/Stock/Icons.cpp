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

#include <stdlib.h>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <boost/filesystem.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/OpenGL/TextureAtlas2D.hpp>
#include <BlendInt/Core/Image.hpp>

#include "BlendIntConfig.hpp"

namespace BlendInt {

	namespace Stock {

		Icons* Icons::instance = 0;

		bool Icons::Initialize ()
		{
			if (!instance)
				instance = new Icons;

			return true;
		}

		void Icons::Release ()
		{
			if (instance) {
				delete instance;
				instance = 0;
			}
		}

		Icons::Icons ()
		{
			CreateIcons();
		}

		Icons::~Icons ()
		{
		}

		void Icons::CreateIcons ()
		{
			namespace fs = boost::filesystem;

			// Create VertexIcons
			float vec[16][2];

			menu_.reset(new VertexIcon(16, 16));

			for (size_t i = 0; i < 6; i++) {
				vec[i][0] = 0.5 * 16 * VertexIcon::menu_tria_vert[i][0];
				vec[i][1] = 0.5 * 16 * VertexIcon::menu_tria_vert[i][1];
			}

			menu_->Load(vec, 6, VertexIcon::menu_tria_face, 2);

			circle_.reset(new VertexIcon(10, 10));

			for (size_t i = 0; i < 16; i++) {
				vec[i][0] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][0];
				vec[i][1] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][1];
			}

			circle_->Load(vec, 16, VertexIcon::scroll_circle_face, 14);

			check_.reset(new VertexIcon(14, 14));

			for (size_t i = 0; i < 6; i++) {
				vec[i][0] = 0.5 * 14 * VertexIcon::check_tria_vert[i][0];
				vec[i][1] = 0.5 * 14 * VertexIcon::check_tria_vert[i][1];
			}

			check_->Load(vec, 6, VertexIcon::check_tria_face, 4);

			num_.reset(new VertexIcon(10, 10));

			for (size_t i = 0; i < 3; i++) {
				vec[i][0] = 0.5 * 10 * VertexIcon::num_tria_vert[i][0];
				vec[i][1] = 0.5 * 10 * VertexIcon::num_tria_vert[i][1];
			}

			num_->Load(vec, 3, VertexIcon::num_tria_face, 1);

			// Create Pixel Icons

			fs::path icon16_path(BLENDINT_INSTALL_PREFIX"/share/BlendInt/datafiles/blender_icons16.png");
			fs::path icon32_path(BLENDINT_INSTALL_PREFIX"/share/BlendInt/datafiles/blender_icons32.png");

			if(!fs::exists(icon16_path)) {
				icon16_path = fs::path(BLENDINT_PROJECT_SOURCE_DIR"/release/datafiles/blender_icons16.png");
			}

			if(!fs::exists(icon32_path)) {
				icon32_path = fs::path(BLENDINT_PROJECT_SOURCE_DIR"/release/datafiles/blender_icons32.png");
			}

//			RefPtr<TextureAtlas2D> texture(new TextureAtlas2D);
//
//			texture->Generate(datatoc_blender_icons16_png_width, datatoc_blender_icons16_png_height, 5, 10, 16, 16, 5, 5);
//			texture->Bind();
//			texture->SetImage(0,
//					GL_RGBA,
//					datatoc_blender_icons16_png_width,
//					datatoc_blender_icons16_png_height,
//					0,
//					GL_RGBA,
//					GL_UNSIGNED_BYTE,
//					datatoc_blender_icons16_png);
//			texture->Reset();
//
//			m_icon_file_16x16.reset(new PixelIcon(16, 16));
//			m_icon_file_16x16->SetTexture(texture, 0);

			// TODO: set texture in PixelIcon and add more stock icons.

			Image image;

			image.Read(icon16_path.native());

			RefPtr<TextureAtlas2D> texture(new TextureAtlas2D);
			texture->Generate(image.width(),
					image.height(),
					16,
					16,
					5,
					10,
					5,
					5);
			texture->Bind();
			texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			texture->SetMinFilter(GL_LINEAR);
			texture->SetMagFilter(GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			texture->SetImage(0,
					GL_RGBA,
					image.width(),
					image.height(),
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					image.pixels());

			texture->Reset();

			GLfloat x1 = (GLfloat)5 / image.width();
			GLfloat y1 = (GLfloat)10 / image.height();
			GLfloat x2 = (GLfloat)(5 + 16) / image.width();
			GLfloat y2 = (GLfloat)(10 + 16) / image.height();

			GLfloat uv[] = {
					x1, y2,
					x2, y2,
					x1, y1,
					x2, y1
			};

			outline_16x16_.reset(new PixelIcon(16, 16, texture, uv));

		}

	}

}
