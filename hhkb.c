
/* See LICENSE file for copyright and license details. */

#include "utils.h"
#include "hhkb.h"

#define VERSION "1.0"
#define CLEANMASK(mask) (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))

cfg_t cfg;

static int running = 1;
char **startup_argv;

static u_mode_t mode = EMPTY_MODE;
static unsigned int numlockmask = 0;
static Display *dpy;
static int screen, sw, sh;
static monitor_t *mons, *selmon;
static Window root, csrwin, hintwin, coverwin;
static win_t coverwinsz;
static cursor_t *csr;
static position_t po;
static hint_position_t *hint_positions;

static cursor_status_t left_cursor_status = CURSOR_UP;
/* static cursor_status_t middle_cursor_status = CURSOR_UP; */
/* static cursor_status_t right_cursor_status = CURSOR_UP; */

static char hint_char_1 = '\0';
static char hint_char_2 = '\0';

comb_key_action_t entr_key_action[5];
comb_key_action_t norm_key_action[25];
comb_key_action_t hint_key_action[2];
comb_key_action_t cover_key_action[5];

// xmodmap -pke
static keycode_t keycodes[] = {
  {24, 'q'}, {25, 'w'}, {26, 'e'}, {27, 'r'}, {28, 't'}, {29, 'y'}, {30, 'u'}, {31, 'i'}, {32, 'o'}, {33, 'p'}, {34, '['}, {35, ']'},
  {38, 'a'}, {39, 's'}, {40, 'd'}, {41, 'f'}, {42, 'g'}, {43, 'h'}, {44, 'j'}, {45, 'k'}, {46, 'l'}, {47, ';'}, {48, '\''},
  {52, 'z'}, {53, 'x'}, {54, 'c'}, {55, 'v'}, {56, 'b'}, {57, 'n'}, {58, 'm'}, {59, ','}, {60, '.'}, {61, '/'},
  {10, '1'}, {11, '2'}, {12, '3'}, {13, '4'}, {14, '5'}, {15, '6'}, {16, '7'}, {17, '8'}, {18, '9'}, {19, '0'},
};

int  init_keys(void);
int  init_monitors(void);
int  init_cursor(void);

void updatenumlockmask(void);
void grab_mode_keys(void);
void handle_event(XEvent ev, comb_key_action_t *keys, int len);

void quit(const arg_t *arg);
void restart(const arg_t *arg);
void set_mode(const arg_t *arg);

void handler_of_norm();
void handler_of_hint();
void handler_of_cover();

void escape(const arg_t *arg);
void move_cursor(const arg_t *arg);
void move_cursor_fast(const arg_t *arg);
void norm_toggle_select(const arg_t *arg);
void norm_toggle_select_line(const arg_t *arg);
void cursor_scroll(const arg_t *arg);
void cursor_button_click(const arg_t *arg);
void hint_rollback(const arg_t *arg);
void hint_redraw();

void move_of_cover(const arg_t *arg);
void move_of_norm(const arg_t *arg);

char keycode2character(int keycode);
void clean_hint_positions();
int  update_hint_positions();
void cover_update_coverwinsz(int x, int y, int w, int h);

void get_cursor_position(int *x, int *y);
void cursor_click(int button);
void cursor_hide();
void cursor_show();
void cursor_up(int button);
void cursor_down(int button);
void cursor_move_absolute(int x, int y);
void cursor_move_relative(int x, int y);
void cursor_position_update_absolute(int x, int y);
void cursor_position_update_relative(int delta_x, int delta_y);

monitor_t *monitor_create(void);
void monitor_cleanup(monitor_t *mon);
void monitor_update(void);
void update_selmon();

void setup(void);
int  run(void (*setup)(void));
int  cleanup(int dummy);
int  help(void);

