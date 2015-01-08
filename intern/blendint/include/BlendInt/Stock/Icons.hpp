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
#include <BlendInt/Gui/VectorIcon.hpp>
#include <BlendInt/Gui/PixelIcon.hpp>

namespace BlendInt {

	/**
	 * @brief Stock Icons
	 *
	 * An instance of Icons is created in Context::Initialize(), use
	 * the static Context::icons to access stock vector and pixel
	 * icons stored in this class.
	 */
	class Icons
	{
		DISALLOW_COPY_AND_ASSIGN(Icons);

	public:

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

#ifndef DEF_ICON_BLANK_SKIP
			/* available */
			BLANK_E_1,
			BLANK_E_2,
			BLANK_E_3,
			BLANK_E_4,
			BLANK_E_5,
			BLANK_E_6,
			BLANK_E_7,
			BLANK_E_8,
			BLANK_E_9,
			BLANK_E_10,
			BLANK_E_11,
			BLANK_E_12,
			BLANK_E_13,
			BLANK_E_14,
			BLANK_E_15,
			BLANK_E_16,
			BLANK_E_17,
			BLANK_E_18,
			BLANK_E_19,
			BLANK_E_20,
			BLANK_E_21,
			BLANK_E_22,
			BLANK_E_23,
			BLANK_E_24,
			BLANK_E_25,
			BLANK_E_26,
#endif

			/* 3D VIEW */
			BBOX,
			WIRE,
			SOLID,
			SMOOTH,
			POTATO,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_F_6,
#endif
			ORTHO,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_F_8,
			BLANK_F_9,
#endif
			LOCKVIEW_OFF,
			LOCKVIEW_ON,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_F_12,
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
			BLANK_F_22,
			BLANK_F_23,
			BLANK_F_24,
			BLANK_F_25,
			BLANK_F_26,
#endif

			/* EDITING */
			PASTEDOWN,
			COPYDOWN,
			PASTEFLIPUP,
			PASTEFLIPDOWN,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_G_5,
			BLANK_G_6,
			BLANK_G_7,
			BLANK_G_8,
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
			BLANK_G_18,
			BLANK_G_19,
			BLANK_G_20,
			BLANK_G_21,
			BLANK_G_22,
			BLANK_G_23,
			BLANK_G_24,
			BLANK_G_25,
			BLANK_G_26,
#endif

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
			BLANK_H_13,
#endif
			STICKY_UVS_LOC,
			STICKY_UVS_DISABLE,
			STICKY_UVS_VERT,
			CLIPUV_DEHLT,
			CLIPUV_HLT,
			SNAP_PEEL_OBJECT,
			GRID,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_H_21,
			BLANK_H_22,
			BLANK_H_23,
			BLANK_H_24,
			BLANK_H_25,
			BLANK_H_26,
#endif

			/* EDITING */
			VERTEXSEL,
			EDGESEL,
			FACESEL,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_I_1,
			BLANK_I_2,
#endif
			ROTATE,
			CURSOR,
			ROTATECOLLECTION,
			ROTATECENTER,
			ROTACTIVE,
			ALIGN,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_I_12,
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
			BLANK_J_22,
			BLANK_J_23,
			BLANK_J_24,
			BLANK_J_25,
			BLANK_J_26,
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
			BLANK_L_16,
			BLANK_L_17,
			BLANK_L_18,
			BLANK_L_19,
			BLANK_L_20,
			BLANK_L_21,
			BLANK_L_22,
			BLANK_L_23,
			BLANK_L_24,
			BLANK_L_25,
			BLANK_L_26,
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

			/* EMPTY */
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_N_1,
			BLANK_N_2,
			BLANK_N_3,
			BLANK_N_4,
			BLANK_N_5,
			BLANK_N_6,
			BLANK_N_7,
			BLANK_N_8,
			BLANK_N_9,
			BLANK_N_10,
			BLANK_N_11,
			BLANK_N_12,
			BLANK_N_13,
			BLANK_N_14,
			BLANK_N_15,
			BLANK_N_16,
			BLANK_N_17,
			BLANK_N_18,
			BLANK_N_19,
			BLANK_N_20,
			BLANK_N_21,
			BLANK_N_22,
			BLANK_N_23,
			BLANK_N_24,
			BLANK_N_25,
			BLANK_N_26,

