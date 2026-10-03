/* Copyright 2024 ~ 2026 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    LAYER_BASE,
    LAYER_NAVIGATION,
    LAYER_SYMBOLS,
    LAYER_META,
    LAYER_ADVANCED,
};

enum td_keycodes {
    TD_CMDCOPY_FN1,
    TD_CMDPASTE_FN2
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
} td_state_t;

static td_state_t td_state;

td_state_t cur_dance(tap_dance_state_t *state);
void cmdcpy_fn2_finished(tap_dance_state_t *state, void *user_data);
void cmdcpy_fn2_reset(tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_BASE] = LAYOUT_ansi_69(
        KC_GRAVE, KC_1,     KC_2,     KC_3,     KC_4,                KC_5,     KC_6,     KC_7,                    KC_8,                 KC_9,     KC_0,      KC_MINS,  KC_EQL,             MO(LAYER_META),     KC_MEDIA_PLAY_PAUSE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,                KC_T,     KC_Y,     KC_U,                    KC_I,                 KC_O,     KC_P,      KC_LBRC,  KC_RBRC,            KC_BSLS,            TO(LAYER_BASE),
        KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,                KC_G,               KC_H,                    KC_J,                 KC_K,     KC_L,      KC_SCLN,  KC_QUOT,            KC_ENT,             KC_TRNS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,                KC_V,     KC_B,     KC_TAB,                  KC_N,                 KC_M,     KC_COMM,   KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,           MT(MOD_MEH, KC_SPC),                     MO(LAYER_NAVIGATION),    MO(LAYER_SYMBOLS),    KC_SPC,              KC_TRNS,            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [LAYER_NAVIGATION] = LAYOUT_ansi_69(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,           KC_9,     KC_0,          KC_MINS,  KC_EQL,   KC_BSPC,            KC_MUTE,
        KC_TAB,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_T,     KC_HOME,  KC_PGDN,  KC_PGUP,        KC_END,   KC_P,          KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_DEL,
        KC_CAPS,  KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_G,               KC_LEFT,  KC_DOWN,        KC_UP,    KC_RGHT,       KC_SCLN,  KC_QUOT,  KC_ENT,             KC_HOME,
        KC_LSFT,            KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_B,     KC_BSPC,  LALT(KC_BSPC),  KC_DEL,   LALT(KC_DEL),  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,            KC_SPC,             KC_TRNS,  KC_TRNS,                  KC_SPC,                  KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [LAYER_SYMBOLS] = LAYOUT_ansi_69(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,   KC_VOLD,  KC_VOLU,  _______,            UG_TOGG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
        UG_TOGG,  KC_LPRN,  KC_RPRN,  KC_LCBR,  KC_RCBR,  UG_SPDU,            _______,  _______,  _______,  _______,   _______,  _______,  _______,            KC_END,
        _______,            KC_LBRC,  KC_RBRC,  KC_LT,    KC_GT,    UG_SPDD,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,            _______,            _______,  _______,            _______,             _______,            _______,  _______,  _______),

    [LAYER_META] = LAYOUT_ansi_69(
        KC_TILD,  BT_HST1,  BT_HST2,  BT_HST3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,    KC_F11,   KC_F12,  _______,            UG_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,            KC_TRNS,
        _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  BAT_LVL,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        QK_BOOT,  _______,  _______,            _______,            _______,  _______,            _______,             _______,            _______,  _______,  _______),

    [LAYER_ADVANCED] = LAYOUT_ansi_69(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,   KC_VOLD,  KC_VOLU,  _______,            UG_TOGG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,            _______,  _______,  _______,  _______,   _______,  _______,  _______,            KC_END,
        _______,            UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,            _______,            _______,  _______,            _______,             _______,            _______,  _______,  _______),

 };

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return TD_SINGLE_TAP;
        else
            return TD_SINGLE_HOLD;
    }
    return TD_UNKNOWN;
}

void cmdcopy_fn1_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            tap_code16(LGUI(KC_C));  // Cmd+C
            break;
        case TD_SINGLE_HOLD:
            layer_on(LAYER_NAVIGATION);
            break;
        default:
            break;
    }
}

void cmdcopy_fn1_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_off(LAYER_NAVIGATION);
            break;
        default:
            break;
    }
    td_state = TD_NONE;
}

void cmdpaste_fn2_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            tap_code16(LGUI(KC_V));  // Cmd+V
            break;
        case TD_SINGLE_HOLD:
            layer_on(LAYER_SYMBOLS);
            break;
        default:
            break;
    }
}

void cmdpaste_fn2_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_off(LAYER_SYMBOLS);
            break;
        default:
            break;
    }
    td_state = TD_NONE;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_NAVIGATION] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_SYMBOLS]    = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [LAYER_META]       = {ENCODER_CCW_CW(UG_HUEU, UG_HUED)},
    [LAYER_ADVANCED]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};
#endif // ENCODER_MAP_ENABLE

tap_dance_action_t tap_dance_actions[] = {
    [TD_CMDCOPY_FN1]    = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cmdcopy_fn1_finished, cmdcopy_fn1_reset),
    [TD_CMDPASTE_FN2]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cmdpaste_fn2_finished, cmdpaste_fn2_reset),
};

// ============================================================================
// Chord table: up to 3 keys. If all non-KC_NO keys are down within
// CHORD_WINDOW_MS of each other (any order, presses or releases),
// those keys are swallowed and the row's output string is typed.
// Otherwise those keys fire their own taps as usual.
// ============================================================================

#define CHORD_WINDOW_MS 15

typedef struct {
    uint16_t keys[3];     // up to 3 keys; use KC_NO to pad unused slots
    const char *output;
} chord_t;

static const chord_t CHORD_TABLE[] = {
    { {KC_P, KC_U, KC_NO}, "partial " },
    { {KC_P, KC_U, KC_NO}, "public " },
    { {KC_P, KC_R, KC_NO}, "private " },
    { {KC_P, KC_O, KC_NO}, "protected " },
    { {KC_S, KC_T, KC_NO}, "static " },
    { {KC_S, KC_R, KC_NO}, "struct " },
    { {KC_V, KC_O, KC_NO}, "void " },
    { {KC_R, KC_D, KC_NO}, "readonly " },
    { {KC_C, KC_N, KC_NO}, "const " },
    { {KC_C, KC_L, KC_NO}, "class " },
    { {KC_I, KC_N, KC_T},  "internal " },
};

#define CHORD_MAX_INFLIGHT 8

typedef struct {
    uint16_t keycode;
    uint16_t down_at;     // timer_read() at keypress
    bool     down;        // physically held?
    bool     pending;     // tap owed? false once emitted or chord-consumed
    bool     consumed;    // chord ate this key; swallow release
    bool     hold_asserted; // released-on-release? we registered this key as held
} inflight_key_t;

static inflight_key_t inflight[CHORD_MAX_INFLIGHT];

static inflight_key_t *find_inflight(uint16_t keycode) {
    for (int i = 0; i < CHORD_MAX_INFLIGHT; i++) {
        if (inflight[i].down || inflight[i].pending) {
            if (inflight[i].keycode == keycode) return &inflight[i];
        }
    }
    return NULL;
}

static void clear_inflight(inflight_key_t *s) {
    s->down = false;
    s->pending = false;
    s->consumed = false;
    s->hold_asserted = false;
    s->keycode = KC_NO;
}

static bool key_in_table(uint16_t keycode) {
    for (size_t r = 0; r < sizeof(CHORD_TABLE) / sizeof(CHORD_TABLE[0]); r++) {
        for (int i = 0; i < 3; i++) {
            if (CHORD_TABLE[r].keys[i] == keycode && keycode != KC_NO) return true;
        }
    }
    return false;
}

static uint32_t chord_decide(uint32_t trigger_time, void *arg) {
    // Window elapsed: emit any still-pending taps as their own keycodes.
    for (int i = 0; i < CHORD_MAX_INFLIGHT; i++) {
        inflight_key_t *s = &inflight[i];

        if (s->pending && !s->consumed) {
            tap_code(s->keycode);
        }

        if (!s->down)
            clear_inflight(s);   // release happened later, sweep
        else {
            s->pending = false; s->consumed = false;
        }
    }
    return 0;
}

static void try_fire_chords(void) {
    uint16_t now = timer_read();
    for (size_t r = 0; r < sizeof(CHORD_TABLE) / sizeof(CHORD_TABLE[0]); r++) {
        const chord_t *c = &CHORD_TABLE[r];
        int need = 0;      // how many keys the row needs
        int got  = 0;      // how many of them are "active"

        for (int i = 0; i < 3; i++) {
            uint16_t k = c->keys[i];
            if (k == KC_NO || k == 0) continue;
            need++;

            inflight_key_t *s = find_inflight(k);
            if (s && (s->down || s->pending)) {
                uint16_t age = (uint16_t)(now - s->down_at);
                if (age <= CHORD_WINDOW_MS) got++;
            }
        }

        if (got == need && need >= 2) {
            // This chord fires.
            for (int i = 0; i < 3; i++) {
                uint16_t k = c->keys[i];
                if (k == KC_NO || k == 0) continue;
                inflight_key_t *s = find_inflight(k);
                if (s) {
                    s->pending = false;   // do not emit this key's tap
                    s->consumed = true;   // release will be swallowed
                }
            }
            send_string(c->output);
            return;
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!key_in_table(keycode)) return true;

    inflight_key_t *s = find_inflight(keycode);
    if (record->event.pressed) {
        if (!s) {
            // see if we have room
            s = NULL;
            for (int i = 0; i < CHORD_MAX_INFLIGHT; i++)
                if (!inflight[i].down && !inflight[i].pending) { s = &inflight[i]; break; }
            if (!s) return true; // give up tracking this one
            s->keycode = keycode;
        }
        s->down = true;
        s->down_at = timer_read();
        s->pending = true;
        s->consumed = false;
        defer_exec(CHORD_WINDOW_MS, chord_decide, NULL);
        try_fire_chords();
        return false;
    } else {
        if (s) {
            if (s->consumed) {
                clear_inflight(s);
            } else if (s->pending) {
                tap_code(s->keycode);
                clear_inflight(s);
            } else if (s->hold_asserted) {
                unregister_code(keycode);
                clear_inflight(s);
            } else {
                clear_inflight(s);
            }
        }
        return false;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, LAYER_NAVIGATION, LAYER_SYMBOLS, LAYER_ADVANCED);
}
