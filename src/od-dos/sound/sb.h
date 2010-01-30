 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS Sound Blaster interface.
  *
  * (c) 1996 Peter Remmers
  */

/* internally used types */
typedef UWORD          BOOL;

typedef enum
{
  SB_Type_StdSB,
  SB_Type_SbPro,
  SB_Type_SB16
}
tSBType;

extern tSBType SB_Type;

extern volatile int IsPlaying;
extern int CurrentBuffer;

BOOL SB_Reset (void);
inline void SB_WriteDSP (UBYTE value);
inline UBYTE SB_ReadDSP (void);
inline void SB_WriteMixer (UBYTE reg, UBYTE data);
inline UBYTE SB_ReadMixer (UBYTE reg);
BOOL SB_DetectMixer (void);
void SB_Play_8Bit_Std (UWORD bytes);
void SB_Play_8Bit_SbPro (UWORD bytes);
void SB_Play_8Bit_SB16 (UWORD bytes);
void SB_Play_16Bit (UWORD bytes);
void SB_IntHandler (void);
void SB_SetRate_LoSpeed (UWORD rate);
void SB_SetRate_HiSpeed (UWORD rate);
void SB_SetRate_SB16 (UWORD rate);
int SB_SetRate (unsigned short rate);
int SB_SetBits (unsigned char bits);
int SB_SetChannels (unsigned char channels);
int SB_SetSigned (int sign);
void SB_SpeakerOff(void);
void SB_SpeakerOn(void);
void SB_SetVolume(UBYTE vol);
void irq2 (void);
void irq5 (void);
void irq7 (void);
void irq10 (void);
int SB_Detect (unsigned short *base, unsigned short *irq, unsigned short *dmalo, unsigned short *dmahi);
int SB_InitBuffers(int bufsize);
int SB_Init (unsigned short base, unsigned short irq, unsigned short dmalo, unsigned short dmahi);
void SB_Done (void);
void SB_DirectWrite(unsigned int size, int freq);
void SB_GetVersion (unsigned char *Maj, unsigned char *Min);
int SB_DetectInitSound(int *dspbits, int *dsprate, int *sndbufsize, unsigned int *direct_buffers, int *stereo_sound);
