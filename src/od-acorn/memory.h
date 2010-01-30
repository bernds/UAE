 /*
  * UAE - The Un*x Amiga Emulator
  *
  * See if this OS has mmap or equivalent
  *
  * Copyright 1996 Bernd Schmidt
  */

#undef USE_MAPPED_MEMORY
#undef CAN_MAP_MEMORY

#define OS_WITHOUT_MEMORY_MANAGEMENT
#ifndef SAVE_MEMORY
#define SAVE_MEMORY
#endif
