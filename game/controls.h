#ifndef CONTROLS_H
#define CONTROLS_H

////////////////
// BUTTONS
////////////////


// Physical buttons
#define BUTTON_SW 6
#define BUTTON_NW 5
#define BUTTON_SE 3
#define BUTTON_NE 4

// Joystick buttons
#define BUTTON_UP 11
#define BUTTON_LEFT 13
#define BUTTON_DOWN 12
#define BUTTON_RIGHT 14
#define BUTTON_SELECT 9
#define BUTTON_START 10
#define BUTTON_A 7
#define BUTTON_B 8

#define BITMASK(x) (1 << (x))

bool game_is_button_pressed(uint8_t button);
bool game_is_any_button_pressed(uint16_t bitmask);

// Internal functions
void controls_setup();
void controls_update();

#endif

