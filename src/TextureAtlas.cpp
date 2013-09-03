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

#include <string.h>
#include <algorithm>
#include <assert.h>

#include <BIL/TextureAtlas.hpp>

// Maximum texture width
#define MAXWIDTH 1024

namespace BIL {

	TextureAtlas::TextureAtlas (const size_t width, const size_t height,
	        const size_t depth)
			: texture_(0), width_(width), height_(height), depth_(depth), data_(
			        0)
	{
		data_ = (unsigned char *) calloc(width_ * height_ * depth_,
		        sizeof(unsigned char));

		nodes_.push_back(Vector3i(1, 1, width - 2));

		if (!data_) {
			fprintf(stderr, "line %d: No more memory for allocating data\n",
			__LINE__);
			exit(EXIT_FAILURE);
		}
	}

	TextureAtlas::~TextureAtlas ()
	{
		if (data_) {
			free(data_);
		}
		if (glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}

		nodes_.clear();
	}

	void TextureAtlas::setRegion (const size_t x, const size_t y,
	        const size_t width, const size_t height, const unsigned char * data,
	        const size_t stride)
	{
		size_t i;
		size_t depth;
		size_t charsize;

		assert(x > 0);
		assert(y > 0);
		assert(x < (width_ - 1));
		assert((x + width) <= (width_ - 1));
		assert(y < (height_ - 1));
		assert((y + height) <= (height_ - 1));

		depth = depth_;
		charsize = sizeof(char);
		for (i = 0; i < height; ++i) {
			memcpy(data_ + ((y + i) * width_ + x) * charsize * depth,
			        data + (i * stride) * charsize, width * charsize * depth);
		}
	}

	Rect TextureAtlas::getRegion (const size_t width, const size_t height)
	{

		int y, best_height, best_width, best_index;
		Rect region(0, 0, width, height);

		Vector3i node; // prev
		size_t i;

		best_height = INT_MAX;
		best_index = -1;
		best_width = INT_MAX;
		for (i = 0; i < nodes_.size(); ++i) {
			y = fit(i, width, height);
			if (y >= 0) {
				node = nodes_[i];
				if (((y + height) < best_height)
				        || (((y + height) == best_height)
				                && (node.z() < best_width))) {
					best_height = y + height;
					best_index = i;
					best_width = node.z();
					region.set_x(node.x());
					region.set_y(y);
				}
			}
		}

		if (best_index == -1) {
			region.set_x (-1);
			region.set_y (-1);
			region.set_width (0);
			region.set_height (0);
			return region;
		}

		node.set_x (region.x());
		node.set_y (region.y() + height);
		node.set_z (width);
		/*
		vector_insert(self->nodes, best_index, node);
		free(node);

		for (i = best_index + 1; i < self->nodes->size; ++i) {
			node = (ivec3 *) vector_get(self->nodes, i);
			prev = (ivec3 *) vector_get(self->nodes, i - 1);

			if (node->x < (prev->x + prev->z)) {
				int shrink = prev->x + prev->z - node->x;
				node->x += shrink;
				node->z -= shrink;
				if (node->z <= 0) {
					vector_erase(self->nodes, i);
					--i;
				} else {
					break;
				}
			} else {
				break;
			}
		}
		texture_atlas_merge (self);
		self->used += width * height;
		 */

		return region;
	}

	void TextureAtlas::upload ()
	{
		if (!texture_) {
			glGenTextures(1, &texture_);
		}

		glBindTexture( GL_TEXTURE_2D, texture_);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (depth_ == 4) {
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0,
			GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data_);
#else
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
					0, GL_RGBA, GL_UNSIGNED_BYTE, data_ );
#endif
		} else if (depth_ == 3) {
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data_);
		} else {
			glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, width_, height_, 0,
			GL_ALPHA, GL_UNSIGNED_BYTE, data_);
		}
	}

	int TextureAtlas::fit (const size_t index, const size_t width,
	        const size_t height)
	{
		if (index > (nodes_.size() - 1)) {
			return -1;
		}

		int x, y, width_left;
		size_t i;
		Vector3i node;

		node = nodes_[index];
		x = node.x();
		y = node.y();
		width_left = width;
		i = index;

		if ((x + width) > (width_ - 1)) {
			return -1;
		}
		y = node.y();
		while (width_left > 0) {
			node = nodes_[i];
			if (node.y() > y) {
				y = node.y();
			}
			if ((y + height) > (height_ - 1)) {
				return -1;
			}
			width_left -= node.z();
			++i;
		}
		return y;
	}

	void TextureAtlas::merge ()
	{
		std::vector<Vector3i>::iterator it;

		for (it = nodes_.begin(); it != nodes_.end();) {

			if (it->y() == (it + 1)->y()) {
				it->set_z(it->z() + (it + 1)->z());
				it = nodes_.erase(it + 1);
			} else {
				it++;
			}
		}
	}

}
