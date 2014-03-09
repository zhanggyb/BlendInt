#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include "InfoWidget.hpp"
#include <BlendInt/Gui/FontCache.hpp>

InfoWidget::InfoWidget ()
: BlendInt::Widget()
{
	
}

InfoWidget::~InfoWidget ()
{
	
}

void InfoWidget::onShowText (BlendInt::MenuItem* item)
{
	m_text = item->text();
}

void InfoWidget::Draw (BlendInt::RedrawEvent* event)
{
	BlendInt::Widget::Draw(event);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(position().x(),
				 position().y(),
				 z());
		
	BlendInt::FontCache::create(BlendInt::Font("Sans"))->print(5, 5, m_text);

	glPopMatrix();
}
