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

#include "MainLayout.hpp"

using namespace BI;
using Stock::Shaders;

MainLayout::MainLayout ()
: VLayout(),
  m_menubar(0),
  m_toolbar(0),
  m_tool_open(0),
  m_tab(0),
  m_scene(0),
  m_image_view(0),
  m_file_input(0),
  m_file_button(0),
  m_btn_open(0),
  m_msg_label(0)
{
	InitOnce();
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	set_margin(0, 0, 0, 0);
	set_space(1);

	m_menubar = CreateMenuBar();
	m_toolbar = CreateToolBar();

    Splitter* splitter = Manage(new Splitter);
    splitter->SetMargin(0, 0, 0, 0);

    ToolBox* tbox = CreateToolBox();

    m_tab = CreateTab();

    ToolBar* bottom = CreateBottomBar();

    splitter->Append(m_tab);
    splitter->Append(tbox);

	Append(m_menubar);
	Append(m_toolbar);
    Append(splitter);
    Append(bottom);

	events()->connect(m_tool_open->clicked(), this, &MainLayout::OnOpenClick);
}

void MainLayout::PerformSizeUpdate(const SizeUpdateRequest& request)
{
	if(request.target() == this) {
		Refresh();
	}

	VLayout::PerformSizeUpdate(request);
}

ResponseType MainLayout::Draw(Profile& profile)
{
	if(refresh()) RenderToBuffer();

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	buffer_.Draw(0.f, 0.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return Accept;
}

void MainLayout::RenderToBuffer()
{
	GLsizei width = size().width();
	GLsizei height = size().height();

	buffer_.SetCoord(0.f, 0.f, size().width(), size().height());
	// Create and set texture to render to.
	GLTexture2D* tex = buffer_.texture();
	if(tex->id() == 0)
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
			GL_TEXTURE_2D, tex->id(), 0);
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

		Profile off_screen_profile(GetGlobalPosition());

		glm::mat4 identity(1.f);
		Shaders::instance->PushUIModelMatrix();
		Shaders::instance->SetUIModelMatrix(identity);

		glClearColor(0.208f, 0.208f, 0.208f, 1.f);
		glClearDepth(1.0);
		glClearStencil(0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f,
		        -100.f);

		Shaders::instance->PushUIProjectionMatrix();
		Shaders::instance->SetUIProjectionMatrix(projection);

        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
		glViewport(0, 0, width, height);

		GLboolean scissor_test;
		glGetBooleanv(GL_SCISSOR_TEST, &scissor_test);
		glDisable(GL_SCISSOR_TEST);

		// Draw frame panel

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			DispatchDrawEvent(p, off_screen_profile);
		}

		// Restore the viewport setting and projection matrix
		glViewport(vp[0], vp[1], vp[2], vp[3]);

		Shaders::instance->PopUIProjectionMatrix();
		Shaders::instance->PopUIModelMatrix();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(scissor_test) {
			glEnable(GL_SCISSOR_TEST);
		}
	}

	fb->reset();
	tex->reset();

	//delete tex; tex = 0;

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &rb);

	fb->reset();
	delete fb; fb = 0;
}

void MainLayout::OnOpenClick()
{
	if(m_file_input->text().empty()) return;

	std::string filename = ConvertFromString(m_file_input->text());

	int index = m_tab->GetIndex();

	switch(index) {
		case 0: {
			RefPtr<Mesh> monkey(new Mesh);

			if(monkey->Load(filename.c_str())) {
				m_scene->PushBack(monkey);

				m_msg_label->SetText("3D Mesh is loaded");
				m_msg_label->Resize(m_msg_label->GetPreferredSize());
			} else {
				m_msg_label->SetText("");
				m_msg_label->Resize(m_msg_label->GetPreferredSize());
			}
			break;
		}

		case 1: {

			if(m_image_view->Open(filename.c_str())) {
				m_msg_label->SetText("Image is loaded");
				
				Size prefer = m_msg_label->GetPreferredSize();
				DBG_PRINT_MSG("prefer size: %d, %d", prefer.width(), prefer.height());
				
				m_msg_label->Resize(m_msg_label->GetPreferredSize());

				DBG_PRINT_MSG("size: %d, %d", m_msg_label->size().width(), m_msg_label->size().height());

			} else {
				m_msg_label->SetText("");
				m_msg_label->Resize(m_msg_label->GetPreferredSize());
			}

			break;
		}

		default: {
			m_msg_label->SetText("");
			m_msg_label->Resize(m_msg_label->GetPreferredSize());
			break;
		}
	}
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}

