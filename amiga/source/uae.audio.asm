                         
;*** ScR *** *** NOTESff ***
;*  There is still room For improvements in the softint's. For example,
;* try If reducing Chip memory accesses increases speed. NOT high priority, though.
;*  This driver should probably have a DMA play mode For 1-4 stereo channels.
;*
;*  The sampler routines are just For fun. Since interrupts must NOT be disabled,
;* there are lots of clicks. But still, I have tried To sample at 22kHz While down-
;* loading files from a BBS On my A4000/040, AND it worked without any serial errors.
  MC68020
;------------
MINBUFFLEN EQU 2  ; the default minbuffer size envarc ahi_minbufflen 
                  ; can overwrite this 1=default size 2=buffer*2 ...                          
VERSION   EQU 4
recordbuffer EQU 65536
REVISION  EQU 2
DATE  Macro
    Dc.b  "22.5.02"
  ENDM
VERS  Macro
    Dc.b  "uae 4.3"
  ENDM
VSTRING Macro
    VERS
    Dc.b  " ("
    DATE
    Dc.b  ")",13,10,0
  ENDM
VERSTAG Macro
    Dc.b  0,"$VER: "
    VERS
    Dc.b  " ("
    DATE
    Dc.b  ")",0
  ENDM

  INCDIR  INCLUDE:

  INCLUDE hardware/all.i

  INCLUDE exec/initializers.i
  INCLUDE exec/resident.i
  INCLUDE exec/alerts.i
  INCLUDE exec/memory.i
  INCLUDE graphics/gfxbase.i
  INCLUDE utility/utility.i
  INCLUDE utility/hooks.i
  INCLUDE devices/audio.i
  INCLUDE dos/dos.i


  INCLUDE lvos/exec_lib.i
  INCLUDE lvos/utility_lib.i
  INCLUDE lvos/dos_lib.i
  INCLUDE lvos/cardres_lib.i

  INCLUDE devices/ahi.i
  INCLUDE libraries/ahi_sub.i
  INCLUDE lvos/ahi_sub_lib.i

  INCLUDE resources/misc.i
  INCLUDE resources/card.i
  INCLUDE macros.i

_ciaa   EQU $bfe001
_ciab   EQU $bfd000

PALFREQ   EQU 3546895
NTSCFREQ  EQU 3579545
MINPER    EQU 62




;* paula.audio extra tags
AHIDB_Paula14Bit  EQU AHIDB_UserBase+0  * Boolean
AHIDB_PaulaTable  EQU AHIDB_UserBase+1  * Boolean

 ;* paulaBase (private)
  STRUCTURE paulaBase,LIB_SIZE
  UBYTE pb_Flags
  UBYTE pb_Pad1
  UWORD pb_Pad2
  APTR  pb_SysLib
  ULONG pb_SegList
  APTR  pb_GfxLib
  APTR  pb_UtilLib
  APTR  pb_DosLib
  APTR  pb_MiscResource
  APTR  pb_CardResource
  LABEL paulaBase_SIZEOF

 ;* paula (private) ahiac_DriverData points to this structure.
  STRUCTURE paula,0
  UBYTE p_Flags
  UBYTE p_Parallel      ;TRUE if pport allocated
  UWORD p_DisableCount
  APTR  p_audioport
  APTR  p_audioreq
  ULONG p_audiodev
  APTR  p_DMAbuffer
  APTR  p_ParBitsUser
  APTR  p_ParPortUser
  APTR  p_CardHandle

  STRUCT  p_PlayInt,IS_SIZE
  STRUCT  p_PlaySoftInt,IS_SIZE
  STRUCT  p_RecInt,IS_SIZE
  STRUCT  p_RecSoftInt,IS_SIZE

  UWORD p_MonitorVolume
  UWORD p_Input

;*** IS_DATA for Play Interrupt (do not change order!)
  LABEL p_PlayIntData
  APTR  p_PlaySoftIntPtr
  UWORD p_AudLen      ;don't change order
  ULONG p_AudPer      ;
  UWORD p_DoubleBufferFlag
  APTR  p_AudPtr1A
  APTR  p_AudPtr2A
  APTR  p_AudPtr3A
  APTR  p_AudPtr4A
  APTR  p_AudPtr1B
  APTR  p_AudPtr2B
  APTR  p_AudPtr3B
  APTR  p_MinBufferLength

;*** IS_DATA for Play Software Interrupt (do not change order!)
  LABEL p_PlaySoftIntData
  APTR  p_PlayerHook
  ULONG p_Reserved
  APTR  p_AudioCtrlP
  FPTR  p_PlayerEntry     ;p_PlayerHook->h_Entry
  FPTR  p_PreTimer
  FPTR  p_PostTimer
  ULONG p_LoopTimes
  APTR  p_MixHook
  APTR  p_Mixbuffer
  APTR  p_AudioCtrlM
  FPTR  p_MixEntry      ;p_MixHook->h_Entry
  APTR  p_AudPtrs
  APTR  p_CalibrationTable

;*** IS_DATA for Aura Record Interrupt (do not change order!)
  LABEL p_RecIntDataAura
  APTR  p_AuraAddress
;*** IS_DATA for Paula Record Interrupt (do not change order!)
  LABEL p_RecIntData
  APTR  p_RecFillPtr
  UWORD p_RecFillCount
  UWORD p_Pad3
  APTR  p_RecBuffer1
  APTR  p_RecBuffer2
  APTR  p_RecSoftIntPtr

  LABEL p_RecordMessage
  ULONG p_rmType
  APTR  p_rmBuffer
  ULONG p_rmLength

  STRUCT  p_CalibrationArray,256
  LABEL paula_SIZEOF

;* p_Flags
  BITDEF  P,14BIT,0
PB_STEREO EQU AHIACB_STEREO   ;=2
PF_STEREO EQU AHIACF_STEREO

Start:
  MOVEQ #-1,d0
  RTS

RomTag:
  Dc.W  RTC_MATCHWORD
  Dc.L  RomTag
  Dc.L  EndCode
  Dc.B  RTF_AUTOINIT
  Dc.B  VERSION       ;version
  Dc.B  NT_LIBRARY
  Dc.B  0       ;pri
  Dc.L  LibName
  Dc.L  IDString
  Dc.L  InitTable

LibName:  Dc.b  "uae.audio",0
IDString: VSTRING
gfxName:  GRAPHICSNAME
utilName: UTILITYNAME
dosName:  DOSNAME
envVar:   Dc.b  "ahi_minbufflen",0
  cnop  0,2

InitTable:
  Dc.L  paulaBase_SIZEOF
  Dc.L  funcTable
  Dc.L  dataTable
  Dc.L  initRoutine

funcTable:
  Dc.l  Open
  Dc.l  Close
  Dc.l  Expunge
  Dc.l  Null
;*
  Dc.l  AHIsub_AllocAudio
  Dc.l  AHIsub_FreeAudio
  Dc.l  AHIsub_Disable
  Dc.l  AHIsub_Enable
  Dc.l  AHIsub_Start
  Dc.l  AHIsub_Update
  Dc.l  AHIsub_Stop
  Dc.l  AHIsub_SetVol
  Dc.l  AHIsub_SetFreq
  Dc.l  AHIsub_SetSound
  Dc.l  AHIsub_SetEffect
  Dc.l  AHIsub_LoadSound
  Dc.l  AHIsub_UnloadSound
  Dc.l  AHIsub_GetAttr
  Dc.l  AHIsub_HardwareControl
  Dc.l  -1