			BLANK_O_1,
			BLANK_O_2,
			BLANK_O_3,
			BLANK_O_4,
			BLANK_O_5,
			BLANK_O_6,
			BLANK_O_7,
			BLANK_O_8,
			BLANK_O_9,
			BLANK_O_10,
			BLANK_O_11,
			BLANK_O_12,
			BLANK_O_13,
			BLANK_O_14,
			BLANK_O_15,
			BLANK_O_16,
			BLANK_O_17,
			BLANK_O_18,
			BLANK_O_19,
			BLANK_O_20,
			BLANK_O_21,
			BLANK_O_22,
			BLANK_O_23,
			BLANK_O_24,
			BLANK_O_25,
			BLANK_O_26,

#endif

			/* */
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_P_1,	// TODO: Icon name?
			BLANK_P_2,	// TODO: Icon name?
			BLANK_P_3,	// TODO: Icon name?
			BLANK_P_4,
			BLANK_P_5,
			BLANK_P_6,
			BLANK_P_7,
			BLANK_P_8,
			BLANK_P_9,
			BLANK_P_10,
			BLANK_P_11,
			BLANK_P_12,
			BLANK_P_13,
			BLANK_P_14,
			BLANK_P_15,
			BLANK_P_16,
			BLANK_P_17,
			BLANK_P_18,
			BLANK_P_19,
			BLANK_P_20,
			BLANK_P_21,
			BLANK_P_22,
			BLANK_P_23,
			BLANK_P_24,
			BLANK_P_25,
			BLANK_P_26,
#endif

			/* FORCE */
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
			BLANK_Q_14,
			BLANK_Q_15,
			BLANK_Q_16,
			BLANK_Q_17,
			BLANK_Q_18,
			BLANK_Q_19,
			BLANK_Q_20,
			BLANK_Q_21,
			BLANK_Q_22,
			BLANK_Q_23,
			BLANK_Q_24,
			BLANK_Q_25,
			BLANK_Q_26,
#endif

			/* PRIMITIVES */
			SURFACE_NCURVE,
			SURFACE_NCIRCLE,
			SURFACE_NSURFACE,
			SURFACE_NCYLINDER,
			SURFACE_NSPHERE,
			SURFACE_NTORUS,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_R_7,
			BLANK_R_8,
			BLANK_R_9,
#endif
			CURVE_BEZCURVE,
			CURVE_BEZCIRCLE,
			CURVE_NCURVE,
			CURVE_NCIRCLE,
			CURVE_PATH,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_R_15,
			BLANK_R_16,
			BLANK_R_17,
			BLANK_R_18,
			BLANK_R_19,
#endif
			COLOR_RED,
			COLOR_GREEN,
			COLOR_BLUE,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_R_23,
			BLANK_R_24,
			BLANK_R_25,
			BLANK_R_26,
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
			BLANK_S_11,
			BLANK_S_12,
#endif
			LAMP_POINT,
			LAMP_SUN,
			LAMP_SPOT,
			LAMP_HEMI,
			LAMP_AREA,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_S_18,
			BLANK_S_19,
#endif
			META_EMPTY,
			META_PLANE,
			META_CUBE,
			META_BALL,
			META_ELLIPSOID,
			META_CAPSULE,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_S_26,
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
			BLANK_T_13,
			BLANK_T_14,
			BLANK_T_15,
			BLANK_T_16,
			BLANK_T_17,
			BLANK_T_18,
			BLANK_T_19,
			BLANK_T_20,
			BLANK_T_21,
			BLANK_T_22,
			BLANK_T_23,
			BLANK_T_24,
			BLANK_T_25,
			BLANK_T_26,
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
			BLANK_U_12,
			BLANK_U_13,
			BLANK_U_14,
			BLANK_U_15,
			BLANK_U_16,
			BLANK_U_17,
			BLANK_U_18,
			BLANK_U_19,
#endif
			RESTRICT_VIEW_OFF,
			RESTRICT_VIEW_ON,
			RESTRICT_SELECT_OFF,
			RESTRICT_SELECT_ON,
			RESTRICT_RENDER_OFF,
			RESTRICT_RENDER_ON,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_U_26,
#endif

