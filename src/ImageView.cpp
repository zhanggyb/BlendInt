/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */
#include <GL/glew.h>
#include <BILO/ImageView.hpp>

namespace BILO {

	ImageView::ImageView ()
	: Widget()
	{
		glShadeModel(GL_FLAT);
		makeCheckImage();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		resize(64 + m_padding.left() + m_padding.right(), 64 + m_padding.top() + m_padding.bottom());
	}

	ImageView::ImageView (Drawable* parent)
	: Widget(parent)
	{
		glShadeModel(GL_FLAT);
		makeCheckImage();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		resize(64 + m_padding.left() + m_padding.right(), 64 + m_padding.top() + m_padding.bottom());
	}

	ImageView::~ImageView ()
	{
	}

	void ImageView::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glRasterPos2i(m_padding.left(), m_padding.bottom());
		glDrawPixels (checkImageWidth, checkImageHeight, GL_RGB, GL_UNSIGNED_BYTE, _checkImage);

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(m_size.width(), 0);
			glVertex2i(m_size.width(), m_size.height());
			glVertex2i(0, m_size.height());
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
				c = (((i & 0x8) == 0) ^ ((j & 0x8)) == 0) * 255;
				_checkImage[i][j][0] = (GLubyte) c;
				_checkImage[i][j][1] = (GLubyte) c;
				_checkImage[i][j][2] = (GLubyte) c;
				_checkImage[i][j][3] = (GLubyte) c;
			}
		}
	}

}