dataTable:
  INITBYTE  LN_TYPE,NT_LIBRARY
  INITLONG  LN_NAME,LibName
  INITBYTE  LIB_FLAGS,LIBF_SUMUSED!LIBF_CHANGED
  INITWORD  LIB_VERSION,VERSION
  INITWORD  LIB_REVISION,REVISION
  INITLONG  LIB_IDSTRING,IDString
  Dc.L    0

initRoutine:
  MOVEM.l d1/a0/a1/a5/a6,-(sp)
  MOVE.l  d0,a5
  MOVE.l  a6,pb_SysLib(a5)
  MOVE.l  a0,pb_SegList(a5)
  LEA gfxName(pc),a1
  MOVEQ #0,d0
  call OpenLibrary
  MOVE.l  d0,pb_GfxLib(a5)
  BNE.b .gfxOK
  ALERT AG_OpenLib!AO_GraphicsLib
  MOVEQ #0,d0
  BRA .exit
.gfxOK
  LEA utilName(pc),a1
  MOVEQ #0,d0
  call  OpenLibrary
  MOVE.l  d0,pb_UtilLib(a5)
  BNE.b .utilOK
  ALERT AG_OpenLib!AO_UtilityLib
  MOVEQ #0,d0
  BRA .exit
.utilOK
  LEA dosName(pc),a1
  MOVEQ #0,d0
  call  OpenLibrary
  MOVE.l  d0,pb_DosLib(a5)
  BNE.b .dosOK
  ALERT AG_OpenLib!AO_DOSLib
  MOVEQ #0,d0
  BRA .exit
.dosOK  MOVE.l  a5,d0
.exit
  MOVEM.l (sp)+,d1/a0/a1/a5/a6
  RTS

Open:
  MOVEQ #0,d0
  ADDQ.w  #1,LIB_OPENCNT(a6)
  BCLR.b  #LIBB_DELEXP,pb_Flags(a6)
  MOVE.l  a6,d0
.exit
  RTS

Close:
  MOVEQ #0,d0
  SUBQ.w  #1,LIB_OPENCNT(a6)
  BNE.b .exit
  BTST.b  #LIBB_DELEXP,pb_Flags(a6)
  BEQ.b .exit
  BSR Expunge
.exit
  RTS

Expunge:
  MOVEM.l d1/d2/a0/a1/a5/a6,-(sp)
  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6
  TST.w LIB_OPENCNT(a5)
  BEQ.b .notopen
  BSET.b  #LIBB_DELEXP,pb_Flags(a5)
  MOVEQ #0,d0
  BRA.b .Expunge_end
.notopen
  MOVE.l  pb_GfxLib(a5),a1
  call  CloseLibrary

  MOVE.l  pb_UtilLib(a5),a1
  call  CloseLibrary

  MOVE.l  pb_DosLib(a5),a1
  call  CloseLibrary

  MOVE.l  pb_SegList(a5),d2
  MOVE.l  a5,a1
  call  Remove

  MOVEQ #0,d0
  MOVE.l  a5,a1
  MOVE.w  LIB_NEGSIZE(a5),d0
  SUB.l d0,a1
  ADD.w LIB_POSSIZE(a5),d0
  call  FreeMem
  MOVE.l  d2,d0
.Expunge_end
  MOVEM.l (sp)+,d1/d2/a0/a1/a5/a6
  RTS

Null:
  MOVEQ #0,d0
  RTS


;****** [driver].audio/--background-- ****************************************
;*
;*   OVERVIEW
;*
;*       AUDIO ID NUMBERS
;*
;*       Just some notes about selecting ID numbers for different modes:
;*       It is up to the driver programmer to chose which modes should be
;*       available to the user. Take care when selecting.
;*       The upper word is the hardware ID, and can only be allocated by
;*       Martin Blom <lcs@lysator.liu.se>. The lower word is free, but in
;*       order to allow enhancements, please only use bit 0 to 3 for modes!
;*       Set the remaining bits to zero.
;*       The ID is currently used by AHI_BestAudioIDA() to select a mode.
;*       In order for this to work, you have to follow some rules.
;*       7 Modes using tables should have higher IDs than modes that uses more
;*         CPU power.
;*       7 Mono modes should have higher IDs than stereo modes
;*       7 Stereo modes should have higher IDs than stereo modes with panning.
;*       Use AHI:Developer/Support/ScanAudioModes to have a look at the modes
;*       currently available.
;*
;*       I do reserve the right to change the rules if I find them incorrect!
;*
;*****************************************************************************
;*
;*

