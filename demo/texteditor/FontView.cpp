/*
 * FontView.cpp
 */

#include "FontView.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/Stock/Shaders.hpp>

/*

vert.glsl:

#version 330

layout(location = 0) in vec4 coord;
uniform mat4 MVP;
out vec2 uv;

void main() {
  gl_Position = MVP * vec4(coord.xy, 0.0, 1.0);
  uv = coord.zw;
}

frag.glsl:

#version 330

in vec2 uv;

uniform sampler2D tex;
uniform vec4 color;

out vec4 FragmentColor;

// filter
const float edge_kernel[9] = float[9](0., 1., 0.,
                                 1., -4., 1.,
                                 0., 1., 0.);

const float gaussian_kernel[9] = float[9](
    1., 2., 1.,
    2., 4., 2.,
    1., 2., 1.);

const float factor = 1.0;

void main() {

    float alpha = texture(tex, uv).r;
    //FragmentColor = vec4(color.rgb, color.a * alpha);

    ivec2 texSize = textureSize(tex, 0);
    float s = float(texSize.s);
    float t = float(texSize.t);

    vec4 sum = vec4(0.0);

    // 1. grey
    //float grey = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    //FragmentColor = vec4(grey, grey, grey, alpha);


    // 2. Sobel horizontal

    for(int i = 0; i < 9; i++)
    {
      vec4 tmp = texture(tex, uv + offset[i] * 2.0);
      sum += tmp * edge_kernel[i];
    }

    sum /= factor;

    //FragmentColor = vec4(sum.rgb, alpha) + color;
    FragmentColor = vec4(color.rgb, sum.r);

    // 3. Gaussian Blur

    // pixel offset
    vec2 offset[9] = vec2[9](
        vec2(-1. / s, 1. / t),  vec2(0., 1. / t),  vec2(1. / s, 1. / t),
        vec2(-1. / s, 0.),      vec2(0., 0.),      vec2(1. / s, 0.),
        vec2(-1. / s, -1. / t), vec2(0., -1. / t), vec2(1. / s, -1. / t)
        );

    for(int i = 0; i < 9; i++)
    {
      vec4 tmp = texture(tex, uv + offset[i]);
      sum += tmp * gaussian_kernel[i];
    }

    sum /= 16.0;

    FragmentColor = vec4(color.rgb, sum.r);
}

 */

/*
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
*/

FontView::FontView ()
//: vao_(0), m_vbo(0)
{
	set_size(200, 200);

	m_font.SetSize(16);
	m_font.SetOutline(true);
	m_font.set_color(BI::Color(0x223365DD));
	//m_font.SetName("STHeiti");
	m_font.SetShadow(true);

	size_t size = BI::FontCache::GetCacheSize ();
	DBG_PRINT_MSG("cache size: %ld", size);

	/*
	glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(0);

    m_program = BI::ShaderManager::instance->text_program();
    */
}

FontView::~FontView ()
{
	//glDeleteVertexArrays(1, &vao_);
}

void FontView::LoadCharacter ()
{
	/*
    BI::Freetype ft;
#ifdef __APPLE__
    ft.Open("/System/Library/Fonts/STHeiti Medium.ttc", 72, 120);
#else
    ft.Open("/home/zhanggyb/.fonts/m/msyh.ttf", 72, 120);
#endif

    if(!ft.valid()) {
    	DBG_PRINT_MSG("%s", "fail to load font");
    }

    m_glyph.Load(ft, L'佛');

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 4, &(m_glyph.glyph().vertexes[0]), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

    /*
    m_program.Create();
    //m_program.AttachShaderPair(text_vertex_shader, text_fragment_shader);
    //m_program.LoadShaderPairFile("vert.glsl", "frag.glsl");
    m_program.LoadShaderFile("vert.glsl", GL_VERTEX_SHADER);
    // m_program.LoadShaderFile("geom.glsl", GL_GEOMETRY_SHADER);
    m_program.LoadShaderFile("frag.glsl", GL_FRAGMENT_SHADER);
    if(!m_program.Link()) {
    	DBG_PRINT_MSG("%s", "Fail to link program");
    }
    */

	// glBindVertexArray(0);
}

BI::ResponseType FontView::Draw (const BI::Profile& event)
{
	//int advance = m_font.Print(position(), L"A仁B义C礼D智E信F");
	//int advance = m_font.Print(mvp, L"ABC");
	//DBG_PRINT_MSG("advance: %d", advance);

	/*
	glBindVertexArray(vao_);

	m_program->Use();

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_glyph.texture());

	m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
	m_program->SetUniform1i("tex", 0);
	m_program->SetUniform4f("color", 0.f, 0.f, 0.f, 1.0f);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			BUFFER_OFFSET(0)
			);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_program->Reset();
	glBindVertexArray(0);
	*/

	return BI::Finish;
}
