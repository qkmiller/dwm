/* See LICENSE file for copyright and license details. */

// Custom function to reset the number of windows allowed in the master area
static void resetnmaster();
void
resetnmaster()
{
	selmon->nmaster = 1;
	arrange(selmon);
}

/* Appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Liberation Sans:size=10" };
static const char dmenufont[]       = "Liberation Sans:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_black[]       = "#000000";
static const char col_cyan[]        = "#005577";
static const char col_orange[]      = "#d75f00";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_black, col_orange,  col_orange  },
};

/* Tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[1]; /*= {
	xprop(1):
	 WM_CLASS(STRING) = instance, class
	 WM_NAME(STRING) = title
	
	class      instance    title       tags mask     isfloating   monitor 
	* { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	* { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};
*/

/* Layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[  Tile  ]",      tile },    /* first entry is default */
	{ "[ Single ]",      monocle },
};

/* Key definitions */
#define MODKEY Mod1Mask
#define MODKEY2 Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                        KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,            KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,              KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,  KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY2,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY2|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY2|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY2|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, 

/* Helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

// Helper for implementing multiple modkeys
#define ADDKEY(MASK, KEY, FUNC, ARG) \
    { MODKEY  MASK,                  KEY,       FUNC,           ARG}, \
    { MODKEY2 MASK,                  KEY,       FUNC,           ARG}, 


/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-l", "5", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_orange, "-sf", col_black, NULL, };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *lockcmd[]  = { "dm-tool", "lock", NULL };
static const char *volupcmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *voldncmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *brupcmd[]  = { "brightnessctl", "set", "+4%", NULL };
static const char *brdncmd[]  = { "brightnessctl", "set", "4%-", NULL };
static const char *restart[]  = { "sudo", "systemctl", "restart", "lightdm", NULL };

static Key keys[] = {
	/*     modifier                key            function        argument */
    // Volume
    ADDKEY(,                       XK_equal,      spawn,          {.v = volupcmd })
    ADDKEY(,                       XK_minus,      spawn,          {.v = voldncmd })
    
    // Screen brightness
    ADDKEY(|ShiftMask,             XK_equal,      spawn,          {.v = brupcmd })
    ADDKEY(|ShiftMask,             XK_minus,      spawn,          {.v = brdncmd })
    
    // Dmenu
    ADDKEY(,                       XK_space,      spawn,          {.v = dmenucmd })                             

    // Lock screen
    ADDKEY(|ControlMask,           XK_l,          spawn,          {.v = lockcmd })

    // Terminal
    ADDKEY(|ShiftMask,             XK_Return,     spawn,          {.v = termcmd })

    // Status bar
	ADDKEY(,                       XK_b,          togglebar,      {0})

    // Layout
	ADDKEY(,                       XK_j,          focusstack,     {.i = +1 })
	ADDKEY(,                       XK_k,          focusstack,     {.i = -1 })
	ADDKEY(,                       XK_i,          incnmaster,     {.i = +1 })
	ADDKEY(,                       XK_d,          incnmaster,     {.i = -1 })
	ADDKEY(,                       XK_r,          resetnmaster,   {0})
	ADDKEY(,                       XK_h,          setmfact,       {.f = -0.05})
	ADDKEY(,                       XK_l,          setmfact,       {.f = +0.05})
	ADDKEY(,                       XK_Return,     zoom,           {0})
	ADDKEY(,                       XK_t,          setlayout,      {0})
	ADDKEY(,                       XK_BackSpace,  killclient,     {0})

    // Toggle last tag(s)
	ADDKEY(,                       XK_Tab,        view,           {0})

    // Other tag/monitor key binds
	ADDKEY(,                       XK_0,          view,           {.ui = ~0 })
	ADDKEY(|ShiftMask,             XK_0,          tag,            {.ui = ~0 })
	ADDKEY(,                       XK_comma,      focusmon,       {.i = -1 })
	ADDKEY(,                       XK_period,     focusmon,       {.i = +1 })
	ADDKEY(|ShiftMask,             XK_comma,      tagmon,         {.i = -1 })
	ADDKEY(|ShiftMask,             XK_period,     tagmon,         {.i = +1 })
	TAGKEYS(                       XK_1,                          0)
	TAGKEYS(                       XK_2,                          1)
	TAGKEYS(                       XK_3,                          2)
	TAGKEYS(                       XK_4,                          3)
	TAGKEYS(                       XK_5,                          4)
	TAGKEYS(                       XK_6,                          5)
	TAGKEYS(                       XK_7,                          6)
	TAGKEYS(                       XK_8,                          7)
	TAGKEYS(                       XK_9,                          8)
    
    // Kill dwm
	ADDKEY(|ShiftMask,             XK_q,          spawn,           {.v = restart})
	ADDKEY(|ShiftMask|ControlMask, XK_q,          quit,            {0})
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY2,        Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY2,        Button2,        togglefloating, {1} },
	{ ClkClientWin,         MODKEY2,        Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,            MODKEY2,        Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY2,        Button3,        toggletag,      {0} },
};