			/* available */
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_V_1,
			BLANK_V_2,
			BLANK_V_3,
			BLANK_V_4,
			BLANK_V_5,
			BLANK_V_6,
			BLANK_V_7,
			BLANK_V_8,
			BLANK_V_9,
			BLANK_V_10,
			BLANK_V_11,
			BLANK_V_12,
			BLANK_V_13,
			BLANK_V_14,
			BLANK_V_15,
			BLANK_V_16,
			BLANK_V_17,
			BLANK_V_18,
			BLANK_V_19,
			BLANK_V_20,
			BLANK_V_21,
			BLANK_V_22,
			BLANK_V_23,
			BLANK_V_24,
			BLANK_V_25,
			BLANK_V_26,
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
			BLANK_W_12,
#endif
			BOIDS,
			STRANDS,
			LIBRARY_DATA_INDIRECT,
			GREASEPENCIL,
			LINE_DATA,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_W_18,
#endif
			GROUP_BONE,
			GROUP_VERTEX,
			GROUP_VCOL,
			GROUP_UVS,
#ifndef DEF_ICON_BLANK_SKIP
			BLANK_W_23,
			BLANK_W_24,
#endif
			RNA,
			RNA_ADD,

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
			BLANK_X_26,
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
			BLANK_Y_11,
			BLANK_Y_12,
			BLANK_Y_13,
			BLANK_Y_14,
			BLANK_Y_15,
			BLANK_Y_16,
			BLANK_Y_17,
			BLANK_Y_18,
			BLANK_Y_19,
			BLANK_Y_20,
			BLANK_Y_21,
			BLANK_Y_22,
			BLANK_Y_23,
			BLANK_Y_24,
			BLANK_Y_25,
			BLANK_Y_26,
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
			BLANK_Z_22,
			BLANK_Z_23,
			BLANK_Z_24,
			BLANK_Z_25,
			BLANK_Z_26,
#endif

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
			BLANK_AA_15,
			BLANK_AA_16,
			BLANK_AA_17,
			BLANK_AA_18,
			BLANK_AA_19,
			BLANK_AA_20,
			BLANK_AA_21,
			BLANK_AA_22,
			BLANK_AA_23,
			BLANK_AA_24,
			BLANK_AA_25,
			BLANK_AA_26,
#endif

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
			BLANK_BB_23,
#endif
			FULLSCREEN_ENTER,
			FULLSCREEN_EXIT,
			BLANK1,	// Not actually blank - this is used all over the place

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

			/* ui */
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
			BLANK005,	// TODO: not blank
#endif
			GO_LEFT,
			PLUG,
			UI,
			NODE,
			NODE_SEL,


			// -----------------------------------------------------------


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

		const RefPtr<VectorIcon>& menu () const
		{
			return menu_;
		}

		const RefPtr<VectorIcon>& circle () const
		{
			return circle_;
		}

		const RefPtr<VectorIcon>& check () const
		{
			return check_;
		}

		const RefPtr<VectorIcon>& num () const
		{
			return num_;
		}

		const RefPtr<VectorIcon>& cross () const
		{
			return cross_;
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

		friend class Context;

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

		void CreateVectorIcons ();

		void CreatePixelIcons16x16 ();

		void CreatePixelIcons32x32 ();

		RefPtr<VectorIcon> menu_;
		RefPtr<VectorIcon> circle_;
		RefPtr<VectorIcon> check_;
		RefPtr<VectorIcon> num_;
		RefPtr<VectorIcon> cross_;

		std::vector<RefPtr<PixelIcon> > icons_16x16_;

		std::vector<RefPtr<PixelIcon> > icons_32x32_;
	};

}

#endif /* _BLENDINT_STOCK_ICONS_HPP_ */
