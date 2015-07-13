/* See LICENSE file for copyright and license details. */

#include "push.c"

/* appearance */
static const char font[]            = "-*-dina-medium-r-*-*-10-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#4d5057";
static const char normbgcolor[]     = "#1d1f21";
static const char normfgcolor[]     = "#c5c8c6";
static const char selbordercolor[]  = "#969896";
static const char selbgcolor[]      = "#4d5057";
static const char selfgcolor[]      = "#c5c8c6";
static const unsigned int borderpx  = 2;    /* border pixel of windows */
static const unsigned int gappx     = 16;   /* gap pixel between windows */
static const unsigned int snap      = 32;   /* snap pixel */
static const Bool showbar           = True; /* False means no bar */
static const Bool topbar            = True; /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class           instance  title  tags mask  isfloating   monitor */
	{ "URxvt",         NULL,     NULL,  1 << 0,    False,       -1 },
	{ "qutebrowser",   NULL,     NULL,  1 << 1,    False,       -1 },
	{ "schismtracker", NULL,     NULL,  1 << 3,    True,        -1 },
	{ "Allegro",       NULL,     NULL,  1 << 3,    False,       -1 },
	{ "Gimp",          NULL,     NULL,  1 << 3,    False,       -1 },
	{ "mednafen",      NULL,     NULL,  1 << 4,    False,       -1 },
	{ "mpv",           NULL,     NULL,  1 << 4,    False,       -1 },
	{ "MuPDF",         NULL,     NULL,  1 << 5,    False,       -1 },
	{ "libreoffice",   NULL,     NULL,  1 << 5,    False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.5;   /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;     /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY|ControlMask,           KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY,                       KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
	
#define XF86AudioPrev 0x1008ff16
#define XF86AudioPlay 0x1008ff14
#define XF86AudioNext 0x1008ff17
#define XF86AudioLowerVolume 0x1008ff11
#define XF86AudioRaiseVolume 0x1008ff13
#define XF86Sleep 0x1008ff2f

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "urxvtc", NULL };
static const char
	*lowercmd[]  = { "amixer", "-c", "0", "set", "Master", "2dB-", NULL },
	*nextcmd[]   = { "moss", "next", NULL },
	*playcmd[]   = { "moss", "toggle", NULL },
	*prevcmd[]   = { "moss", "prev", NULL },
	*raisecmd[]  = { "amixer", "-c", "0", "set", "Master", "2dB+", NULL },
	*sleepcmd[]  = { "sudo", "pm-suspend", NULL },
	*xsynccmd[]  = { "xsync", NULL },
	*xsyncrcmd[] = { "xsync", "-r", NULL };

static Key keys[] = {
	/* modifier         key        function        argument */
	{ MODKEY,           XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask, XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,           XK_x,      spawn,          {.v = xsynccmd } },
	{ MODKEY|ShiftMask, XK_x,      spawn,          {.v = xsyncrcmd } },
	{ MODKEY,           XK_b,      togglebar,      {0} },
	{ MODKEY,           XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,           XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,           XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,           XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,           XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,           XK_Return, zoom,           {0} },
	{ MODKEY,           XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask, XK_c,      killclient,     {0} },
	{ MODKEY,           XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,           XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,           XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,           XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask, XK_space,  togglefloating, {0} },
	{ MODKEY,           XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,           XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,           XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(            XK_1,                      0)
	TAGKEYS(            XK_2,                      1)
	TAGKEYS(            XK_3,                      2)
	TAGKEYS(            XK_4,                      3)
	TAGKEYS(            XK_5,                      4)
	TAGKEYS(            XK_6,                      5)
	TAGKEYS(            XK_7,                      6)
	TAGKEYS(            XK_8,                      7)
	TAGKEYS(            XK_9,                      8)
	{ MODKEY|ShiftMask, XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask, XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask, XK_k,      pushup,         {0} },
	{ 0, XF86AudioPrev,            spawn,          {.v = prevcmd } },
	{ 0, XF86AudioPlay,            spawn,          {.v = playcmd } },
	{ 0, XF86AudioNext,            spawn,          {.v = nextcmd } },
	{ 0, XF86AudioLowerVolume,     spawn,          {.v = lowercmd } },
	{ 0, XF86AudioRaiseVolume,     spawn,          {.v = raisecmd } },
	{ 0, XF86Sleep,                spawn,          {.v = sleepcmd } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click         event mask  button   function        argument */
	{ ClkLtSymbol,   0,          Button1, setlayout,      {0} },
	{ ClkLtSymbol,   0,          Button3, setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,   0,          Button2, zoom,           {0} },
	{ ClkStatusText, 0,          Button2, spawn,          {.v = termcmd } },
	{ ClkClientWin,  MODKEY,     Button1, movemouse,      {0} },
	{ ClkClientWin,  MODKEY,     Button2, togglefloating, {0} },
	{ ClkClientWin,  MODKEY,     Button3, resizemouse,    {0} },
	{ ClkTagBar,     0,          Button1, view,           {0} },
	{ ClkTagBar,     0,          Button3, toggleview,     {0} },
	{ ClkTagBar,     MODKEY,     Button1, tag,            {0} },
	{ ClkTagBar,     MODKEY,     Button3, toggletag,      {0} },
};

