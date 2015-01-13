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

#ifndef _BLENDINT_GUI_ABSTRACTFORM_HPP_
#define _BLENDINT_GUI_ABSTRACTFORM_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <vector>

#include <glm/glm.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Object.hpp>

namespace BlendInt {

	struct ColorScheme;

	class Jitter: public std::vector<glm::vec2>
	{
	public:
		Jitter () :
				std::vector<glm::vec2>()
		{
			resize(8);

			this->operator [](0) = glm::vec2(0.468813, -0.481430);
			this->operator [](1) = glm::vec2(-0.155755, -0.352820);
			this->operator [](2) = glm::vec2(0.219306, -0.238501);
			this->operator [](3) = glm::vec2(-0.393286, -0.110949);
			this->operator [](4) = glm::vec2(-0.024699, 0.013908);
			this->operator [](5) = glm::vec2(0.343805, 0.147431);
			this->operator [](6) = glm::vec2(-0.272855, 0.269918);
			this->operator [](7) = glm::vec2(0.095909, 0.388710);
		}

		~Jitter ()
		{
			clear();
		}
	};

	/**
	 * @brief Abstract form class
	 *
	 * An AbstractForm is the base class for all GUI elements in 2 dimension.
	 *
	 * The default size is 0 at (0, 0) and the subclass must provide a public
	 * method to resize the form, this method usually need to call Update for redraw
	 * the form.
	 *
	 * @sa Update()
	 */
	class AbstractForm: public Object
	{
	public:

		AbstractForm ()
		: Object()
		{}

		AbstractForm (int width, int height)
		: Object(),
		  size_(width, height)
		{}

		AbstractForm (const Size& size)
		: Object(),
		  size_(size)
		{}

		virtual ~AbstractForm ()
		{}

		AbstractForm (const AbstractForm& orig)
		{
			size_ = orig.size_;
		}

		AbstractForm& operator = (const AbstractForm& orig)
		{
			size_ = orig.size();
			return *this;
		}

		void Resize (int width, int height);

		void Resize (const Size& size);

		const Size& size () const
		{
			return size_;
		}

		virtual void Draw (float x, float y, short gamma = 0) const = 0;

		static void SetDefaultBorderWidth (int border);

		static inline float default_border_width ()
		{
			return border_width;
		}

		static const Jitter kJit;

	protected:

		virtual void PerformSizeUpdate (const Size& size) = 0;

		/**
		 * @brief preset the size of the form
		 * @param width
		 * @param height
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (int width, int height)
		{
			size_.set_width(width);
			size_.set_height(height);
		}

		/**
		 * @brief preset the size of the form
		 * @param size
		 *
		 * @note this function should be called only in the constructor of subclass to set
		 * the size without through Update() for performance.
		 */
		inline void set_size (const Size& size)
		{
			size_ = size;
		}

		static void GenerateVertices (
				const Size& size,
				float border,
				int round_type,
				float radius,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		static void GenerateVertices (
				const Size& size,
				float border,
				int round_type,
				float radius,
				Orientation shadedir,
				short shadetop,
				short shadedown,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		static const float cornervec[WIDGET_CURVE_RESOLU][2];

	private:

		static inline float make_shaded_offset (short shadetop, short shadedown, float fact);

		static void GenerateTriangleStripVertices (
						const std::vector<GLfloat>* inner,
						const std::vector<GLfloat>* edge,
						unsigned int num,
						std::vector<GLfloat>* strip);

		static float border_width;

		Size size_;

	};

}

#endif /* _BLENDINT_ABSTRACTFORM_HPP_ */
