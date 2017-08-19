#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "controls.h"

#define BUTTONS 16 // total buttons

static volatile uint8_t *clockport, *latchbport, *latchjport, *dataport;
static uint8_t clockpin, latchbpin, latchjpin, datapin;

static uint16_t buttons; // buttons currently pressed

bool game_is_button_pressed(uint8_t button)
{
    return (buttons >> button) & 1;
}

bool game_is_any_button_pressed(uint16_t bitmask)
{
    return (buttons & bitmask);
}

void controls_setup()
{
    clockport = portOutputRegister(digitalPinToPort(CTL_CLOCK));
    clockpin  = digitalPinToBitMask(CTL_CLOCK); 
    latchbport = portOutputRegister(digitalPinToPort(CTL_LATCHB));
    latchbpin  = digitalPinToBitMask(CTL_LATCHB); 
    latchjport = portOutputRegister(digitalPinToPort(CTL_LATCHJ));
    latchjpin  = digitalPinToBitMask(CTL_LATCHJ); 
    dataport = portInputRegister(digitalPinToPort(CTL_DATA));
    datapin  = digitalPinToBitMask(CTL_DATA);

    pinMode(CTL_CLOCK, OUTPUT);
    pinMode(CTL_LATCHB, OUTPUT);
    pinMode(CTL_LATCHJ, OUTPUT);
    pinMode(CTL_DATA, INPUT);
    
    digitalWrite(CTL_CLOCK, HIGH);
    digitalWrite(CTL_LATCHB, HIGH);
    digitalWrite(CTL_LATCHJ, LOW);
}

void controls_update()
{
    cli();
    // update button state
    *latchjport |= latchjpin;
    *latchjport &= ~latchjpin;
    *latchbport &= ~latchbpin;
    *latchbport |= latchbpin;
    for (uint8_t button = 0 ; button < BUTTONS ; ++button)
    {
        *clockport &= ~clockpin;
        *clockport |= clockpin;
        if (*dataport & datapin)
        {
            buttons &= ~(1 << button);
        }
        else
        {
            buttons |= (1 << button);
        }
    }
    sei();
}

