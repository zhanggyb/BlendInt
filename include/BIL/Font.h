/*
 * Font.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <FTGL/ftgl.h>
#include <string>
#include <vector>
#include <GL/gl.h>

#include <BIL/Color.h>
#include <BIL/Point.h>
#include <BIL/Rect.h>

namespace BIL {

	class Font
	{
	public:

		enum FONT_TYPE
		{
			BITMAP, /** Raster fonts */
			PIXMAP, POLYGON, OUTLINE, EXTRUDED, TEXTURE, /** Vector fonts */
			BUFFER /** Textured fonts */
		};

		Font (const std::string& name = DEFAULT_FONT, FONT_TYPE type = TEXTURE);

		Font (const Font& orig);

		Font& operator = (const Font& orig);

		virtual ~Font ();

		float getSize (void) const
		{
			return _size;
		}

		void setSize (float size)
		{
			_size = size;
		}

		const std::string& getName (void) const
		{
			return _name;
		}

		void setName (const std::string& name)
		{
			_name = name;
		}

		static const std::string& getDefaultFont (void)
		{
			return DEFAULT_FONT;
		}

	private:

		std::string _name; /** Font anme, e.g. "Arial" */
		std::string _filename; /** filename or empty */

		std::vector<GLfloat> _aspect; /** aspect ratio or scale, n = 3 */

		Point3Df _pos; /** initial position for draw the text */

		GLfloat _angle; /** angle in radians */

		GLint _blue; /** blue: 3 or 5 large kernel */

		GLint _shadow; /** shadow level */

		GLint _shadow_x; /** shadow offset */
		GLint _shadow_y; /** shadow offset */

		Color _shadow_col; /** shadow color */

		Color _orig_col; /** store color here when drawing shadow or blue */

		GLdouble _m[16]; /** Multiplied this matrix with the current one before draw the text! see blf_draw_start */

		Rect2Df _clip_rec;	/** clipping rectangle */

		GLuint _dpi;	/** font dpi (Default 72). */

		GLuint _size;	/** font size */

		GLint _max_tex_size;	/** max texture size */

		GLuint _tex_bind_state;	/** cache current OpenGL texture to save calls into the API */

		GLint flags;	/** font options */

		FT_Library _ft_lib;	/** freetype2 lib handle */

		FT_Face _face;	/** freetype2 face */

		static const std::string DEFAULT_FONT;
	};

} /* namespace BIL */
#endif /* FONT_H_ */
