/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */
#include <GL/glew.h>
#include <iostream>
#include <stdlib.h>

#include <BIL/ShaderManager.hpp>

namespace BIL {

	const char* ShaderManager::text_vs_shader_ =
			"attribute vec4 coord;"
			"varying vec2 texpos;"
			""
			"void main(void) {"
			"  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord.xy, 0, 1);"
			"  texpos = coord.zw;"
			"}";

	const char* ShaderManager::text_fs_shader_ =
			"varying vec2 texpos;"
			"uniform sampler2D tex;"
			"uniform vec4 color;"
			""
			"void main(void) {"
			"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;"
			"}";

	ShaderManager* ShaderManager::shader_manager = 0;

	bool ShaderManager::initialize()
	{
		if(!shader_manager) {
			shader_manager = new ShaderManager;
		}

		return shader_manager->setup();
	}

	void ShaderManager::release()
	{
		if(shader_manager) {
			delete shader_manager;
			shader_manager = 0;
		}
	}

	ShaderManager* ShaderManager::instance()
	{
		if (!shader_manager) {
			std::cerr << "The Shader Manager is not initialized successfully! Exit" << std::endl;
			exit (EXIT_FAILURE);
		}

		return shader_manager;
	}

	ShaderManager::ShaderManager()
	: text_attribute_coord_(-1),
	text_uniform_tex_(-1),
	text_uniform_color_(-1),
	text_vbo_(0)
	{

	}

	ShaderManager::~ShaderManager()
	{
		text_program_.clear();

		glDeleteBuffers(1, &text_vbo_);
	}

	bool ShaderManager::setup ()
	{
		text_program_.attachShaderPair(text_vs_shader_, text_fs_shader_);
		if(!text_program_.link()) {
			return false;
		}

		text_attribute_coord_ = text_program_.getAttributeLocation("coord");
		text_uniform_tex_ = text_program_.getUniformLocation("tex");
		text_uniform_color_ = text_program_.getUniformLocation("color");
		if(text_attribute_coord_ == -1 || text_uniform_tex_ == -1 || text_uniform_color_ == -1) {
			std::cerr << "Error: cannot get attributes and uniforms" << std::endl;
			return false;
		}

		// Create the vertex buffer object
		if(glIsBuffer(text_vbo_)) {
			glDeleteBuffers(1, &text_vbo_);
		}

		glGenBuffers(1, &text_vbo_);
		/*
		if(!glIsBuffer(text_vbo_)) {
			std::cerr << "Error: cannot generate buffer object for text" << std::endl;
			return false;
		}
		*/

		return true;
	}
}
