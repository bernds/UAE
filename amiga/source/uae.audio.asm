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

VERSION   EQU 4
REVISION  EQU 2
DATE  Macro
    Dc.b  "17.4.97"
  ENDM
VERS  Macro
    Dc.b  "uae 4.2"
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

  incdir  include:

  include hardware/all.i

  include exec/initializers.i
  include exec/resident.i
  include exec/alerts.i
  include exec/memory.i
  include graphics/gfxbase.i
  include utility/utility.i
  include utility/hooks.i
  include devices/audio.i
  include dos/dos.i


  include lvos/exec_lib.i
  include lvos/utility_lib.i
  include lvos/dos_lib.i
  include lvos/cardres_lib.i

  include devices/ahi.i
  include libraries/ahi_sub.i
  include lvos/ahi_sub_lib.i

  include resources/misc.i
  include resources/card.i
  include macros.i

_ciaa   EQU $bfe001
_ciab   EQU $bfd000

PALFREQ   EQU 3546895
NTSCFREQ  EQU 3579545
MINPER    EQU 62

RECORDSAMPLES EQU 1024



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
  APTR  p_AudPtr4B

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
miscName: MISCNAME
cardName: Dc.b  "card.resource",0
envVar:   Dc.b  "AHIpaulaFilterFreq",0
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
.dosOK
  LEA miscName(pc),a1
  call  OpenResource
  MOVE.l  d0,pb_MiscResource(a5)
  BNE.b .miscOK
  ALERT AG_OpenRes!AO_MiscRsrc
  MOVEQ #0,d0
  BRA .exit
.miscOK
  LEA cardName(pc),a1
  call  OpenResource
  MOVE.l  d0,pb_CardResource(a5)    ;Don't fail on error

  MOVE.l  a5,d0
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

;* BeginIO(ioRequest)(a1) (From amiga.lib)
BeginIO:
  MOVE.l  a1,a0   ;probably not neccesary
  push  a6
  MOVE.l  IO_DEVICE(a1),a6
  JSR -30(a6)
  pop a6
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
  MOVE.l  #-1,p_audiodev(a3)
  MOVE.l  #-1,p_ParBitsUser(a3)
  MOVE.l  #-1,p_ParPortUser(a3)
  MOVE.l  a2,p_AudioCtrlP(a3)   ;player Hook
  MOVE.l  a2,p_AudioCtrlM(a3)   ;mixer Hook
  LEA p_PlaySoftInt(a3),a0
  MOVE.l  a0,p_PlaySoftIntPtr(a3)
  LEA p_RecSoftInt(a3),a0
  MOVE.l  a0,p_RecSoftIntPtr(a3)
  MOVE.l  #AHIST_S16S,p_rmType(a3)
  MOVE.l  #RECORDSAMPLES,p_rmLength(a3)

* Translate Tags To flags
  MOVEQ #0,d2
  MOVE.l  pb_UtilLib(a5),a6
  MOVE.l  #AHIDB_Paula14Bit,d0
  MOVEQ #0,d1
  MOVE.l  d3,a0       ;tag list
  call  GetTagData
  TST.l d0
  BEQ.b .no14bit
  MOVEQ #PF_14BIT,d2
.no14bit
  MOVE.l  ahiac_Flags(a2),d0
  AND.b #PF_STEREO,d0     ;same as AHIACF_STEREO
  OR.b  d2,d0
  MOVE.b  p_Flags(a3),d1
  AND.b #~(PF_STEREO|PF_14BIT),d1
  OR.b  d0,d1
  MOVE.b  d1,p_Flags(a3)

* Check If a table should be Used (14 bit calibration)
  MOVE.l  #AHIDB_PaulaTable,d0
  MOVEQ #0,d1
  MOVE.l  d3,a0       ;tag list
  call  GetTagData
  TST.l d0
  BEQ .notable

* Load 'ENV:CyberSound/SoundDrivers/14Bit_Calibration', allocate
* AND initialize the table.
* FIXIT: The calibration File should MOVE To a special chunk in
* 'DEVS:AudioModes/PAULA'.
  MOVE.l  pb_DosLib(a5),a6
  LEA .calibname(pc),a0
  MOVE.l  a0,d1
  MOVE.l  #MODE_OLDFILE,d2
  call  Open
  MOVE.l  d0,d4
  BEQ .nocalib
  MOVE.l  d0,d1
  LEA p_CalibrationArray(a3),a0
  MOVE.l  a0,d2
  MOVE.l  #256,d3
  call  Read
  CMP.l d0,d3
  BEQ .tableloaded
.nocalib
; Fill defaults
  LEA p_CalibrationArray(a3),a0
  MOVE.w  #254-1,d0
.initcalib
  MOVE.b  #$55,(a0)+
  DBF d0,.initcalib
  MOVE.b  #$7f,(a0)+
.tableloaded
  MOVE.l  d4,d1
  BEQ.b .nofile
  call  Close
.nofile
  MOVE.l  pb_SysLib(a5),a6
  MOVE.l  #65536*2,d0
  MOVE.l  #MEMF_PUBLIC,d1
  call  AllocVec
  MOVE.l  d0,p_CalibrationTable(a3)
  BEQ.b .notable
  MOVE.l  d0,a0     ;table
  LEA p_CalibrationArray(a3),a1
  BSR.w _CreateTable
.notable


* allocate audio.device
  MOVE.l  pb_SysLib(a5),a6
  call  CreateMsgPort
  MOVE.l  d0,p_audioport(a3)
  BEQ .error_noport
  MOVEQ #ioa_SIZEOF,d0
  MOVE.l  #MEMF_PUBLIC!MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_audioreq(a3)
  BEQ .error_noreqmem
  MOVE.l  d0,a0
  MOVE.l  p_audioport(a3),MN_REPLYPORT(a0)
  CLR.w ioa_AllocKey(a0)
  MOVE.b  #127,LN_PRI(a0)     ;steal it!
  LEA .audiochannelarray(pc),a1
  MOVE.l  a1,ioa_Data(a0)
  MOVE.l  #1,ioa_Length(a0)
  LEA .audioname(pc),a0
  MOVEQ #0,d0
  MOVE.l  p_audioreq(a3),a1
  MOVEQ #0,d1
  call  OpenDevice
  MOVE.l  d0,p_audiodev(a3)
  BNE .error_noaudiodev   ;somebody already owns the hardware (could be us!)
  MOVE.l  p_audioreq(a3),a1           ;bernd#1
  MOVE.w  #CMD_RESET,IO_COMMAND(a1)
  BSR.w BeginIO       ;clear attach, stop sound.
  MOVE.l  p_audioport(a3),a0
  call  WaitPort
  MOVE.l  p_audioport(a3),a0
  call  GetMsg

  MOVE.l  pb_DosLib(a5),a6
  MOVEQ #1,d1
  call  Delay

