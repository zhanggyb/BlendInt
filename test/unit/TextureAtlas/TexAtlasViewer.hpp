#ifndef _TEXATLAS_VIEWER_HPP_
#define _TEXATLAS_VIEWER_HPP_

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/OpenGL/TextureAtlas2D.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BI = BlendInt;

class TexAtlasViewer: public BI::Widget
{
public:

	TexAtlasViewer ();
	
	virtual ~TexAtlasViewer ();
	
protected:
	
	virtual BI::ResponseType Draw (const BI::Profile& profile);
	
private:
	
	GLuint m_vao;
	
	BI::TextureAtlas2D m_atlas;

	BI::RefPtr<BI::GLSLProgram> m_program;
	BI::RefPtr<BI::GLArrayBuffer> m_vbo;
	BI::RefPtr<BI::GLArrayBuffer> m_tbo;	// texture coords
	
	static const char* vertex_shader;
	static const char* fragment_shader;
};

#endif
