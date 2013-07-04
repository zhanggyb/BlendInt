/*
 * Theme.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_THEME_H_
#define _BIL_THEME_H_

namespace BIL {

	typedef struct uiWidgetColors
	{
		char outline[4];
		char inner[4];
		char inner_sel[4];
		char item[4];
		char text[4];
		char text_sel[4];
		short shaded;
		short shadetop, shadedown;
		short alpha_check;
	} uiWidgetColors;

	typedef struct uiWidgetStateColors
	{
		char inner_anim[4];
		char inner_anim_sel[4];
		char inner_key[4];
		char inner_key_sel[4];
		char inner_driven[4];
		char inner_driven_sel[4];
		float blend, pad;
	} uiWidgetStateColors;

	typedef struct uiPanelColors
	{
		char header[4];
		char back[4];
		short show_header;
		short show_back;
		int pad;
	} uiPanelColors;

	typedef struct uiGradientColors
	{
		char gradient[4];
		char high_gradient[4];
		int show_grad;
		int pad2;
	} uiGradientColors;

	typedef struct ThemeUI
	{
		/* Interface Elements (buttons, menus, icons) */
		uiWidgetColors wcol_regular, wcol_tool, wcol_text;
		uiWidgetColors wcol_radio, wcol_option, wcol_toggle;
		uiWidgetColors wcol_num, wcol_numslider;
		uiWidgetColors wcol_menu, wcol_pulldown, wcol_menu_back, wcol_menu_item,
		        wcol_tooltip;
		uiWidgetColors wcol_box, wcol_scroll, wcol_progress, wcol_list_item;

		uiWidgetStateColors wcol_state;

		uiPanelColors panel; /* depricated, but we keep it for do_versions (2.66.1) */

		/* fac: 0 - 1 for blend factor, width in pixels */
		float menu_shadow_fac;
		short menu_shadow_width;

		short pad;

		char iconfile[256];	// FILE_MAXFILE length
		float icon_alpha;

		/* Axis Colors */
		char xaxis[4], yaxis[4], zaxis[4];
	} ThemeUI;

	/* try to put them all in one, if needed a special struct can be created as well
	 * for example later on, when we introduce wire colors for ob types or so...
	 */
	typedef struct ThemeSpace
	{
		/* main window colors */
		char back[4];
		char title[4]; /* panel title */
		char text[4];
		char text_hi[4];

		/* header colors */
		char header[4]; /* region background */
		char header_title[4]; /* unused */
		char header_text[4];
		char header_text_hi[4];

		/* button/tool regions */
		char button[4]; /* region background */
		char button_title[4]; /* panel title */
		char button_text[4];
		char button_text_hi[4];

		/* listview regions */
		char list[4]; /* region background */
		char list_title[4]; /* panel title */
		char list_text[4];
		char list_text_hi[4];

		/* float panel */
		/*	char panel[4];			unused */
		/*	char panel_title[4];	unused */
		/*	char panel_text[4];		unused */
		/*	char panel_text_hi[4];	unused */

		/* note, cannot use name 'panel' because of DNA mapping old files */
		uiPanelColors panelcolors;

		uiGradientColors gradients;

		char shade1[4];
		char shade2[4];

		char hilite[4];
		char grid[4];

		char wire[4], wire_edit[4], select[4];
		char lamp[4], speaker[4], empty[4], camera[4], pad[4];
		char active[4], group[4], group_active[4], transform[4];
		char vertex[4], vertex_select[4], vertex_unreferenced[4];
		char edge[4], edge_select[4];
		char edge_seam[4], edge_sharp[4], edge_facesel[4], edge_crease[4];
		char face[4], face_select[4]; /* solid faces */
		char face_dot[4]; /*  selected color */
		char extra_edge_len[4], extra_edge_angle[4], extra_face_angle[4],
		        extra_face_area[4];
		char normal[4];
		char vertex_normal[4];
		char bone_solid[4], bone_pose[4], bone_pose_active[4];
		char strip[4], strip_select[4];
		char cframe[4];
		char freestyle_edge_mark[4], freestyle_face_mark[4];

		char nurb_uline[4], nurb_vline[4];
		char act_spline[4], nurb_sel_uline[4], nurb_sel_vline[4],
		        lastsel_point[4];

		char handle_free[4], handle_auto[4], handle_vect[4], handle_align[4],
		        handle_auto_clamped[4];
		char handle_sel_free[4], handle_sel_auto[4], handle_sel_vect[4],
		        handle_sel_align[4], handle_sel_auto_clamped[4];

		char ds_channel[4], ds_subchannel[4]; /* dopesheet */

		char console_output[4], console_input[4], console_info[4],
		        console_error[4];
		char console_cursor[4], console_select[4], pad1[4];

		char vertex_size, outline_width, facedot_size;
		char noodle_curving;

		/* syntax for textwindow and nodes */
		char syntaxl[4], syntaxs[4];
		char syntaxb[4], syntaxn[4];
		char syntaxv[4], syntaxc[4];
		char syntaxd[4], syntaxr[4];

		char movie[4], movieclip[4], mask[4], image[4], scene[4], audio[4]; /* for sequence editor */
		char effect[4], transition[4], meta[4];
		char editmesh_active[4];

		char handle_vertex[4];
		char handle_vertex_select[4];
		char pad2[4];

		char handle_vertex_size;

		char marker_outline[4], marker[4], act_marker[4], sel_marker[4],
		        dis_marker[4], lock_marker[4];
		char bundle_solid[4];
		char path_before[4], path_after[4];
		char camera_path[4];
		char hpad[3];

		char preview_back[4];
		char preview_stitch_face[4];
		char preview_stitch_edge[4];
		char preview_stitch_vert[4];
		char preview_stitch_stitchable[4];
		char preview_stitch_unstitchable[4];
		char preview_stitch_active[4];

		char match[4]; /* outliner - filter match */
		char selected_highlight[4]; /* outliner - selected item */

		char skin_root[4]; /* Skin modifier root color */

		/* NLA */
		char anim_active[4]; /* Active Action + Summary Channel */
		char anim_non_active[4]; /* Active Action = NULL */

		char nla_tweaking[4]; /* NLA 'Tweaking' action/strip */
		char nla_tweakdupli[4]; /* NLA - warning color for duplicate instances of tweaking strip */

		char nla_transition[4], nla_transition_sel[4]; /* NLA "Transition" strips */
		char nla_meta[4], nla_meta_sel[4]; /* NLA "Meta" strips */
		char nla_sound[4], nla_sound_sel[4]; /* NLA "Sound" strips */
	} ThemeSpace;

	/* set of colors for use as a custom color set for Objects/Bones wire drawing */
	typedef struct ThemeWireColor
	{
		char solid[4];
		char select[4];
		char active[4];

		short flag;
		short pad;
	} ThemeWireColor;

	/* A theme */
	typedef struct bTheme
	{
		struct bTheme *next, *prev;
		char name[32];

		ThemeUI tui;

		/* Individual Spacetypes */
		ThemeSpace tbuts;
		ThemeSpace tv3d;
		ThemeSpace tfile;
		ThemeSpace tipo;
		ThemeSpace tinfo;
		ThemeSpace tact;
		ThemeSpace tnla;
		ThemeSpace tseq;
		ThemeSpace tima;
		ThemeSpace text;
		ThemeSpace toops;
		ThemeSpace ttime;
		ThemeSpace tnode;
		ThemeSpace tlogic;
		ThemeSpace tuserpref;
		ThemeSpace tconsole;
		ThemeSpace tclip;

		/* 20 sets of bone colors for this theme */
		ThemeWireColor tarm[20];
		/*ThemeWireColor tobj[20];*/

		int active_theme_area, pad;
	} bTheme;

	class Theme
	{
	public:
		Theme ();
		virtual ~Theme ();
	};

} /* namespace BIL */
#endif /* THEME_H_ */