int init_keys(void) {
  comb_key_action_t entr_key_action_tmp[] = {
    { cfg.quit.t.k.mod                            , cfg.quit.t.k.key                            , quit                    , { 0                }}  ,
    { cfg.restart.t.k.mod                         , cfg.restart.t.k.key                         , restart                 , { 0                }}  ,
    { cfg.set_to_norm_mode.t.k.mod                , cfg.set_to_norm_mode.t.k.key                , set_mode                , { .ui = NORM_MODE  }}  ,
    { cfg.set_to_hint_mode.t.k.mod                , cfg.set_to_hint_mode.t.k.key                , set_mode                , { .ui = HINT_MODE  }}  ,
    { cfg.set_to_cover_mode.t.k.mod               , cfg.set_to_cover_mode.t.k.key               , set_mode                , { .ui = COVER_MODE }}  ,
  };

  comb_key_action_t norm_key_action_tmp[] = {
    { cfg.escape_norm.t.k.mod                     , cfg.escape_norm.t.k.key                     , escape                  , { 0                 }} ,
    { cfg.move_left.t.k.mod                       , cfg.move_left.t.k.key                       , move_cursor             , { .ui = LEFT        }} ,
    { cfg.move_down.t.k.mod                       , cfg.move_down.t.k.key                       , move_cursor             , { .ui = DOWN        }} ,
    { cfg.move_up.t.k.mod                         , cfg.move_up.t.k.key                         , move_cursor             , { .ui = UP          }} ,
    { cfg.move_right.t.k.mod                      , cfg.move_right.t.k.key                      , move_cursor             , { .ui = RIGHT       }} ,
    { cfg.norm_vim_move_up.t.k.mod                , cfg.norm_vim_move_up.t.k.key                , move_cursor_fast        , { .ui = N_U         }} ,
    { cfg.norm_vim_move_middle_vertical.t.k.mod   , cfg.norm_vim_move_middle_vertical.t.k.key   , move_cursor_fast        , { .ui = N_V         }} ,
    { cfg.norm_vim_move_down.t.k.mod              , cfg.norm_vim_move_down.t.k.key              , move_cursor_fast        , { .ui = N_L         }} ,
    { cfg.norm_vim_move_begin.t.k.mod             , cfg.norm_vim_move_begin.t.k.key             , move_cursor_fast        , { .ui = N_B         }} ,
    { cfg.norm_vim_move_middle_horizontal.t.k.mod , cfg.norm_vim_move_middle_horizontal.t.k.key , move_cursor_fast        , { .ui = N_H         }} ,
    { cfg.norm_vim_move_end.t.k.mod               , cfg.norm_vim_move_end.t.k.key               , move_cursor_fast        , { .ui = N_E         }} ,
    { cfg.norm_vim_move_center.t.k.mod            , cfg.norm_vim_move_center.t.k.key            , move_cursor_fast        , { .ui = N_C         }} ,
    { cfg.norm_vim_move_upleft.t.k.mod            , cfg.norm_vim_move_upleft.t.k.key            , move_cursor_fast        , { .ui = N_A         }} ,
    { cfg.norm_vim_move_bottomleft.t.k.mod        , cfg.norm_vim_move_bottomleft.t.k.key        , move_cursor_fast        , { .ui = N_S         }} ,
    { cfg.norm_vim_move_bottomright.t.k.mod       , cfg.norm_vim_move_bottomright.t.k.key       , move_cursor_fast        , { .ui = N_D         }} ,
    { cfg.norm_vim_move_upright.t.k.mod           , cfg.norm_vim_move_upright.t.k.key           , move_cursor_fast        , { .ui = N_F         }} ,
    { cfg.norm_select.t.k.mod                     , cfg.norm_select.t.k.key                     , norm_toggle_select      , { 0                 }} ,
    { cfg.norm_select_line.t.k.mod                , cfg.norm_select_line.t.k.key                , norm_toggle_select_line , { 0                 }} ,
    { cfg.norm_scroll_up.t.k.mod                  , cfg.norm_scroll_up.t.k.key                  , cursor_scroll           , { .ui = UP          }} ,
    { cfg.norm_scroll_down.t.k.mod                , cfg.norm_scroll_down.t.k.key                , cursor_scroll           , { .ui = DOWN        }} ,
    { cfg.norm_scroll_left.t.k.mod                , cfg.norm_scroll_left.t.k.key                , cursor_scroll           , { .ui = LEFT        }} ,
    { cfg.norm_scroll_right.t.k.mod               , cfg.norm_scroll_right.t.k.key               , cursor_scroll           , { .ui = RIGHT       }} ,
    { cfg.cursor_left_click.t.k.mod               , cfg.cursor_left_click.t.k.key               , cursor_button_click     , { .ui = LEFT_BUT    }} ,
    { cfg.cursor_middle_click.t.k.mod             , cfg.cursor_middle_click.t.k.key             , cursor_button_click     , { .ui = MIDDLE_BUT  }} ,
    { cfg.cursor_right_click.t.k.mod              , cfg.cursor_right_click.t.k.key              , cursor_button_click     , { .ui = RIGHT_BUT   }} ,
  };

  comb_key_action_t hint_key_action_tmp[] = {
    { cfg.escape_hint.t.k.mod                     , cfg.escape_hint.t.k.key                     , escape                  , { 0                 }} ,
    { cfg.hint_rollback.t.k.mod                   , cfg.hint_rollback.t.k.key                   , hint_rollback           , { 0                 }} ,
  };

  comb_key_action_t cover_key_action_tmp[] = {
    { cfg.escape_cover.t.k.mod                    , cfg.escape_cover.t.k.key                    , escape                  , { 0                 }} ,
    { cfg.move_left.t.k.mod                       , cfg.move_left.t.k.key                       , move_cursor             , { .ui = LEFT        }} ,
    { cfg.move_down.t.k.mod                       , cfg.move_down.t.k.key                       , move_cursor             , { .ui = DOWN        }} ,
    { cfg.move_up.t.k.mod                         , cfg.move_up.t.k.key                         , move_cursor             , { .ui = UP          }} ,
    { cfg.move_right.t.k.mod                      , cfg.move_right.t.k.key                      , move_cursor             , { .ui = RIGHT       }} ,
  };

  memcpy(entr_key_action , entr_key_action_tmp , sizeof(entr_key_action_tmp));
  memcpy(norm_key_action , norm_key_action_tmp , sizeof(norm_key_action_tmp));
  memcpy(hint_key_action , hint_key_action_tmp , sizeof(hint_key_action_tmp));
  memcpy(cover_key_action, cover_key_action_tmp, sizeof(cover_key_action_tmp));

  return EXIT_SUCCESS;
}

