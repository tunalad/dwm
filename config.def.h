// vim: set ts=4 sw=4 noexpandtab:
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx			= 2;			/* border pixel of windows */
static const unsigned int gappx				= 5;			/* gaps size between windows */
static const unsigned int snap				= 32;			/* snap pixel */
static const unsigned int minwsz			= 10;			/* Minimal heigt of a client for smfact */
static const unsigned int systraypinning	= 0;			/* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing	= 2;			/* systray spacing */
static const int systraypinningfailfirst	= 1;			/*	1: if pinning fails, display systray on the first monitor,
																False: display systray on the last monitor */
static const int showsystray				= 1;	 		/* 0 means no systray */
static const int showbar					= 1;			/* 0 means no bar */
static const int topbar						= 1;			/* 0 means bottom bar */
static const char *fonts[]					= { "DejaVu Sans Mono:size=10", "DejaVuSansM Nerd Font Propo:size=10" };
static const char dmenufont[]				= "DejaVu Sans Mono:size=10";
static const char col_gray1[]				= "#222222";	// background color
static const char col_gray2[]				= "#444444";	// inactive window border color
static const char col_gray3[]				= "#bbbbbb";	// font color
static const char col_gray4[]				= "#eeeeee";	// current tag and current window font color
static const char col_cyan[]				= "#005577";	// top bar second color (blue)
static const char col_red[]					= "#770000";	// active window border color
static const char *colors[][4] = {
	/*					fg			bg			border		*/
	[SchemeNorm] = {	col_gray3,	col_gray1,	col_gray2	},
	[SchemeSel]  = {	col_gray4,	col_cyan,	col_red		},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

/* environment vars */
#define TERMINAL_ENVVAR "TERMINAL"

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class 		instance 	title 		tags mask 	isfloating 	monitor */
	{ "Firefox",		NULL, 		NULL,		1 << 8, 	0, 		-1 },
	{ NULL,				NULL, 		"Ferdi",	1 << 9, 	0, 		-1 },
	{ "Steam",			NULL, 		NULL, 		1 << 7, 	1, 		-1 }, // older steam
	{ "steamwebhelper", NULL, 		NULL,		1 << 7, 	0, 		-1 }, // later steam
	{ "Cadence", 		NULL, 		NULL, 		0,			1, 		-1 },
	{ "SpeedCrunch", 	NULL, 		NULL, 		0,			1, 		-1 },
	{ "Lingot", 		NULL,		NULL, 		0,			1, 		-1 },
	{ "ROX-Filer", 		NULL,		NULL, 		0, 			1, 		-1 },
	{ NULL,				"Ripcord",	NULL, 		1 << 9, 	0, 		-1 },
	{ "webamp-desktop", NULL,		NULL, 		0,			1, 		-1 },
	{ "megasync",	   NULL,		NULL, 		0,			1, 		-1 },
	{ "Godot_Engine",   NULL,		NULL, 		0,			1, 		-1 },
	{ "zenity", 		NULL,		NULL, 		0,			1, 		-1 },
};