BI::ToolBar* MainLayout::CreateToolBar()
{
	ToolBar* toolbar = Manage(new ToolBar);

	HBlockLayout* hblock = Manage (new HBlockLayout);
	
	m_tool_open = Manage(new ToolButton);

	ToolButton* btn1 = Manage(new ToolButton);
	btn1->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::SEQ_PREVIEW), String("SEQ PREVIEW"));

	ToolButton* btn2 = Manage(new ToolButton);
	btn2->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::SEQ_LUMA_WAVEFORM), String("SEQ LUMA WAVEFORM"));

	ToolButton* btn3 = Manage(new ToolButton);
	btn3->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::SEQ_CHROMA_SCOPE), String("SEQ CHROMA SCOPE"));

	ToolButton* btn4 = Manage(new ToolButton);
	btn4->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::SEQ_HISTOGRAM), String("SEQ HISTOGRAM"));

	ToolButton* btn5 = Manage(new ToolButton);
	btn5->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::SEQ_SPLITVIEW), String("SEQ SPLITVIEW"));

	hblock->Append(m_tool_open);
	hblock->Append(btn1);
	hblock->Append(btn2);
	hblock->Append(btn3);
	hblock->Append(btn4);
	hblock->Append(btn5);
	hblock->Resize(hblock->GetPreferredSize());

	HBlockLayout* hblock2 = Manage (new HBlockLayout);

	ToolButton* btn6 = Manage(new ToolButton);
	btn6->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::MAN_TRANS), String("MAN TRANS"));

	ToolButton* btn7 = Manage(new ToolButton);
	btn7->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::MAN_ROT), String("MAN ROT"));

	ToolButton* btn8 = Manage(new ToolButton);
	btn8->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::MAN_SCALE), String("MAN SCALE"));

	hblock2->Append(btn6);
	hblock2->Append(btn7);
	hblock2->Append(btn8);
	hblock2->Resize(hblock2->GetPreferredSize());

	HBlockLayout* hblock3 = Manage (new HBlockLayout);

	ToolButton* btn9 = Manage(new ToolButton);
	btn9->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::IMAGE_RGB), String("IMAGE RGB"));

	ToolButton* btn10 = Manage(new ToolButton);
	btn10->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::IMAGE_RGB_ALPHA), String("IMAGE RGB ALPHA"));

	ToolButton* btn11 = Manage(new ToolButton);
	btn11->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::IMAGE_ALPHA), String("IMAGE ALPHA"));

	ToolButton* btn12 = Manage(new ToolButton);
	btn12->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::IMAGE_ZDEPTH), String("IMAGE ZDEPTH"));

	ToolButton* btn13 = Manage(new ToolButton);
	btn13->SetAction(BI::Stock::Icons::instance->icon_16x16(Stock::IMAGEFILE), String("IMAGEFILE"));

	hblock3->Append(btn9);
	hblock3->Append(btn10);
	hblock3->Append(btn11);
	hblock3->Append(btn12);
	hblock3->Append(btn13);
	hblock3->Resize(hblock3->GetPreferredSize());

	toolbar->Append(hblock);
	toolbar->Append(hblock2);
	toolbar->Append(hblock3);

	events()->connect(m_tool_open->clicked(), this, &MainLayout::OnOpenClick);

	return toolbar;
}

