 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Joystick emulation for AmigaOS
  * 
  * Copyright 1996, 1997 Samuel Devulder
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "joystick.h"

int nr_joysticks;

#include <hardware/custom.h>
#include <hardware/cia.h>

#define CIAAPRA 0xBFE001 
#define CUSTOM  0xDFF000

static struct Custom *custom= (struct Custom*) CUSTOM;
static struct CIA *cia = (struct CIA *) CIAAPRA;

void read_joystick(int nr, unsigned int *dir, int *button)
{
    int bot, right, top, left, joy,fire;
    
    *dir = 0;
    *button = 0;
    if (nr >= nr_joysticks) return;

    joy   = custom->joy1dat;
    fire  = !( cia->ciapra & 0x0080 ) ? 1 : 0;

    right = (joy & 0x0002) ? 1 : 0;
    left  = (joy & 0x0200) ? 1 : 0;
    bot   = (joy & 0x0001) ? 1 : 0;
    top   = (joy & 0x0100) ? 1 : 0;
    
    *button = fire;
    *dir = bot | (right << 1) | (top << 8) | (left << 9);
}

void init_joystick(void)
{
    nr_joysticks = 1;
}

void close_joystick(void)
{
}