* test If mode supports recording
  BTST  #PB_14BIT,p_Flags(a3)
  BNE .dontgetsampler     ;no record if 14 bit mode

* try To allocate parallel port
  CLR.b p_Parallel(a3)
  MOVE.l  pb_MiscResource(a5),a6
  MOVEQ #MR_PARALLELBITS,d0
  LEA IDString(pc),a1
  JSR MR_ALLOCMISCRESOURCE(a6)
  MOVE.l  d0,p_ParBitsUser(a3)
  BNE .no_parrallel
  MOVEQ #MR_PARALLELPORT,d0
  LEA IDString(pc),a1
  JSR MR_ALLOCMISCRESOURCE(a6)
  MOVE.l  d0,p_ParPortUser(a3)
  BNE .no_parrallel

  MOVE.b  #TRUE,p_Parallel(a3)
  ;move.b #0,_ciaa+ciaddrb      ;make PB0-PB7 inputs
.no_parrallel

 ;* allocate Aura sampler
  CLR.l p_AuraAddress(a3)
  MOVE.l  pb_CardResource(a5),d0
  BEQ .no_aura
  MOVE.l  d0,a6
  call  GetCardMap
  TST.l d0
  BEQ .no_aura
  MOVE.l  d0,a0
  MOVE.l  cmm_IOMemory(a0),d2
  BEQ .no_aura

  base  exec
  MOVEQ #CardHandle_SIZEOF,d0
  MOVE.l  #MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_CardHandle(a3)
  BEQ .no_aura

  MOVE.l  pb_CardResource(a5),a6
  MOVE.l  d0,a1
  MOVE.l  #IDString,LN_NAME(a1)
  MOVE.b  #CARDF_RESETREMOVE|CARDF_IFAVAILABLE,cah_CardFlags(a1)
  call  OwnCard
  TST.l d0
  BNE .no_aura

  MOVE.l  p_CardHandle(a3),a1
  call  BeginCardAccess
  TST.l d0
  BEQ .no_aura

  MOVE.l  d2,p_AuraAddress(a3)
.no_aura
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
  MOVE.b  #NT_INTERRUPT,LN_TYPE+p_RecInt(a3)
  MOVE.l  #LibName,LN_NAME+p_RecInt(a3)
  MOVE.l  #Interrupt_Dummy,IS_CODE+p_RecInt(a3)
  CLR.l IS_DATA+p_RecInt(a3)

 ;* p_RecSoftInt (caused by p_RecInt when the record buffer has been filled)
  MOVE.b  #32,LN_PRI+p_RecSoftInt(a3)
  MOVE.b  #NT_INTERRUPT,LN_TYPE+p_RecSoftInt(a3)
  MOVE.l  #LibName,LN_NAME+p_RecSoftInt(a3)
  MOVE.l  #RecordSoftInt,IS_CODE+p_RecSoftInt(a3)
  MOVE.l  a3,IS_DATA+p_RecSoftInt(a3)

;* Make sure no interrupts occur until AHIsub_Start() is called
  MOVE.w  #INTF_AUD0!INTF_AUD1!INTF_AUD2!INTF_AUD3,CUSTOM+INTENA

;* Update ahiac_MixFreq to what the mixing/sampling frequency really is
  MOVE.l  ahiac_MixFreq(a2),d1
  BSR calcperiod
  MOVE.l  d0,ahiac_MixFreq(a2)    ;store actual freq

  MOVEQ #AHISB_KNOWHIFI|AHISF_KNOWSTEREO|AHISF_CANRECORD|AHISF_MIXING|AHISF_TIMING,d0
.exit
  popm  d2-d7/a2-a6
  RTS

.error_noaudiodev
.error_noreqmem
.error_noport
.error_nopaula
  MOVEQ #AHISF_ERROR,d0
  BRA.b .exit

.audiochannelarray
  Dc.b  1+2+4+8
.audioname
  AUDIONAME
.calibname
  Dc.b  "ENV:CyberSound/SoundDrivers/14Bit_Calibration",0
  Even
.cardhandle:
  Dc.l  0,0   ;ln_Succ, ln_Pred
  Dc.b  0   ;ln_Type
  Dc.b  0   ;ln_Pri
  Dc.l  IDString  ;ln_Name
  Dc.l  0,0,0   ;cah_CardRemoved, cah_CardInserted, cah_CardStatus
  Dc.b  (CARDF_RESETREMOVE|CARDF_IFAVAILABLE)
  Even

;in:
;* d1 MixFreq
;* a5 paulaBase
;out:
;* d0 New MixFreq
;* d1.w Period
;description:
;*  Calculate and return the best period and the actual frequency.
calcperiod:
	pushm d2-d5
	move.l d1,d5
	MOVE.l #0,d0
	jsr $F0FFB0
	MOVE.l d0,d1
	MOVE.l #1,d0
	jsr $F0FFB0
	; now, d0 contains the divisor UAE uses for its output frequency.
	; d1 contains the nominal output frequency of UAE.
	; Multiply the two and divide by desired frequency (which we'll
	; assume to be 44100 Hz), and we get the necessary divider
    	MOVE.l  pb_UtilLib(a5),a1
   	jsr _LVOUMult32(a1)
	MOVE.l #44100,d5  ; @@@ potentially change this
  	MOVE.l d5,d1
        jsr _LVOUDivMod32(a1)
	MOVE.l  d0,d1
        MOVE.l  d5,d0
	popm  d2-d5
	RTS

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

