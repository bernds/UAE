 /*
  * UAE - The Un*x Amiga Emulator
  *
  *  DOS Gravis Ultrasound interface - Headerfile
  *
  * (c) 1996 Michael Sontheimer
  */


int GUS_Init( int *dspbits, int *rate, int *sndbufsize, unsigned int *direct_buffers, int *stereo_sound );
void GUS_Reset( UBYTE ubGUSDMA1, UBYTE ubGUSDMA2, UBYTE ubGUSGF1Int );
void GUS_PlayBuffer( void *buf, unsigned long size );
void GUS_Cleanup( void );
void GUS_StartDMA( ULONG ulPosInGUSRAM, int bLoop );
void GUS_DummyFunc( void );
void GUS_GF1InterruptHandler( void );
void GUS_WorkOnVoices( void );
void GUS_GF1Delay( void );
void GUS_StartVoice( void );
void GUS_StopVoice( void );

