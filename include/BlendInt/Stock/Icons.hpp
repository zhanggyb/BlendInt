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

		enum IconType {

			/* SEQUENCE / IMAGE EDITOR */
			SEQ_SEQUENCER,
			SEQ_PREVIEW,
			SEQ_LUMA_WAVEFORM,
			SEQ_CHROMA_SCOPE,
			SEQ_HISTOGRAM,
			SEQ_SPLITVIEW,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK_A_7,
				BLANK_A_8,
				BLANK_A_9,
			#endif
			IMAGE_RGB,	// XXX CHANGE TO STRAIGHT ALPHA, Z ETC
			IMAGE_RGB_ALPHA,
			IMAGE_ALPHA,
			IMAGE_ZDEPTH,
			IMAGEFILE,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK_A_15,
				BLANK_A_16,
				BLANK_A_17,
				BLANK_A_18,
				BLANK_A_19,
				BLANK_A_20,
				BLANK_A_21,
				BLANK_A_22,
				BLANK_A_23,
				BLANK_A_24,
				BLANK_A_25,
				BLANK_A_26,
			#endif

			/* SHADING / TEXT */
			MATPLANE,
			MATSPHERE,
			MATCUBE,
			MONKEY,
			HAIR,
			ALIASED,
			ANTIALIASED,
			MAT_SPHERE_SKY,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK_B_9,
				BLANK_B_10,
				BLANK_B_11,
				BLANK_B_12,
			#endif
			WORDWRAP_OFF,
			WORDWRAP_ON,
			SYNTAX_OFF,
			SYNTAX_ON,
			LINENUMBERS_OFF,
			LINENUMBERS_ON,
			SCRIPTPLUGINS,		// XXX CREATE NEW
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK_B_20,
				BLANK_B_21,
				BLANK_B_22,
				BLANK_B_23,
				BLANK_B_24,
				BLANK_B_25,
				BLANK_B_26,
			#endif


			/* FILE SELECT */
			RECOVER_AUTO,
			SAVE_PREFS,
			LINK_BLEND,
			APPEND_BLEND,
			IMPORT,
			EXPORT,
			EXTERNAL_DATA,
			LOAD_FACTORY,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK_C_9,
				BLANK_C_10,
				BLANK_C_11,
				BLANK_C_12,
				BLANK_C_13,
			#endif
			LOOP_BACK,
			LOOP_FORWARDS,
			BACK,
			FORWARD,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK_C_18,
				BLANK_C_19,
				BLANK_C_20,
				BLANK_C_21,
				BLANK_C_22,
				BLANK_C_23,
				BLANK_C_24,
			#endif
			FILE_BACKUP,
			DISK_DRIVE,


			/* FILE SELECT */
			SORTALPHA,
			SORTBYEXT,
			SORTTIME,
			SORTSIZE,
			LONGDISPLAY,
			SHORTDISPLAY,
			GHOST,
			IMGDISPLAY,
			SAVE_AS,
			SAVE_COPY,
			BOOKMARKS,
			FONTPREVIEW,
			FILTER,
			NEWFOLDER,
			OPEN_RECENT,
			FILE_PARENT,
			FILE_REFRESH,
			FILE_FOLDER,
			FILE_BLANK,
			FILE_BLEND,
			FILE_IMAGE,
			FILE_MOVIE,
			FILE_SCRIPT,
			FILE_SOUND,
			FILE_FONT,
			FILE_TEXT,

			// -------------------------------

			NONE,
			QUESTION,
			ERROR,
			CANCEL,
			TRIA_RIGHT,
			TRIA_DOWN,
			TRIA_LEFT,
			TRIA_UP,
			ARROW_LEFTRIGHT,
			PLUS,
			DISCLOSURE_TRI_DOWN,
			DISCLOSURE_TRI_RIGHT,
			RADIOBUT_OFF,
			RADIOBUT_ON,
			MENU_PANEL,
			BLENDER,
			GRIP,
			DOT,
			COLLAPSEMENU,
			X,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK005,
			#endif
			GO_LEFT,
			PLUG,
			UI,
			NODE,
			NODE_SEL,

				/* ui */
			FULLSCREEN,
			SPLITSCREEN,
			RIGHTARROW_THIN,
			BORDERMOVE,
			VIEWZOOM,
			ZOOMIN,
			ZOOMOUT,
			PANEL_CLOSE,
			COPY_ID,
			EYEDROPPER,
			LINK_AREA,
			AUTO,
			CHECKBOX_DEHLT,
			CHECKBOX_HLT,
			UNLOCKED,
			LOCKED,
			UNPINNED,
			PINNED,
			SCREEN_BACK,
			RIGHTARROW,
			DOWNARROW_HLT,
			DOTSUP,
			DOTSDOWN,
			LINK,
			INLINK,
			PLUGIN,

			/* various ui */
			HELP,
			GHOST_ENABLED,
			COLOR,  /* see COLOR_RED/GREEN/BLUE */
			LINKED,
			UNLINKED,
			HAND,
			ZOOM_ALL,
			ZOOM_SELECTED,
			ZOOM_PREVIOUS,
			ZOOM_IN,
			ZOOM_OUT,
			RENDER_REGION,
			BORDER_RECT,
			BORDER_LASSO,
			FREEZE,
			STYLUS_PRESSURE,
			GHOST_DISABLED,
			NEW,
			FILE_TICK,
			QUIT,
			URL,
			RECOVER_LAST,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK038,
			#endif
			FULLSCREEN_ENTER,
			FULLSCREEN_EXIT,
			BLANK1,	// Not actually blank - this is used all over the place

			/* BUTTONS */
			LAMP,
			MATERIAL,
			TEXTURE,
			ANIM,
			WORLD,
			SCENE,
			EDIT,
			GAME,
			RADIO,
			SCRIPT,
			PARTICLES,
			PHYSICS,
			SPEAKER,
			TEXTURE_SHADED,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK042,
				BLANK043,
				BLANK044,
				BLANK045,
				BLANK046,
				BLANK047,
				BLANK048,
				BLANK049,
				BLANK050,
				BLANK051,
				BLANK052,
				BLANK052b,
			#endif
				/* EDITORS */
			VIEW3D,
			IPO,
			OOPS,
			BUTS,
			FILESEL,
			IMAGE_COL,
			INFO,
			SEQUENCE,
			TEXT,
			IMASEL,
			SOUND,
			ACTION,
			NLA,
			SCRIPTWIN,
			TIME,
			NODETREE,
			LOGIC,
			CONSOLE,
			PREFERENCES,
			CLIP,
			ASSET_MANAGER,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK057,
				BLANK058,
				BLANK059,
				BLANK060,
				BLANK061,
			#endif

				/* MODES */
			OBJECT_DATAMODE,	// XXX fix this up
			EDITMODE_HLT,
			FACESEL_HLT,
			VPAINT_HLT,
			TPAINT_HLT,
			WPAINT_HLT,
			SCULPTMODE_HLT,
			POSE_HLT,
			PARTICLEMODE,
			LIGHTPAINT,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK063,
				BLANK064,
				BLANK065,
				BLANK066,
				BLANK067,
				BLANK068,
				BLANK069,
				BLANK070,
				BLANK071,
				BLANK072,
				BLANK073,
				BLANK074,
				BLANK075,
				BLANK076,
				BLANK077,
				BLANK077b,
			#endif

				/* DATA */
			SCENE_DATA,
			RENDERLAYERS,
			WORLD_DATA,
			OBJECT_DATA,
			MESH_DATA,
			CURVE_DATA,
			META_DATA,
			LATTICE_DATA,
			LAMP_DATA,
			MATERIAL_DATA,
			TEXTURE_DATA,
			ANIM_DATA,
			CAMERA_DATA,
			PARTICLE_DATA,
			LIBRARY_DATA_DIRECT,
			GROUP,
			ARMATURE_DATA,
			POSE_DATA,
			BONE_DATA,
			CONSTRAINT,
			SHAPEKEY_DATA,
			CONSTRAINT_BONE,
			CAMERA_STEREO,
			PACKAGE,
			UGLYPACKAGE,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK079b,
			#endif

				/* DATA */
			BRUSH_DATA,
			IMAGE_DATA,
			FILE,
			FCURVE,
			FONT_DATA,
			RENDER_RESULT,
			SURFACE_DATA,
			EMPTY_DATA,
			SETTINGS,
			RENDER_ANIMATION,
			RENDER_STILL,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK080F,
			#endif
			BOIDS,
			STRANDS,
			LIBRARY_DATA_INDIRECT,
			GREASEPENCIL,
			LINE_DATA,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK084,
			#endif
			GROUP_BONE,
			GROUP_VERTEX,
			GROUP_VCOL,
			GROUP_UVS,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK089,
				BLANK090,
			#endif
			RNA,
			RNA_ADD,

				/* available */
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK092,
				BLANK093,
				BLANK094,
				BLANK095,
				BLANK096,
				BLANK097,
				BLANK098,
				BLANK099,
				BLANK100,
				BLANK101,
				BLANK102,
				BLANK103,
				BLANK104,
				BLANK105,
				BLANK106,
				BLANK107,
				BLANK108,
				BLANK109,
				BLANK110,
				BLANK111,
				BLANK112,
				BLANK113,
				BLANK114,
				BLANK115,
				BLANK116,
				BLANK116b,
			#endif

				/* OUTLINER */
			OUTLINER_OB_EMPTY,
			OUTLINER_OB_MESH,
			OUTLINER_OB_CURVE,
			OUTLINER_OB_LATTICE,
			OUTLINER_OB_META,
			OUTLINER_OB_LAMP,
			OUTLINER_OB_CAMERA,
			OUTLINER_OB_ARMATURE,
			OUTLINER_OB_FONT,
			OUTLINER_OB_SURFACE,
			OUTLINER_OB_SPEAKER,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK120,
				BLANK121,
				BLANK122,
				BLANK123,
				BLANK124,
				BLANK125,
				BLANK126,
				BLANK127,
			#endif
			RESTRICT_VIEW_OFF,
			RESTRICT_VIEW_ON,
			RESTRICT_SELECT_OFF,
			RESTRICT_SELECT_ON,
			RESTRICT_RENDER_OFF,
			RESTRICT_RENDER_ON,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK127b,
			#endif

				/* OUTLINER */
			OUTLINER_DATA_EMPTY,
			OUTLINER_DATA_MESH,
			OUTLINER_DATA_CURVE,
			OUTLINER_DATA_LATTICE,
			OUTLINER_DATA_META,
			OUTLINER_DATA_LAMP,
			OUTLINER_DATA_CAMERA,
			OUTLINER_DATA_ARMATURE,
			OUTLINER_DATA_FONT,
			OUTLINER_DATA_SURFACE,
			OUTLINER_DATA_SPEAKER,
			OUTLINER_DATA_POSE,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK130,
				BLANK131,
				BLANK132,
				BLANK133,
				BLANK134,
				BLANK135,
				BLANK136,
				BLANK137,
				BLANK138,
				BLANK139,
				BLANK140,
				BLANK141,
				BLANK142,
				BLANK142b,
			#endif

				/* PRIMITIVES */
			MESH_PLANE,
			MESH_CUBE,
			MESH_CIRCLE,
			MESH_UVSPHERE,
			MESH_ICOSPHERE,
			MESH_GRID,
			MESH_MONKEY,
			MESH_CYLINDER,
			MESH_TORUS,
			MESH_CONE,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK610,
				BLANK611,
			#endif
			LAMP_POINT,
			LAMP_SUN,
			LAMP_SPOT,
			LAMP_HEMI,
			LAMP_AREA,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK617,
				BLANK618,
			#endif
			META_EMPTY,
			META_PLANE,
			META_CUBE,
			META_BALL,
			META_ELLIPSOID,
			META_CAPSULE,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK625,
			#endif

				/* PRIMITIVES */
			SURFACE_NCURVE,
			SURFACE_NCIRCLE,
			SURFACE_NSURFACE,
			SURFACE_NCYLINDER,
			SURFACE_NSPHERE,
			SURFACE_NTORUS,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK636,
				BLANK637,
				BLANK638,
			#endif
			CURVE_BEZCURVE,
			CURVE_BEZCIRCLE,
			CURVE_NCURVE,
			CURVE_NCIRCLE,
			CURVE_PATH,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK644,
				BLANK645,
				BLANK646,
				BLANK647,
				BLANK648,
			#endif
			COLOR_RED,
			COLOR_GREEN,
			COLOR_BLUE,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK652,
				BLANK653,
				BLANK654,
				BLANK655,
			#endif

				/* EMPTY */
			FORCE_FORCE,
			FORCE_WIND,
			FORCE_VORTEX,
			FORCE_MAGNETIC,
			FORCE_HARMONIC,
			FORCE_CHARGE,
			FORCE_LENNARDJONES,
			FORCE_TEXTURE,
			FORCE_CURVE,
			FORCE_BOID,
			FORCE_TURBULENCE,
			FORCE_DRAG,
			FORCE_SMOKEFLOW,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK673,
				BLANK674,
				BLANK675,
				BLANK676,
				BLANK677,
				BLANK678,
				BLANK679,
				BLANK680,
				BLANK681,
				BLANK682,
				BLANK683,
				BLANK684,
				BLANK685,

				/* EMPTY */
				BLANK690,
				BLANK691,
				BLANK692,
				BLANK693,
				BLANK694,
				BLANK695,
				BLANK696,
				BLANK697,
				BLANK698,
				BLANK699,
				BLANK700,
				BLANK701,
				BLANK702,
				BLANK703,
				BLANK704,
				BLANK705,
				BLANK706,
				BLANK707,
				BLANK708,
				BLANK709,
				BLANK710,
				BLANK711,
				BLANK712,
				BLANK713,
				BLANK714,
				BLANK715,

				/* EMPTY */
				BLANK720,
				BLANK721,
				BLANK722,
				BLANK733,
				BLANK734,
				BLANK735,
				BLANK736,
				BLANK737,
				BLANK738,
				BLANK739,
				BLANK740,
				BLANK741,
				BLANK742,
				BLANK743,
				BLANK744,
				BLANK745,
				BLANK746,
				BLANK747,
				BLANK748,
				BLANK749,
				BLANK750,
				BLANK751,
				BLANK752,
				BLANK753,
				BLANK754,
				BLANK755,

				/* EMPTY */
				BLANK760,
				BLANK761,
				BLANK762,
				BLANK763,
				BLANK764,
				BLANK765,
				BLANK766,
				BLANK767,
				BLANK768,
				BLANK769,
				BLANK770,
				BLANK771,
				BLANK772,
				BLANK773,
				BLANK774,
				BLANK775,
				BLANK776,
				BLANK777,
				BLANK778,
				BLANK779,
				BLANK780,
				BLANK781,
				BLANK782,
				BLANK783,
				BLANK784,
				BLANK785,
			#endif

				/* MODIFIERS */
			MODIFIER,
			MOD_WAVE,
			MOD_BUILD,
			MOD_DECIM,
			MOD_MIRROR,
			MOD_SOFT,
			MOD_SUBSURF,
			HOOK,
			MOD_PHYSICS,
			MOD_PARTICLES,
			MOD_BOOLEAN,
			MOD_EDGESPLIT,
			MOD_ARRAY,
			MOD_UVPROJECT,
			MOD_DISPLACE,
			MOD_CURVE,
			MOD_LATTICE,
			CONSTRAINT_DATA,
			MOD_ARMATURE,
			MOD_SHRINKWRAP,
			MOD_CAST,
			MOD_MESHDEFORM,
			MOD_BEVEL,
			MOD_SMOOTH,
			MOD_SIMPLEDEFORM,
			MOD_MASK,

				/* MODIFIERS */
			MOD_CLOTH,
			MOD_EXPLODE,
			MOD_FLUIDSIM,
			MOD_MULTIRES,
			MOD_SMOKE,
			MOD_SOLIDIFY,
			MOD_SCREW,
			MOD_VERTEX_WEIGHT,
			MOD_DYNAMICPAINT,
			MOD_REMESH,
			MOD_OCEAN,
			MOD_WARP,
			MOD_SKIN,
			MOD_TRIANGULATE,
			MOD_WIREFRAME,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK167,
				BLANK168,
				BLANK169,
				BLANK170,
				BLANK171,
				BLANK172,
				BLANK173,
				BLANK174,
				BLANK175,
				BLANK176,
				BLANK177,
			#endif

				/* ANIMATION */
			REC,
			PLAY,
			FF,
			REW,
			PAUSE,
			PREV_KEYFRAME,
			NEXT_KEYFRAME,
			PLAY_AUDIO,
			PLAY_REVERSE,
			PREVIEW_RANGE,
			ACTION_TWEAK,
			PMARKER_ACT,
			PMARKER_SEL,
			PMARKER,
			MARKER_HLT,
			MARKER,
			SPACE2,	// XXX
			SPACE3,	// XXX
			KEYINGSET,
			KEY_DEHLT,
			KEY_HLT,
			MUTE_IPO_OFF,
			MUTE_IPO_ON,
			VISIBLE_IPO_OFF,
			VISIBLE_IPO_ON,
			DRIVER,

				/* ANIMATION */
			SOLO_OFF,
			SOLO_ON,
			FRAME_PREV,
			FRAME_NEXT,
			NLA_PUSHDOWN,
			IPO_CONSTANT,
			IPO_LINEAR,
			IPO_BEZIER,
			IPO_SINE,
			IPO_QUAD,
			IPO_CUBIC,
			IPO_QUART,
			IPO_QUINT,
			IPO_EXPO,
			IPO_CIRC,
			IPO_BOUNCE,
			IPO_ELASTIC,
			IPO_BACK,
			IPO_EASE_IN,
			IPO_EASE_OUT,
			IPO_EASE_IN_OUT,
			#ifndef DEF_ICON_BLANK_SKIP
				/* available */
				BLANK203,
				BLANK204,
				BLANK205,
				BLANK206,
				BLANK207,
			#endif

				/* EDITING */
			VERTEXSEL,
			EDGESEL,
			FACESEL,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK209,
				BLANK210,
			#endif
			ROTATE,
			CURSOR,
			ROTATECOLLECTION,
			ROTATECENTER,
			ROTACTIVE,
			ALIGN,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK211,
			#endif
			SMOOTHCURVE,
			SPHERECURVE,
			ROOTCURVE,
			SHARPCURVE,
			LINCURVE,
			NOCURVE,
			RNDCURVE,
			PROP_OFF,
			PROP_ON,
			PROP_CON,
			SCULPT_DYNTOPO,
			PARTICLE_POINT,
			PARTICLE_TIP,
			PARTICLE_PATH,

				/* EDITING */
			MAN_TRANS,
			MAN_ROT,
			MAN_SCALE,
			MANIPUL,
			SNAP_OFF,
			SNAP_ON,
			SNAP_NORMAL,
			SNAP_INCREMENT,
			SNAP_VERTEX,
			SNAP_EDGE,
			SNAP_FACE,
			SNAP_VOLUME,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK220,
			#endif
			STICKY_UVS_LOC,
			STICKY_UVS_DISABLE,
			STICKY_UVS_VERT,
			CLIPUV_DEHLT,
			CLIPUV_HLT,
			SNAP_PEEL_OBJECT,
			GRID,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK221,
				BLANK222,
				BLANK224,
				BLANK225,
				BLANK226,
				BLANK226b,
			#endif

				/* EDITING */
			PASTEDOWN,
			COPYDOWN,
			PASTEFLIPUP,
			PASTEFLIPDOWN,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK227,
				BLANK228,
				BLANK229,
				BLANK230,
			#endif
			SNAP_SURFACE,
			AUTOMERGE_ON,
			AUTOMERGE_OFF,
			RETOPO,
			UV_VERTEXSEL,
			UV_EDGESEL,
			UV_FACESEL,
			UV_ISLANDSEL,
			UV_SYNC_SELECT,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK240,
				BLANK241,
				BLANK242,
				BLANK243,
				BLANK244,
				BLANK245,
				BLANK246,
				BLANK247,
				BLANK247b,
			#endif

				/* 3D VIEW */
			BBOX,
			WIRE,
			SOLID,
			SMOOTH,
			POTATO,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK248,
			#endif
			ORTHO,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK249,
				BLANK250,
			#endif
			LOCKVIEW_OFF,
			LOCKVIEW_ON,
			#ifndef DEF_ICON_BLANK_SKIP
				BLANK251,
			#endif
			AXIS_SIDE,
			AXIS_FRONT,
			AXIS_TOP,
			NDOF_DOM,
			NDOF_TURN,
			NDOF_FLY,
			NDOF_TRANS,
			LAYER_USED,
			LAYER_ACTIVE,
			#ifndef DEF_ICON_BLANK_SKIP
				/* available */
				BLANK254,
				BLANK255,
				BLANK256,
				BLANK257,
				BLANK257b,
				BLANK258,
				BLANK259,
				BLANK260,
				BLANK261,
				BLANK262,
				BLANK263,
				BLANK264,
				BLANK265,
				BLANK266,
				BLANK267,
				BLANK268,
				BLANK269,
				BLANK270,
				BLANK271,
				BLANK272,
				BLANK273,
				BLANK274,
				BLANK275,
				BLANK276,
				BLANK277,
				BLANK278,
				BLANK279,
				BLANK280,
				BLANK281,
				BLANK282,
				BLANK282b,
			#endif


				/* brush icons */

			BRUSH_ADD,
			BRUSH_BLOB,
			BRUSH_BLUR,
			BRUSH_CLAY,
			BRUSH_CLAY_STRIPS,
			BRUSH_CLONE,
			BRUSH_CREASE,
			BRUSH_DARKEN,
			BRUSH_FILL,
			BRUSH_FLATTEN,
			BRUSH_GRAB,
			BRUSH_INFLATE,
			BRUSH_LAYER,
			BRUSH_LIGHTEN,
			BRUSH_MASK,
			BRUSH_MIX,
			BRUSH_MULTIPLY,
			BRUSH_NUDGE,
			BRUSH_PINCH,
			BRUSH_SCRAPE,
			BRUSH_SCULPT_DRAW,
			BRUSH_SMEAR,
			BRUSH_SMOOTH,
			BRUSH_SNAKE_HOOK,
			BRUSH_SOFTEN,
			BRUSH_SUBTRACT,
			BRUSH_TEXDRAW,
			BRUSH_TEXFILL,
			BRUSH_TEXMASK,
			BRUSH_THUMB,
			BRUSH_ROTATE,
			BRUSH_VERTEXDRAW,

				/* Matcaps */
			MATCAP_01,
			MATCAP_02,
			MATCAP_03,
			MATCAP_04,
			MATCAP_05,
			MATCAP_06,
			MATCAP_07,
			MATCAP_08,
			MATCAP_09,
			MATCAP_10,
			MATCAP_11,
			MATCAP_12,
			MATCAP_13,
			MATCAP_14,
			MATCAP_15,
			MATCAP_16,
			MATCAP_17,
			MATCAP_18,
			MATCAP_19,
			MATCAP_20,
			MATCAP_21,
			MATCAP_22,
			MATCAP_23,
			MATCAP_24,

			/* vector icons, VICO_ prefix added */
			/*
			DEF_VICO(VIEW3D_VEC)
			DEF_VICO(EDIT_VEC)
			DEF_VICO(EDITMODE_VEC_DEHLT)
			DEF_VICO(EDITMODE_VEC_HLT)
			DEF_VICO(DISCLOSURE_TRI_RIGHT_VEC)
			DEF_VICO(DISCLOSURE_TRI_DOWN_VEC)
			DEF_VICO(MOVE_UP_VEC)
			DEF_VICO(MOVE_DOWN_VEC)
			DEF_VICO(X_VEC)
			DEF_VICO(SMALL_TRI_RIGHT_VEC)
			*/

			LAST_ICON = MATCAP_24
		};

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

			const RefPtr<PixelIcon>& icon_16x16 (int index) const
			{
				return icons_16x16_[index];
			}

			const RefPtr<PixelIcon>& icon_32x32 (int index) const
			{
				return icons_32x32_[index];
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

			std::vector<RefPtr<PixelIcon> > icons_16x16_;

			std::vector<RefPtr<PixelIcon> > icons_32x32_;
		};

	}

}

#endif /* _BLENDINT_STOCK_ICONS_HPP_ */