int init_monitors(void) {
  dpy = XOpenDisplay(NULL);
  if (!dpy)
    return EXIT_FAILURE;

  screen = DefaultScreen(dpy);
  root = XRootWindow(dpy, screen);
  sw = DisplayWidth(dpy, screen);
  sh = DisplayHeight(dpy, screen);
  monitor_update();

  return EXIT_SUCCESS;
}

int init_cursor(void) {
  csr = ecalloc(1, sizeof(cursor_t));
  csr->rate_factor = cfg.cursor_rate_factor.t.l;
  csr->speed = cfg.cursor_min_speed.t.i;
  csr->min_speed = cfg.cursor_min_speed.t.i;
  csr->max_speed = cfg.cursor_max_speed.t.i;
  csr->repeat_count = 0;
  csr->repeat_count_reset_time = time(NULL);
  csr->is_hidden = 0;
  csr->w = cfg.cursor_size_width.t.i;
  csr->h = cfg.cursor_size_height.t.i;
  csr->bg_color = cfg.cursor_bg_color.t.s;

  return EXIT_SUCCESS;
}

void updatenumlockmask(void) {
  unsigned int i, j;
  XModifierKeymap *modmap;

  numlockmask = 0;
  modmap = XGetModifierMapping(dpy);
  for (i = 0; i < 8; i++) {
    for (j = 0; j < modmap->max_keypermod; j++)
      if (modmap->modifiermap[i * modmap->max_keypermod + j] == XKeysymToKeycode(dpy, XK_Num_Lock))
        numlockmask = (1 << i);
  }
  XFreeModifiermap(modmap);
}

void grab_mode_keys(void) {
  updatenumlockmask();

  KeyCode code;
  unsigned int i, j;
  unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask | LockMask };
  XUngrabKey(dpy, AnyKey, AnyModifier, root);
  for (i = 0; i < LENGTH(entr_key_action); i++) {
    if ((code = XKeysymToKeycode(dpy, entr_key_action[i].key)))
      for (j = 0; j < LENGTH(modifiers); j++)
        XGrabKey(dpy, code, entr_key_action[i].mod | modifiers[j], root, True, GrabModeAsync, GrabModeAsync);
  }
}

void handle_event(XEvent ev, comb_key_action_t *keys, int len) {
  // processing mod + key first
  for (int i = 0; i < len; i++) {
    if (keys[i].mod != NoSymbol && CLEANMASK(ev.xkey.state) == CLEANMASK(keys[i].mod) && (KeyCode)(ev.xkey.keycode) == XKeysymToKeycode(dpy, keys[i].key)) {
      keys[i].func(&keys[i].arg);
      return;
    }
  }

  // processing _ + key last
  for (int i = 0; i < len; i++) {
    if (keys[i].mod == NoSymbol && (KeyCode)(ev.xkey.keycode) == XKeysymToKeycode(dpy, keys[i].key)) {
      keys[i].func(&keys[i].arg);
      return;
    }
  }
}

void quit(const arg_t *arg) {
  running = 0;
}

void restart(const arg_t *arg) {
  execvp(startup_argv[0], startup_argv);
}

void set_mode(const arg_t *arg) {
  mode = arg->ui;

  switch (mode) {
    case NORM_MODE:
      handler_of_norm() ; break;
    case HINT_MODE:
      handler_of_hint() ; break;
    case COVER_MODE:
      handler_of_cover(); break;
    default:
      break;
  }
}