;****** [driver].audio/AHIsub_AllocAudio *************************************
;*
;*   NAME
;*       AHIsub_AllocAudio -- Allocates and initializes the audio hardware.
;*
;*   SYNOPSIS
;*       result = AHIsub_AllocAudio( tags, audioctrl);
;*       D0                          A1    A2
;*
;*       ULONG AHIsub_AllocAudio( struct TagItem *, struct AHIAudioCtrlDrv * );
;*
;*   IMPLEMENTATION
;*       Allocate and initialize the audio hardware. Decide if and how you
;*       wish to use the mixing routines provided by 'ahi.device', by looking
;*       in the AHIAudioCtrlDrv structure and parsing the tag list for tags
;*       you support.
;*
;*       1) Use mixing routines with timing:
;*           You will need to be able to play any number of samples from
;*           about 80 up to 65535 with low overhead.
;*           7 Update AudioCtrl->ahiac_MixFreq to nearest value that your
;*             hardware supports.
;*           7 Return AHISF_MIXING|AHISF_TIMING.
;*       2) Use mixing routines without timing:
;*           If the hardware can't play samples with any length, use this
;*           alternative and provide timing yourself. The buffer must
;*           take less than about 20 ms to play, preferable less than 10!
;*           7 Update AudioCtrl->ahiac_MixFreq to nearest value that your
;*             hardware supports.
;*           7 Store the number of samples to mix each pass in
;*             AudioCtrl->ahiac_BuffSamples.
;*           7 Return AHISF_MIXING
;*           Alternatively, you can use the first method and call the
;*           mixing hook several times in a row to fill up a buffer.
;*           In that case, AHIsub_GetAttr(AHIDB_MaxPlaySamples) should
;*           return the size of the buffer plus AudioCtrl->ahiac_MaxBuffSamples.
;*           If the buffer is so large that it takes more than (approx.) 10 ms to
;*           play it for high sample frequencies, AHIsub_GetAttr(AHIDB_Realtime)
;*           should return FALSE.
;*       3) Don't use mixing routines:
;*           If your hardware can handle everything without using the CPU to
;*           mix the channels, you tell 'ahi.device' this by not setting
;*           neither the AHISB_MIXING nor the AHISB_TIMING bit.
;*
;*       If you can handle stereo output from the mixing routines, also set
;*       bit AHISB_KNOWSTEREO.
;*
;*       If you can handle hifi (32 bit) output from the mixing routines,
;*       set bit AHISB_KNOWHIFI.
;*
;*       If this driver can be used to record samples, set bit AHISB_CANRECORD,
;*       too (regardless if you use the mixing routines in AHI or not).
;*
;*       If the sound card has hardware to do DSP effects, you can set the
;*       AHISB_CANPOSTPROCESS bit. The output from the mixing routines will
;*       then be two separate buffers, one wet and one dry. You should then
;*       apply the Fx on the wet buffer, and post-mix the two buffers before
;*       you send the samples to the DAC. (V4)
;*
;*   INPUTS
;*       tags - pointer to a taglist.
;*       audioctrl - pointer to an AHIAudioCtrlDrv structure.
;*
;*   TAGS
;*       The tags are from the audio database (AHIDB_#? in <devices/ahi.h>),
;*       NOT the tag list the user called ahi.device/AHI_AllocAudio() with.
;*
;*   RESULT
;*       Flags, defined in <libraries/ahi_sub.h>.
;*
;*   EXAMPLE
;*
;*   NOTES
;*       You don't have to clean up on failure, AHIsub_FreeAudio() will
;*       always be called.
;*
;*   BUGS
;*
;*   SEE ALSO
;*       AHIsub_FreeAudio(), AHIsub_Start()
;*
;*****************************************************************************
;*
;*

AHIsub_AllocAudio:
  pushm d2-d7/a2-a6
  MOVE.l  a6,a5

  MOVE.l  a1,d3
  
* Allocate the 'paula' structure (our variables)
  MOVE.l  pb_SysLib(a5),a6
  MOVE.l  #paula_SIZEOF,d0
  MOVE.l  #MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,ahiac_DriverData(a2)
  BEQ .error_nopaula
  MOVE.l  d0,a3

* Initialize some Fields...
  MOVE.l  #0,p_audiodev(a3)
  MOVE.l  #-1,p_ParBitsUser(a3)
  MOVE.l  #-1,p_ParPortUser(a3)
  MOVE.l  a2,p_AudioCtrlP(a3)   ;player Hook
  MOVE.l  a2,p_AudioCtrlM(a3)   ;mixer Hook
  LEA p_PlaySoftInt(a3),a0
  MOVE.l  a0,p_PlaySoftIntPtr(a3)
  LEA p_RecSoftInt(a3),a0
  MOVE.l  a0,p_RecSoftIntPtr(a3)
  MOVE.l  #AHIST_S16S,p_rmType(a3)
  MOVE.l  #recordbuffer,p_rmLength(a3)
  MOVE.l  ahiac_Flags(a2),d0
  move.l d0,ahiac_Flags(a2)
  MOVE.b  p_Flags(a3),d1
  OR.b  d0,d1
  or.b #AHIACB_HIFI,d1
  MOVE.b  d1,p_Flags(a3)
  tst.l alloc_1
  bne .error1
 
.dontgetsampler


 ;* initialize interrupts (Only dummy function pointers at this time)

 ;* p_PlayInt (the main playback interrupt)
  MOVE.b  #NT_INTERRUPT,LN_TYPE+p_PlayInt(a3)
  MOVE.l  #LibName,LN_NAME+p_PlayInt(a3)
  MOVE.l  #Interrupt_Dummy,IS_CODE+p_PlayInt(a3)
  LEA p_PlayIntData(a3),a1
  MOVE.l  a1,IS_DATA+p_PlayInt(a3)

 ;* p_PlaySoftInt (caused by p_PlayInt, here are the mixing and conversion done)
  MOVE.b  #NT_INTERRUPT,LN_TYPE+p_PlaySoftInt(a3)
  MOVE.l  #LibName,LN_NAME+p_PlaySoftInt(a3)
  MOVE.l  #SoftInt_Dummy,IS_CODE+p_PlaySoftInt(a3)
  LEA p_PlaySoftIntData(a3),a1
  MOVE.l  a1,IS_DATA+p_PlaySoftInt(a3)

 ;* p_RecInt (the interrupt used for recording)

;* Make sure no interrupts occur until AHIsub_Start() is called
  ;MOVE.w  #INTF_AUD0!INTF_AUD1!INTF_AUD2!INTF_AUD3,CUSTOM+INTENA

;* Update ahiac_MixFreq to what the mixing/sampling frequency really is
  MOVE.l  ahiac_MixFreq(a2),d0
  MOVE.l  d0,ahiac_MixFreq(a2)    ;store actual freq

;* Check the ahi_minbufflen variable.
;* If the mixing frequency is higher than this one, set disable the filter,
;* else enable it.
  MOVE.l #MINBUFFLEN,d5       ;Default minbuffsize
  MOVE.l  pb_DosLib(a5),a6
  SUBQ.l  #8,sp       ;local label
  MOVE.w  #("0"<<8)|0,(sp)    ;Initialize as "0"
  LEA envVar(pc),a0
  MOVE.l  a0,d1
  MOVE.l  sp,d2
  MOVEQ.l #8,d3
  MOVEQ.l #0,d4
  call  GetVar
  CMP.l #-1,d0
  BEQ .gotfreq
  MOVE.l  sp,d1
  PEA.l 0.w
  MOVE.l  sp,d2
  call  StrToLong
  MOVE.l  (sp)+,d5
.gotfreq
  ADDQ.l  #8,sp
; d5 is now the freq
  move.l d5,minbufflen
.filter_set
  move.b #1,alloc_1
  move.l #1,p_audiodev(a3)
  MOVEQ #AHISF_KNOWHIFI|AHISF_KNOWSTEREO|AHISF_CANRECORD|AHISF_MIXING|AHISF_TIMING,d0
.exit
  popm  d2-d7/a2-a6
  RTS

.error_noaudiodev
.error_noreqmem
.error_noport
.error_nopaula

  clr.l alloc_1
  
  MOVEQ #AHISF_ERROR,d0
  BRA.b .exit
.error1
  MOVEQ #AHISF_ERROR,d0
  BRA.b .exit 
.audiochannelarray
  Dc.b  1+2+4+8
.audioname
  AUDIONAME
  Even


;in:
* d0  Frequency
;out:
* d0  Closest frequency
* d1  Index
findfreq:
  LEA uae(pc),a0
  CMP.l (a0),d0
  BLS.b .2
.findfreq
  CMP.l (a0)+,d0
  BHI.b .findfreq
  MOVE.l  -4(a0),d1
  SUB.l d0,d1
  SUB.l -8(a0),d0
  CMP.l d1,d0
  bhs.b .1
  SUBQ.l  #4,a0
.1
  SUBQ.l  #4,a0
.2
  MOVE.l  (a0),d0
  MOVE.l  a0,d1
  SUB.l #uae,d1
  LSR.l #2,d1
  RTS

uae  
  dc.l 17640
  dc.l 18900
  dc.l 19200
  dc.l 22050
  dc.l 27348
  Dc.l  32000         ; DAT/1.5
  dc.l 33075
  dc.l 37800
  Dc.l  44100         ; CD
  dc.l  48000
  dc.l  88200
  dc.l  96000
  ;dc.l 48000         ; DAT
FREQUENCIES   EQU (*-uae)
  Dc.l  -1



****** [driver].audio/AHIsub_FreeAudio **************************************
*
*   NAME
*       AHIsub_FreeAudio -- Deallocates the audio hardware.
*
*   SYNOPSIS
*       AHIsub_FreeAudio( audioctrl );
*                         A2
*
*       void AHIsub_FreeAudio( struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       Deallocate the audio hardware AND other resources allocated in
*       AHIsub_AllocAudio(). AHIsub_Stop() will always be called by
*       'ahi.device' before this Call is made.
*
*   INPUTS
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   NOTES
*       It must be safe To Call this routine Even If AHIsub_AllocAudio()
*       was never called, failed OR called more than once.
*
*   SEE ALSO
*       AHIsub_AllocAudio()
*
*****************************************************************************
*
*

AHIsub_FreeAudio:
  pushm d2-d7/a2-a6
  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6

  MOVE.l  ahiac_DriverData(a2),d0
  BEQ .nopaula
  MOVE.l  d0,a3
  tst.l p_audiodev(a3)
  beq .nopaula
  clr.l alloc_1
.nopaula
  MOVEQ #0,d0
  popm  d2-d7/a2-a6
  RTS


****** [driver].audio/AHIsub_Disable ****************************************
*
*   NAME
*       AHIsub_Disable -- Temporary turn Off audio interrupt/task
*
*   SYNOPSIS
*       AHIsub_Disable( audioctrl );
*                       A2
*
*       void AHIsub_Disable( struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       If you are lazy, Then call exec.library/Disable().
*       If you are smart, only Disable your own interrupt OR task.
*
*   INPUTS
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   NOTES
*       This call should be guaranteed To preserve all registers.
*       This call nests.
*
*   SEE ALSO
*       AHIsub_Enable(), exec.library/Disable()
*
*****************************************************************************
*
*

AHIsub_Disable:
  pushm  a3/a6
  MOVE.l  ahiac_DriverData(a2),a3
  ADDQ.w  #1,p_DisableCount(a3)
  move.l $4,a6
  move.b #1,disableint
  popm a3/a6
  RTS

****** [driver].audio/AHIsub_Enable *****************************************
*
*   NAME
*       AHIsub_Enable -- Turn On audio interrupt/task
*
*   SYNOPSIS
*       AHIsub_Enable( audioctrl );
*                      A2
*
*       void AHIsub_Enable( struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       If you are lazy, Then call exec.library/Enable().
*       If you are smart, only Enable your own interrupt OR task.
*
*   INPUTS
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   NOTES
*       This call should be guaranteed To preserve all registers.
*       This call nests.
*
*   SEE ALSO
*       AHIsub_Disable(), exec.library/Enable()
*
*****************************************************************************
*
*

AHIsub_Enable:
  pushm  a3/a6
  MOVE.l  ahiac_DriverData(a2),a3
  SUBQ.w  #1,p_DisableCount(a3)
  BNE.b .exit
  move.b #0,disableint
.exit
  popm a3/a6
  RTS

****** [driver].audio/AHIsub_Start ******************************************
*
*   NAME
*       AHIsub_Start -- Starts PlayBack OR recording
*
*   SYNOPSIS
*       error = AHIsub_Start( flags, audioctrl );
*       D0                    D0     A2
*
*       ULONG AHIsub_Start(ULONG, struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       What To do depends what you returned in AHIsub_AllocAudio().
*
*     * First, assume bit AHISB_PLAY in flags is set. This means that you
*       should begin PlayBack.
*
*     - AHIsub_AllocAudio() returned AHISF_MIXING|AHISF_TIMING:
*
*       A) Allocate a mixing Buffer of ahiac_BuffSize bytes. The Buffer must
*          be long aligned!
*       B) Create/Start an interrupt OR task that will do 1-6 over AND over
*          again Until AHIsub_Stop() is called. Note that it is NOT a good
*          idea To do the actual mixing AND conversion in a real hardware
*          interrupt. Signal a task OR create a Software Interrupt To do
*          the number crunching.
*
*       1) call the user Hook ahiac_PlayerFunc with the following parameters:
*                  A0 - (struct Hook *)
*                  A2 - (struct AHIAudioCtrlDrv *)
*                  A1 - Set To Null.
*
*       2) [call the ahiac_PreTimer Function. If it returns True (Z will be
*          cleared so you don't have To test d0), skip Step 3 AND 4. This
*          is Used To avoid overloading the CPU. This Step is optional.
*          A2 is assumed To Point To struct AHIAudioCtrlDrv. All registers
*          except d0 are preserved.  (V4)]
*
*       3) call the mixing Hook (ahiac_MixerFunc) with the following
*          parameters:
*                  A0 - (struct Hook *)           - The Hook itself
*                  A2 - (struct AHIAudioCtrlDrv *)
*                  A1 - (WORD *[])                - The mixing Buffer.
*          Note that ahiac_MixerFunc preserves ALL registers.
*          The user Hook ahiac_SoundFunc will be called by the mixing
*          routine when a sample have been processed, so you don't have To
*          worry about that.
*          How the Buffer will be filled is indicated by ahiac_Flags.
*          It is always filled with signed 16-bit (32 bit If AHIDBB_HIFI in
*          in ahiac_Flags is set) words, Even If PlayBack is 8 bit. If
*          AHIDBB_STEREO is set (in ahiac_Flags), Data For left AND right
*          channel are interleaved:
*           1st sample left channel,
*           1st sample right channel,
*           2nd sample left channel,
*           ...,
*           ahiac_BuffSamples:th sample left channel,
*           ahiac_BuffSamples:th sample right channel.
*          If AHIDBB_STEREO is cleared, the mono Data is stored:
*           1st sample,
*           2nd sample,
*           ...,
*           ahiac_BuffSamples:th sample.
*          Note that neither AHIDBB_STEREO nor AHIDBB_HIFI will be set If
*          you didn't report that you understand these flags when
*          AHI_AllocAudio() was called.
*
*          For AHI V2, the Type of Buffer is also available in ahiac_BuffType.
*          It is suggested that you Use this value instead. ahiac_BuffType
*          can be one of AHIST_M16S, AHIST_S16S, AHIST_M32S AND AHIST_S32S.
*
*       4) Convert the Buffer If needed AND feed it To the audio hardware.
*          Note that you may have To clear CPU caches If you are using DMA
*          To play the Buffer, AND the Buffer is NOT allocated in non-
*          cachable RAM.
*
*       5) [call the ahiac_PostTimer Function. A2 is assumed To Point To
*          struct AHIAudioCtrlDrv. All registers are preserved.  (V4)]
*
*       6) Wait Until the whole Buffer has been played, Then Repeat.
*
*       Use double buffering If possible!
*
*       You may DECREASE ahiac_BuffSamples slightly, For example To force an
*       Even number of samples To be mixed. By doing this you will make
*       ahiac_PlayerFunc To be called at wrong frequency so be careful!
*       Even If ahiac_BuffSamples is defined ULONG, it will never be greater
*       than 65535.
*
*       ahiac_BuffSize is the Largest size of the mixing Buffer that will be
*       needed Until AHIsub_Stop() is called.
*
*       ahiac_MaxBuffSamples is the Maximum number of samples that will be
*       mixed (Until AHIsub_Stop() is called). You can Use this value If you
*       need To allocate DMA buffers.
*
*       ahiac_MinBuffSamples is the minimum number of samples that will be
*       mixed. Most drivers will ignore it.
*
*       If AHIsub_AllocAudio() returned with the AHISB_CANPOSTPROCESS bit set,
*       ahiac_BuffSize is large enough To hold two buffers. The mixing Buffer
*       will be filled with the wet Buffer first, immediately followed by the
*       dry Buffer. I.e., ahiac_BuffSamples sample Frames wet Data, Then
*       ahiac_BuffSamples sample Frames dry Data. The DSP fx should only be
*       applied To the wet Buffer, AND the two buffers should Then be added
*       together. (V4)
*
*     - If AHIsub_AllocAudio() returned AHISF_MIXING, do as described above,
*       except calling ahiac_PlayerFunc. ahiac_PlayerFunc should be called
*       ahiac_PlayerFreq times per second, clocked by timers On your Sound
*       card OR by using 'realtime.library'. No other AMIGA resources may
*       be Used For timing (like direct CIA timers).
*       ahiac_MinBuffSamples AND ahiac_MaxBuffSamples are undefined If
*       AHIsub_AllocAudio() returned AHISF_MIXING (AHISB_TIMING bit NOT set).
*
*     - If AHIsub_AllocAudio() returned with neither the AHISB_MIXING nor
*       the AHISB_TIMING bit set, Then just Start PlayBack. Don't forget To
*       call ahiac_PlayerFunc ahiac_PlayerFreq times per second. Only your
*       own timing hardware OR 'realtime.library' may be Used. Note that
*       ahiac_MixerFunc, ahiac_BuffSamples, ahiac_MinBuffSamples,
*       ahiac_MaxBuffSamples AND ahiac_BuffSize are undefined. ahiac_MixFreq
*       is the frequency the user wants To Use For recording, If you support
*       that.
*
*     * Second, assume bit AHISB_RECORD in flags is set. This means that you
*       should Start To sample. Create a interrupt OR task that does the
*       following:
*
*       Allocate a Buffer (you chose size, but try To keep it reasonable
*       small To avoid delays - it is suggested that RecordFunc is called
*       at least 4 times/second For the lowers sampling rate, AND more often
*       For higher rates), AND fill it with the sampled Data. The Buffer must
*       be long aligned, AND it's size must be evenly divisible by four.
*       The Format should always be AHIST_S16S (Even with 8 bit mono samplers),
*       which means:
*           1st sample left channel,
*           1st sample right channel (same as prev. If mono),
*           2nd sample left channel,
*           ... etc.
*       Each sample is a signed word (WORD). The sample rate should be equal
*       To the mixing rate.
*
*       call the ahiac_SamplerFunc Hook with the following parameters:
*           A0 - (struct Hook *)           - The Hook itself
*           A2 - (struct AHIAudioCtrlDrv *)
*           A1 - (struct AHIRecordMessage *)
*       The message should be filled as follows:
*           ahirm_Type - Set To AHIST_S16S.
*           ahirm_Buffer - A Pointer To the filled Buffer.
*           ahirm_Samples - How many sample Frames stored.
*       You must NOT destroy the Buffer Until Next time the Hook is called.
*
*       Repeat Until AHIsub_Stop() is called.
*
*     * Note that both bits may be set when this Function is called.
*
*   INPUTS
*       flags - See <libraries/ahi_sub.h>.
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   RESULT
*       Returns AHIE_OK If successful, Else an error code as defined
*       in <devices/ahi.h>. AHIsub_Stop() will always be called, Even
*       If this call failed.
*
*   NOTES
*       The driver must be able To Handle multiple calls To this routine
*       without preceding calls To AHIsub_Stop().
*
*   SEE ALSO
*       AHIsub_Update(), AHIsub_Stop()
*
*****************************************************************************
*
*
*

