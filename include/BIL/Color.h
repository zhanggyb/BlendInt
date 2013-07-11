/*
 * Color.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_COLOR_H_
#define _BIL_COLOR_H_

#include <GL/gl.h>

namespace BIL {

	class Color
	{
	public:

		enum RGBA
		{
			None, White, Black
		};

		Color (RGBA val)
				: _red(1.0), _green(1.0), _blue(1.0), _alpha(1.0)
		{
			switch (val) {
				case None:
					_alpha = 0.0;
					break;
				case White:
					break;
				case Black:
					_red = 0.0;
					_green = 0.0;
					_blue = 0.0;
					break;
				default:
					break;
			}
		}

		Color ()
				: _red(1.0), _green(1.0), _blue(1.0), _alpha(1.0)
		{
		}

		Color (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0)
				: _red(red), _green(green), _blue(blue), _alpha(alpha)
		{
			correct();
		}

		Color (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha = 255)
		{
			_red = red / 255.0;
			_green = green / 255.0;
			_blue = blue / 255.0;
			_alpha = alpha / 255.0;

			correct();
		}

		Color (GLdouble red, GLdouble green, GLdouble blue,
		        GLdouble alpha = 1.0)
				: _red(red), _green(green), _blue(blue), _alpha(alpha)
		{
			correct();
		}

		Color (GLint red, GLint green, GLint blue, GLint alpha = 255)
		{
			_red = red / 255.0;
			_green = green / 255.0;
			_blue = blue / 255.0;
			_alpha = alpha / 255.0;

			correct();
		}

		Color (GLuint red, GLuint green, GLuint blue, GLuint alpha = 255)
		{
			_red = red / 255.0;
			_green = green / 255.0;
			_blue = blue / 255.0;
			_alpha = alpha / 255.0;

			correct();
		}

		Color (const Color& orig)
		{
			_red = orig.red();
			_green = orig.green();
			_blue = orig.blue();
			_alpha = orig.alpha();
		}

		Color& operator = (const Color& orig)
		{
			_red = orig.red();
			_green = orig.green();
			_blue = orig.blue();
			_alpha = orig.alpha();

			return *this;
		}

		virtual ~Color ()
		{
		}

		inline GLdouble red (void) const
		{
			return _red;
		}

		inline GLdouble green (void) const
		{
			return _green;
		}

		inline GLdouble blue (void) const
		{
			return _blue;
		}

		inline GLdouble alpha (void) const
		{
			return _alpha;
		}

	private:

		void correct (void)
		{
			_red = (_red < 0.0) ? 0.0 : _red;
			_red = (_red > 1.0) ? 1.0 : _red;

			_green = (_green < 0.0) ? 0.0 : _green;
			_green = (_green > 1.0) ? 1.0 : _green;

			_blue = (_blue < 0.0) ? 0.0 : _blue;
			_blue = (_blue > 1.0) ? 1.0 : _blue;

			_alpha = (_alpha < 0.0) ? 0.0 : _alpha;
			_alpha = (_alpha > 1.0) ? 1.0 : _alpha;

			return;
		}

		GLdouble _red;
		GLdouble _green;
		GLdouble _blue;
		GLdouble _alpha;
	};
}

#endif /* COLOR_H_ */