void handler_of_norm() {
  arg_t empty_arg;

  XEvent ev;
  XColor color;
  XSetWindowAttributes attr;
  Colormap colormap;

  get_cursor_position(&po.x, &po.y);
  cursor_hide();

  colormap = DefaultColormap(dpy, screen); XParseColor(dpy, colormap, csr->bg_color, &color); XAllocColor(dpy, colormap, &color);
  attr.background_pixel = color.pixel;
  attr.override_redirect = True;

  csrwin = XCreateWindow(dpy, root, po.x, po.y, csr->w, csr->h, 0, DefaultDepth(dpy, screen),  InputOutput, DefaultVisual(dpy, screen), CWBackPixel|CWOverrideRedirect, &attr);
  XMapWindow(dpy, csrwin);
  XSync(dpy, False);

  XSelectInput(dpy, csrwin, KeyPressMask);
  XGrabKeyboard(dpy, csrwin, True, GrabModeAsync, GrabModeAsync, CurrentTime);
  while (running && mode == NORM_MODE && !XNextEvent(dpy, &ev)) {
    if (ev.xkey.type != KeyPress)
      continue;

    handle_event(ev, norm_key_action, LENGTH(norm_key_action));
  }

  /* clean down cursor */
  if (left_cursor_status == CURSOR_DOWN)
    norm_toggle_select(&empty_arg);

  /* clean windows */
  XUnmapWindow(dpy, csrwin);
  XDestroyWindow(dpy, csrwin);
  XFlush(dpy);

  cursor_position_update_absolute(po.x, po.y);
  cursor_show();
  XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
}

void handler_of_hint() {
  char character;
  int depth;
  XEvent ev;
  XSetWindowAttributes attr;
  Visual *visual;
  XFontStruct* fontinfo;
  arg_t empty_arg;
  XColor color;
  Colormap colormap;

  update_hint_positions();

  get_cursor_position(&po.x, &po.y);
  cursor_hide();

  visual = XDefaultVisual(dpy, screen);
  depth = XDefaultDepth(dpy, screen);

  colormap = DefaultColormap(dpy, screen); XParseColor(dpy, colormap, cfg.hint_bg_color.t.s, &color); XAllocColor(dpy, colormap, &color);
  attr.background_pixel = color.pixel;
  attr.override_redirect = True;
  attr.event_mask = ExposureMask;

  hintwin = XCreateWindow(dpy, root, 0, 0, selmon->mw, selmon->mh, 0, depth, InputOutput, visual, CWColormap|CWBorderPixel|CWBackPixel|CWOverrideRedirect|CWEventMask, &attr);

  unsigned long opacity = (unsigned long)(0xFFFFFFFFul * (1 - cfg.hint_window_transparency.t.d));
  Atom XA_NET_WM_WINDOW_OPACITY = XInternAtom(dpy, "_NET_WM_WINDOW_OPACITY", False);
  XChangeProperty(dpy, hintwin, XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&opacity, 1L);

  fontinfo = XLoadQueryFont(dpy, cfg.font.t.s);
  XSetFont(dpy, DefaultGC(dpy, screen), fontinfo->fid);

  XMapWindow(dpy, hintwin);
  XSync(dpy, False);

  XSelectInput(dpy, hintwin, KeyPressMask|ExposureMask);
  XGrabKeyboard(dpy, hintwin, True, GrabModeAsync, GrabModeAsync, CurrentTime);
  while (running && mode == HINT_MODE && !XNextEvent(dpy, &ev)) {
    switch(ev.type) {
      case Expose:
        hint_redraw();
        break;
      case KeyPress:
        handle_event(ev, hint_key_action, LENGTH(hint_key_action));

        if (mode != HINT_MODE)
          break;

        hint_redraw();

        character = keycode2character(ev.xkey.keycode);
        if (!character)
          continue;

        if (!hint_char_1) { hint_char_1 = character; } else if (!hint_char_2) { hint_char_2 = character; }

        hint_redraw();

        if (hint_char_1 && hint_char_2) {
          hint_position_t *hr;
          for (hr = hint_positions; hr; hr = hr->next) {
            if (hr->text[0] == hint_char_1 && hr->text[1] == hint_char_2) {
              cursor_position_update_absolute(selmon->mx + hr->x, selmon->my + hr->y - csr->h);
              escape(&empty_arg);
              break;
            }
          }
        }
        break;
      default:
        break;
    }
  }

  hint_char_1 = '\0'; hint_char_2 = '\0';
  XUnmapWindow(dpy, hintwin);
  XDestroyWindow(dpy, hintwin);

  cursor_show();
  XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);

  if (mode == NORM_MODE) {
    handler_of_norm();
  }
}

