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

#define FN1_MAC MO(MAC_FN1)
#define FN1_WIN MO(WIN_FN1)

// ---- Tap dance: tap = Cmd+C, hold = FN2 layer ----

enum td_keycodes {
    TD_CMDCPY_FN2
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
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,                KC_T,     KC_Y,     KC_U,                    KC_I,                 KC_O,     KC_P,      KC_LBRC,  KC_RBRC,            KC_BSLS,            KC_DEL,
        KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,                KC_G,               KC_H,                    KC_J,                 KC_K,     KC_L,      KC_SCLN,  KC_QUOT,            KC_ENT,             QK_BOOT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,                KC_V,     KC_B,     KC_TAB,                  KC_N,                 KC_M,     KC_COMM,   KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,           MT(MOD_MEH, KC_SPC),                     MO(LAYER_NAVIGATION),    MO(LAYER_SYMBOLS),              KC_SPC,              TD(TD_CMDCPY_FN2),            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [LAYER_NAVIGATION] = LAYOUT_ansi_69(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,  KC_EQL,   KC_BSPC,            KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_DEL,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,      KC_SCLN,  KC_QUOT,  KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_BSPC,  KC_N,     KC_M,     KC_COMM,   KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,            KC_SPC,             KC_TRNS,  KC_TRNS,            KC_SPC,              TD(TD_CMDCPY_FN2),  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [LAYER_SYMBOLS] = LAYOUT_ansi_69(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,   KC_VOLD,  KC_VOLU,  _______,            UG_TOGG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,            _______,  _______,  _______,  _______,   _______,  _______,  _______,            KC_END,
        _______,            UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,            _______,            _______,  _______,            _______,             _______,            _______,  _______,  _______),

    [LAYER_META] = LAYOUT_ansi_69(
        KC_TILD,  BT_HST1,  BT_HST2,  BT_HST3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,    KC_F11,   KC_F12,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  BAT_LVL,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        _______,  _______,  _______,            _______,            _______,  _______,            _______,             _______,            _______,  _______,  _______),

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

void cmdcpy_fn2_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            tap_code16(LGUI(KC_C)); // Cmd+C
            break;
        case TD_SINGLE_HOLD:
            layer_on(LAYER_META);
            break;
        default:
            break;
    }
}

void cmdcpy_fn2_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_off(LAYER_META);
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
    [LAYER_META]       = {ENCODER_CCW_CW(_______, _______)},
    [LAYER_ADVANCED]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};
#endif // ENCODER_MAP_ENABLE

tap_dance_action_t tap_dance_actions[] = {
    [TD_CMDCPY_FN2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cmdcpy_fn2_finished, cmdcpy_fn2_reset),
};
