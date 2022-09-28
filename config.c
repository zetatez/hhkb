
/* See LICENSE file for copyright and license details. */

#include "config.h"

extern cfg_t cfg;

int cfg_parse_mod(char *modstr) {
    int mod_mask;
    if (!strlen(modstr)) { mod_mask = NoSymbol; }
    else if (!strcmp(modstr, "Control") || !strcmp(modstr, "Control_L") || !strcmp(modstr, "Control_R" )) { mod_mask = ControlMask ; }
    else if (!strcmp(modstr, "Alt"    ) || !strcmp(modstr, "Alt_L"    ) || !strcmp(modstr, "Alt_R"     )) { mod_mask = Mod1Mask    ; }
    else if (!strcmp(modstr, "Super"  ) || !strcmp(modstr, "Super_L"  ) || !strcmp(modstr, "Super_R"   )) { mod_mask = Mod4Mask    ; }
    else if (!strcmp(modstr, "Shift"  ) || !strcmp(modstr, "Shift_L"  ) || !strcmp(modstr, "Shift_R"   )) { mod_mask = ShiftMask   ; }
    else if (!strcmp(modstr, "mod1"                                                                    )) { mod_mask = Mod1Mask    ; }
    else if (!strcmp(modstr, "mod2"                                                                    )) { mod_mask = Mod2Mask    ; }
    else if (!strcmp(modstr, "mod3"                                                                    )) { mod_mask = Mod3Mask    ; }
    else if (!strcmp(modstr, "mod4"                                                                    )) { mod_mask = Mod4Mask    ; }
    else if (!strcmp(modstr, "mod5"                                                                    )) { mod_mask = Mod5Mask    ; }
    else { mod_mask = 0; }
    return mod_mask;
}

void cfg_parse_comb_key(char *val, int *mod_mask, KeySym *key) {
    int mask = 0;
    KeySym ks = NoSymbol;
    char *m1, *m2, *k, *tmp;
    m1 = m2 = NULL;

    k = val + strlen(val) - 1;
    while(k > val) {
        if (*k == '+') { tmp = k; k++; *tmp = '\0'; break; }
        k--;
    }

    if (k == val) val = "";

    if (*val != '\0') {
        m1 = val;
        while(*val != '\0'){
            if (*val == '|') { tmp = val; val++; m2 = val; *tmp = '\0'; }
            if (m1 && m2) break;
            val++;
        }
    }

    if (*k != '\0')
        ks = XStringToKeysym(k);

    if (m1)
        mask = cfg_parse_mod(m1);

    if (m1 && m2)
        mask |= cfg_parse_mod(m2);

    if (!m1 && !m2)
        mask = cfg_parse_mod("");

    *mod_mask = mask;
    *key = ks;
}