freqlist:
  Dc.l  4410          ; CD/10
  Dc.l  4800          ; DAT/10
  Dc.l  5513          ; CD/8
  Dc.l  6000          ; DAT/8
  Dc.l  7350          ; CD/6
  Dc.l  8000          ; 5- and A-Law, DAT/6
  Dc.l  9600          ; DAT/5
  Dc.l  11025         ; CD/4
  Dc.l  12000         ; DAT/4
  Dc.l  14700         ; CD/3
  Dc.l  16000         ; DAT/3
  Dc.l  17640         ; CD/2.5
  Dc.l  18900
  Dc.l  19200         ; DAT/2.5
  Dc.l  22050         ; CD/2
  Dc.l  24000         ; DAT/2
  Dc.l  27429
FREQUENCIES_OCS   EQU (*-freqlist)
  Dc.l  29400         ; CD/1.5
uae Dc.l  32000         ; DAT/1.5
  ;dc.l 33075
  ;dc.l 37800
  Dc.l  44100         ; CD
  ;dc.l 48000         ; DAT
FREQUENCIES   EQU (*-uae)
  Dc.l  -1

* _CreateTable directly stolen from Christian Buchner's CyberSound
* audio SUB system (with permission).

* _CreateTable **************************************************************

    ; Parameters

    ; a0 = Table address
    ; (MUST have enough space for 65536 UWORDS)
    ; a1 = Additive Array
    ; 256 UBYTEs
    ;
    ; the table is organized as follows:
    ; 32768 UWORDS positive range, ascending order
    ; 32768 UWORDS negative range, ascending order
    ; access: (a0,d0.l*2)
    ; where d0.w is signed word sample data
    ; and the upper word of d0.l is *cleared!*


_CreateTable  MOVEM.l a2/d2-d6,-(sp)

    LEA 128(a1),a2

    MOVE.l  a2,a1     ; count the number of steps
    MOVEQ #128-1,d0   ; in the positive range
    MOVEQ #0,d5
.countpositive  MOVE.b  (a1)+,d1
    EXT.w d1
    EXT.l d1
    ADD.l d1,d5
    DBRA  d0,.countpositive ; d5=number of steps
    MOVE.l  #32768,d6   ; reset stretch counter

    MOVE.l  a2,a1     ; middle value in calibdata
    MOVE.w  #32768-1,d0   ; number of positive values -1
    MOVEQ #0,d1     ; HI value
    MOVEQ #0,d2     ; LO value
    MOVEQ #0,d3     ; counter
.fetchnext2 MOVE.b  (a1)+,d4    ; add calibtable to counter
    EXT.w d4
    ADD.w d4,d3
.outerloop2 TST.w d3
    BGT.s .positive2
.negative2  ADDQ.w  #1,d1     ; increment HI value
    SUB.w d4,d2     ; reset LO value
    BRA.s .fetchnext2
.positive2  MOVE.b  d1,(a0)+    ; store HI and LO value
    MOVE.b  d2,(a0)+
    SUB.l d5,d6     ; stretch the table
    BPL.s .repeat2    ; to 32768 entries
    ADD.l #32768,d6
    ADDQ.w  #1,d2     ; increment LO value
    SUBQ.w  #1,d3     ; decrement counter
.repeat2  DBRA  d0,.outerloop2

    MOVE.l  a2,a1     ; count the number of steps
    MOVEQ #128-1,d0   ; in the negative range
    MOVEQ #0,d5
.countnegative  MOVE.b  -(a1),d1
    EXT.w d1
    EXT.l d1
    ADD.l d1,d5
    DBRA  d0,.countnegative ; d5=number of steps
    MOVE.l  #32768,d6   ; reset stretch counter

    ADD.l #2*32768,a0   ; place at the end of the table
    MOVE.l  a2,a1     ; middle value in calibdata
    MOVE.w  #32768-1,d0   ; number of negative values -1
    MOVEQ #-1,d1      ; HI value
    MOVEQ #-1,d2      ; LO value
    MOVEQ #0,d3     ; counter
.fetchnext1 MOVE.b  -(a1),d4    ; add calibtable to counter
    EXT.w d4
    ADD.w d4,d3
    ADD.w d4,d2     ; maximize LO value
.outerloop1 TST.w d3
    BGT.s .positive1
.negative1  SUBQ.w  #1,d1
    BRA.s .fetchnext1
.positive1  MOVE.b  d2,-(a0)    ; store LO and HI value
    MOVE.b  d1,-(a0)
    SUB.l d5,d6     ; stretch the table
    BPL.s .repeat1    ; to 32768 entries
    ADD.l #32768,d6
    SUBQ.w  #1,d2     ; decrement lo value
    SUBQ.w  #1,d3     ; decrement counter
.repeat1  DBRA  d0,.outerloop1

    MOVEM.l (sp)+,a2/d2-d6
    RTS


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

  MOVE.l  p_CalibrationTable(a3),d0
  BEQ.b .notable
  MOVE.l  d0,a1
  call  FreeVec
.notable
  TST.l p_audiodev(a3)
  BNE.b .noaudiodev
  MOVE.l  p_audioreq(a3),a1         ;bernd#1
  MOVE.w  #CMD_RESET,IO_COMMAND(a1) ;Restore audio interrupts
  BSR.w BeginIO
  MOVE.l  p_audioport(a3),a0
  call  WaitPort
  MOVE.l  p_audioport(a3),a0
  call  GetMsg
  MOVE.l  p_audioreq(a3),a1
  SUBQ.l  #1,p_audiodev(a3)
  call  CloseDevice
.noaudiodev
  MOVE.l  p_audioreq(a3),d0
  BEQ.b .noaudioreq
  MOVE.l  d0,a1
  call  FreeVec
.noaudioreq
  MOVE.l  p_audioport(a3),d0
  BEQ.b .noaudioport
  MOVE.l  d0,a0
  call  DeleteMsgPort
.noaudioport
  MOVE.l  pb_CardResource(a5),d0
  BEQ .noaura
  MOVE.l  d0,a6
  MOVE.l  p_CardHandle(a3),d0
  BEQ .noaura
  MOVE.l  d0,a1
  call  EndCardAccess
  MOVE.l  p_CardHandle(a3),a1
  MOVEQ #CARDF_REMOVEHANDLE,d0
  call  ReleaseCard
