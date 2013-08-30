// cpp

#include <GL/glew.h>

#include <iostream>

#include <BIL/Widget.hpp>
#include "ShaderWidget.hpp"

using namespace BIL;
using namespace std;

static const char* vs_source =
		"attribute vec2 coord2d;"
		"attribute vec3 v_color;"
		"varying vec3 f_color;"
		""
		"void main(void) {"
		"  gl_Position = vec4(coord2d, 0.0, 1.0);"
		"  f_color = v_color;"
		"}";


static const char* fs_source =
		"uniform float fade;"
		"varying vec3 f_color;"
		""
		"void main(void) {"
		"  gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, fade);"
		"}";


GLint attribute_coord2d, attribute_v_color;
GLint uniform_fade;

struct attributes {
  GLfloat coord2d[2];
  GLfloat v_color[3];
};

ShaderWidget::ShaderWidget(Traceable* parent)
: Widget(parent), vbo_triangle_(0)
{
	 struct attributes triangle_attributes[] = {
	    {{ 0.0,  0.8}, {1.0, 1.0, 0.0}},
	    {{-0.8, -0.8}, {0.0, 0.0, 1.0}},
	    {{ 0.8, -0.8}, {1.0, 0.0, 0.0}}
	  };

	  glGenBuffers(1, &vbo_triangle_);
	  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);


	program_.addShader(vs_source, GL_VERTEX_SHADER);
	program_.addShader(fs_source, GL_FRAGMENT_SHADER);
	program_.link();

	const char* attribute_name;
	  attribute_name = "coord2d";
	  attribute_coord2d = glGetAttribLocation(program_.id(), attribute_name);
	  if (attribute_coord2d == -1) {
	    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  }
	  attribute_name = "v_color";
	  attribute_v_color = glGetAttribLocation(program_.id(), attribute_name);
	  if (attribute_v_color == -1) {
	    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  }
	  const char* uniform_name;
	  uniform_name = "fade";
	  uniform_fade = glGetUniformLocation(program_.id(), uniform_name);
	  if (uniform_fade == -1) {
	    fprintf(stderr, "Could not bind uniform_fade %s\n", uniform_name);
	  }
}

ShaderWidget::~ShaderWidget ()
{
	  glDeleteBuffers(1, &vbo_triangle_);
}


void ShaderWidget::render() {
	  glUseProgram(program_.id());

	  /*
	  glEnableVertexAttribArray(attribute_coord2d);
	  /\* Describe our vertices array to OpenGL (it can't guess its format automatically) *\/
	  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	  glVertexAttribPointer(
	    attribute_coord2d, // attribute
	    2,                 // number of elements per vertex, here (x,y)
	    GL_FLOAT,          // the type of each element
	    GL_FALSE,          // take our values as-is
	    0,                 // no extra data between each position
	    0                  // offset of first element
	  );

	  glEnableVertexAttribArray(attribute_v_color);
	  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
	  glVertexAttribPointer(
	    attribute_v_color, // attribute
	    3,                 // number of elements per vertex, here (x,y)
	    GL_FLOAT,          // the type of each element
	    GL_FALSE,          // take our values as-is
	    0,                 // no extra data between each position
	    0                  // offset of first element
	  );
	  */

	  glEnableVertexAttribArray(attribute_coord2d);
	  glEnableVertexAttribArray(attribute_v_color);
	  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_);
	  glVertexAttribPointer(
	    attribute_coord2d,   // attribute
	    2,                   // number of elements per vertex, here (x,y)
	    GL_FLOAT,            // the type of each element
	    GL_FALSE,            // take our values as-is
	    sizeof(struct attributes),  // next coord2d appears every 5 floats
	    0                    // offset of first element
	  );
	  glVertexAttribPointer(
	    attribute_v_color,      // attribute
	    3,                      // number of elements per vertex, here (r,g,b)
	    GL_FLOAT,               // the type of each element
	    GL_FALSE,               // take our values as-is
	    sizeof(struct attributes),  // stride
	    //(GLvoid*) (2 * sizeof(GLfloat))     // offset of first element
	    (GLvoid*) offsetof(struct attributes, v_color)  // offset
	  );

	  /* Push each element in buffer_vertices to the vertex shader */
	  glDrawArrays(GL_TRIANGLES, 0, 3);

	  glDisableVertexAttribArray(attribute_coord2d);
	  glDisableVertexAttribArray(attribute_v_color);
}