int cfg_set_item(char *desc, char *val) {
    int mod_mask = 0;
    KeySym key = NoSymbol;
    if (!strcmp(desc, "font"                                 )) { val++; val[strlen(val)-1] = '\0'; memcpy(cfg.font.t.s, val, strlen(val)*sizeof(char))                            ; return 0; }
    else if (!strcmp(desc, "cursor-rate-factor"              )) { char *leftover; cfg.cursor_rate_factor.t.l = strtol(val, &leftover, 10)                                          ; return 0; }
    else if (!strcmp(desc, "cursor-min-speed"                )) { cfg.cursor_min_speed.t.i = atoi(val)                                                                             ; return 0; }
    else if (!strcmp(desc, "cursor-max-speed"                )) { cfg.cursor_max_speed.t.i = atoi(val)                                                                             ; return 0; }
    else if (!strcmp(desc, "cursor-size-width"               )) { cfg.cursor_size_width.t.i = atoi(val)                                                                            ; return 0; }
    else if (!strcmp(desc, "cursor-size-hegiht"              )) { cfg.cursor_size_height.t.i = atoi(val)                                                                           ; return 0; }
    else if (!strcmp(desc, "cursor-bg-color"                 )) { val++; val[strlen(val)-1] = '\0'; memcpy(cfg.cursor_bg_color.t.s, val, strlen(val)*sizeof(char))                 ; return 0; }
    else if (!strcmp(desc, "hint-window-transparency"        )) { char *endp; cfg.hint_window_transparency.t.d = strtod(val, &endp)                                                ; return 0; }
    else if (!strcmp(desc, "hint-bg-color"                   )) { val++; val[strlen(val)-1] = '\0'; memcpy(cfg.hint_bg_color.t.s, val, strlen(val)*sizeof(char))                   ; return 0; }
    else if (!strcmp(desc, "hint-letters"                    )) { val++; val[strlen(val)-1] = '\0'; memcpy(cfg.hint_letters.t.s, val, strlen(val)*sizeof(char))                    ; return 0; }
    else if (!strcmp(desc, "hint-rect-width"                 )) { cfg.hint_rect_width.t.i = atoi(val)                                                                              ; return 0; }
    else if (!strcmp(desc, "hint-rect-height"                )) { cfg.hint_rect_height.t.i = atoi(val)                                                                             ; return 0; }
    else if (!strcmp(desc, "hint-rect-font-size"             )) { cfg.hint_rect_font_size.t.i = atoi(val)                                                                          ; return 0; }
    else if (!strcmp(desc, "cover-window-transparency"       )) { char *endp; cfg.cover_window_transparency.t.d = strtod(val, &endp)                                               ; return 0; }
    else if (!strcmp(desc, "cover-bg-color"                  )) { val++; val[strlen(val)-1] = '\0'; memcpy(cfg.cover_bg_color.t.s, val, strlen(val)*sizeof(char))                  ; return 0; }
    else if (!strcmp(desc, "quit"                            )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.quit.t.k                            = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "restart"                         )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.restart.t.k                         = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "set-to-norm-mode"                )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.set_to_norm_mode.t.k                = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "set-to-hint-mode"                )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.set_to_hint_mode.t.k                = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "set-to-cover-mode"               )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.set_to_cover_mode.t.k               = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "escape-norm"                     )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.escape_norm.t.k                     = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "escape-hint"                     )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.escape_hint.t.k                     = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "escape-cover"                    )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.escape_cover.t.k                    = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "move-left"                       )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.move_left.t.k                       = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "move-down"                       )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.move_down.t.k                       = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "move-up"                         )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.move_up.t.k                         = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "move-right"                      )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.move_right.t.k                      = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-up"                )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_up.t.k                = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-middle-vertical"   )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_middle_vertical.t.k   = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-down"              )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_down.t.k              = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-begin"             )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_begin.t.k             = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-end"               )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_end.t.k               = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-middle-horizontal" )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_middle_horizontal.t.k = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-center"            )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_center.t.k            = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-upleft"            )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_upleft.t.k            = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-bottomleft"        )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_bottomleft.t.k        = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-bottomright"       )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_bottomright.t.k       = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-vim-move-upright"           )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_vim_move_upright.t.k           = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-select"                     )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_select.t.k                     = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-select-line"                )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_select_line.t.k                = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-scroll-up"                  )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_scroll_up.t.k                  = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-scroll-down"                )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_scroll_down.t.k                = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-scroll-left"                )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_scroll_left.t.k                = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "norm-scroll-right"               )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.norm_scroll_right.t.k               = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "cursor-left-click"               )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.cursor_left_click.t.k               = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "cursor-middle-click"             )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.cursor_middle_click.t.k             = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "cursor-rihgt-click"              )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.cursor_right_click.t.k              = (comb_key_t){ mod_mask, key }; return 0; }
    else if (!strcmp(desc, "hint-rollback"                   )) { cfg_parse_comb_key(val, &mod_mask, &key); cfg.hint_rollback.t.k                   = (comb_key_t){ mod_mask, key }; return 0; }
    else { // not found
        return 1;
    }

    return 0;
}

// cfg load and parse config file
void cfg_load_and_parse_config_file(const char *file) {
    const int maxlen = 256;
    FILE *fp = NULL;
    char line[maxlen];
    char *desc, *val;

    fp = fopen(file, "r");
    if (!fp) return;

    while(!feof(fp)) {
        fgets(line, maxlen, fp);
        char *l = strim(line);

        if (l[0] == '#' || l[0] == '\0')
            continue;

        if (startwith(l, "map ")) {
            l = l+4;
            val = l;
            desc = l + strlen(l) - 1;
            while(!isspace(*desc) && desc > l) desc--;
            if (*desc != '\0')
                desc++;
            while(!isspace(*l)) l++;
            *l = '\0';
        } else {
            desc = l;
            val = l;
            while(!isspace(*val)) val++;
            while(isspace(*val)) val++;
            while(!isspace(*l)) l++;
            *l = '\0';
        }
        cfg_set_item(desc, val);
        desc = val = "";
    }
    fclose(fp);
}

// cfg source config file
void cfg_source_config_file(void) {
    int len = 1024;
    char *file = (char*)malloc(len*sizeof(char));

    char *home_dir = getenv("HOME");
    char *xdg_config_home = getenv("XDG_CONFIG_HOME");

    cfg_load_and_parse_config_file("/etc/hhkbrc");

    if (home_dir) {
        if (sprintf(file, "%s/.hhkbrc", home_dir) >= 0)
            cfg_load_and_parse_config_file(file);
    }

    if (xdg_config_home) {
        if (sprintf(file, "%s/hhkb/hhkbrc", xdg_config_home) >= 0)
            cfg_load_and_parse_config_file(file);
    } else {
        if (home_dir)
            if (sprintf(file, "%s/.config/hhkb/hhkbrc", home_dir) >= 0)
                cfg_load_and_parse_config_file(file);
    }

    free(file);
}

void cfg_default(void) {
    cfg.font                            = (cfg_item_t){{ .s = "-misc-dejavu sans-medium-o-normal--0-0-0-0-p-0-adobe-standard" }, "font"                            };
    cfg.cursor_rate_factor              = (cfg_item_t){{ .l = 1000000000000000                                                }, "cursor-rate-factor"              };
    cfg.cursor_min_speed                = (cfg_item_t){{ .i = 8                                                               }, "cursor-min-speed"                };
    cfg.cursor_max_speed                = (cfg_item_t){{ .i = 128                                                             }, "cursor-max-speed"                };
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

int init_cfg(void) {
    cfg_default();
    cfg_source_config_file();
    return EXIT_SUCCESS;
}
