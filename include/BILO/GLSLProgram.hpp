/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_PROGRAM_HPP_
#define _BIL_PROGRAM_HPP_

#include <GL/gl.h>

#include <vector>

#include <BILO/GLSLShader.hpp>

namespace BILO {

	class GLSLProgram
	{
	public:

		GLSLProgram ();

		~GLSLProgram ();

		void attachShaderPair (const char* vertex_shader, const char* fragment_shader);

		void attachShaderPair (const std::string& vertex_shader, const std::string& fragment_shader);

		void attachShader (const char* buf, GLenum type);

		void attachShader (const std::string& filename, GLenum type);

		bool link ();

		bool isValid () const;

		void clear ();

		void activate ();

		void deactivate ();

		void print_log ();

		GLuint id () const
		{
			return id_;
		}

		GLint getAttributeLocation (const char *name);

		GLint getUniformLocation (const char *name);

	private:

		GLuint id_;

		std::vector<GLSLShader*> shaders_;
	};

}

#endif /* _BIL_PROGRAM_HPP_ */
