/*
 * FontView.hpp
 */

#ifndef FONTVIEW_HPP_
#define FONTVIEW_HPP_

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/TextureGlyph.hpp>

namespace BI = BlendInt;

class FontView: public BI::Widget
{
public:

	FontView ();

	virtual ~FontView();

	void LoadCharacter ();

protected:

	virtual BI::ResponseType Draw (const BI::RedrawEvent& event);

private:

	GLuint m_vao;
	GLuint m_vbo;

    BI::TextureGlyph m_glyph;
    BI::GLSLProgram m_program;

};

#endif /* FONTVIEW_HPP_ */
