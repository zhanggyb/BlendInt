/**
 * Main Layout
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Splitter.hpp>

#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/RadioButton.hpp>

#include "MainSpace.hpp"

using namespace BI;
using Stock::Shaders;

MainSpace::MainSpace ()
: VLayout(),
  m_toolbar(0),
  workspace_(0),
  message_(0)
{
	InitOnce();
}

MainSpace::~MainSpace ()
{

}

void MainSpace::InitOnce ()
{
	set_margin(0, 0, 0, 0);
	set_space(1);

	m_toolbar = CreateToolBar();

    Splitter* splitter = Manage(new Splitter);
    splitter->SetMargin(0, 0, 0, 0);

    ToolBox* tbox = CreateSideBox();
    ToolBar* bottom = CreateBottomBar();

    workspace_ = Manage(new Workspace);
    splitter->Append(workspace_);
    splitter->Append(tbox);

	Append(m_toolbar);
    Append(splitter);
    Append(bottom);
}

void MainSpace::PerformRefresh(const RefreshRequest& request)
{
	refresh_ = true;
	ReportRefresh(request);
}

void MainSpace::PerformSizeUpdate(const SizeUpdateRequest& request)
{
	if(request.target() == this) {
		refresh_ = true;
	}

	VLayout::PerformSizeUpdate(request);
}

ResponseType MainSpace::Draw(Profile& profile)
{
	if(refresh_) {

		RenderToBuffer();

		refresh_ = false;
	}

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	buffer_.Draw(position().x(), position().y());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return Accept;
}

void MainSpace::RenderToBuffer()
{
	GLsizei width = size().width();
	GLsizei height = size().height();

	GLfloat left = position().x();
	GLfloat bottom = position().y();

	GLfloat right = left + width;
	GLfloat top = bottom + height;

	buffer_.SetCoord(0.f, 0.f, size().width(), size().height());
	// Create and set texture to render to.
	GLTexture2D* tex = buffer_.texture();
	if(!tex->texture())
		tex->generate();

	tex->bind();
	tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
	tex->SetMinFilter(GL_NEAREST);
	tex->SetMagFilter(GL_NEAREST);
	tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLFramebuffer* fb = new GLFramebuffer;
	fb->generate();
	fb->bind();

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, tex->texture(), 0);
	//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

	// Critical: Create a Depth_STENCIL renderbuffer for this off-screen rendering
	GLuint rb;
	glGenRenderbuffers(1, &rb);

	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
			size().width(), size().height());
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, rb);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, rb);

	if(GLFramebuffer::CheckStatus()) {

		fb->bind();

		glClearColor(0.208f, 0.208f, 0.208f, 1.f);
		glClearDepth(1.0);
		glClearStencil(0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glm::mat4 origin;

		Shaders::instance->GetUIProjectionMatrix(origin);

		glm::mat4 projection = glm::ortho(left, right, bottom, top, 100.f,
		        -100.f);

		Shaders::instance->SetUIProjectionMatrix(projection);

        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
		glViewport(0, 0, size().width(), size().height());

		// Draw frame panel

		Profile off_screen_profile(position());

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			DispatchDrawEvent(p, off_screen_profile);
		}

		// Restore the viewport setting and projection matrix
		glViewport(vp[0], vp[1], vp[2], vp[3]);

		Shaders::instance->SetUIProjectionMatrix(origin);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	fb->reset();
	tex->reset();

	//delete tex; tex = 0;

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &rb);

	fb->reset();
	delete fb; fb = 0;
}

void MainSpace::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}

BI::ToolBar* MainSpace::CreateToolBar()
{
	ToolBar* toolbar = Manage(new ToolBar);

	ComboBox* combo1 = Manage(new ComboBox);

	MenuButton* menu1 = Manage(new MenuButton("File"));
	MenuButton* menu2 = Manage(new MenuButton("Render"));
	MenuButton* menu3 = Manage(new MenuButton("Help"));

	TextEntry* input = Manage(new TextEntry);

	toolbar->Append(combo1);
	toolbar->Append(menu1);
	toolbar->Append(menu2);
	toolbar->Append(menu3);
	toolbar->Append(input);

	return toolbar;
}

BI::ToolBox* MainSpace::CreateSideBox()
{
	ToolBox* toolbox = Manage(new ToolBox);

	HBlockLayout* hblock = Manage(new HBlockLayout);
	RadioButton* btn1 = Manage(new RadioButton);
	RadioButton* btn2 = Manage(new RadioButton);
	RadioButton* btn3 = Manage(new RadioButton);
	RadioButton* btn4 = Manage(new RadioButton);

	hblock->Append(btn1);
	hblock->Append(btn2);
	hblock->Append(btn3);
	hblock->Append(btn4);

	toolbox->Append(hblock);

	Expander* expander1 = CreateTransformExpander();
	toolbox->Append(expander1);

	Expander* expander2 = CreateLightExpander();
	toolbox->Append(expander2);

	Expander* expander3 = CreateColorExpander();
	toolbox->Append(expander3);

	return toolbox;
}

BI::Expander* MainSpace::CreateTransformExpander()
{
	Expander* expander = Manage(new Expander("Transform"));

	Button* btn1 = Manage(new Button("Translate"));
	Button* btn2 = Manage(new Button("Rotate"));
	Button* btn3 = Manage(new Button("Scale"));

	VBlockLayout* vblock = Manage(new VBlockLayout);
	vblock->Append(btn1);
	vblock->Append(btn2);
	vblock->Append(btn3);

	expander->Setup(vblock);

	return expander;
}

BI::Expander* MainSpace::CreateLightExpander()
{
	Expander* expander = Manage(new Expander("Light"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);
	NumericalSlider* ns3 = Manage(new NumericalSlider);

	VBlockLayout* vblock = Manage(new VBlockLayout);
	vblock->Append(ns1);
	vblock->Append(ns2);
	vblock->Append(ns3);

	expander->Setup(vblock);

	return expander;
}

BI::Expander* MainSpace::CreateColorExpander()
{
	Expander* expander = Manage(new Expander("Color"));

	ColorSelector* cs = Manage(new ColorSelector);

	expander->Setup(cs);

	return expander;
}

BI::ToolBar* MainSpace::CreateBottomBar ()
{
	ToolBar* toolbar = Manage(new ToolBar);
	toolbar->SetMargin(2, 2, 2, 2);

	message_ = Manage(new Label("Ready..."));
	toolbar->Append(message_);

	return toolbar;
}
