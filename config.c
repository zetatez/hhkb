
/* See LICENSE file for copyright and license details. */

#include "config.h"

extern cfg_t cfg;

void
cfg_default(void) {
  cfg.font                            = (cfg_item_t){{ .s = "-misc-dejavu sans-medium-o-normal--0-0-0-0-p-0-adobe-standard" }, "font"                            };
  cfg.cursor_rate_factor              = (cfg_item_t){{ .l = 1000000000000000                                                }, "cursor-rate-factor"              };
  cfg.cursor_min_speed                = (cfg_item_t){{ .i = 12                                                              }, "cursor-min-speed"                };
  cfg.cursor_max_speed                = (cfg_item_t){{ .i = 148                                                             }, "cursor-max-speed"                };
  cfg.cursor_size_width               = (cfg_item_t){{ .i = 10                                                              }, "cursor-size-width"               };
  cfg.cursor_size_height              = (cfg_item_t){{ .i = 10                                                              }, "cursor-size-hegiht"              };
  cfg.cursor_bg_color                 = (cfg_item_t){{ .s = "#0000FF"                                                       }, "cursor-bg-color"                 };
  cfg.hint_window_transparency        = (cfg_item_t){{ .d = 0.618                                                           }, "hint-window-transparency"        };
  cfg.hint_bg_color                   = (cfg_item_t){{ .s = "#00BFFF"                                                       }, "hint-bg-color"                   };
  cfg.hint_letters                    = (cfg_item_t){{ .s = "abcdefghijklmnopqrstuvwxyz"                                    }, "hint-letters"                    };
  cfg.hint_rect_width                 = (cfg_item_t){{ .i = 20                                                              }, "hint-rect-width"                 };
  cfg.hint_rect_height                = (cfg_item_t){{ .i = 20                                                              }, "hint-rect-height"                };
  cfg.hint_rect_font_size             = (cfg_item_t){{ .i = 8                                                               }, "hint-rect-font-size"             };
  cfg.cover_window_transparency       = (cfg_item_t){{ .d = 0.618                                                           }, "cover-window-transparency"       };
  cfg.cover_bg_color                  = (cfg_item_t){{ .s = "#0000FF"                                                       }, "cover-bg-color"                  };
  cfg.quit                            = (cfg_item_t){{ .k = {MODKEY|ControlMask, XStringToKeysym("q")                      }}, "quit"                            };
  cfg.restart                         = (cfg_item_t){{ .k = {MODKEY|ControlMask, XStringToKeysym("r")                      }}, "restart"                         };
  cfg.set_to_norm_mode                = (cfg_item_t){{ .k = {MODKEY|ControlMask, XStringToKeysym("n")                      }}, "set-to-norm-mode"                };
  cfg.set_to_hint_mode                = (cfg_item_t){{ .k = {MODKEY|ControlMask, XStringToKeysym("h")                      }}, "set-to-hint-mode"                };
  cfg.set_to_cover_mode               = (cfg_item_t){{ .k = {MODKEY|ControlMask, XStringToKeysym("c")                      }}, "set-to-cover-mode"               };
  cfg.escape_norm                     = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("Escape")                 }}, "escape-norm"                     };
  cfg.escape_hint                     = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("Escape")                 }}, "escape-hint"                     };
  cfg.escape_cover                    = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("Escape")                 }}, "escape-cover"                    };
  cfg.move_left                       = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("h")                      }}, "move-left"                       };
  cfg.move_down                       = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("j")                      }}, "move-down"                       };
  cfg.move_up                         = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("k")                      }}, "move-up"                         };
  cfg.move_right                      = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("l")                      }}, "move-right"                      };
  cfg.norm_vim_move_up                = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("u")                      }}, "norm-vim-move-up"                };
  cfg.norm_vim_move_middle_vertical   = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("m")                      }}, "norm-vim-move-middle-vertical"   };
  cfg.norm_vim_move_down              = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("l")                      }}, "norm-vim-move-down"              };
  cfg.norm_vim_move_begin             = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("b")                      }}, "norm-vim-move-begin"             };
  cfg.norm_vim_move_end               = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("e")                      }}, "norm-vim-move-end"               };
  cfg.norm_vim_move_middle_horizontal = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("c")                      }}, "norm-vim-move-middle-horizontal" };
  cfg.norm_vim_move_center            = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("c")                      }}, "norm-vim-move-center"            };
  cfg.norm_vim_move_upleft            = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("a")                      }}, "norm-vim-move-upleft"            };
  cfg.norm_vim_move_bottomleft        = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("s")                      }}, "norm-vim-move-bottomleft"        };
  cfg.norm_vim_move_bottomright       = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("d")                      }}, "norm-vim-move-bottomright"       };
  cfg.norm_vim_move_upright           = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("f")                      }}, "norm-vim-move-upright"           };
  cfg.norm_select                     = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("v")                      }}, "norm-select"                     };
  cfg.norm_select_line                = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("v")                      }}, "norm-select-line"                };
  cfg.norm_scroll_up                  = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("o")                      }}, "norm-scroll-up"                  };
  cfg.norm_scroll_down                = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("i")                      }}, "norm-scroll-down"                };
  cfg.norm_scroll_left                = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("i")                      }}, "norm-scroll-left"                };
  cfg.norm_scroll_right               = (cfg_item_t){{ .k = {ShiftMask         , XStringToKeysym("o")                      }}, "norm-scroll-right"               };
  cfg.cursor_left_click               = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("n")                      }}, "cursor-left-click"               };
  cfg.cursor_middle_click             = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("m")                      }}, "cursor-middle-click"             };
  cfg.cursor_right_click              = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("comma")                  }}, "cursor-rihgt-click"              };
  cfg.hint_rollback                   = (cfg_item_t){{ .k = {NoSymbol          , XStringToKeysym("BackSpace")              }}, "hint-rollback"                   };
}

int
init_cfg(void) {
  cfg_default();
  return EXIT_SUCCESS;
}