void handler_of_cover() {
  XEvent ev;
  XColor color;
  XSetWindowAttributes attr;
  Colormap colormap;

  get_cursor_position(&po.x, &po.y);
  cursor_hide();

  colormap = DefaultColormap(dpy, screen); XParseColor(dpy, colormap, cfg.cover_bg_color.t.s, &color); XAllocColor(dpy, colormap, &color);
  attr.border_pixel = BlackPixel(dpy, screen);
  attr.background_pixel = color.pixel;
  attr.override_redirect = True;

  coverwin = XCreateWindow(dpy, root, selmon->wx, selmon->wy, selmon->ww, selmon->wh, 0, DefaultDepth(dpy, screen), InputOutput, DefaultVisual(dpy, screen), CWBorderPixel|CWBackPixel|CWOverrideRedirect, &attr);

  cover_update_coverwinsz(selmon->mx, selmon->my, selmon->ww, selmon->wh);
  cursor_position_update_absolute(selmon->mx + selmon->ww/2, selmon->my + selmon->mh/2);

  unsigned long opacity = (unsigned long)(0xFFFFFFFFul * (1 - cfg.cover_window_transparency.t.d));
  Atom XA_NET_WM_WINDOW_OPACITY = XInternAtom(dpy, "_NET_WM_WINDOW_OPACITY", False);
  XChangeProperty(dpy, coverwin, XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&opacity, 1L);

  XMapWindow(dpy, coverwin);
  XSync(dpy, False);

  XSelectInput(dpy, coverwin, KeyPressMask);
  XGrabKeyboard(dpy, coverwin, True, GrabModeAsync, GrabModeAsync, CurrentTime);
  while (running && mode == COVER_MODE && !XNextEvent(dpy, &ev)) {
    if (ev.xkey.type != KeyPress)
      continue;

    if (coverwinsz.w < csr->w + 2 && coverwinsz.h < csr->h + 2)
      break;

    handle_event(ev, cover_key_action, LENGTH(cover_key_action));
  }

  /* clean windows */
  XUnmapWindow(dpy, coverwin);
  XDestroyWindow(dpy, coverwin);
  XFlush(dpy);

  cursor_show();
  XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);

  if (mode == NORM_MODE) {
    handler_of_norm();
  }
}

void escape(const arg_t *arg) {
  switch (mode) {
    case NORM_MODE:
      mode = EMPTY_MODE; break;
    case COVER_MODE:
      mode = NORM_MODE;  break;
    case HINT_MODE:
      mode = NORM_MODE;  break;
    default:
      break;
  };
}

void move_cursor(const arg_t *arg) {
  switch (mode) {
    case NORM_MODE:
      move_of_norm(arg);  break;
    case COVER_MODE:
      move_of_cover(arg); break;
    case HINT_MODE:
      break;
    default:
      break;
  };
}

void move_cursor_fast(const arg_t *arg) {
  int x, y, offset;
  offset = 1;
  if (mode != NORM_MODE)
    return;

  x = po.x; y = po.y;
  switch (arg->ui) {
    case N_U:
      x = po.x;
      y = selmon->my + offset;
      break;
    case N_V:
      x = po.x;
      y = selmon->my + selmon->wh/2;
      break;
    case N_L:
      x = po.x;
      y = selmon->my + selmon->wh - csr->h - offset;
      break;
    case N_B:
      x = selmon->mx + offset;
      y = po.y;
      break;
    case N_H:
      x = selmon->mx + selmon->ww/2;
      y = po.y;
      break ;
    case N_E:
      x = selmon->mx + selmon->ww - csr->w - offset;
      y = po.y;
      break ;
    case N_C:
      x = selmon->mx + selmon->ww / 2;
      y = selmon->wy + selmon->wh / 2;
      break;
    case N_A:
      x = selmon->mx + selmon->ww / 4;
      y = selmon->my + selmon->wh / 4;
      break;
    case N_S:
      x = selmon->mx + selmon->ww / 4;
      y = selmon->my + 3 * selmon->wh / 4;
      break;
    case N_D:
      x = selmon->mx + 3 * selmon->ww / 4;
      y = selmon->my + 3 * selmon->wh / 4;
      break;
    case N_F:
      x = selmon->mx + 3 * selmon->ww / 4;
      y = selmon->my + selmon->wh / 4;
      break;
    default:
      break;
  };

  cursor_position_update_absolute(x, y);

  if (left_cursor_status == CURSOR_UP) {
    XMoveResizeWindow(dpy, csrwin, po.x, po.y, csr->w, csr->h);
    XFlush(dpy);
  }
}


