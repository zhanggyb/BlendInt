/*
 * TextureAtlas.cpp
 *
 *  Created on: 2013年7月15日
 *      Author: zhanggyb
 */

#include <GL/gl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <limits.h>

#include <BIL/TextureAtlas.h>

namespace BIL {

    TextureAtlas::TextureAtlas (const size_t w, const size_t h, const size_t d)
        : _width(w), _height(h), _depth(d), _used(0), _id(0)
    {
        Tuple3<int> node(1, 1, w-2);

        assert((d == 1) || (d == 3) || (d == 4));

        _nodes.push_back(node);

        _data = (unsigned char*) calloc(_width * _height * _depth,
                                        sizeof(unsigned char));

        if (_data == NULL)
            throw;
    }

    TextureAtlas::~TextureAtlas ()
    {
        _nodes.clear();

        if (_data) {
            free(_data);
        }
        if (_id) {
            glDeleteTextures(1, &_id);
        }
    }

    void TextureAtlas::setRegion (const size_t x, const size_t y,
                                  const size_t w, const size_t h, const unsigned char* data,
                                  const size_t stride)
    {
        size_t i, depth, charsize;

        assert(x > 0);
        assert(y > 0);
        assert(x < (_width - 1));
        assert((x + w) <= (_width - 1));
        assert(y < (_height - 1));
        assert((y + h) <= (_height - 1));

        depth = _depth;
        charsize = sizeof(char);

        for (i = 0; i < h; ++i) {
            memcpy(_data + ((y + i) * _width + x) * charsize * depth,
                   data + (i * stride) * charsize, w * charsize * depth);
        }
    }

    Tuple4i TextureAtlas::getRegion (const size_t w, const size_t h)
    {
        Tuple4i region(0, 0, w, h);
        int y, best_height, best_width, best_index;
        Tuple3i node, prev;
        size_t i;

        best_height = INT_MAX;
        best_index = -1;
        best_width = INT_MAX;

        for (i = 0; i < _nodes.size(); ++i) {
            y = fit(i, w, h);
            if (y >= 0) {
                node = _nodes[i];
                if (((y + h) < best_height)
                    || (((y + h) == best_height) && (node.coord.z < best_width))) {
                    best_height = y + h;
                    best_index = i;
                    best_width = node.coord.z;
                    region.rect.x = node.coord.x;
                    region.rect.y = y;
                }
            }
        }

        if (best_index == -1) {
            region.rect.x = -1;
            region.rect.y = -1;
            region.rect.w = 0;
            region.rect.h = 0;
            return region;
        }

        node.coord.x = region.rect.x;
        node.coord.y = region.rect.y + h;
        node.coord.z = w;

        vector<Tuple3i>::iterator it = _nodes.begin();
        advance(it, best_index);
        it = _nodes.insert(it, node);

        for (it = it + 1, i = best_index + 1; i < _nodes.size(); ++i) {
            node = *it;
            prev = *(it - 1);

            if (node.coord.x < (prev.coord.x + prev.coord.z)) {
                int shrink = prev.coord.x +
                    prev.coord.z - node.coord.x;
                node.coord.x += shrink;
                node.coord.z -= shrink;
                if (node.coord.z <= 0) {
                    it = _nodes.erase(it);
                    --i;
                } else {
                    break;
                }
            } else {
                break;
            }
        }

        merge();
        _used += w * h;

        return region;
    }

    void TextureAtlas::clear (void)
    {
        Tuple3i node(1, 1, 1);

        assert(_data);

        _nodes.clear();
        _used = 0;
        node.coord.z = _width - 2;

        _nodes.push_back(node);
        memset(_data, 0, _width * _height * _depth);
    }

    void TextureAtlas::upload (void)
    {
        assert(_data);

        if (!_id) {
            glGenTextures(1, &_id);
        }

        glBindTexture( GL_TEXTURE_2D, _id);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if (_depth == 4) {
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0,
                          GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, _data);
#else
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _width, _height,
                          0, GL_RGBA, GL_UNSIGNED_BYTE, _data );
#endif
        } else if (_depth == 3) {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB,
                          GL_UNSIGNED_BYTE, _data);
        } else {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, _width, _height, 0,
                          GL_ALPHA, GL_UNSIGNED_BYTE, _data);
        }
    }

    int TextureAtlas::fit (const size_t index, const size_t w, const size_t h)
    {
        Tuple3i node;
        int x, y, width_left;
        size_t i;

        node = _nodes[index];
        x = node.coord.x;
        y = node.coord.y;
        width_left = w;
        i = index;

        if ((x + w) > (_width - 1))
            return -1;

        while (width_left > 0) {
            node = _nodes[i];
            if (node.coord.y > y) {
                y = node.coord.y;
            }
            if ((y + h) > (_height - 1))
                return -1;
            width_left -= node.coord.z;
            ++i;
        }
        return y;
    }

    void TextureAtlas::merge (void)
    {
        Tuple3i node, next;
        size_t i;
        vector<Tuple3i>::iterator it;

        for (it = _nodes.begin(), i = 0; i < _nodes.size() - 1; ++i) {
            // TODO: check the following lines are correct
            node = *it;
            next = *(it + 1);
            if (node.coord.y == next.coord.y) {
                node.coord.z += next.coord.z;
                it = _nodes.erase(it + 1);
                --i;
            } else {
                it++;
            }
        }
    }

} /* namespace BIL */
