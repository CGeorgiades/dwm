/* See LICENSE file for copyright and license details. */
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
// This sets how many workspaces there are
#define WORKSPACES 4
static const char *tagnames[] = { "1", "2", "3", "4", "[1]", "[2]", "[3]", "[4]"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating  */
	{ "Gimp",     NULL,       NULL,          -1,           1 },
	{ "java",     NULL,       "Eclipse IDE ",-1,           1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
//	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define MY_TAG(MASK, KEY, TAG) \
		{ MASK,                KEY,      view,           {.ui = TAG} }, \
		{ MASK|ShiftMask,      KEY,      tag,            {.ui = TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static const char *termcmd[]  = { "urxvt", NULL };

void reexec_dwm(const Arg* arg) {
	char* exe[] = {"dwm", 0};
	if (DWM_EXECUTABLE != 0 && strcmp(DWM_EXECUTABLE, "") != 0)
		exe[0] = DWM_EXECUTABLE;
	execvp(exe[0], &exe[0]);
	fprintf(stderr, "dwm re-load failed.\n");
	perror(" failed");
	exit(EXIT_SUCCESS);
}
void toggle_time(const Arg* arg) {
	time_enable ^= 1;
	drawbars();
}
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_grave,  incnmaster,     {.i = +1 } },
	{ Mod1Mask,                     XK_grave,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {-1} },
	{ MODKEY|ShiftMask,             XK_Tab,    viewwsp,        {-1} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_x,      setlayout,      {0} },
	{ MODKEY,                       XK_f,      togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_r,      reexec_dwm,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      toggle_time,    {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,           {0} },
  MY_TAG(MODKEY,                  XK_1,                      0 )
  MY_TAG(MODKEY,                  XK_2,                      1 )
	MY_TAG(MODKEY,                  XK_3,                      2 )
  MY_TAG(MODKEY,                  XK_4,                      3 )
	MY_TAG(Mod1Mask,                XK_1,                      4 )
	MY_TAG(Mod1Mask,                XK_2,                      5 )
	MY_TAG(Mod1Mask,                XK_3,                      6 )
	MY_TAG(Mod1Mask,                XK_4,                      7 )
	{ ControlMask,           XK_1,      tagwsp,         {0}},
	{ ControlMask,           XK_2,      tagwsp,         {1}},
	{ ControlMask,           XK_3,      tagwsp,         {2}},
	{ ControlMask,           XK_4,      tagwsp,         {3}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

