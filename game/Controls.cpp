#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "controls.h"

// Button input
#define CLOCK 11
#define LATCHB 12
#define LATCHJ 13
#define DATA A4
#define BUTTONS 16 // total buttons

static volatile uint8_t *clockport, *latchbport, *latchjport, *dataport;
static uint8_t clockpin, latchbpin, latchjpin, datapin;

static uint16_t buttons; // buttons currently pressed
static unsigned long ticks;
static unsigned long last_update = 0;

void update(unsigned long delta); // update logic (ms since last update), enforced by ups / ticks

void game_set_ups(int ups)
{
    if (ups)
        ticks = 1000 / ups;
    else
        ticks = 1;
}

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
    clockport = portOutputRegister(digitalPinToPort(CLOCK));
    clockpin  = digitalPinToBitMask(CLOCK); 
    latchbport = portOutputRegister(digitalPinToPort(LATCHB));
    latchbpin  = digitalPinToBitMask(LATCHB); 
    latchjport = portOutputRegister(digitalPinToPort(LATCHJ));
    latchjpin  = digitalPinToBitMask(LATCHJ); 
    dataport = portInputRegister(digitalPinToPort(DATA));
    datapin  = digitalPinToBitMask(DATA);

    pinMode(CLOCK, OUTPUT);
    pinMode(LATCHB, OUTPUT);
    pinMode(LATCHJ, OUTPUT);
    pinMode(DATA, INPUT);
    
    digitalWrite(CLOCK, HIGH);
    digitalWrite(LATCHB, HIGH);
    digitalWrite(LATCHJ, LOW);
}

void controls_update()
{
    unsigned long cur_time = millis();

    if ((cur_time - last_update >= ticks)/* && step == 0*/)
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
        update(cur_time - last_update);
        last_update = cur_time;
    }
}
