/* See LICENSE file for copyright and license details. */

#include "push.c"

/* appearance */
static const char *fonts[] = {
	"Cousine:size=8:rgba:rgb"
};
static const char dmenufont[]       = "Cousine:size=8:rgba=rgb";
static const char normbordercolor[] = "#696969";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#eeeeee";
static const char selbordercolor[]  = "#3371b4";
static const char selbgcolor[]      = "#3371b4";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "chromium", "chromium",    NULL,    1 << 1,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "urxvt", NULL };
static const char
	*F1cmd[]  = { "xdotool", "key", "F1", NULL },
	*F2cmd[]  = { "xdotool", "key", "F2", NULL },
	*F3cmd[]  = { "xdotool", "key", "F3", NULL },
	*F4cmd[]  = { "xdotool", "key", "F4", NULL },
	*F5cmd[]  = { "xdotool", "key", "F5", NULL },
	*F6cmd[]  = { "xdotool", "key", "F6", NULL },
	*F7cmd[]  = { "xdotool", "key", "F7", NULL },
	*F8cmd[]  = { "xdotool", "key", "F8", NULL },
	*F9cmd[]  = { "xdotool", "key", "F9", NULL },
	*F10cmd[]  = { "xdotool", "key", "F10", NULL },
	*F11cmd[]  = { "xdotool", "key", "F11", NULL },
	*F12cmd[]  = { "xdotool", "key", "F12", NULL },
	*pgupcmd[]  = { "xdotool", "key", "Prior", NULL },
	*pgdncmd[]  = { "xdotool", "key", "Next", NULL },
	*homecmd[]  = { "xdotool", "key", "Home", NULL },
	*endcmd[]  = { "xdotool", "key", "End", NULL },
	*xsynccmd[]  = { "xsync", NULL },
	*xsyncrcmd[] = { "xsync", "-r", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_x,      spawn,          {.v = xsynccmd } },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          {.v = xsyncrcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ Mod4Mask,                     XK_1,      spawn,          {.v = F1cmd } },
	{ Mod4Mask,                     XK_2,      spawn,          {.v = F2cmd } },
	{ Mod4Mask,                     XK_3,      spawn,          {.v = F3cmd } },
	{ Mod4Mask,                     XK_4,      spawn,          {.v = F4cmd } },
	{ Mod4Mask,                     XK_5,      spawn,          {.v = F5cmd } },
	{ Mod4Mask,                     XK_6,      spawn,          {.v = F6cmd } },
	{ Mod4Mask,                     XK_7,      spawn,          {.v = F7cmd } },
	{ Mod4Mask,                     XK_8,      spawn,          {.v = F8cmd } },
	{ Mod4Mask,                     XK_9,      spawn,          {.v = F9cmd } },
	{ Mod4Mask,                     XK_0,      spawn,          {.v = F10cmd } },
	{ Mod4Mask,                     XK_minus,  spawn,          {.v = F11cmd } },
	{ Mod4Mask,                     XK_equal,  spawn,          {.v = F12cmd } },
	{ Mod4Mask,                     XK_Up,     spawn,          {.v = pgupcmd } },
	{ Mod4Mask,                     XK_Down,   spawn,          {.v = pgdncmd } },
	{ Mod4Mask,                     XK_Left,   spawn,          {.v = homecmd } },
	{ Mod4Mask,                     XK_Right,  spawn,          {.v = endcmd } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        toggleview,     {0} },
	{ ClkTagBar,            0,              Button3,        toggletag,      {0} },
	{ ClkTagBar,            MODKEY,         Button1,        view,           {0} },
	{ ClkTagBar,            MODKEY,         Button3,        tag,            {0} },
};

