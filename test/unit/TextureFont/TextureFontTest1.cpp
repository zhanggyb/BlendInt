#include "TextureFontTest1.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/TextureGlyph.hpp>
#include <BlendInt/Core/Freetype.hpp>

using namespace BlendInt;

static const char* text_vertex_shader =
		"#version 330\n"
		"layout(location = 0) in vec4 coord;"
		"uniform mat4 MVP;"
		"out vec2 texpos;"
		""
		"void main(void) {"
		"  gl_Position = MVP * vec4(coord.xy, 0.0, 1.0);"
		"  texpos = coord.zw;"
		"}";

static const char* text_fragment_shader =
		"#version 330\n"
		"in vec2 texpos;"
		"uniform sampler2D tex;"
		"uniform vec4 color = vec4(0.f, 0.f, 0.f, 1.f);"
		"out vec4 FragmentColor;"
		""
		"void main(void) {"
		"	float alpha = texture(tex, texpos).r;"	// GL 3.2 only support GL_R8 in glTexImage2D internalFormat
		"	FragmentColor = vec4(color.rgb, color.a * alpha);"
		"}";

TextureFontTest1::TextureFontTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TextureFontTest1::~TextureFontTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TextureFontTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("TextureFont - Foo1", 640, 480);

    // TODO: add test code here

    Freetype ft;
#ifdef __APPLE__
    ft.Open("/System/Library/Fonts/STHeiti Medium.ttc", 72, 96);
#else
    ft.Open("/home/zhanggyb/.fonts/m/msyh.ttf", 72, 96);
#endif

    if(!ft.valid()) {
    	DBG_PRINT_MSG("%s", "fail to load font");
    }

    TextureGlyph font;

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    font.Load(ft, L'舞');

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(font.glyph().vertexes[0]), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLSLProgram program;
    program.Create();
    program.AttachShaderPair(text_vertex_shader, text_fragment_shader);
    if(!program.Link()) {
    	DBG_PRINT_MSG("%s", "Fail to link program");
    }

    program.Use();
	GLint text_attribute_coord_ = program.GetAttributeLocation("coord");
	GLint text_uniform_tex_ = program.GetUniformLocation("tex");
	GLint text_uniform_mvp = program.GetUniformLocation("MVP");
	GLint text_uniform_color_ = program.GetUniformLocation("color");
	if(text_attribute_coord_ == -1 || text_uniform_tex_ == -1 || text_uniform_mvp == -1 || text_uniform_color_ == -1) {
		DBG_PRINT_MSG("%s", "Error: cannot get attributes and uniforms");
	}
	program.Reset();

    glBindVertexArray(0);

    glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 projection = glm::ortho(0.f, 640.f, 0.f, 480.f, 100.f, -100.f);
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(100.f, 100.f, 0.f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(win)) {

    	/* Render here */
    	Interface::instance->Draw();

    	// Draw texture glyph here

    	glBindVertexArray(vao);

    	program.Use();

    	glActiveTexture(GL_TEXTURE0);

    	glBindTexture(GL_TEXTURE_2D, font.texture());

    	program.SetUniformMatrix4fv(text_uniform_mvp, 1, GL_FALSE, glm::value_ptr(projection * view * model));
		program.SetUniform1i(text_uniform_tex_, 0);
		program.SetUniform4f(text_uniform_color_, 1.f, 0.1f, 0.1f, 1.0f);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(
				0,
				4,
				GL_FLOAT,
				GL_FALSE,
				0,
				BUFFER_OFFSET(0)
				);

		glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0);

    	glBindTexture(GL_TEXTURE_2D, 0);

    	program.Reset();

    	glBindVertexArray(0);

    	/* Swap front and back buffers */
    	glfwSwapBuffers(win);

    	/* Poll for and process events */
#ifdef __APPLE__
    	glfwPollEvents();
#else
    	glfwWaitEvents();
#endif  // __APPLE__
    }

    ft.Close();
    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
