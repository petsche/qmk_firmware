#include QMK_KEYBOARD_H
#include "bigswitch.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(LCTL(KC_M)),
};

// Override default init_user
void matrix_init_user(void) {
  setPinOutput(B0);
  setPinOutput(D5);
}

// Toggle LED on each press for fun
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    togglePin(B0);
    togglePin(D5);
  }
  return true;
}
