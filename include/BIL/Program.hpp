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

#ifndef _BIL_PROGRAM_HPP_
#define _BIL_PROGRAM_HPP_

#include <GL/glew.h>

#include <vector>

#include <BIL/Shader.hpp>

namespace BIL {

	namespace GL {

		class Program
		{
		public:

			Program ();

			~Program ();

			void addShader (const char* buf, GLenum type);

			void addShader (const std::string& filename, GLenum type);

			bool link ();

			bool isValid () const;

			GLuint id () const
			{
				return id_;
			}

		private:

			GLuint id_;

			std::vector<Shader*> shaders_;
		};

	}
}

#endif /* _BIL_PROGRAM_HPP_ */
