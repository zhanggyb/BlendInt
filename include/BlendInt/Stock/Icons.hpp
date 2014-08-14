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

#ifndef _BLENDINT_STOCK_ICONS_HPP_
#define _BLENDINT_STOCK_ICONS_HPP_

#include <BlendInt/Core/RefPtr.hpp>
#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Gui/PixelIcon.hpp>

namespace BlendInt {

	class Interface;

	namespace Stock {

		/**
		 * @brief class for Stock Icons
		 *
		 * MUST initialized in Interface::Initialize()
		 */
		class Icons
		{
		DISALLOW_COPY_AND_ASSIGN(Icons);

		public:

			static Icons* instance;

			const RefPtr<VertexIcon>& menu () const
			{
				return menu_;
			}

			const RefPtr<VertexIcon>& circle () const
			{
				return circle_;
			}

			const RefPtr<VertexIcon>& check () const
			{
				return check_;
			}

			const RefPtr<VertexIcon>& num () const
			{
				return num_;
			}

			const RefPtr<PixelIcon>& seq_sequencer_16x16 () const
			{
				return seq_sequencer_16x16_;
			}

			const RefPtr<PixelIcon>& seq_preview_16x16 () const
			{
				return seq_preview_16x16_;
			}

			const RefPtr<PixelIcon>& seq_luma_waveform_16x16 () const
			{
				return seq_luma_waveform_16x16_;
			}

			const RefPtr<PixelIcon>& seq_chroma_scope_16x16 () const
			{
				return seq_chroma_scope_16x16_;
			}

			const RefPtr<PixelIcon>& seq_histogram_16x16 () const
			{
				return seq_histogram_16x16_;
			}

			const RefPtr<PixelIcon>& seq_splitview_16x16 () const
			{
				return seq_splitview_16x16_;
			}

			const RefPtr<PixelIcon>& seq_sequencer_32x32 () const
			{
				return seq_sequencer_16x16_;
			}

			const RefPtr<PixelIcon>& seq_preview_32x32 () const
			{
				return seq_preview_16x16_;
			}

			const RefPtr<PixelIcon>& seq_luma_waveform_32x32 () const
			{
				return seq_luma_waveform_16x16_;
			}

			const RefPtr<PixelIcon>& seq_chroma_scope_32x32 () const
			{
				return seq_chroma_scope_16x16_;
			}

			const RefPtr<PixelIcon>& seq_histogram_32x32 () const
			{
				return seq_histogram_16x16_;
			}

			const RefPtr<PixelIcon>& seq_splitview_32x32 () const
			{
				return seq_splitview_16x16_;
			}

		private:

			friend class BlendInt::Interface;

			static bool Initialize ();

			static void Release ();

			/**
			 * @brief Default constructor
			 */
			Icons ();

			/**
			 * @brief private destructor
			 */
			~Icons ();

			/**
			 * @brief Create default icons
			 *
			 * Call in constructor
			 */
			void CreateIcons ();

			void CreateVertexIcons ();

			void CreatePixelIcons16x16 ();

			void CreatePixelIcons32x32 ();

			RefPtr<VertexIcon> menu_;
			RefPtr<VertexIcon> circle_;
			RefPtr<VertexIcon> check_;
			RefPtr<VertexIcon> num_;

			// 16 x 16 icons

			// SEQUENCE / IMAGE EDITOR
			RefPtr<PixelIcon> seq_sequencer_16x16_;
			RefPtr<PixelIcon> seq_preview_16x16_;
			RefPtr<PixelIcon> seq_luma_waveform_16x16_;
			RefPtr<PixelIcon> seq_chroma_scope_16x16_;
			RefPtr<PixelIcon> seq_histogram_16x16_;
			RefPtr<PixelIcon> seq_splitview_16x16_;
			// SKIP 3 blank
			RefPtr<PixelIcon> image_rbg_16x16_;
			RefPtr<PixelIcon> image_rbg_alpha_16x16_;
			RefPtr<PixelIcon> image_alpha_16x16_;
			RefPtr<PixelIcon> image_zdepth_16x16_;
			RefPtr<PixelIcon> imagefile_16x16_;

			// 32 x 32 icons

			RefPtr<PixelIcon> seq_sequencer_32x32_;
			RefPtr<PixelIcon> seq_preview_32x32_;
			RefPtr<PixelIcon> seq_luma_waveform_32x32_;
			RefPtr<PixelIcon> seq_chroma_scope_32x32_;
			RefPtr<PixelIcon> seq_histogram_32x32_;
			RefPtr<PixelIcon> seq_splitview_32x32_;
			// SKIP 3 blank
			RefPtr<PixelIcon> image_rbg_32x32_;
			RefPtr<PixelIcon> image_rbg_alpha_32x32_;
			RefPtr<PixelIcon> image_alpha_32x32_;
			RefPtr<PixelIcon> image_zdepth_32x32_;
			RefPtr<PixelIcon> imagefile_32x32_;

		};

	}

}

#endif /* _BLENDINT_STOCK_ICONS_HPP_ */