void norm_toggle_select(const arg_t *arg) {
  int button = 1;
  if (left_cursor_status == CURSOR_UP) {
    cursor_show();
    XMoveResizeWindow(dpy, csrwin, selmon->ww, selmon->wh, csr->w, csr->h);
    XFlush(dpy);
    cursor_down(button);
  } else {
    get_cursor_position(&po.x, &po.y);
    cursor_hide();
    cursor_up(button);
    XMoveResizeWindow(dpy, csrwin, po.x, po.y, csr->w, csr->h);
    XFlush(dpy);
  }
}

void norm_toggle_select_line(const arg_t *arg) {
  arg_t empty_arg, arg_new;

  if (left_cursor_status == CURSOR_DOWN)
    left_cursor_status = CURSOR_UP;

  arg_new.ui = N_B; move_cursor_fast(&arg_new);
  norm_toggle_select(&empty_arg);
  arg_new.ui = N_E; move_cursor_fast(&arg_new);
}

void cursor_scroll(const arg_t *arg)  {
  switch (arg->ui) {
    case UP:
      cursor_click(4); break;
    case DOWN:
      cursor_click(5); break;
    case LEFT:
      cursor_click(6); break;
    case RIGHT:
      cursor_click(7); break;
    default:
      break;
  }
}

void cursor_button_click(const arg_t *arg)  {
  switch (arg->ui) {
    case LEFT_BUT:
      cursor_click(1); break;
    case MIDDLE_BUT:
      cursor_click(2); break;
    case RIGHT_BUT:
      cursor_click(3); break;
    default:
      break;
  }
}

void hint_rollback(const arg_t *arg) {
  if (!hint_char_2 && hint_char_1)
    hint_char_1 = '\0';
}

void hint_redraw() {
  XClearWindow(dpy, hintwin);
  XMoveResizeWindow(dpy, hintwin, selmon->mx, selmon->my, selmon->mw, selmon->mh);

  hint_position_t *hr;
  for (hr = hint_positions; hr; hr = hr->next) {
    if ((!hint_char_1 && !hint_char_2) || (hint_char_1 && !hint_char_2 && hr->text[0] == hint_char_1) || (hint_char_1 && hint_char_2 && hr->text[0] == hint_char_1 && hr->text[1] == hint_char_2))
      XDrawString(dpy, hintwin, DefaultGC(dpy, screen), hr->x, hr->y, hr->text, strlen(hr->text));
  }

  XFlush(dpy);
}

void move_of_cover(const arg_t *arg) {
  switch (arg->ui) {
    case LEFT:
      coverwinsz.w /= 2;
      break;
    case DOWN:
      coverwinsz.h /= 2;
      if (coverwinsz.h > csr->h)
        coverwinsz.y += coverwinsz.h;
      break;
    case UP:
      coverwinsz.h /= 2;
      break;
    case RIGHT:
      coverwinsz.w /= 2;
      if (coverwinsz.w > csr->w)
        coverwinsz.x += coverwinsz.w;
      break;
    default:
      break;
  };

  coverwinsz.w = coverwinsz.w < csr->w ? csr->w : coverwinsz.w;
  coverwinsz.h = coverwinsz.h < csr->h ? csr->h : coverwinsz.h;

  cursor_position_update_absolute(coverwinsz.x + coverwinsz.w/2, coverwinsz.y + coverwinsz.h/2);
  XMoveResizeWindow(dpy, coverwin, coverwinsz.x, coverwinsz.y, coverwinsz.w, coverwinsz.h);
  XFlush(dpy);
}

void move_of_norm(const arg_t *arg) {
  csr->repeat_count++;

  long double rate = 1;
  long double duration = (double)(time(NULL) - csr->repeat_count_reset_time);

  if (duration * csr->rate_factor > 1) {
    rate = 1.0 * csr->repeat_count / duration;
    csr->repeat_count = 0;
    csr->repeat_count_reset_time = time(NULL);
    csr->speed = rate > csr->min_speed ? (int)rate : csr->min_speed;
  }

  switch (arg->ui) {
    case LEFT:
      cursor_position_update_relative(-csr->speed, 0); break;
    case DOWN:
      cursor_position_update_relative(0, +csr->speed); break;
    case UP:
      cursor_position_update_relative(0, -csr->speed); break;
    case RIGHT:
      cursor_position_update_relative(+csr->speed, 0); break;
    default:
      break;
  };

  if (left_cursor_status == CURSOR_UP) {
    XMoveResizeWindow(dpy, csrwin, po.x, po.y, csr->w, csr->h);
    XFlush(dpy);
  }
}