AHIsub_Start:
  pushm d2-d7/a2-a6
  clr.l disableint
  MOVE.l  d0,d7
  LEA CUSTOM,a4
  MOVE.l  ahiac_DriverData(a2),a3
  cmp.l #0,a3
  beq .error_unknown
  tst.l p_audiodev(a3)
  beq .error_unknown
  clr.l p_RecBuffer1(a3)
  clr.l p_RecBuffer2(a3)
  BTST  #AHISB_PLAY,d0
  ;BEQ .dont_play
 
**
*** AHISB_PLAY
**
  MOVE.l  ahiac_MixFreq(a2),d1
  MOVE.l  d1,p_AudPer(a3)
  MOVEQ #AHISF_PLAY,d0
  call  AHIsub_Stop     ;Stop current playback if any.
  call  AHIsub_Update     ;fill variables

  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6
  
  MOVE.l  ahiac_BuffSize(a2),d0
  muls.l  minbufflen,d0
  MOVE.l  #MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_Mixbuffer(a3)
  BEQ .error_nomem
  
 
* The init*bit? routines allocates p_DMAbuffer, sets up p_AudPtr, sets the Volume,
* stores the correct interrupt routines in p_PlayInt's AND p_PlaySoftInt's IS_CODE.

  PEA .1(pc)
  MOVE.b  p_Flags(a3),d0
  BRA.w initahisound
