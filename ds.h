
/* See LICENSE file for copyright and license details. */

#include "config.h"

typedef enum { EMPTY_MODE, NORM_MODE, COVER_MODE, HINT_MODE            } u_mode_t;
typedef enum { UP, DOWN, LEFT, RIGHT                                   } direction_t;
typedef enum { LEFT_BUT, MIDDLE_BUT, RIGHT_BUT                         } cursor_button_t;
typedef enum { N_U, N_V, N_L, N_B, N_H, N_E, N_C, N_A, N_S, N_D, N_F   } cursor_fast_t;
typedef enum { CURSOR_UP, CURSOR_DOWN                                  } cursor_status_t;

typedef struct monitor_t monitor_t;
struct monitor_t {
  int id;
  int mx, my, mw, mh;   /* screen size */
  int wx, wy, ww, wh;   /* window area */
  monitor_t *next;
};

typedef struct win_t win_t;
struct win_t{
  short x, y, w, h;
};

typedef struct cursor_t cursor_t;
struct cursor_t {
  long long rate_factor;
  float sensitivity;
  int speed;
  int min_speed;
  int max_speed;
  int repeat_count;
  time_t repeat_count_reset_time;
  int is_hidden;
  int w;
  int h;
  char *bg_color;
  /* char *fg_color; */
};

typedef struct position_t position_t;
struct position_t {
  int x, y;
};

typedef struct keycode_t keycode_t;
struct keycode_t {
  int keycode;
  char character;
};

typedef struct hint_position_t hint_position_t;
struct hint_position_t {
  int x;
  int y;
  char text[2];
  hint_position_t *next;
};

typedef union arg_t arg_t;
union arg_t {
  int i;
  unsigned int ui;
  float f;
  const void *v;
};

typedef struct comb_key_action_t comb_key_action_t;
struct comb_key_action_t {
  int mod;
  KeySym key;
  KeyCode keycode;
  void (*func)(const arg_t *);
  const arg_t arg;
};