.noaura
  MOVE.l  pb_SysLib(a5),a6
  MOVE.l  p_CardHandle(a3),d0
  BEQ .nocardhandle
  MOVE.l  d0,a1
  call  FreeVec
.nocardhandle
  MOVE.l  pb_MiscResource(a5),a6
  TST.l p_ParPortUser(a3)
  BNE.b .noparport
  MOVEQ #MR_PARALLELPORT,d0
  JSR MR_FREEMISCRESOURCE(a6)
.noparport
  TST.l p_ParBitsUser(a3)
  BNE.b .noparbits
  MOVEQ #MR_PARALLELBITS,d0
  JSR MR_FREEMISCRESOURCE(a6)
.noparbits
  MOVE.l  pb_SysLib(a5),a6
  MOVE.l  a3,a1
  CLR.l ahiac_DriverData(a2)
  call  FreeVec
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
*       If you are lazy, Then Call exec.library/Disable().
*       If you are smart, only Disable your own interrupt OR task.
*
*   INPUTS
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   NOTES
*       This Call should be guaranteed To preserve all registers.
*       This Call nests.
*
*   SEE ALSO
*       AHIsub_Enable(), exec.library/Disable()
*
*****************************************************************************
*
*

AHIsub_Disable:
  push  a3
  MOVE.l  ahiac_DriverData(a2),a3
  ADDQ.w  #1,p_DisableCount(a3)
  MOVE.w  #INTF_AUD0,CUSTOM+INTENA
;  moveq #2,d0
;  moveq #0,d1
;  jsr.l $f0ffb0
  pop a3
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
*       If you are lazy, Then Call exec.library/Enable().
*       If you are smart, only Enable your own interrupt OR task.
*
*   INPUTS
*       audioctrl - Pointer To an AHIAudioCtrlDrv structure.
*
*   NOTES
*       This Call should be guaranteed To preserve all registers.
*       This Call nests.
*
*   SEE ALSO
*       AHIsub_Disable(), exec.library/Enable()
*
*****************************************************************************
*
*

AHIsub_Enable:
  push  a3
  MOVE.l  ahiac_DriverData(a2),a3
  SUBQ.w  #1,p_DisableCount(a3)
  BNE.b .exit
  MOVE.w  #INTF_SETCLR!INTF_AUD0,CUSTOM+INTENA