.1
  TST.l d0
  BNE .exit

* Install play interrupt
  move.l p_AudPer(a3),d2
  moveq #0,d3
  move.w p_AudLen(a3),d3
  muls.l minbufflen,d3
  moveq #0,d1
  tst.l $f0ffc0
  beq .error_nomem
  moveq #0,d0
  jsr $f0ffc0
  tst.l d0                      
  beq .error_nomem              
  LEA p_PlayInt(a3),a1
  MOVEQ #INTB_EXTER,d0
  move.l a3,p_base
  call  AddIntServer
  CLR.w p_DisableCount(a3)

  MOVE.l  a5,a6
  move.b #1,playing

.dont_play

  BTST  #AHISB_RECORD,d7
  BEQ .dont_record

**
*** AHISB_RECORD
**
  ;MOVEQ #AHISF_RECORD,d0
  ;call  AHIsub_Stop     ;Stop current recording if any.
  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6
  MOVE.l  ahiac_BuffSize(a2),d0
  muls.l  minbufflen,d0
  MOVE.l  #MEMF_PUBLIC,d1
  call  AllocVec
  MOVE.l  d0,p_RecBuffer1(a3)
  BEQ .error_nomem
  MOVE.l  d0,p_RecFillPtr(a3)
  MOVE.l  ahiac_BuffSamples(a2),d0
  asl.l #1,d0
  move.l d0,p_rmLength(a3)
  MOVE.l  ahiac_BuffSize(a2),d0
  muls.l  minbufflen,d0
  MOVE.l  #MEMF_PUBLIC,d1
  call  AllocVec
  MOVE.l  d0,p_RecBuffer2(a3)
  BEQ .error_nomem
* Install Record interrupt
  move.b #1,playing
.dont_record
.return


  MOVEQ #AHIE_OK,d0
.exit
  popm  d2-d7/a2-a6
  RTS
.error_nomem
  MOVEQ #AHIE_NOMEM,d0
  BRA.b .exit
.error_unknown
  MOVEQ #AHIE_UNKNOWN,d0
  BRA.b .exit


;in:
* a2  AudioCtrl
* a3  paula
* a5  paulaBase
* a6  ExecBase
initahisound:                  ;init ahisound
  MOVE.l  #ahisoundint,IS_CODE+p_PlayInt(a3)
  LEA softint_ahisound(pc),a0
  MOVE.l  a0,IS_CODE+p_PlaySoftInt(a3)
  MOVE.l  ahiac_MaxBuffSamples(a2),d0 ;Max. # of 16 bit samples
  muls.l   minbufflen,d0 
  ADDQ.l  #1,d0
  BCLR  #0,d0       ;force even
  LSL.l #3,d0       ;Double buffer + 2W8 bit + Stereo
  MOVE.l  #MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_DMAbuffer(a3)
  BEQ .nomem
  MOVE.l  d0,p_AudPtr1A(a3)
  ;MOVE.l  d0,p_AudPtr2A(a3)
  ;MOVE.l  d0,p_AudPtr3A(a3)
  ;MOVE.l  d0,p_AudPtr4A(a3)
  move.l ahiac_MaxBuffSamples(a2),d1
  muls.l minbufflen,d1
  lsl.l #2,d1
  add.l d1,d0
  MOVE.l  d0,p_AudPtr1B(a3)
  ;MOVE.l  d0,p_AudPtr2B(a3)
  ;MOVE.l  d0,p_AudPtr3B(a3)

  MOVEQ #0,d0
  RTS
.nomem
  MOVEQ #AHIE_NOMEM,d0
  RTS

