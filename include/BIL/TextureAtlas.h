/*
 * TextureAtlas.h
 *
 *  Created on: 2013年7月15日
 *      Author: zhanggyb
 */

/* Modified from freetype-gl project */

#ifndef _BIL_TEXTUREATLAS_H_
#define _BIL_TEXTUREATLAS_H_

#include <vector>
#include <stdlib.h>

#include <BIL/Tuple.h>

using namespace std;

namespace BIL {

	class TextureAtlas
	{
	public:

		/**
		 * @brief Constructor
		 * @param w width of the atlas
		 * @param h height of the atlas
		 * @param d bit depth of the atlas
		 *
		 * Create a new empty texture atlas.
		 */
		TextureAtlas (const size_t w, const size_t h, const size_t d);

		/**
		 * @brief destructor
		 */
		virtual ~TextureAtlas ();

		/**
		 * @brief Upload atlas to video memory
		 */
		void upload (void);

		/**
		 * @brief Allocate a new region in the atlas
		 * @param w width of the region to allocate
		 * @param h height of the region to allocate
		 * @return Coordinates of the allocated region
		 */
		Tuple4i getRegion (const size_t w, const size_t h);

		// void setRegion
		/**
		 * @brief Upload data to the specified atlas region
		 * @param x
		 */
		void setRegion (const size_t x, const size_t y, const size_t w,
		        const size_t h, const unsigned char* data, const size_t stride);

		/**
		 * @brief Remove all allocated regions from the atlas.
		 */
		void clear (void);

	private:

		int fit (const size_t index, const size_t w, const size_t h);

		void merge (void);

		/** Allocated nodes */
		vector<Tuple3i> _nodes;

		/** Width (in pixels) of the underlying texture */
		size_t _width;

		/** Height (in pixels) of the underlying texture */
		size_t _height;

		/** Depth (in bytes) of the underlying texture */
		size_t _depth;

		/** Allocated surface size */
		size_t _used;

		/** Texture identity (OpenGL) */
		unsigned int _id;

		/** Atlas data */
		unsigned char* _data;
	};

} /* namespace BIL */
#endif /* TEXTUREATLAS_H_ */
