
/* See LICENSE file for copyright and license details. */

#include <X11/Xlib.h>
#include "utils.h"

#define MODKEY Mod1Mask
#define SUPKEY Mod4Mask

typedef struct comb_key_t comb_key_t;
struct comb_key_t {
  int mod;
  KeySym key;
};

typedef union cfg_type_t cfg_type_t;
union cfg_type_t {
  unsigned int ui;
  int i;
  long l;
  float f;
  double d;
  char s[1024];
  comb_key_t k;
};

typedef struct cfg_item_t cfg_item_t;
struct cfg_item_t {
  cfg_type_t t;
  const char *description;
};

typedef struct cfg_t cfg_t;
struct cfg_t {
  cfg_item_t font;
  cfg_item_t cursor_rate_factor;
  cfg_item_t cursor_min_speed;
  cfg_item_t cursor_max_speed;
  cfg_item_t cursor_size_width;
  cfg_item_t cursor_size_height;
  cfg_item_t cursor_bg_color;
  cfg_item_t hint_window_transparency;
  cfg_item_t hint_bg_color;
  cfg_item_t hint_letters;
  cfg_item_t hint_rect_width;
  cfg_item_t hint_rect_height;
  cfg_item_t hint_rect_font_size;
  cfg_item_t cross_window_transparency;
  cfg_item_t cross_bg_color;
  cfg_item_t quit;
  cfg_item_t restart;
  cfg_item_t set_to_norm_mode;
  cfg_item_t set_to_hint_mode;
  cfg_item_t set_to_cross_mode;
  cfg_item_t escape_norm;
  cfg_item_t escape_hint;
  cfg_item_t escape_cross;
  cfg_item_t escape_hist;
  cfg_item_t move_left;
  cfg_item_t move_down;
  cfg_item_t move_up;
  cfg_item_t move_right;
  cfg_item_t norm_vim_move_up;
  cfg_item_t norm_vim_move_middle_vertical;
  cfg_item_t norm_vim_move_down;
  cfg_item_t norm_vim_move_begin;
  cfg_item_t norm_vim_move_middle_horizontal;
  cfg_item_t norm_vim_move_end;
  cfg_item_t norm_vim_move_center;
  cfg_item_t norm_vim_move_upleft;
  cfg_item_t norm_vim_move_bottomleft;
  cfg_item_t norm_vim_move_bottomright;
  cfg_item_t norm_vim_move_upright;
  cfg_item_t norm_select;
  cfg_item_t norm_select_line;
  cfg_item_t norm_scroll_up;
  cfg_item_t norm_scroll_down;
  cfg_item_t norm_scroll_left;
  cfg_item_t norm_scroll_right;
  cfg_item_t cursor_left_click;
  cfg_item_t cursor_middle_click;
  cfg_item_t cursor_right_click;
  cfg_item_t hint_rollback;
};

int init_cfg(void);
