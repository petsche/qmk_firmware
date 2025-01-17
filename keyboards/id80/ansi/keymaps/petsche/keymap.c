/* Copyright 2020 Sergey Vlasov <sigprof@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,     KC_F9,   KC_F10,  KC_F11,  KC_F12,     MO(1),     KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,            KC_HOME,
        KC_TAB,      KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,    KC_LBRC, KC_RBRC, KC_BSLS,        KC_END,
        KC_BSPC,       KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,
        KC_LCTL,   MO(1),   KC_LALT,                       KC_SPC,                              KC_RALT,     KC_RGUI,        KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        RESET,     _______, _______, _______, _______,   _______, _______, _______, _______,   _______, KC_PSCR, KC_SLCK, KC_PAUS,    KC_TRNS,   KC_INS,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            KC_PGUP,
        _______,     RGB_TOG, RGB_MOD, RGB_RMOD, _______, _______, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______,        KC_PGDN,
        KC_CAPS,       RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, KC_LEFT, NK_TOGG, _______, _______, _______, _______, _______,
        _______,            RGB_HUD, RGB_SAD, RGB_VAD,  _______, _______,  KC_UP, _______, _______, _______, _______, _______,          _______,
        _______,   KC_TRNS,   _______,                      _______,                              _______,     _______,        _______, _______,  _______
    ),
};

static uint8_t x = 0;
static uint8_t a,b,c;
uint8_t fast_rand(void) {
  ++x;
  a = (a^c^x);
  b = (b+a);
  c = ((c+(b>>1))^a);
  return(c);
}

static bool init = false;
static uint8_t static_hue = 0;
static bool rand_tog = false;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!init) {
    static_hue = rgblight_get_hue();
	init = true;
  }
  if (!record->event.pressed) {
    switch (keycode) {
      case RGB_MOD:
	  case RGB_RMOD:
      case RGB_HUI:
      case RGB_HUD:
        return false;
	  default:
	    return true;
    }
  }
  switch (keycode) {
    case RGB_MOD:
	  rand_tog = !rand_tog;
	  return false;
	case RGB_RMOD:
	  set_current_wpm(0);
	  rgblight_sethsv_noeeprom(static_hue, rgblight_get_sat(), rgblight_get_val());
	  return false;
    case RGB_HUI:
      rgblight_sethsv(static_hue, rgblight_get_sat(), rgblight_get_val());
      rgblight_increase_hue();
	  static_hue += RGBLIGHT_HUE_STEP;
      return false;
    case RGB_HUD:
      rgblight_sethsv(static_hue, rgblight_get_sat(), rgblight_get_val());
      rgblight_decrease_hue();
	  static_hue -= RGBLIGHT_HUE_STEP;
      return false;
	default:
	  break;
  }
  if (rand_tog) {
    rgblight_sethsv_noeeprom(fast_rand(), rgblight_get_sat(), rgblight_get_val());
    return true;
  }
  const uint8_t value = get_current_wpm() / 15;
  rgblight_sethsv_noeeprom(static_hue - value, rgblight_get_sat(), rgblight_get_val());
  return true;
}