char keycode2character(int keycode) {
  for (int i = 0; i < LENGTH(keycodes); i++) {
    if (keycodes[i].keycode == keycode)
      return keycodes[i].character;
  }

  return '\0';
}

void clean_hint_positions() {
  hint_position_t *m;
  while(hint_positions) {
    m = hint_positions;
    hint_positions = hint_positions->next;
    free(m);
  }
}

int update_hint_positions() {
  clean_hint_positions();

  int sz, w, h;
  sz = strlen(cfg.hint_letters.t.s);

  w = selmon->mw/sz;
  h = selmon->mh/sz;

  // head is empty
  hint_positions = ecalloc(1, sizeof(hint_position_t));
  hint_positions->next = NULL;

  hint_position_t *tmp;
  tmp = hint_positions;

  for (int i = 0; i < sz; i++) {
    for (int j = 0; j < sz; j++) {
      hint_position_t *hr;
      hr = ecalloc(1, sizeof(hint_position_t));
      hr->next = NULL;
      hr->x = i * w + w/2;     /* hr->x = selmon->mx + i * w + w/2;   */
      hr->y = j * h + 4*h/5;   /* hr->y = selmon->my + j * h + 4*h/5; */
      hr->text[0] = cfg.hint_letters.t.s[j];
      hr->text[1] = cfg.hint_letters.t.s[i];
      tmp->next = hr;
      tmp = tmp->next;
    }
  }

  // clean empty head
  tmp = hint_positions;
  hint_positions = hint_positions->next;
  free(tmp);

  return EXIT_SUCCESS;
}

void cover_update_coverwinsz(int x, int y, int w, int h) {
  coverwinsz.x = x; coverwinsz.y = y; coverwinsz.w = w; coverwinsz.h = h;
}

void get_cursor_position(int *x, int *y) {
  Window root_return, child_return;
  int root_x, root_y, win_x, win_y;
  unsigned int mask;
  XQueryPointer(dpy, root, &root_return, &child_return, &root_x, &root_y, &win_x, &win_y, &mask);
  *x = root_x; *y = root_y;
}

void cursor_click(int button) {
  get_cursor_position(&po.x, &po.y);
  cursor_show();
  XMoveResizeWindow(dpy, csrwin, selmon->ww, selmon->wh, csr->w, csr->h);
  XFlush(dpy);

  XTestFakeButtonEvent(dpy, button, True, 0);
  XTestFakeButtonEvent(dpy, button, False, 0);
  XFlush(dpy);

  cursor_hide();
  XMoveResizeWindow(dpy, csrwin, po.x, po.y, csr->w, csr->h);
  XFlush(dpy);
}

void cursor_hide() {
  if (csr->is_hidden) return;
  XFixesHideCursor(dpy, root);
  XSync(dpy, False);
  csr->is_hidden = 1;
}

void cursor_show() {
  if (!csr->is_hidden) return;
  XFixesShowCursor(dpy, root);
  XSync(dpy, False);
  csr->is_hidden = 0;
}

void cursor_up(int button) {
  left_cursor_status = CURSOR_UP;
  XTestFakeButtonEvent(dpy, button, False, 0);
  XFlush(dpy);
}

void cursor_down(int button) {
  left_cursor_status = CURSOR_DOWN;
  XTestFakeButtonEvent(dpy, button, True, 0);
  XFlush(dpy);
}

void cursor_move_absolute(int x, int y) {
  XTestFakeMotionEvent(dpy, screen, x, y, 0);
  XFlush(dpy);
}

void cursor_move_relative(int x, int y) {
  XTestFakeRelativeMotionEvent(dpy, x, y, 0);
  XFlush(dpy);
}

void cursor_position_update_absolute(int x, int y) {
  po.x = x; po.y = y;

  // make sure in screen: multiple monitors
  po.x = x < 0 ? 0 : x;
  po.y = y < 0 ? 0 : y;
  po.x = x > sw - csr->w ? sw - csr->w : x;
  po.y = y > sh - csr->h ? sh - csr->h : y;

  // update selmon
  update_selmon();

  cursor_move_absolute(po.x, po.y);
}

void cursor_position_update_relative(int delta_x, int delta_y) {
  po.x += delta_x; po.y += delta_y;

  // make sure in screen: multiple monitors
  po.x = po.x < 0 ? 0 : po.x;
  po.y = po.y < 0 ? 0 : po.y;
  po.x = po.x > sw - csr->w ? sw - csr->w : po.x;
  po.y = po.y > sh - csr->h ? sh - csr->h : po.y;

  // update selmon
  update_selmon();

  cursor_move_absolute(po.x, po.y);
}

