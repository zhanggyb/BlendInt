/*
 * FontView.hpp
 */

#ifndef FONTVIEW_HPP_
#define FONTVIEW_HPP_

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/GlyphTexture.hpp>
#include <BlendInt/Gui/Font.hpp>

namespace BI = BlendInt;

class FontView: public BI::Widget
{
public:

	FontView ();

	virtual ~FontView();

	void LoadCharacter ();

protected:

	virtual BI::ResponseType Draw (const BI::Profile& event);

private:

	/*
	GLuint m_vao;
	GLuint m_vbo;

    BI::TextureGlyph m_glyph;
    BI::RefPtr<BI::GLSLProgram> m_program;
	*/
	BI::Font m_font;

};

#endif /* FONTVIEW_HPP_ */