;  moveq #2,d0
;  moveq #1,d1
;  jsr.l $f0ffb0
.exit
  pop a3
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
*       1) Call the user Hook ahiac_PlayerFunc with the following parameters:
*                  A0 - (struct Hook *)
*                  A2 - (struct AHIAudioCtrlDrv *)
*                  A1 - Set To Null.
*
*       2) [Call the ahiac_PreTimer Function. If it returns True (Z will be
*          cleared so you don't have To test d0), skip Step 3 AND 4. This
*          is Used To avoid overloading the CPU. This Step is optional.
*          A2 is assumed To Point To struct AHIAudioCtrlDrv. All registers
*          except d0 are preserved.  (V4)]
*
*       3) Call the mixing Hook (ahiac_MixerFunc) with the following
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
*       5) [Call the ahiac_PostTimer Function. A2 is assumed To Point To
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
*       Call ahiac_PlayerFunc ahiac_PlayerFreq times per second. Only your
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
*       Call the ahiac_SamplerFunc Hook with the following parameters:
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
*       If this Call failed.
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

  MOVE.l  d0,d7
  LEA CUSTOM,a4
  MOVE.l  ahiac_DriverData(a2),a3

  BTST  #AHISB_PLAY,d0
  BEQ .dont_play

**
*** AHISB_PLAY
**
  MOVEQ #AHISF_PLAY,d0
  call  AHIsub_Stop     ;Stop current playback if any.
  call  AHIsub_Update     ;fill variables

  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6

  MOVE.l  ahiac_BuffSize(a2),d0
  MOVE.l  #MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_Mixbuffer(a3)
  BEQ .error_nomem

  MOVE.l  ahiac_MixFreq(a2),d1
  BSR.w calcperiod
  MOVE.l  d1,p_AudPer(a3)
        MOVE.l #0,flag
        CMP.w #$7f,d1 ;use old routines
        BGE .10
        MOVE.l #1,flag
.10
* The init*bit? routines allocates p_DMAbuffer, sets up p_AudPtr, sets the Volume,
* stores the correct interrupt routines in p_PlayInt's AND p_PlaySoftInt's IS_CODE.

  PEA .1(pc)
  MOVE.b  p_Flags(a3),d0
  AND.b #PF_STEREO!PF_14BIT,d0
  BEQ.w init16bitM
  CMP.b #PF_STEREO,d0
  BEQ.w init16bitS
  CMP.b #PF_14BIT,d0
  BEQ.w init14bitM
  BRA.w init14bitS
.1
  TST.l d0
  BNE .exit

* Install play interrupt
  LEA p_PlayInt(a3),a1
  MOVEQ #INTB_AUD0,d0
  call  SetIntVector
  CLR.w p_DisableCount(a3)

  MOVE.w  #INTF_SETCLR!INTF_AUD0,INTENA(a4)  ;enable
  MOVE.w  #INTF_SETCLR!INTF_AUD0,INTREQ(a4)  ;start
  MOVE.l  a5,a6


.dont_play
  BTST  #AHISB_RECORD,d7
  BEQ .dont_record
**
*** AHISB_RECORD
**
  BTST  #PB_14BIT,p_Flags(a3)   ;Sanity check...
  BNE .error_unknown

  MOVEQ #AHISF_RECORD,d0
  call  AHIsub_Stop     ;Stop current recording if any.

  MOVE.l  a6,a5
  MOVE.l  pb_SysLib(a5),a6

  MOVE.l  #RECORDSAMPLES*4,d0
  MOVE.l  #MEMF_PUBLIC,d1
  call  AllocVec
  MOVE.l  d0,p_RecBuffer1(a3)
  BEQ .error_nomem

  MOVE.l  d0,p_RecFillPtr(a3)
  MOVE.w  #RECORDSAMPLES,p_RecFillCount(a3)

  MOVE.l  #RECORDSAMPLES*4,d0
  MOVE.l  #MEMF_PUBLIC,d1
  call  AllocVec
  MOVE.l  d0,p_RecBuffer2(a3)
  BEQ .error_nomem

  MOVE.l  ahiac_MixFreq(a2),d1
  BSR.w calcperiod
  LSR.w #1,d1       ;Period/2 => Frequency72
  MOVE.w  d1,AUD2PER(a4)
  MOVE.w  d1,AUD3PER(a4)
  MOVE.w  p_MonitorVolume(a3),AUD2VOL(a4)
  MOVE.w  p_MonitorVolume(a3),AUD3VOL(a4)

* Install Record interrupt
  MOVE.w  p_Input(a3),d0
  BEQ .parsampler
  CMP.w #1,d0
  BEQ .aurasampler
  BRA .error_unknown
.parsampler
  TST.b p_Parallel(a3)      ;Parrallel port allocated?
  BEQ .error_unknown
  LEA p_RecIntData(a3),a1
  MOVE.l  a1,IS_DATA+p_RecInt(a3)
  MOVE.l  #RecordInterrupt,IS_CODE+p_RecInt(a3)
  BRA .setrecint
.aurasampler
  TST.l p_AuraAddress(a3)   ;Aura sampler allocated?
  BEQ .error_unknown
  LEA p_RecIntDataAura(a3),a1
  MOVE.l  a1,IS_DATA+p_RecInt(a3)
  MOVE.l  #RecordInterruptAura,IS_CODE+p_RecInt(a3)
.setrecint
  LEA p_RecInt(a3),a1
  MOVEQ #INTB_AUD3,d0
  call  SetIntVector

  MOVE.w  #DMAF_AUD2!DMAF_AUD3,DMACON(a4)   ;disable DMA
  MOVE.w  #INTF_SETCLR!INTF_AUD3,INTENA(a4) ;enable
  MOVE.w  #INTF_SETCLR!INTF_AUD3,INTREQ(a4) ;start

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
* a4  CUSTOM
* a5  paulaBase
* a6  ExecBase

init16bitM:
  MOVE.l  #AudioInterrupt2,IS_CODE+p_PlayInt(a3)
  MOVE.l  #SoftInt_16bitM,IS_CODE+p_PlaySoftInt(a3)

  MOVE.l  ahiac_MaxBuffSamples(a2),d0 ;Max. # of 8 bit samples

  ADDQ.l  #1,d0
  BCLR  #0,d0       ;force even
  MOVE.l  d0,d2       ;d2 = channel size
  LSL	#1,d2
;  move.l d0,ahiac_BuffSamples(a2)
  LSL.l   #1,d0       ;Double buffer
  LSL.l   #1,d0       ;it's actually 16 bits
  MOVE.l  #MEMF_CHIP!MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_DMAbuffer(a3)
  BEQ .nomem

  MOVE.l  d0,p_AudPtr1A(a3)
  MOVE.l  d0,p_AudPtr2A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr1B(a3)
  MOVE.l  d0,p_AudPtr2B(a3)

  MOVEQ #0,d0
  RTS
.nomem
  MOVEQ #AHIE_NOMEM,d0
  RTS

;in:
* a2  AudioCtrl
* a3  paula
* a5  paulaBase
* a6  ExecBase
init16bitS:
  MOVE.l  #AudioInterrupt2,IS_CODE+p_PlayInt(a3)
  MOVE.l  #SoftInt_16bitS,IS_CODE+p_PlaySoftInt(a3)

  MOVE.l  ahiac_MaxBuffSamples(a2),d0 ;Max. # of 8 bit samples
  ADDQ.l  #1,d0
  BCLR  #0,d0       ;force even
  MOVE.l  d0,d2       ;d2 = channel size
  LSL	#1,d2
;  move.l d0,ahiac_BuffSamples(a2)

  LSL.l #2,d0       ;Double buffer + Stereo
  LSL.l   #1,d0       ;it's actually 16 bits
  MOVE.l  #MEMF_CHIP!MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_DMAbuffer(a3)
  BEQ .nomem

  MOVE.l  d0,p_AudPtr1A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr2A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr1B(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr2B(a3)

  MOVEQ #0,d0
  RTS
.nomem
  MOVEQ #AHIE_NOMEM,d0
  RTS

;in:
* a2  AudioCtrl
* a3  paula
* a5  paulaBase
* a6  ExecBase
init14bitM:
  MOVE.l  #AudioInterrupt4,IS_CODE+p_PlayInt(a3)
  LEA SoftInt_14bitM(pc),a0
  TST.l p_CalibrationTable(a3)
  BEQ.b .nocalib
  LEA SoftInt_14CbitM(pc),a0
.nocalib
  MOVE.l  a0,IS_CODE+p_PlaySoftInt(a3)

  MOVE.l  ahiac_MaxBuffSamples(a2),d0 ;Max. # of 16 bit samples
  ADDQ.l  #1,d0
  BCLR  #0,d0       ;force even
  MOVE.l  d0,d2       ;d2 = channel size

  LSL.l #2,d0       ;Double buffer + 2W8 bit
  MOVE.l  #MEMF_CHIP!MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_DMAbuffer(a3)
  BEQ .nomem

  MOVE.l  d0,p_AudPtr1A(a3)
  MOVE.l  d0,p_AudPtr2A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr4A(a3)
  MOVE.l  d0,p_AudPtr3A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr1B(a3)
  MOVE.l  d0,p_AudPtr2B(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr4B(a3)
  MOVE.l  d0,p_AudPtr3B(a3)

  MOVE.w  #64,AUD0VOL(a4)
  MOVE.w  #64,AUD1VOL(a4)
  MOVE.w  #1,AUD2VOL(a4)
  MOVE.w  #1,AUD3VOL(a4)

  MOVEQ #0,d0
  RTS
.nomem
  MOVEQ #AHIE_NOMEM,d0
  RTS

;in:
* a2  AudioCtrl
* a3  paula
* a5  paulaBase
* a6  ExecBase
init14bitS:
  MOVE.l  #AudioInterrupt4,IS_CODE+p_PlayInt(a3)
  LEA SoftInt_14bitS(pc),a0
  TST.l p_CalibrationTable(a3)
  BEQ .nocalib
  ;lea  SoftInt_14CbitS(pc),a0         changed for uae
.nocalib
  MOVE.l  a0,IS_CODE+p_PlaySoftInt(a3)


  MOVE.l  ahiac_MaxBuffSamples(a2),d0 ;Max. # of 16 bit samples
        ASL.l #2,d0
        MOVE.l d0,ahiac_MaxBuffSamples(a2)
  ADDQ.l  #1,d0
  BCLR  #0,d0       ;force even
  MOVE.l  d0,d2       ;d2 = channel size

  LSL.l #3,d0       ;Double buffer + 2W8 bit + Stereo
  MOVE.l  #MEMF_CHIP!MEMF_PUBLIC|MEMF_CLEAR,d1
  call  AllocVec
  MOVE.l  d0,p_DMAbuffer(a3)
  BEQ .nomem

  MOVE.l  d0,p_AudPtr1A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr2A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr3A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr4A(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr1B(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr2B(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr3B(a3)
  ADD.l d2,d0
  MOVE.l  d0,p_AudPtr4B(a3)

  MOVE.w  #$4849,AUD0VOL(a4)        ;changed for uae
  MOVE.w  #64,AUD1VOL(a4)
  MOVE.w  #1,AUD2VOL(a4)
  MOVE.w  #1,AUD3VOL(a4)

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
*       This Call must be safe from interrupts.
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
*       It must be safe To Call this routine Even If AHIsub_Start() was never
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
  BTST  #AHISB_PLAY,d0
  BEQ .dontplay

**
*** AHISB_PLAY
**
  MOVEQ #2,d0
  MOVEQ #0,d1
  jsr.l $F0FFB0
  MOVEQ #3,d0
  MOVEQ #0,d3
  jsr.l $F0FFB0
  MOVEQ #4,d0
  MOVEQ #0,d3
  jsr.l $F0FFB0

  MOVEQ #0,d0
  MOVE.w  d0,AUD0VOL(a4)
  MOVE.w  d0,AUD1VOL(a4)
  MOVE.w  d0,AUD2VOL(a4)
  MOVE.w  d0,AUD3VOL(a4)
  MOVE.w  #DMAF_AUDIO,DMACON(a4)    ;disable audio DMA

  MOVE.w  #INTF_AUD0,INTENA(a4)
  MOVE.w  #INTF_AUD0,INTREQ(a4)   ;Clear any waiting interrupts
  MOVE.l  #Interrupt_Dummy,IS_CODE+p_PlayInt(a3)
  LEA p_PlayInt(a3),a1
  MOVEQ #INTB_AUD0,d0
  call  SetIntVector

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
  BTST  #PB_14BIT,p_Flags(a3)   ;Sanity check...
  BNE .dontrecord

  MOVE.w  #INTF_AUD3,INTENA(a4)
  MOVE.w  #INTF_AUD3,INTREQ(a4)   ;Clear any waiting interrupts

  MOVE.l  #Interrupt_Dummy,IS_CODE+p_RecInt(a3)
  LEA p_RecInt(a3),a1
  MOVEQ #INTB_AUD3,d0
  call  SetIntVector

  MOVE.w  #0,AUD2VOL(a4)
  MOVE.w  #0,AUD3VOL(a4)

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
*           AHIDB_Realtime - Return True OR False.
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

* Only For AHIDB_Paula14Bit False (== recording available)
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
  MOVE.l  #RECORDSAMPLES,d0
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
  MOVEQ #2,d0
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
  lsr.l #2,d0
  BRA .exit
.not_freqs

  CMP.l #AHIDB_Frequency,d0
  BNE.b .not_freq
  ADD.w d1,d1
  ADD.w d1,d1
  LEA uae(pc),a0
; move.l  (a0,d1.w),d1
; bsr calcperiod
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
.author   Dc.b  "Martin 'Leviticus' Blom",0
.copyright  Dc.b  "Public Domain",0
.anno   Dc.b  "14 bit routines by Christian Buchner.",0
.input0   Dc.b  "Parallel port sampler",0
.input1   Dc.b  "Aura sampler",0
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
*       something, Return True. If attribute is unknown To you OR unsupported,
*       Return False.
*
*   NOTES
*       This Call must be safe from interrupts.
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
  MOVE.w  #INTF_AUD0!INTF_AUD1!INTF_AUD2!INTF_AUD3,INTREQ(a0)
SoftInt_Dummy:
  RTS

;in:
* d0  scratch
* d1  INTENAR && INTREQR
* a0  CUSTOM
* a1  &(paulaBase->p_RecIntData)
* a5  &RecordInterrupt
* a6  ExecBase
RecordInterrupt:

* This Function will be executed up To 28000 times per second - that's once per
* rasterline! It has To be as fast as possible.

  MOVE.w  #INTF_AUD2!INTF_AUD3,INTREQ(a0) ;Clear the interrupt flags
  MOVEQ #0,d0
  ;move.b _ciaa+ciaprb,d0     ;read parallel port
 IFD  MC020
  ;move.l convtable(pc,d0.w*4),d0   ;1 unsigned byte -> 2 signed words
 Else
  ADD.w d0,d0
  ADD.w d0,d0
  ;move.l convtable(pc,d0.w),d0
 ENDC
  MOVE.w  d0,AUD2DAT(a0)      ;right
  MOVE.w  d0,AUD3DAT(a0)      ;left

  MOVE.l  (a1),a5       ;p_RecFillPtr
  MOVE.l  d0,(a5)+      ;store sample in buffer
  MOVE.l  a5,(a1)+      ;update pointer
  SUBQ.w  #1,(a1)       ;p_ReqFillCount
  BEQ ri_Filled     ;branch if buffer filled
  RTS

;convtable
;CNT  SET 0
; REPT  256
; dc.b  CNT-128,CNT-128,CNT-128,CNT-128
; SET CNT+1
; ENDR

;in:
* d0  scratch
* d1  INTENAR && INTREQR
* a0  CUSTOM
* a1  &(paulaBase->p_RecIntDataAura)
* a5  &RecordInterrupt
* a6  ExecBase
RecordInterruptAura:
  MOVE.w  #INTF_AUD2!INTF_AUD3,INTREQ(a0) ;Clear the interrupt flags
  MOVE.l  (a1)+,a5
  MOVE.l  (a5),d0       ;read aura sampler
  EOR.l #$80008000,d0
  MOVE.l  (a1),a5       ;p_RecFillPtr
  MOVE.l  d0,(a5)+      ;store sample in buffer
  MOVE.l  a5,(a1)+      ;update pointer

  MOVE.w  d0,d1
  LSR.w #8,d0
  MOVE.b  d0,d1

  MOVE.w  d1,AUD2DAT(a0)      ;right
  MOVE.w  d1,AUD3DAT(a0)      ;left

  SUBQ.w  #1,(a1)       ;p_ReqFillCount
  BEQ ri_Filled     ;branch if buffer filled
  RTS

ri_Filled:

* This part is only executed every RECORDSAMPLES:th time... No need To hurry.

  MOVE.l  8(a1),d0      ;p_RecBuffer2->
  MOVE.l  4(a1),8(a1)     ;p_RecBuffer1->p_RecBuffer2
  MOVE.l  d0,4(a1)      ;            ->p_RecBuffer1
  MOVE.l  d0,-4(a1)     ;p_RecFillPtr
  MOVE.w  #RECORDSAMPLES,(a1)   ;p_ReqFillCount
  MOVE.l  12(a1),a1     ;p_RecSoftIntPtr
  JMP _LVOCause(a6)


;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  paulaBase
* a5  scratch
RecordSoftInt:

* This Function is NOT executed many times per second AND is therefore NOT
* fully optimized... ;)
  push  a2
  MOVE.w  p_MonitorVolume(a1),CUSTOM+AUD2VOL
  MOVE.w  p_MonitorVolume(a1),CUSTOM+AUD3VOL
  MOVE.l  p_RecBuffer2(a1),p_rmBuffer(a1)
  MOVE.l  p_AudioCtrlP(a1),a2
  LEA p_RecordMessage(a1),a1
  MOVE.l  ahiac_SamplerFunc(a2),a0
  MOVE.l  h_Entry(a0),a5
  JSR (a5)
  pop a2
  RTS

;in:
* d0  scratch
* d1  INTENAR && INTREQR
* a0  CUSTOM
* a1  &(paulaBase->p_PlayIntData)
* a5  &AudioInterrupt
* a6  ExecBase

AudioInterrupt2:        ;Two hardware channels used
	MOVEM.l a2/d2/d3,-(sp)
	MOVEQ #1,d3       ; Enable UAE-side AHI
       	moveq #0,d2
	MOVE.w  4(a1),d2      ; Length
	MOVE.l  6(a1),d1      ; Period
	LEA.l   12(a1),a2
	NOT.w 10(a1)        ;double buffering
	BEQ.b .1
	ADD.w #4*4,a2
.1
  	LEA p_AudPtrs-p_PlaySoftIntPtr(a1),a5;

	MOVE.l  a2,(a5)
	MOVE.l  (a2)+,a0
	MOVEQ #3,d0
	jsr  $F0FFB0      ; Left channel
	MOVE.l  (a2)+,a0
	MOVEQ #4,d0
	jsr  $F0FFB0      ; Right channel

        moveq #2,d0
	moveq #1,d1
	jsr  $F0FFB0      ; Enable AHI.

	MOVE.l  (a1),a1       ;p_PlaySoftInt
	MOVEM.l (sp)+,a2/d2/d3
	MOVE.w  #INTF_AUD0,$DFF09C    ;Clear the interrupt
	JMP _LVOCause(a6)     ;start PlaySoftInt

AudioInterrupt4:        ;Four hardware channels used

  MOVEM.l (a1)+,d0/d1     ;p_PlaySoftIntPtr,p_AudLen/p_AudPer



  MOVE.l  d1,AUD1LEN(a0)
  MOVE.l  d1,AUD2LEN(a0)
  MOVE.l  d1,AUD3LEN(a0)

        MOVE.l  d1,AUD0LEN(a0)
  NOT.l (a1)+       ;double buffering

  LEA p_AudPtrs-p_AudPtr1A(a1),a5
  BEQ.b .1

  ADD.w #4*4,a1

.1


  MOVE.l  a1,(a5)
  MOVE.l  (a1)+,AUD0LC(a0)
  MOVE.l  (a1)+,AUD1LC(a0)
  MOVE.l  (a1)+,AUD2LC(a0)
  MOVE.l  (a1)+,AUD3LC(a0)
  MOVE.l  d0,a1       ;p_PlaySoftInt
        MOVE.w  #INTF_AUD0,INTREQ(a0)   ;Clear the interrupt

  JMP _LVOCause(a6)     ;start PlaySoftInt

;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  &(paulaBase->p_PlaySoftIntData)
* a5  scratch

SoftInt_16bitM:
  pushm a2/a3/a4/a6
  MOVE.l  a1,a5
  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  JSR (a3)        ;call Player Hook
  JSR (a4)        ;call PreTimer
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a5
  JSR (a3)        ;call Mixer Hook

* convert AND transfer Buffer
  MOVE.l  (a5),a0
.loop
  MOVE.w  (a1)+,(a0)+

;  MOVE.w  (a1)+,(a0)+
  DBF d0,.loop
.exit
  JSR (a6)        ;call PostTimer, a2 ok
  popm  a2/a3/a4/a6
  MOVE.w  #DMAF_SETCLR|DMAF_AUD0|DMAF_AUD1,CUSTOM+DMACON
  RTS

;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  &(paulaBase->p_PlaySoftIntData)
* a5  scratch

SoftInt_16bitS:
  pushm a2/a3/a4/a6
  MOVE.l  a1,a5
  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  JSR (a3)        ;call Player Hook
  JSR (a4)        ;call PreTimer
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a5
  JSR (a3)        ;call Mixer Hook

* convert AND transfer Buffer
  MOVEM.l (a5),a0/a3
.loop
; REPT 2
;left
  MOVE.w  (a1)+,(a3)+
;right
  MOVE.w  (a1)+,(a0)+
; ENDR
  DBF d0,.loop
.exit
  JSR (a6)        ;call PostTimer, a2 ok
  popm  a2/a3/a4/a6
  MOVE.w  #DMAF_SETCLR|DMAF_AUD0|DMAF_AUD1,CUSTOM+DMACON
  RTS

;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  &(paulaBase->p_PlaySoftIntData)
* a5  scratch

SoftInt_14bitM:
  pushm a2/a3/a4/a6
  MOVE.l  a1,a5
  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  JSR (a3)        ;call Player Hook
  JSR (a4)        ;call PreTimer
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a5
  JSR (a3)        ;call Mixer Hook

* convert AND transfer Buffer
  MOVEM.l (a5),a0/a3/a4/a5    ;a3/a4 unused
.loop
 REPT 2
  MOVE.b  (a1)+,(a0)+
  MOVE.b  (a1)+,d1
  LSR.b #2,d1
  MOVE.b  d1,(a5)+
 ENDR
  DBF d0,.loop
.exit
  JSR (a6)        ;call PostTimer, a2 ok
  popm  a2/a3/a4/a6
  MOVE.w  #DMAF_SETCLR|DMAF_AUD0|DMAF_AUD1|DMAF_AUD2|DMAF_AUD3,CUSTOM+DMACON
  RTS

;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  &(paulaBase->p_PlaySoftIntData)
* a5  scratch

SoftInt_14CbitM:
  pushm a2/a3/a4/a6
  MOVE.l  a1,a5
  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  JSR (a3)        ;call Player Hook
  JSR (a4)        ;call PreTimer
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a4/a5
  JSR (a3)        ;call Mixer Hook

* convert AND transfer Buffer, using the table
  MOVE.l  (a4),a0
  MOVE.l  12(a4),a4
  MOVEQ #0,d1
.loop

 REPT 2
 IFD  MC020
  MOVE.w  (a1)+,d1      ;fetch 16 bit sample
  MOVE.w  (a5,d1.l*2),d1
 Else
  MOVEQ #0,d1
  MOVE.w  (a1)+,d1      ;fetch 16 bit sample
  ADD.w d1,d1
  MOVE.w  (a5,d1.l),d1
 ENDC
  MOVE.b  d1,(a4)+      ;low byte
  LSR.w #8,d1
  MOVE.b  d1,(a0)+      ;high byte
 ENDR
  DBF d0,.loop
.exit
  JSR (a6)        ;call PostTimer, a2 ok
  popm  a2/a3/a4/a6
       MOVE.w #DMAF_SETCLR|DMAF_AUD0|DMAF_AUD1|DMAF_AUD2|DMAF_AUD3,CUSTOM+DMACON
  RTS

;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  &(paulaBase->p_PlaySoftIntData)
* a5  scratch

SoftInt_14bitS:
  pushm a2/a3/a4/a6
  MOVE.l  a1,a5

  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  JSR (a3)        ;call Player Hook
  JSR (a4)        ;call PreTimer
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a5
  JSR (a3)        ;call Mixer Hook
  push  a2
* convert AND transfer Buffer
* TODO: Use a table!

  MOVEM.l (a5),a0/a2/a3/a5
        ;move.l d2,-(a7)

.loop
 REPT 2
;left

        ;add.b #1,check
  MOVE.b  (a1)+,(a2)+                      ;used for uae
  MOVE.b  (a1)+,(a3)+
        ;move.b check,(a2)+
;right
  MOVE.b  (a1)+,(a0)+
  MOVE.b  (a1)+,(a5)+
 ENDR
  DBF d0,.loop
        ;movem.l (a7)+,d2
  pop a2
.exit
  JSR (a6)        ;call PostTimer, a2 ok
  popm  a2/a3/a4/a6
  ;move.w #DMAF_SETCLR|DMAF_AUD0|DMAF_AUD1|DMAF_AUD2|DMAF_AUD3,CUSTOM+DMACON
  RTS

;in:
* d0  scratch
* d1  scratch
* a0  scratch
* a1  &(paulaBase->p_PlaySoftIntData)
* a5  scratch

SoftInt_14CbitS:
  pushm a2/a3/a4/a6
  MOVE.l  a1,a5
  MOVEM.l (a5)+,a0/a1/a2/a3/a4/a6
  JSR (a3)        ;call Player Hook
  JSR (a4)        ;call PreTimer
  BNE .exit
  MOVEM.l (a5),d0/a0/a1/a2/a3/a4/a5
  JSR (a3)        ;call Mixer Hook
  push  a2
* convert AND transfer Buffer, using the table
  MOVEM.l (a4),a0/a2/a3/a4
  MOVEQ #0,d1
.loop
 REPT 2

;left
 IFD  MC020
  MOVE.w  (a1)+,d1      ;fetch 16 bit sample
  MOVE.w  (a5,d1.l*2),d1
 Else
  MOVEQ #0,d1
  MOVE.w  (a1)+,d1      ;fetch 16 bit sample
  ADD.w d1,d1
  MOVE.w  (a5,d1.l),d1
 ENDC
  MOVE.b  d1,(a3)+      ;low byte
  LSR.w #8,d1
  MOVE.b  d1,(a2)+      ;high byte

;right
 IFD  MC020
  MOVE.w  (a1)+,d1      ;fetch 16 bit sample
  MOVE.w  (a5,d1.l*2),d1
 Else
  MOVEQ #0,d1
  MOVE.w  (a1)+,d1      ;fetch 16 bit sample
  ADD.w d1,d1
  MOVE.w  (a5,d1.l),d1
 ENDC
  MOVE.b  d1,(a4)+      ;low byte
  LSR.w #8,d1
  MOVE.b  d1,(a0)+      ;high byte
 ENDR

  DBF d0,.loop
  pop a2
.exit
  JSR (a6)        ;call PostTimer
  popm  a2/a3/a4/a6
  ;move.w #DMAF_SETCLR|DMAF_AUD0|DMAF_AUD1|DMAF_AUD2|DMAF_AUD3,CUSTOM+DMACON
  RTS
flag    Dc.l 0
EndCode:
check        Dc.b 0
 END
