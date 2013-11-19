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

#include <GL/glew.h>
#include <BlendInt/ImageView.hpp>

namespace BlendInt {

	ImageView::ImageView ()
	: Widget()
	{
		glShadeModel(GL_FLAT);
		makeCheckImage();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		Resize(64, 64);
	}

	ImageView::ImageView (AbstractWidget* parent)
	: Widget(parent)
	{
		glShadeModel(GL_FLAT);
		makeCheckImage();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		Resize(64, 64);
	}

	ImageView::~ImageView ()
	{
	}

	void ImageView::Render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glRasterPos2i(0, 0);
		glDrawPixels (checkImageWidth, checkImageHeight, GL_RGB, GL_UNSIGNED_BYTE, _checkImage);

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size().width(), 0);
			glVertex2i(size().width(), size().height());
			glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void ImageView::makeCheckImage ()
	{
		int i, j, c;
		for (i = 0; i < checkImageHeight; i++) {
			for (j = 0; j < checkImageWidth; j++) {
				c = (((i & 0x8) == 0) ^ (((j & 0x8) == 0))) * 255;
				_checkImage[i][j][0] = (GLubyte) c;
				_checkImage[i][j][1] = (GLubyte) c;
				_checkImage[i][j][2] = (GLubyte) c;
				_checkImage[i][j][3] = (GLubyte) c;
			}
		}
	}

}
