#include QMK_KEYBOARD_H

// Use disabled BL keycodes because they will not be processed by quantum

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT(BL_ON, KC_NO, KC_NO,
               KC_NO, BL_OFF, KC_NO,
               KC_NO, KC_NO, BL_TOGG),

  [1] = LAYOUT(TO(0)  , RGB_HUD, RGB_HUI,
               TO(0)  , RGB_SAD, RGB_SAI,
               BL_STEP, RGB_VAD, RGB_VAI),

};

const uint16_t PROGMEM rgb_combo[] = {BL_ON, BL_OFF, BL_TOGG, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  [0] = COMBO_ACTION(rgb_combo),
};

// Toggle layer 1 for combo
void process_combo_event(uint16_t combo_index, bool pressed) {
  if (!pressed) {
    layer_invert(1);
  }
}

uint8_t rc_to_index[MATRIX_ROWS][MATRIX_COLS] = {{2, 1, 0}, {5, 4, 3}, {8, 7, 6}};
uint8_t led_state[RGBLED_NUM] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // Start on
#define STATE_NUM 5

void update_led(uint8_t index, uint8_t state) {
  uint8_t hue = rgblight_get_hue();
  switch (state) {
    case 0: // Off
      rgblight_sethsv_at(0, 0, 0, index);
      return;
    case 1: // Red default
      break;
    case 2: // Yellow default
      hue += 13;
      break;
    case 3: // Green default
      hue += 65;
      break;
    case 4:  // Blue default
      hue += 170;
      break;
    default:
      break;
  }
  rgblight_sethsv_at(hue, rgblight_get_sat(), rgblight_get_val(), index);
}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
    case 0:  // Set all LEDs to saved state
      for (uint8_t index = 0; index < RGBLED_NUM; ++index) {
        update_led(index, led_state[index]);
      }
      break;
    case 1: // Set all LEDs on to saved color
      rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
      break;
    default:
      break;
  }
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (keycode == BL_STEP) {
    if (record->event.pressed) {
       // Reset color and LED state
       rgblight_sethsv(0, 255, 255);
       for (uint8_t i = 0; i < RGBLED_NUM; ++i) {
         led_state[i] = 1;
       }
    }
    return false;
  }
  if(get_highest_layer(layer_state) == 0) {
    if(record->event.pressed) {
      // Cycle the state of the pressed key
	    const uint8_t index = rc_to_index[record->event.key.row][record->event.key.col];
      ++(led_state[index]);
      if (led_state[index] == STATE_NUM) {
        led_state[index] = 0;
      }
      update_led(index, led_state[index]);
    }
  }
  return true;
}