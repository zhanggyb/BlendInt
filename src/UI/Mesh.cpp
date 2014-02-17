/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <BlendInt/Mesh.hpp>

namespace BlendInt {

	Mesh::Mesh ()
	: AbstractPrimitive(), m_vb(0), m_ib(0)
	{
	}

	void Mesh::Render (const glm::mat4& mvp)
	{
	}

	bool Mesh::LoadObj (const char* filename)
	{
		using namespace std;

		ifstream in(filename, ios::in);
		if (!in) {
			cerr << "Cannot open " << filename << endl;
			return false;
		}
		vector<int> nb_seen;

		m_vertices.clear();
		m_elements.clear();

		string line;
		while (getline(in, line)) {
			if (line.substr(0, 2) == "v ") {
				istringstream s(line.substr(2));
				glm::vec4 v;
				s >> v.x;
				s >> v.y;
				s >> v.z;
				v.w = 1.0;
				m_vertices.push_back(v);
			} else if (line.substr(0, 2) == "f ") {
				istringstream s(line.substr(2));
				GLushort a, b, c;
				s >> a;
				s >> b;
				s >> c;
				a--;
				b--;
				c--;
				m_elements.push_back(a);
				m_elements.push_back(b);
				m_elements.push_back(c);
			} else if (line[0] == '#') { /* ignoring this line */
			} else { /* ignoring this line */
			}
		}

		m_normals.resize(m_vertices.size(), glm::vec3(0.0, 0.0, 0.0));
		nb_seen.resize(m_vertices.size(), 0);
		for (unsigned int i = 0; i < m_elements.size(); i += 3) {
			GLushort ia = m_elements[i];
			GLushort ib = m_elements[i + 1];
			GLushort ic = m_elements[i + 2];
			glm::vec3 normal = glm::normalize(
			        glm::cross(
			                glm::vec3(m_vertices[ib])
			                        - glm::vec3(m_vertices[ia]),
			                glm::vec3(m_vertices[ic])
			                        - glm::vec3(m_vertices[ia])));

			int v[3];
			v[0] = ia;
			v[1] = ib;
			v[2] = ic;
			for (int j = 0; j < 3; j++) {
				GLushort cur_v = v[j];
				nb_seen[cur_v]++;
				if (nb_seen[cur_v] == 1) {
					m_normals[cur_v] = normal;
				} else {
					// average
					m_normals[cur_v].x = m_normals[cur_v].x
					        * (1.0 - 1.0 / nb_seen[cur_v])
					        + normal.x * 1.0 / nb_seen[cur_v];
					m_normals[cur_v].y = m_normals[cur_v].y
					        * (1.0 - 1.0 / nb_seen[cur_v])
					        + normal.y * 1.0 / nb_seen[cur_v];
					m_normals[cur_v].z = m_normals[cur_v].z
					        * (1.0 - 1.0 / nb_seen[cur_v])
					        + normal.z * 1.0 / nb_seen[cur_v];
					m_normals[cur_v] = glm::normalize(m_normals[cur_v]);
				}
			}
		}

		return true;
	}

	Mesh::~Mesh ()
	{
		Destroy(m_vb);
		Destroy(m_ib);
	}

	void Mesh::InitOnce ()
	{
	}

}