/* layout(s) */
static const float mfact		= 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact		= 0.00; /* factor of tiled clients [0.00..0.95] */
static const int nmaster		= 1;	/* number of clients in master area */
static const int resizehints	= 1;	/* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/*	symbol		arrange function */
	{	"[]=",		tile		},	/* first entry is default */
	{	"><>",		NULL		},	/* no layout function means floating behavior */
	{	"[M]",		monocle		},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,		view,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,		toggleview,		{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,		tag,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,		toggletag,		{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]			= "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]	= { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *termcmd[]	= { "alacritty", NULL };
static const char *jgmenucmd[]	= { "jgmenu_run", NULL };

#include "src/dwm-movestack.c"
#include "src/dwm-shiftview.c"
static const Key keys[] = {
	/* modifier						key			function			argument */
	{ MODKEY,						XK_r,		spawn,				{.v = dmenucmd } }, // spawn dmenu
	{ MODKEY,						XK_Return,	spawn,				{.v = termcmd} }, // spawn $TERMINAL
	{ MODKEY|ShiftMask,				XK_b,		togglebar,			{0} }, // toggle bar

	// shift focus between windows
	{ MODKEY, 						XK_j,		focusstack,			{.i = +1 } },
	{ MODKEY, 						XK_k,		focusstack,			{.i = -1 } },

	// increase and decrease master stack size
	{ MODKEY,						XK_i,		incnmaster,			{.i = +1 } },
	{ MODKEY,						XK_d,		incnmaster,			{.i = -1 } },

	// resize tiling windows width
	{ MODKEY,						XK_h,		setmfact,			{.f = -0.05} },
	{ MODKEY,						XK_l,		setmfact,			{.f = +0.05} },

	// resize tiling windows height
	{ MODKEY|ShiftMask,				XK_h,		setsmfact,			{.f = +0.05} },
	{ MODKEY|ShiftMask,				XK_l,		setsmfact,			{.f = -0.05} },

	{ MODKEY|ShiftMask,				XK_Return,	zoom,				{0} }, // focused windows moves to master
	{ MODKEY,						XK_Tab,		view,				{0} }, // tab between current and previous tags state
	{ MODKEY,						XK_q,		killclient,			{0} }, // kill focused window
	{ MODKEY,						XK_t,		setlayout,			{.v = &layouts[0]} }, // tiling layout
	{ MODKEY,						XK_f,		setlayout,			{.v = &layouts[1]} }, // float layout
	{ MODKEY,						XK_m,		setlayout,			{.v = &layouts[2]} }, // monocle layout
	{ MODKEY,						XK_space,	setlayout,			{0} }, // toggle between current and previous layout
	//{ MODKEY|ShiftMask,				XK_space,  togglefloatin	g, {0} }, // toggle floating for a window (maybe better than making everything float?)
	{ MODKEY|ShiftMask,				XK_f,		togglefullscr,  	{0} }, // toggle fullscreen mode
	{ MODKEY,						XK_0,		view,				{.ui = ~0 } }, // show all windows
	{ MODKEY|ShiftMask,				XK_0,		tag,				{.ui = ~0 } }, // fix to mod+shift+0 (I think xd)

	// shift focus between monitors and move windows between them
	{ MODKEY,						XK_comma,	focusmon,			{.i = -1 } },
	{ MODKEY,						XK_period,	focusmon,			{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_comma,	tagmon,				{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_period,	tagmon,				{.i = +1 } },

	// shift between tags
	{ MODKEY,						XK_b,		shiftview,			{.i = -1 } },
	{ MODKEY, 		 				XK_n, 	   	shiftview,	  		{.i = +1 } },

	// kill dwm
	{ MODKEY|ShiftMask,				XK_q,		quit,				{0} },

	// moving focused tiling window in the stack
	{ MODKEY|ShiftMask,				XK_j,		movestack,			{.i = +1 } },
	{ MODKEY|ShiftMask,			 	XK_k,	  	movestack,	  		{.i = -1 } },

	// moving and resizing focused floating window
	{ MODKEY,						XK_Down,	moveresize,			{.v = "0x 25y 0w 0h" }	},
	{ MODKEY, 				   		XK_Up,		moveresize,	 		{.v = "0x -25y 0w 0h" }	},
	{ MODKEY, 				   		XK_Right,	moveresize,	 		{.v = "25x 0y 0w 0h" }	},
	{ MODKEY, 				   		XK_Left,	moveresize,	 		{.v = "-25x 0y 0w 0h" }	},
	{ MODKEY|ShiftMask,				XK_Down,	moveresize,	 		{.v = "0x 0y 0w 25h" }	},
	{ MODKEY|ShiftMask,			 	XK_Up,		moveresize,	 		{.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,			 	XK_Right,	moveresize,	 		{.v = "0x 0y 25w 0h" }	},
	{ MODKEY|ShiftMask,			 	XK_Left,	moveresize,	 		{.v = "0x 0y -25w 0h" } },

	// moving and resizing focused floating window to corners
	{ MODKEY|ControlMask,			XK_Up,		moveresizeedge,		{.v = "t"} },
	{ MODKEY|ControlMask,			XK_Down,	moveresizeedge, 	{.v = "b"} },
	{ MODKEY|ControlMask,			XK_Left,	moveresizeedge, 	{.v = "l"} },
	{ MODKEY|ControlMask,			XK_Right,	moveresizeedge, 	{.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,		moveresizeedge, 	{.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,	moveresizeedge, 	{.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,	moveresizeedge, 	{.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,	moveresizeedge, 	{.v = "R"} },

	// pin focused windows to top
	{ MODKEY|ShiftMask,				XK_space,	togglealwaysontop,	{0} },

	// tags
	TAGKEYS(						XK_1,							0)
	TAGKEYS(						XK_2,					  		1)
	TAGKEYS(						XK_3,					  		2)
	TAGKEYS(						XK_4,					  		3)
	TAGKEYS(						XK_5,					  		4)
	TAGKEYS(						XK_6,					  		5)
	TAGKEYS(						XK_7,					  		6)
	TAGKEYS(						XK_8,					  		7)
	TAGKEYS(						XK_9,					  		8)
	TAGKEYS(						XK_0,					  		9)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click				event mask		button			function		argument */
	// tag bar
	{ ClkTagBar,			0,				Button1,		view,			{0} },
	{ ClkTagBar,			0,				Button3,		toggleview,		{0} },
	{ ClkTagBar,			MODKEY,			Button1,		tag,			{0} },
	{ ClkTagBar,			MODKEY,			Button3,		toggletag,		{0} },

	{ ClkTagBar,			0,				Button4,		shiftview,		{.i = -1 } },
	{ ClkTagBar,			0,				Button5,		shiftview,		{.i = +1 } },

	// layout symbol
	{ ClkLtSymbol,			0,				Button1,		setlayout,		{0} },
	{ ClkLtSymbol,			0,				Button3,		setlayout,		{.v = &layouts[2]} },

	// client window (move, toggle float, resize)
	{ ClkClientWin,			MODKEY,			Button1,		movemouse,		{0} },
	{ ClkClientWin,		 	MODKEY, 	 	Button2,		togglefloating,	{0} },
	{ ClkClientWin,		 	MODKEY, 	 	Button3,		resizemouse,	{0} },


	// statusbar
	{ ClkWinTitle,			0,				Button3,		spawn,			{.v = jgmenucmd } },
	//{ ClkStatusText,		0,				Button2,		spawn,		  	{.v = termcmd } },
	//{ ClkWinTitle,			0,			  	Button2,		zoom,			{0} },

	// background/root window
	{ ClkRootWin,			0,				Button3,		spawn,			{.v = jgmenucmd } },

};
