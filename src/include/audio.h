 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Sound emulation stuff
  *
  * Copyright 1995, 1996, 1997 Bernd Schmidt
  */

#define PERIOD_MAX ULONG_MAX

extern struct audio_channel_data {
    unsigned long adk_mask;
    unsigned long evtime;
    unsigned long per;
    uae_u8 dmaen, intreq2, data_written;
    uaecptr lc, pt;
    int state, wper;
    unsigned int wlen;
    int current_sample, last_sample;
    int vol;
    int *voltbl;
    uae_u16 dat, nextdat, len;
} audio_channel[];

extern void aud0_handler (void);
extern void aud1_handler (void);
extern void aud2_handler (void);
extern void aud3_handler (void);

extern void AUDxDAT (int nr, uae_u16 value);
extern void AUDxVOL (int nr, uae_u16 value);
extern void AUDxPER (int nr, uae_u16 value);
extern void AUDxLCH (int nr, uae_u16 value);
extern void AUDxLCL (int nr, uae_u16 value);
extern void AUDxLEN (int nr, uae_u16 value);

extern int init_audio (void);
extern void ahi_install (void);
extern void audio_reset (void);
extern void update_audio (void);
extern void schedule_audio (void);
extern void audio_evhandler (void);
extern void audio_channel_enable_dma (struct audio_channel_data *cdp);
extern void audio_channel_disable_dma (struct audio_channel_data *cdp);