monitor_t *monitor_create(void) {
  monitor_t *m = ecalloc(1, sizeof(monitor_t));
  return m;
}

void monitor_cleanup(monitor_t *mon) {
  monitor_t *m;

  if (mon == mons) {
    mons = mons->next;
  } else {
    for (m = mons; m && m->next != mon; m = m->next);
    m->next = mon->next;
  }
  free(mon);
}

void monitor_update(void) {
  if (XineramaIsActive(dpy)) {
    monitor_t *m;
    int i, c, n, rx, ry, wx, wy;
    unsigned int dummyuint;
    Window dummywin;

    XineramaScreenInfo *info = XineramaQueryScreens(dpy, &n);

    // get cached monitors num
    for (c = 0, m = mons; m; m = m->next, c++);

    //  allocate new monitors if n > c
    for (i = c; i < n; i++) {
      for (m = mons; m && m->next; m = m->next);
      if (m)
        m->next = monitor_create();
      else
        mons = monitor_create();
    }

    // update all monitors
    for (i = 0, m = mons; i < n && m; m = m->next, i++) {
      m->id = i;
      m->mx = m->wx = info[i].x_org;
      m->my = m->wy = info[i].y_org;
      m->mw = m->ww = info[i].width;
      m->mh = m->wh = info[i].height;
    }

    // remove monitors if c > n
    for (i = n; i < c; i++) {
      for (m = mons; m && m->next; m = m->next);
      if (m == selmon)
        selmon = mons;
      monitor_cleanup(m);
    }

    // find selected monitor
    XQueryPointer(dpy, root, &dummywin, &dummywin, &rx, &ry, &wx, &wy, &dummyuint);
    for(i=0, m=mons; i < n && m; m = m->next, i++)
      if (wx >= m->mx && wx <= m->wx + m->ww && wy >= m->my && wy <= m->my + m->mh)
        selmon = m;

    XFree(info);
  } else {
    if (!mons)
      mons = monitor_create();

    if (mons->mw != sw || mons->mh != sh) {
      mons->mw = mons->ww = sw;
      mons->mh = mons->wh = sh;
    }
    selmon = mons;
  }

  if (!selmon)
    selmon = mons;
}

void update_selmon() {
  monitor_t *m;
  int i, n, rx, ry, wx, wy;
  unsigned int dummyuint;
  Window dummywin;

  XineramaQueryScreens(dpy, &n);
  XQueryPointer(dpy, root, &dummywin, &dummywin, &rx, &ry, &wx, &wy, &dummyuint);
  for(i=0, m=mons; i < n && m; m = m->next, i++) {
    if (wx >= m->mx && wx <= m->wx + m->ww && wy >= m->my && wy <= m->my + m->mh)
      selmon = m;
  }
}

void setup(void) {
  if (init_cfg())      exit(cleanup(1));
  if (init_keys())     exit(cleanup(1));
  if (init_monitors()) exit(cleanup(1));
  if (init_cursor())   exit(cleanup(1));

  // TODO: < 10:44:04 2022-09-28 -- Author: Lorenzo >: move to init check
  // test font is ok before jump into set
  XFontStruct* fontinfo = XLoadQueryFont(dpy, cfg.font.t.s);
  if (!fontinfo) exit(cleanup(1));

  if (!strlen(cfg.hint_letters.t.s)) exit(cleanup(1));

}

int run(void (*setup)(void)) {
  setup();

  XEvent ev;
  grab_mode_keys();
  while (running && !XNextEvent(dpy, &ev))
    handle_event(ev, entr_key_action, LENGTH(entr_key_action));

  return EXIT_SUCCESS;
}

int cleanup(int dummy) {
  XCloseDisplay(dpy);
  XDestroyWindow(dpy, csrwin);
  XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
  XSync(dpy, False);
  while (mons) monitor_cleanup(mons);
  free(csr);
  clean_hint_positions();
  return EXIT_SUCCESS;
}

int help(void) {
  char *helpstr = "\
  hhkb [-hvf]\n\
  \n\
  args: \n\
    -h: show help\n\
    -v: show version\n\
    -f: run in foreground\n\
  ";
  puts(helpstr);
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  int run_in_daemon = 1;

  startup_argv = argv;
  if (argc == 2) {
    if (!strcmp("-h", argv[1]))
      exit(help());
    else if (!strcmp("-v", argv[1]))
      exit(puts("hhkb-" VERSION));
    else if (!strcmp("-f", argv[1]))
      run_in_daemon = 0;
    else
      exit(puts("usage: hhkb [-hvf]"));
  }

  if (run_in_daemon)
    daemon(0,0);

  exit(cleanup(run(setup)));
}