****** [driver].audio/AHIsub_Update *****************************************
*
*   NAME
*       AHIsub_Update -- Update some variables
*
*   SYNOPSIS
*       AHIsub_Update( flags, audioctrl );
*                      D0     A2
*
*       void AHIsub_Update(ULONG, struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       All you have To do is To update some variables:
*       Mixing & timing: ahiac_PlayerFunc, ahiac_MixerFunc, ahiac_SamplerFunc,
*       ahiac_BuffSamples (AND perhaps ahiac_PlayerFreq If you Use it).
*       Mixing only: ahiac_PlayerFunc, ahiac_MixerFunc, ahiac_SamplerFunc AND
*           ahiac_PlayerFreq.
*       Nothing: ahiac_PlayerFunc, ahiac_SamplerFunc AND ahiac_PlayerFreq.
*
*   INPUTS
*       flags - Currently no flags defined.
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   RESULT
*
*   NOTES
*       This call must be safe from interrupts.
*
*   SEE ALSO
*       AHIsub_Start()
*
*****************************************************************************
*
*
*

AHIsub_Update:
  pushm d2-d7/a2-a6

  call  AHIsub_Disable    ;make sure we don't get an interrupt
          ;while updating our local variables
  MOVE.l  ahiac_DriverData(a2),a3

  MOVE.l  ahiac_PlayerFunc(a2),a0
  MOVE.l  a0,p_PlayerHook(a3)
  MOVE.l  h_Entry(a0),p_PlayerEntry(a3)
  move.l ahiac_BuffSamples(a2),d0
  cmp.w p_AudLen(a3),d0
  beq .l1
  tst.l playing
  beq .l1
  moveq #1,d0
  moveq #0,d1
  jsr $f0ffc0
  moveq #0,d0
  moveq #0,d1
  move.l ahiac_MixFreq(a2),d2
  ;move.l p_AudPer(a3),d2
  move.l ahiac_BuffSamples(a2),d3
  move.w d3,p_AudLen(a3)
  muls.l minbufflen,d3
  jsr $f0ffc0
.l1  
  MOVE.l  ahiac_BuffSamples(a2),d0
  MOVE.w  d0,p_AudLen(a3)
  SUBQ.l  #1,d0
  MOVE.l  d0,p_LoopTimes(a3)  ;See softints. (Unrolled)

  MOVE.l  ahiac_MixerFunc(a2),a0
  MOVE.l  a0,p_MixHook(a3)
  MOVE.l  h_Entry(a0),p_MixEntry(a3)

  MOVE.l  ahiac_PreTimer(a2),p_PreTimer(a3)
  MOVE.l  ahiac_PostTimer(a2),p_PostTimer(a3)

  call  AHIsub_Enable
  MOVEQ #0,d0
  popm  d2-d7/a2-a6
  RTS


****** [driver].audio/AHIsub_Stop *******************************************
*
*   NAME
*       AHIsub_Stop -- Stops PlayBack.
*
*   SYNOPSIS
*       AHIsub_Stop( flags, audioctrl );
*                    D0     A2
*
*       void AHIsub_Stop( ULONG, struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       Stop PlayBack AND/OR recording, remove all resources allocated by
*       AHIsub_Start().
*
*   INPUTS
*       flags - See <libraries/ahi_sub.h>.
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   NOTES
*       It must be safe To call this routine Even If AHIsub_Start() was never
*       called, failed OR called more than once.
*
*   SEE ALSO
*       AHIsub_Start()
*
*****************************************************************************
*
*

AHIsub_Stop:
  pushm d2-d7/a2-a6
  LEA CUSTOM,a4
  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6
  MOVE.l  ahiac_DriverData(a2),a3
  push  d0
  tst.l p_audiodev(a3)
  beq .dontplay
  tst.l playing
  beq .dontplay
  BTST  #AHISB_PLAY,d0
  ;BEQ .dontplay

**
*** AHISB_PLAY
**
  MOVEQ #1,d0
  MOVEQ #0,d1
  JSR $f0ffc0                 ;close winuae ahi
  clr.l playing
  MOVEQ #0,d0
  LEA p_PlayInt(a3),a1
  MOVEQ #INTB_EXTER,d0
  call  RemIntServer

  MOVE.l  p_DMAbuffer(a3),d0
  BEQ.b .nodmamem
  MOVE.l  d0,a1
  CLR.l p_DMAbuffer(a3)
  call  FreeVec
.nodmamem
  MOVE.l  p_Mixbuffer(a3),d0
  BEQ.b .nomixmem
  MOVE.l  d0,a1
  CLR.l p_Mixbuffer(a3)
  call  FreeVec
.nomixmem

.dontplay
  pop d0
  BTST  #AHISB_RECORD,d0
  BEQ .dontrecord

**
*** AHISB_RECORD
**
  MOVE.l  p_RecBuffer1(a3),d0
  BEQ.b .norecmem1
  MOVE.l  d0,a1
  CLR.l p_RecBuffer1(a3)
  call  FreeVec
.norecmem1
  MOVE.l  p_RecBuffer2(a3),d0
  BEQ.b .norecmem2
  MOVE.l  d0,a1
  CLR.l p_RecBuffer2(a3)
  call  FreeVec
.norecmem2
.dontrecord
.return
  MOVEQ #0,d0
  popm  d2-d7/a2-a6
  RTS

****** [driver].audio/AHIsub_#? *********************************************
*
*   NAME
*       AHIsub_SetEffect -- Set effect.
*       AHIsub_SetFreq -- Set frequency.
*       AHIsub_SetSound -- Set Sound.
*       AHIsub_SetVol -- Set Volume AND stereo panning.
*       AHIsub_LoadSound -- Prepare a Sound For PlayBack.
*       AHIsub_UnloadSound -- Discard a Sound.
*
*   SYNOPSIS
*       See functions in 'ahi.device'.
*
*   IMPLEMENTATION
*       If AHIsub_AllocAudio() did NOT Return with bit AHISB_MIXING set,
*       all user calls To these Function will be routed To the driver.
*
*       If AHIsub_AllocAudio() did Return with bit AHISB_MIXING set, the
*       calls will first be routed To the driver, AND only handled by
*       'ahi.device' If the driver returned AHIS_UNKNOWN. This way it is
*       possible To ADD effects that the Sound card handles On its own, like
*       Filter AND echo effects.
*
*       For what each Function does, see the autodocs For 'ahi.device'.
*
*   INPUTS
*       See functions in 'ahi.device'.
*
*   NOTES
*       See functions in 'ahi.device'.
*
*   SEE ALSO
*       ahi.device/AHI_SetEffect(), ahi.device/AHI_SetFreq(),
*       ahi.device/AHI_SetSound(), ahi.device/AHI_SetVol(),
*       ahi.device/AHI_LoadSound(), ahi.device/AHI_UnloadSound()
*
*
*****************************************************************************
*
*

AHIsub_SetVol:
AHIsub_SetFreq:
AHIsub_SetSound:
AHIsub_SetEffect:
AHIsub_LoadSound:
AHIsub_UnloadSound:
  MOVEQ #AHIS_UNKNOWN,d0
  RTS

****** [driver].audio/AHIsub_GetAttr ****************************************
*
*   NAME
*       AHIsub_GetAttr -- Returns information about audio modes OR driver
*
*   SYNOPSIS
*       AHIsub_GetAttr( attribute, argument, Default, taglist, audioctrl );
*       D0              D0         D1        D2       A1       A2
*
*       LONG AHIsub_GetAttr( ULONG, LONG, LONG, struct TagItem *,
*                            struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       Return the attribute based On a tag List AND an AHIAudioCtrlDrv
*       structure, which are the same that will be passed To
*       AHIsub_AllocAudio() by 'ahi.device'. If the attribute is
*       unknown To you, Return the Default.
*
*   INPUTS
*       attribute - Is really a Tag AND can be one of the following:
*           AHIDB_Bits - Return how many output bits the tag List will
*               result in.
*           AHIDB_MaxChannels - Return the resulting number of channels.
*           AHIDB_Frequencies - Return how many mixing/sampling frequencies
*               you support
*           AHIDB_Frequency - Return the argument:th frequency
*                Example: You support 3 frequencies 32, 44.1 AND 48 kHz.
*                   If argument is 1, Return 44100.
*           AHIDB_Index - Return the Index which gives the frequency closest
*               To argument.
*               Example: You support 3 frequencies 32, 44.1 AND 48 kHz.
*                   If argument is 40000, Return 1 (=> 44100).
*           AHIDB_Author - Return Pointer To name of driver author:
*               "Martin 'Leviticus' Blom"
*           AHIDB_Copyright - Return Pointer To copyright notice, including
*               the ')' character: "?????1996 Martin Blom" OR "Public Domain"
*           AHIDB_Version - Return Pointer version string, normal AMIGA
*               Format: "paula 1.5 (18.2.96)\r\n"
*           AHIDB_Annotation - Return Pointer To an annotation string, which
*               can be several lines.
*           AHIDB_Record - Are you a sampler, too? Return True OR False.
*           AHIDB_FullDuplex - Return True OR False.
*           AHIDB_Realtime - Return True OR FALSE.
*           AHIDB_MaxPlaySamples - Normally, Return the Default. See
*               AHIsub_AllocAudio(), section 2.
*           AHIDB_MaxRecordSamples - Return the size of the Buffer you fill
*               when recording.
*
*           The following are associated with AHIsub_HardwareControl() AND are
*           new For V2.
*           AHIDB_MinMonitorVolume
*           AHIDB_MaxMonitorVolume - Return the lower/upper limit For
*               AHIC_MonitorVolume. If unsupported but always 1.0, Return
*               1.0 For both.
*           AHIDB_MinInputGain
*           AHIDB_MaxInputGain - Return the lower/upper limit For
*               AHIC_InputGain. If unsupported but always 1.0, Return 1.0 For
*               both.
*           AHIDB_MinOutputVolume
*           AHIDB_MaxOutputVolume - Return the lower/upper limit For
*               AHIC_OutputVolume.
*           AHIDB_Inputs - Return how many inputs you have.
*           AHIDB_Input - Return a short string describing the argument:th
*               input. Number 0 should be the Default one. Example strings
*               can be "Line 1", "Mic", "Optical" OR whatever.
*           AHIDB_Outputs - Return how many outputs you have.
*           AHIDB_Output - Return a short string describing the argument:th
*               output. Number 0 should be the Default one. Example strings
*               can be "Line 1", "Headphone", "Optical" OR whatever.
*       argument - extra info For some attributes.
*       Default - What you should Return For unknown attributes.
*       taglist - Pointer To a tag List that eventually will be fed To
*           AHIsub_AllocAudio(), OR Null.
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure that eventually
*           will be fed To AHIsub_AllocAudio(), OR Null.
*
*   NOTES
*
*   SEE ALSO
*       AHIsub_AllocAudio(), AHIsub_HardwareControl(),
*       ahi.device/AHI_GetAudioAttrsA()
*
*****************************************************************************
*
*

AHIsub_GetAttr:
  pushm a3/a5-a6
  MOVE.l  a6,a5
  MOVE.l  pb_UtilLib(a5),a6

 IFD  MC020
  TST.l a1
 Else
  CMP.l #0,a1
 ENDC
  BEQ .notaglist      ;no tag list!

  MOVE.l  a1,a3

  pushm d0-d1

  MOVE.l  #AHIDB_Paula14Bit,d0
  MOVEQ #FALSE,d1
  MOVE.l  a3,a0
  call  GetTagData
  TST.l d0
  BNE .2

* Only For AHIDB_Paula14Bit FALSE (== recording available)
  popm  d0-d1

  CMP.l #AHIDB_Record,d0
  BNE.b .not_record1
  MOVEQ #TRUE,d0
  BRA .exit
.not_record1
  CMP.l #AHIDB_FullDuplex,d0
  BNE.b .not_fullduplex1
  MOVEQ #TRUE,d0
  BRA .exit
.not_fullduplex1
  CMP.l #AHIDB_MaxRecordSamples,d0
  BNE.b .not_mrs1
  move.l #recordbuffer,d0
  BRA .exit
.not_mrs1
  CMP.l #AHIDB_MinMonitorVolume,d0
  BNE.b .not_minmonvol1
  MOVEQ #0,d0
  BRA .exit
.not_minmonvol1
  CMP.l #AHIDB_MaxMonitorVolume,d0
  BNE.b .not_maxmonvol1
  MOVE.l  #$10000,d0
  BRA .exit
.not_maxmonvol1
  CMP.l #AHIDB_MinInputGain,d0
  BNE.b .not_mingain1
  MOVE.l  #$10000,d0
  BRA .exit
.not_mingain1
  CMP.l #AHIDB_MaxInputGain,d0
  BNE.b .not_maxgain1
  MOVE.l  #$10000,d0
  BRA .exit
.not_maxgain1
  CMP.l #AHIDB_Inputs,d0
  BNE.b .not_inputs
  MOVEQ #1,d0
  BRA .exit
.not_inputs
  CMP.l #AHIDB_Input,d0
  BNE.b .not_input
  LSL.l #2,d1
  MOVE.l  .inputs(pc,d1.w),d0
  BRA .exit
.not_input
  BRA .3
.inputs:
  Dc.l  .input0
  Dc.l  .input1

.2
* Only For AHIDB_Paula14Bit True (== no recording)
  popm  d0-d1
  CMP.l #AHIDB_Record,d0
  BNE.b .not_record2
  MOVEQ #FALSE,d0
  BRA .exit
.not_record2
  CMP.l #AHIDB_FullDuplex,d0
  BNE.b .not_fullduplex2
  MOVEQ #FALSE,d0
  BRA .exit
.not_fullduplex2

.3
* Common Tags (AHIDB_14bit)

  CMP.l #AHIDB_Bits,d0
  BNE.b .not_bits
  MOVE.l  #AHIDB_Paula14Bit,d0
  MOVEQ #FALSE,d1
  MOVE.l  a3,a0
  call  GetTagData
  TST.l d0
  BEQ.b .no14bit
  MOVEQ #14,d0
  skipw
.no14bit
  MOVEQ #8,d0
  BRA .exit
.not_bits
.notaglist
  CMP.l #AHIDB_Frequencies,d0
  BNE.b .not_freqs
  MOVE.l  #FREQUENCIES,d0
  LSR.l #2,d0
  BRA .exit
.not_freqs

  CMP.l #AHIDB_Frequency,d0
  BNE.b .not_freq
  ADD.w d1,d1
  ADD.w d1,d1
  lea uae(pc),a0
  MOVE.l  (a0,d1.w),d0
  BRA .exit
.not_freq
  CMP.l #AHIDB_Index,d0
  BNE.b .not_index
  MOVE.l  d1,d0
  BSR findfreq
  MOVE.l  d1,d0
  BRA .exit
.not_index
  CMP.l #AHIDB_Author,d0
  BNE.b .not_author
  LEA .author(pc),a0
  MOVE.l  a0,d0
  BRA .exit
.not_author
  CMP.l #AHIDB_Copyright,d0
  BNE.b .not_copyright
  LEA .copyright(pc),a0
  MOVE.l  a0,d0
  BRA .exit
.not_copyright
  CMP.l #AHIDB_Version,d0
  BNE.b .not_version
  LEA IDString(pc),a0
  MOVE.l  a0,d0
  BRA .exit
.not_version
  CMP.l #AHIDB_Annotation,d0
  BNE.b .not_anno
  LEA .anno(pc),a0
  MOVE.l  a0,d0
  BRA .exit
.not_anno
  CMP.l #AHIDB_Realtime,d0
  BNE.b .not_realtime
  MOVEQ #TRUE,d0
  BRA .exit
.not_realtime
  CMP.l #AHIDB_Outputs,d0
  BNE.b .not_outputs
  MOVEQ #1,d0
  BRA .exit
.not_outputs
  CMP.l #AHIDB_Output,d0
  BNE.b .not_output
  LEA .output(pc),a0
  MOVE.l  a0,d0
  BRA .exit
.not_output
* Unknown attribute, Return Default.
  MOVE.l  d2,d0
.exit
  popm  a3/a5-a6
  RTS
.author   Dc.b  "based on paula from Martin Blohm changed by Bernd Roesch",0
.copyright  Dc.b  "Public Domain",0
.anno   Dc.b  "",0
.input0   Dc.b  "Default",0
.input1   Dc.b  "Default",0
.output   Dc.b  "Line",0
  Even


****** [driver].audio/AHIsub_HardwareControl ********************************
*
*   NAME
*       AHIsub_HardwareControl -- Modify Sound card settings
*
*   SYNOPSIS
*       AHIsub_HardwareControl( attribute,  argument, audioctrl );
*       D0                      D0          D1        A2
*
*       LONG AHIsub_HardwareControl( ULONG, LONG, struct AHIAudioCtrlDrv * );
*
*   IMPLEMENTATION
*       Set OR Return the state of a particular hardware component. AHI uses
*       AHIsub_GetAttr() To supply the user with limits AND what Tags are
*       available.
*
*   INPUTS
*       attribute - Is really a Tag AND can be one of the following:
*           AHIC_MonitorVolume - Set the input monitor Volume To argument.
*           AHIC_MonitorVolume_Query - Return the current input monitor
*               Volume (argument is ignored).
*
*           AHIC_InputGain - Set the input gain To argument. (V2)
*           AHIC_InputGain_Query (V2)
*
*           AHIC_OutputVolume - Set the output Volume To argument. (V2)
*           AHIC_OutputVolume_Query (V2)
*
*           AHIC_Input - Use the argument:th input source (Default is 0). (V2)
*           AHIC_Input_Query (V2)
*
*           AHIC_Output - Use the argument:th output destination (Default
*               is 0). (V2)
*           AHIC_Output_Query (V2)
*
*       argument - What value attribute should be set To.
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   RESULT
*       Return the state of selected attribute. If you were asked To set
*       something, Return TRUE. If attribute is unknown To you OR unsupported,
*       Return FALSE.
*
*   NOTES
*       This call must be safe from interrupts.
*
*   SEE ALSO
*       ahi.device/AHI_ControlAudioA(), AHIsub_GetAttr()
*
*****************************************************************************
*
*

AHIsub_HardwareControl:
  CMP.l #AHIC_MonitorVolume,d0
  BNE.b .dontsetmonvol
  MOVE.l  ahiac_DriverData(a2),a1
  LSR.l #8,d1
  LSR.l #2,d1
  MOVE.w  d1,p_MonitorVolume(a1)
  BRA.b .exit
.dontsetmonvol
  CMP.l #AHIC_MonitorVolume_Query,d0
  BNE.b .dontgetmonvol
  MOVE.l  ahiac_DriverData(a2),a1
  MOVEQ #0,d0
  MOVE.w  p_MonitorVolume(a1),d0
  LSL.l #8,d0
  LSL.l #2,d0
  BRA.b .quit
.dontgetmonvol
  CMP.l #AHIC_Input,d0
  BNE.b .dontsetinput
  MOVE.l  ahiac_DriverData(a2),a1
  MOVE.w  d1,p_Input(a1)
  BRA.b .exit
.dontsetinput
  CMP.l #AHIC_Input_Query,d0
  BNE.b .dontgetinput
  MOVE.l  ahiac_DriverData(a2),a1
  MOVEQ #0,d0
  MOVE.w  p_Input(a1),d0
  BRA.b .quit
.dontgetinput
  MOVEQ #FALSE,d0
.quit
  RTS
.exit
  MOVEQ #TRUE,d0
  RTS







;----------------------------------------------------------------------------

Interrupt_Dummy:

SoftInt_Dummy:
  RTS

;in:
* d0  scratch
* d1  INTENAR && INTREQR
* a0  CUSTOM
* a1  &(paulaBase->p_PlayIntData)
* a5  &AudioInterrupt
* a6  ExecBase



ahisoundint:
  tst.b disableint
  bne .out   
  moveq #4,d0
  moveq #0,d1
  jsr $f0ffc0       ;check if our interrupt
  tst.l d0
  bgt ourint
.out  moveq #0,d0
  rts
ourint  MOVEM.l (a1)+,d0/d1     ;p_PlaySoftIntPtr,p_AudLen/p_AudPer
  move.b #1,disableint  
  ;MOVE.l  d1,AUD1LEN(a0)
  ;MOVE.l  d1,AUD2LEN(a0)
  ;MOVE.l  d1,AUD3LEN(a0)

  ;MOVE.l  d1,AUD0LEN(a0)
  NOT.l (a1)+       ;double buffering

  LEA p_AudPtrs-p_AudPtr1A(a1),a0
  BEQ.b .1

  ADD.w #4*4,a1

.1
  MOVE.l  a1,(a0)
  MOVE.l  (a1)+,a0
  move.l d0,-(a7)
  moveq #2,d0
  moveq #0,d1
  jsr $f0ffc0             ;write samples
  move.l (a7)+,a1
                    ;p_PlaySoftInt
  jsr _LVOCause(a6)     ;start PlaySoftInt
  moveq #0,d0
  rts
softint_ahisound:           ;@1
 
  pushm a2-a6/d7/d6
  MOVE.l  a1,a5
  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  jsr     (a4)         ;call PreTimer
  move.l  d0,d7
  moveq #0,d6
  move.l a0,a0store
  move.l a1,a1store
  move.l a2,a2store
  move.l a3,a3store
  move.l a5,a5store
  clr.l buffoffset
  bra .st
.repeat
  move.l a0store,a0
  move.l a1store,a1
  move.l a2store,a2
  move.l a3store,a3
  move.l a5store,a5
.st  JSR (a3)        ;call Player Hook
 
  tst.l d7
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a5
  JSR (a3)        ;call Mixer Hook

* convert AND transfer Buffer
  MOVEM.l (a5),a0/a3
  add.l buffoffset,a0
.loop
  move.w (a1),(a0)+
  move.w 4(a1),(a0)+
  addq #8,a1
  DBF d0,.loop
  move.l a5store,a5
  move.l 8(a5),a0
  sub.l a0,a1
  move.l a1,d0
  asr.l #1,d0
  add.l d0,buffoffset
;**************************************+ rec stuff#
  move.l a5,-(a7)
.rloop  move.l p_base,a5
  cmp.l #0,p_RecBuffer2(a5)
  beq .norec
  move.l p_RecBuffer2(a5),a0
  moveq #3,d0
  jsr $f0ffc0                           ;read samples
  tst.l d0
  blt .norec 
  movem.l d0/a2,-(a7)
  MOVE.l  p_RecBuffer2(a5),p_rmBuffer(a5)
  MOVE.l  p_AudioCtrlP(a5),a2
  LEA p_RecordMessage(a5),a1
  MOVE.l  ahiac_SamplerFunc(a2),a0
  MOVE.l  h_Entry(a0),a5
  JSR (a5)
  movem.l (a7)+,a2/d0
  cmp.l #1,d0
  bgt .rloop
.norec
 move.l (a7)+,a5
;**************************************+ rec stuff end
.exit
  move.l ahiac_BuffSamples(a2),d0
  add.l   d0,d6
  muls.l minbufflen,d0
  cmp.l   d0,d6
  blo     .repeat
  JSR (a6)        ;call PostTimer, a2 ok
  popm  a2-a6/d7/d6

  clr.l disableint
  RTS

flag    Dc.l 0
minbufflen dc.l 0
alloc_1 dc.l 0
disableint dc.l 0
playing dc.l 0
a0store dc.l 0
a1store dc.l 0
a2store dc.l 0
a3store dc.l 0
a5store dc.l 0
p_base  dc.l 0
buffoffset dc.l 0
EndCode:
check        Dc.b 0
 End