BI::MenuBar* MainLayout::CreateMenuBar()
{
	MenuBar* menubar = Manage(new MenuBar);

	RefPtr<Menu> file_menu(new Menu);

    file_menu->SetRoundType(RoundAll);
    file_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddAction("MenuItem2", "Ctrl + 1");
    file_menu->AddAction("MenuItem3", "Ctrl + 1");
    file_menu->AddAction("MenuItem4", "Ctrl + 1");
    file_menu->AddAction("MenuItem5");

	menubar->Append(String("File"), file_menu);

	RefPtr<Menu> edit_menu(new Menu);

	edit_menu->SetRoundType(RoundAll);
	edit_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
	edit_menu->AddAction("MenuItem2", "Ctrl + 1");
	edit_menu->AddAction("MenuItem3", "Ctrl + 1");
	edit_menu->AddAction("MenuItem4", "Ctrl + 1");
	edit_menu->AddAction("MenuItem5");

	menubar->Append(String("Edit"), edit_menu);

	RefPtr<Menu> help_menu(new Menu);

	help_menu->SetRoundType(RoundAll);
	help_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
	help_menu->AddAction("MenuItem2", "Ctrl + 1");
	help_menu->AddAction("MenuItem3", "Ctrl + 1");
	help_menu->AddAction("MenuItem4", "Ctrl + 1");
	help_menu->AddAction("MenuItem5");

	menubar->Append(String("Help"), help_menu);


	return menubar;
}

BI::ToolBox* MainLayout::CreateToolBox()
{
	ToolBox* toolbox = Manage(new ToolBox);

	Expander* expander1 = CreateTransformExpander();
	toolbox->Append(expander1);

	Expander* expander2 = CreateLightExpander();
	toolbox->Append(expander2);

	Expander* expander3 = CreateColorExpander();
	toolbox->Append(expander3);

	return toolbox;
}

BI::Expander* MainLayout::CreateTransformExpander()
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

BI::Expander* MainLayout::CreateLightExpander()
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

BI::Expander* MainLayout::CreateColorExpander()
{
	Expander* expander = Manage(new Expander("Color"));

	ColorSelector* cs = Manage(new ColorSelector);

	expander->Setup(cs);

	return expander;
}

BI::Tab* MainLayout::CreateTab ()
{
	Tab* tab = Manage(new Tab);

    m_scene = Manage(new Viewport3D);
    m_image_view = Manage(new ImageView);

    tab->Add("3D View", m_scene);
    tab->Add("Image", m_image_view);

	return tab;
}

BI::ToolBar* MainLayout::CreateBottomBar ()
{
	ToolBar* toolbar = Manage(new ToolBar);
	toolbar->SetMargin(2, 2, 2, 2);

	HLayout* box = Manage(new HLayout);
	box->SetMargin(0, 0, 0, 0);
	box->SetSpace(-1);
	Label* label = Manage(new Label("Select 3D model or image file: "));
	m_file_input = Manage(new TextEntry);
	m_file_button = Manage(new FileButton);
	m_file_input->SetRoundType(RoundTopLeft | RoundBottomLeft);
	m_file_button->SetRoundType(RoundTopRight | RoundBottomRight);
	box->Append(label);
	box->Append(m_file_input);
	box->Append(m_file_button);

	m_btn_open = Manage(new Button("Open"));

	ComboBox* combo = Manage(new ComboBox);

	Label* info_label = Manage(new Label("Info: "));
	m_msg_label = Manage(new Label("Ready..."));
	//m_msg_label->SetBackgroundColor(Color(0x8888EFFF));

	toolbar->Append(box);
	toolbar->Append(m_btn_open);
	toolbar->Append(combo);
	toolbar->Append(info_label);
	toolbar->Append(m_msg_label);

	events()->connect(m_file_button->file_selected(), this, &MainLayout::OnFileSelected);
	events()->connect(m_btn_open->clicked(), this, &MainLayout::OnOpenClick);

	return toolbar;
}

void MainLayout::OnFileSelected ()
{
	m_file_input->SetText(m_file_button->file());

	HLayout* box = dynamic_cast<HLayout*>(m_file_input->parent());
	box->Resize(box->GetPreferredSize());
}
