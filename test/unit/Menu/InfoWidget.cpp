#include <GL/gl.h>

#include "InfoWidget.hpp"
#include <BlendInt/FontCache.hpp>

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

void InfoWidget::Draw ()
{
	BlendInt::Widget::Draw();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(position().x(),
				 position().y(),
				 z());
		
	BlendInt::FontCache::create(BlendInt::Font("Sans"))->print(5, 5, m_text);

	glPopMatrix();
}