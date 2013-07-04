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

#include <BIL/Color.h>

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

		const Color& getColor (void) const
		{
			return _color;
		}

		void setColor (const Color& color)
		{
			_color = color;
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
		float _size; /** Font size */
		Color _color; /** Font color */

		static const std::string DEFAULT_FONT;
	};

} /* namespace BIL */
#endif /* FONT_H_ */
