 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Joystick emulation stubs
  * 
  * Copyright 1997 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "joystick.h"

int nr_joysticks;

void read_joystick(int nr, unsigned int *dir, int *button)
{
    *dir = 0;
    *button = 0;
}

void init_joystick(void)
{
    nr_joysticks = 0;
}

void close_joystick(void)
{
}

