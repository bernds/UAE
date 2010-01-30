 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS Gravis Ultrasound interface.
  *
  * Version 0.3
  *
  * (c) 1996,1997 Michael Sontheimer
  *
  * Please, do not change the code ! Write me an email with the bugfix,
  * the improvement or the suggestion.
  * My email address: Michael.Sontheimer@primus.baynet.de
  *
  */


//#define DEBUG

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <pc.h>
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>


#include "sysconfig.h"
#include "sysdeps.h"
#include "sound/gus.h"
#include "sound/dma.h"



#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


// GUS Ports

#define GUS_COMMAND   0x103
#define GUS_DATAHI    0x105
#define GUS_DATALO    0x104
#define GUS_IRQSTATE  0x006
#define GUS_TIMERCTRL 0x008
#define GUS_TIMERDATA 0x009
#define GUS_VOICE     0x102
#define GUS_DRAM      0x107
#define GUS_MIXER     0x000
#define GUS_DMACTRL   0x00b     // Beide Adressen sind identisch !!!
#define GUS_IRQCTRL   0x00b
#define GUS_REGISTERCTRL 0x00f

// GUS Kommandos

#define GUS_CMD_RESET       0x4c
#define GUS_CMD_DRAMDMACTRL 0x41
#define GUS_CMD_DMASTARTADR 0x42
#define GUS_CMD_DRAMIOADRLO 0x43
#define GUS_CMD_DRAMIOADRHI 0x44
#define GUS_CMD_TIMERCTRL   0x45
#define GUS_CMD_COUNTER1    0x46
#define GUS_CMD_COUNTER2    0x47
#define GUS_CMD_SAMPLEFREQ  0x48
#define GUS_CMD_SAMPLECTRL  0x49

#define GUS_CMD_VOICECTRLW  0x00
#define GUS_CMD_FREQUCTRLW  0x01
#define GUS_CMD_STARTHIW    0x02
#define GUS_CMD_STARTLOW    0x03
#define GUS_CMD_ENDHIW      0x04
#define GUS_CMD_ENDLOW      0x05
#define GUS_CMD_RAMPRATEW   0x06
#define GUS_CMD_VOLUMEW     0x09
#define GUS_CMD_CURRENTHIW  0x0a
#define GUS_CMD_CURRENTLOW  0x0b
#define GUS_CMD_PANPOSW     0x0c
#define GUS_CMD_VOLUMECTRLW 0x0d
#define GUS_CMD_ACTIVEVOICESW 0x0e

#define GUS_CMD_VOICECTRLR  0x80
#define GUS_CMD_FREQUCTRLR  0x81
#define GUS_CMD_STARTHIR    0x82
#define GUS_CMD_STARTLOR    0x83
#define GUS_CMD_ENDHIR      0x84
#define GUS_CMD_ENDLOR      0x85
#define GUS_CMD_VOLUMER     0x89
#define GUS_CMD_CURRENTHIR  0x8a
#define GUS_CMD_CURRENTLOR  0x8b
#define GUS_CMD_PANPOSR     0x8c
#define GUS_CMD_VOLUMECTRLR 0x8d
#define GUS_CMD_ACTIVEVOICESR 0x8e
#define GUS_CMD_IRQSOURCER  0x8f

#define ADDR_HI(x) ((UWORD)((((ULONG)(x))>>7L)&0x1fffL))
#define ADDR_LO(x)  ((UWORD)((((ULONG)(x))&0x7fL)<<9L))


#define LSW( x ) ((UWORD)(((ULONG)(x))&0xffffL))
#define MSW( x ) ((UWORD)(((ULONG)(x))>>16L))
#define LSB( x ) ((UBYTE)(((UWORD)(x))&0xffL))
#define MSB( x ) ((UBYTE)(((UWORD)(x))>>8))
#define CONVTO16B( x ) ((((x)>>1L)&0x0001ffffL)|((x)&0x000c0000L))
#define NUMPARAS(bytesize) (((bytesize)+15) >> 4)



// Variablen


UWORD uwGUSPort;
UBYTE ubGUSGF1Int;
UBYTE ubGUSMIDIInt;
UBYTE ubGUSDMA1;
UBYTE ubGUSDMA2;
UBYTE ubGUSUsedBits;           // 0 if 8 bit or 4 if 16 bit
UWORD uwGUSUsedFreq;
UBYTE ubGUSGF1IRQVector;       // Physikalischer Vektor des GF1 IRQ's
UBYTE *pGUSBuffer;             // Zwischenpuffer
ULONG nGUSBufferWriteIndex;
ULONG nGUSBufferReadIndex;
ULONG ulBytesInBuffer;          // Anzahl der Bytes im Puffer
ULONG ulGUSSndBufSize;
ULONG ulDMABufMaxSize;


int bIgnoreGF1IRQ=TRUE;
int bDMAInProgress=FALSE;
int bWarnFlag=TRUE;             // Flag, ob Warnungen ausgeben werden sollen
int nWarnCnt=0;
#define MAXWARNS 5              // Anzahl der maximalen Warnungen

_go32_dpmi_seginfo   old_pmintGF1;  /* the old interrupt handler         */
_go32_dpmi_seginfo   pm_wrapperGF1; /* the new interrupt handler         */

UBYTE ubDummy;




static __inline__ void WriteGUSb( UWORD uwReg, UBYTE ubByte )
{
       outportb( uwGUSPort+uwReg, ubByte );
}

static __inline__ UBYTE ReadGUSb( UWORD uwReg )
{
       return inportb( uwGUSPort+uwReg );
}

static __inline__ void WriteGUSw( UWORD uwReg, UWORD uwWord )
{
       outportw( uwGUSPort+uwReg, uwWord );
}

static __inline__ UWORD ReadGUSw( UWORD uwReg )
{
       return inportw( uwGUSPort+uwReg );
}



_go32_dpmi_seginfo  dma_mem;            // Zeiger auf DMA-Speicher
ULONG dmabufadr;

void GUS_GF1Delay( void )
{

    int i;

    for( i=0; i<6; i++ )
        inportb( uwGUSPort+GUS_DATAHI );


}

/*******************************

void GUS_Reset( UBYTE ubGUSDMA1, UBYTE ubGUSDMA2, ubGUSGF1Int )

*******************************/

void GUS_Reset( UBYTE ubGUSDMA1, UBYTE ubGUSDMA2, UBYTE ubGUSGF1Int  )
{
    int i, j;
    UBYTE nDMA, nInt;

    WriteGUSb( GUS_MIXER, 1|2 );  // Latches aus, Alle Aus/Eingaben aus

    // Reset
    WriteGUSb( GUS_COMMAND, GUS_CMD_RESET );
    WriteGUSb( GUS_DATAHI, 0 );
    delay( 100 );
    WriteGUSb( GUS_COMMAND, GUS_CMD_RESET );
    WriteGUSb( GUS_DATAHI, 1 );
    delay( 100 );

    // DMA Control Register reset
    WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
    WriteGUSb( GUS_DATAHI, 0 );
    delay( 10 );

    // Timer Control Register
    WriteGUSb( GUS_COMMAND, GUS_CMD_TIMERCTRL );
    WriteGUSb( GUS_DATAHI, 0 );
    delay( 10 );

    // Sampling Control Register
    WriteGUSb( GUS_COMMAND, GUS_CMD_SAMPLECTRL );
    WriteGUSb( GUS_DATAHI, 0 );
    delay( 10 );

    // Number of Voices
    WriteGUSb( GUS_COMMAND, GUS_CMD_ACTIVEVOICESW );
    WriteGUSb( GUS_DATAHI, 0xc0 | (15-1) );             // 15 Voices (left/right)

    // Evtl. anstehende DMA-IRQ's holen
    ReadGUSb( GUS_IRQSTATE );
    WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
    ReadGUSb( GUS_DATAHI );

    // Evtl. Sampleinterrupts lesen
    WriteGUSb( GUS_COMMAND, GUS_CMD_SAMPLECTRL );
    ReadGUSb( GUS_DATAHI );

    // IRQ-Status lesen
    WriteGUSb( GUS_COMMAND, GUS_CMD_IRQSOURCER );
    ReadGUSb( GUS_DATAHI );

    // Alle Stimmen abschalten

    for( i=0; i<32; i++ )
    {
         WriteGUSb( GUS_VOICE, (UBYTE)i );
         WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
         WriteGUSb( GUS_DATAHI, 3 );                // Stimme stoppen
         WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
         WriteGUSb( GUS_DATAHI, 3 );                // Lautstaerke 0
         WriteGUSb( GUS_COMMAND, GUS_CMD_RAMPRATEW );
         WriteGUSb( GUS_DATAHI, 0 );                // Ramp 0

    }

    // Evtl. anstehende DMA-IRQ's holen
    ReadGUSb( GUS_IRQSTATE );
    WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
    ReadGUSb( GUS_DATAHI );

    // Evtl. Sampleinterrupts lesen
    WriteGUSb( GUS_COMMAND, GUS_CMD_SAMPLECTRL );
    ReadGUSb( GUS_DATAHI );

    // IRQ-Status lesen
    WriteGUSb( GUS_COMMAND, GUS_CMD_IRQSOURCER );
    ReadGUSb( GUS_DATAHI );


    // Reset beenden und GF1 Master IRQ einschalten
    WriteGUSb( GUS_COMMAND, GUS_CMD_RESET );
    WriteGUSb( GUS_DATAHI, 7 );


    if( ubGUSDMA1 == 0 )     // IRQ's und DMA einstellen ?
        return;             // Nein, dann Ende


    // Digital ASIC einstellen

    WriteGUSb( GUS_REGISTERCTRL, 5 );   // IRQ's-Register waehlen
    WriteGUSb( GUS_MIXER, 1|2 );    // Alles Abschalten
    WriteGUSb( GUS_IRQCTRL, 0 );    // IRQ's loeschen
    WriteGUSb( GUS_REGISTERCTRL, 0 );   // Alte Registerkonfig wieder einschalten

    // DMA selektieren

    nDMA=0;
    for( j=0; j<2; j++ )
    {
        switch( ((j!=0)?(ubGUSDMA1):(ubGUSDMA2)) )
        {
            case 1:
                nDMA |= 1;
                break;
            case 3:
                nDMA |= 2;
                break;
            case 5:
                nDMA |= 3;
                break;
            case 6:
                nDMA |= 4;
                break;
            case 7:
                nDMA |= 5;
                break;
        }
        if( j==0 )
            nDMA=nDMA<<3;
    }

    if( ubGUSDMA1 == ubGUSDMA2 )    // Gleichen DMA fuer beides ?
        nDMA = (nDMA&0x3)|(1<<6);


    // Interrupt der GUS setzen

    nInt=0;
    for( j=0; j<2; j++ )
    {
        switch( (j!=0)?(ubGUSGF1Int):(ubGUSMIDIInt) )
        {
            case 2:
                nInt |= 1;
                break;
            case 5:
                nInt |= 2;
                break;
            case 3:
                nInt |= 3;
                break;
            case 7:
                nInt |= 4;
                break;
            case 11:
                nInt |= 5;
                break;
            case 12:
                nInt |= 6;
                break;
            case 15:
                nInt |= 7;
                break;
        }
        if( j==0 )
            nInt=nInt<<3;
    }

    if( ubGUSGF1Int == ubGUSMIDIInt )
        nInt = (nInt&0x3)|(1<<6);



    disable();

    WriteGUSb( GUS_MIXER, 1|2 );     // DMA-Einstellung aktivieren
    WriteGUSb( GUS_DMACTRL, nDMA | 0x80 );    // DMA-Kanal selektieren, welcher im SET angegebe wurde


    WriteGUSb( GUS_MIXER, 1|2|64 );  // IRQ's selektieren
    WriteGUSb( GUS_IRQCTRL, nInt );

    WriteGUSb( GUS_MIXER, 1|2 );     // DMA-Einstellung aktivieren
    WriteGUSb( GUS_DMACTRL, nDMA );  // DMA-Kanal selektieren, welcher im SET angegebe wurde

    WriteGUSb( GUS_MIXER, 1|2|64 );  // IRQ's selektieren
    WriteGUSb( GUS_IRQCTRL, nInt );

    enable();

    // IRQ/DMA locken

    WriteGUSb( GUS_VOICE, 0 );

    WriteGUSb( GUS_MIXER, 8 | ((ubGUSGF1Int==ubGUSMIDIInt)?16:0 ) );

    WriteGUSb( GUS_VOICE, 0 );



}



/*******************************

    int GUS_Init( void )

*******************************/

int GUS_Init( int *dspbits, int *rate, int *sndbufsize, unsigned int *direct_buffers, int *stereo_sound )
{

    char *szGUSENVStr, *pz;
    int i, j;
    extern void (*SND_Write)(void *buf, unsigned long size);



    szGUSENVStr = getenv( "ULTRASND" );
    if( szGUSENVStr == 0 )
        return 0;


    if( (*dspbits != 8 ) && (*dspbits != 16 ) )
    {
        printf( "%d bits are not supported on the GUS !\n", dspbits );
        return 0;
    }


    ubGUSUsedBits = (*dspbits==8)?0:4;

    if( (*rate < 1000) || (*rate > 44100 ))
    {
        fprintf( stderr, "%dHz are not supported on the GUS !\n", *rate );
        return 0;
    }

    uwGUSUsedFreq = *rate;



    ulDMABufMaxSize = 8192;
    ulGUSSndBufSize = ((ULONG)*sndbufsize)*32;



    fprintf( stderr, "GUS configured for %d bits, %d Hz\n", *dspbits, *rate );
    fprintf( stderr, "Soundbuffersize: %d bytes\n", ulGUSSndBufSize );
    fprintf( stderr, "DMA-buffersize: %d bytes\n", ulDMABufMaxSize );
    fprintf( stderr, "Fragmentsize: %d bytes\n", *sndbufsize );


    uwGUSPort = (UWORD)strtol( szGUSENVStr, (char **)NULL, 16 );
    pz = strchr( szGUSENVStr, ',' );
    if( (pz == 0) || (uwGUSPort ==0)  )
        return 0;

    ubGUSDMA1 = (UBYTE)atol( pz+1 );
    pz = strchr( pz+1, ',' );
    if( pz == 0 )
        return 0;

    ubGUSDMA2 = (UBYTE)atol( pz+1 );

    pz = strchr( pz+1, ',' );
    if( pz == 0 )
        return 0;

    ubGUSGF1Int = (UBYTE)atol( pz+1 );
    if( ubGUSGF1Int == 0 )
        return 0;

    pz = strchr( pz+1, ',' );
    if( pz == 0 )
        return 0;

    ubGUSMIDIInt = (UBYTE)atol( pz+1 );
    if( ubGUSMIDIInt == 0 )
        return 0;

    // Speicher fuer DMA holen


    memset( &dma_mem, 0, sizeof( _go32_dpmi_seginfo ) );
    dma_mem.size = NUMPARAS( ulDMABufMaxSize+1000 );
    dmabufadr = DMA_AllocDMABuf( &dma_mem );


    _go32_dpmi_lock_code( GUS_PlayBuffer, (ULONG)GUS_DummyFunc - (ULONG)GUS_PlayBuffer);
    _go32_dpmi_lock_data( &uwGUSPort, (ULONG)&ubDummy - (ULONG)&uwGUSPort);


    if( dmabufadr == NULL)
    {
      fprintf( stderr, "Cannot allocate DOS memory for DMA buffers!\n" );
      return 0;
    }


    // Zwischenpuffer allokieren

    pGUSBuffer = (UBYTE *)malloc( ulGUSSndBufSize+1000 );
    if( pGUSBuffer == 0 )
    {
        fprintf( stderr, "Cannot allocate soundbuffer !\n" );
        return 0;
    }

    nGUSBufferWriteIndex = nGUSBufferReadIndex = 0;
    ulBytesInBuffer = 0;

    // GUS-Reset

    GUS_Reset( ubGUSDMA1, ubGUSDMA2, ubGUSGF1Int );


    // Test, ob wir in das GUS-RAM etwas schreiben koennen

    WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRLO );
    WriteGUSw( GUS_DATALO, 0 );
    WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRHI );
    WriteGUSw( GUS_DATAHI, 0 );

    WriteGUSb( GUS_DRAM, 0x15 );
    delay( 100 );
    if( ReadGUSb( GUS_DRAM ) != 0x15 )      // Bytes unterschiedlich ?
        return 0;

    // GUS-RAM loeschen

    j=-1;
    for( i=0; i<(int)(ulDMABufMaxSize*2+1000); i++ )
    {
        if( j != MSW(i) )       // Muss das Highword veraendert werden ?
        {
            j = MSW(i);

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRHI );
            WriteGUSw( GUS_DATALO, MSW( i ) );
        }

        WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRLO );
        WriteGUSw( GUS_DATALO,  LSW( i ) );

        WriteGUSb( GUS_DRAM, 0 );

    }


    // Interrupthandler installieren

    memset( &pm_wrapperGF1, 0, sizeof( _go32_dpmi_seginfo ) );
    pm_wrapperGF1.pm_offset = (int) GUS_GF1InterruptHandler;
    if(_go32_dpmi_allocate_iret_wrapper( &pm_wrapperGF1 ) )
    {
        fprintf( stderr, "Cannot allocate protected mode wrapper for GUS interrupt!\n");
        if( dmabufadr != 0 )
            _go32_dpmi_free_dos_memory( &dma_mem );

        return 0;
    }


    ubGUSGF1IRQVector = (ubGUSGF1Int>7)?(ubGUSGF1Int+0x68):(ubGUSGF1Int+0x08);   // Physik. Vektor berechnen

    _go32_dpmi_get_protected_mode_interrupt_vector( ubGUSGF1IRQVector, &old_pmintGF1 );   // Alten Interrupt merken
    _go32_dpmi_set_protected_mode_interrupt_vector( ubGUSGF1IRQVector, &pm_wrapperGF1 );  // Neuen Interrupt merken


    // Interrupt erlauben

    if( ubGUSGF1Int > 7 )
    {
        outportb( 0xA1, inportb( 0xA1 ) & ~(1 << ( ubGUSGF1Int-8 ) ) );
        outportb( 0xA0, 0x20 );
    }
    else
    {
        outportb( 0x21, inportb( 0x21 ) & ~(1 << ubGUSGF1Int));
        outportb( 0x20, 0x20 );
    }


    // Hier wird die Stimme zum erstenmal gestertet


    WriteGUSb( GUS_VOICE, 0 );                      // Stimme0

    WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMEW );      // Lautstaerke auf Maximum
    WriteGUSw( GUS_DATALO, 61247 );

    WriteGUSb( GUS_COMMAND, GUS_CMD_FREQUCTRLW );   // 15 Stimmen = x / 40!

//    WriteGUSw( GUS_DATALO, (UWORD)((((((((ULONG)uwGUSUsedFreq)/1000L)<<9L)+(44100L>>1L))/44100L)<<1L)) );
    WriteGUSw( GUS_DATALO, uwGUSUsedFreq/40 );


    WriteGUSb( GUS_COMMAND, GUS_CMD_CURRENTLOW );   // Current
    WriteGUSw( GUS_DATALO, 32 );
    WriteGUSb( GUS_COMMAND, GUS_CMD_CURRENTHIW );
    WriteGUSw( GUS_DATALO, 0 );

    WriteGUSb( GUS_COMMAND, GUS_CMD_STARTHIW );     // Start
    WriteGUSw( GUS_DATALO, 0 );
    WriteGUSb( GUS_COMMAND, GUS_CMD_STARTLOW );
    WriteGUSw( GUS_DATALO, 32 );

    if( ubGUSUsedBits == 0 )        // 8 oder 16 Bit ?
    {
        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDHIW );       // Ende setzen
        WriteGUSw( GUS_DATALO, ADDR_HI( ulDMABufMaxSize+32 ) );
        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDLOW );
        WriteGUSw( GUS_DATALO, ADDR_LO( ulDMABufMaxSize+32 ) );
    }
    else
    {
        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDHIW );       // Ende
        WriteGUSw( GUS_DATALO, ADDR_HI( CONVTO16B( ulDMABufMaxSize+32 ) ) );
        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDLOW );
        WriteGUSw( GUS_DATALO, ADDR_LO( CONVTO16B( ulDMABufMaxSize+32 ) ) );
    }

    WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
    WriteGUSb( GUS_DATAHI, 3|4 );
    GUS_GF1Delay();
    WriteGUSb( GUS_DATAHI, 3|4 );         // Rollover an


    WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
    WriteGUSb( GUS_DATAHI, 32|ubGUSUsedBits );
    GUS_GF1Delay();
    WriteGUSb( GUS_DATAHI, 32|ubGUSUsedBits );  // Stimme starten, Loop aus


    bIgnoreGF1IRQ = FALSE;


    // GUS-Initialisierung beendet, puhhh

    fprintf( stderr, "GUS found at port: %Xh, dma1: %d, dma2: %d, GF1-int: %d, MIDI-int: %d\n", uwGUSPort, ubGUSDMA1, ubGUSDMA2, ubGUSGF1Int, ubGUSMIDIInt );


    atexit( GUS_Cleanup );


    SND_Write = GUS_PlayBuffer;  // Zeiger auf meine Abspielroutine eintragen
    direct_buffers[0] = 0;       // Direct access not supported
    direct_buffers[1] = 0;
    *stereo_sound = 0;


    printf( "First ....(%ld,%X)\n", ulBytesInBuffer, &ulBytesInBuffer );


    return 1;

}


/*******************************

    int GUS_Cleanup( void )

*******************************/


void GUS_Cleanup( void )
{


    fprintf( stderr, "Freeing up all GUS-stuff\n" );

    // Warten, bis DMA beendet ist

    if( bDMAInProgress == TRUE )
    {
        do
        {
            delay( 10 );
        }
        while( bDMAInProgress == TRUE );
    }


    // Stimmen anhalten

    WriteGUSb( GUS_VOICE, 0 );

    WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
    WriteGUSb( GUS_DATAHI, 3 );
    GUS_GF1Delay();
    WriteGUSb( GUS_DATAHI, 3 );


    WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
    WriteGUSb( GUS_DATAHI, 3 );
    GUS_GF1Delay();
    WriteGUSb( GUS_DATAHI, 3 );

    // GUS resetten

    GUS_Reset( 0, 0, 0 );

    // Interrupts wieder sperren

    if( ubGUSGF1Int > 7 )
        outportb (0xA1, inportb(0xA1) | (1 << (ubGUSGF1Int-8)));
    else
        outportb (0x21, inportb(0x21) | (1 << ubGUSGF1Int));

    _go32_dpmi_set_protected_mode_interrupt_vector( ubGUSGF1IRQVector, &old_pmintGF1 );
    _go32_dpmi_free_iret_wrapper( &pm_wrapperGF1 );


    // Speicher freigeben

    if( pGUSBuffer != 0 )
        free( pGUSBuffer );

    if( dmabufadr != 0 )
        _go32_dpmi_free_dos_memory( &dma_mem );  // Speicher fuer DMA freigeben

    GUS_Reset( 0, 0, 0 );

}

/*******************************

    void GUS_PlayBuffer( void *buf, unsigned long size )

*******************************/


// Diese Routine wird vom UAE aufgerufen, wenn der Soundbuffer
// zu leeren ist


void GUS_PlayBuffer( void *buf, unsigned long size )
{
    ULONG            i, j;


    j=0;
    while( ulBytesInBuffer>ulDMABufMaxSize*2 );
    /*
    {
        fprintf( stderr, "Waiting ....(%ld,%X)\n", ulBytesInBuffer, &ulBytesInBuffer );
        fflush( stderr );
        j++;
        if( j>50 )
        {
            fprintf( stderr, "Breche ab ...\n" );
            fflush( stderr );
            break;
        }

    } */

    disable();

    // Daten in den GUS-Buffer kopieren

    if( size+ulBytesInBuffer>ulGUSSndBufSize )
    {
        if( bWarnFlag == TRUE )
        {
            fprintf( stderr, "Error: GUS-soundbuffer overflow !\n" );
            nWarnCnt++;
            if( nWarnCnt>MAXWARNS )
            {
                bWarnFlag = FALSE;
                fprintf( stderr, "GUS: No further messages !\n" );
            }
        }

    }
    else
    {
        // GUS-Buffer fuellen

        // Koennen wir auf einmal kopieren ?

        i = ulGUSSndBufSize-nGUSBufferWriteIndex;
        if( i < size )
        {
            memcpy( pGUSBuffer+nGUSBufferWriteIndex, buf, i );

            memcpy( pGUSBuffer, ((UBYTE*)buf)+i, size-i );

        }
        else
        {
            memcpy( pGUSBuffer+nGUSBufferWriteIndex, buf, size );
        }

        nGUSBufferWriteIndex = (nGUSBufferWriteIndex+size)&(ulGUSSndBufSize-1);

        ulBytesInBuffer += size;

    }

    enable();

}


/*******************************

void GUS_StartDMA( ULONG ulPosInGUSRAM )

*******************************/



void GUS_StartDMA( ULONG ulPosInGUSRAM, int bLoop )
{

    ULONG   i, pDMABuf, j;
    ULONG   ulTemp;


    // I hope, everything is aligned to 4 byte (32bit), otherwise
    // the machine my crash :(


    if( bDMAInProgress == TRUE )
    {
        fprintf( stderr, "Error in StartDMA, DMA already im progress !\n" );
        return;
    }

    bDMAInProgress=TRUE;

    // Kopiert die Daten aus dem Soundbuffer in den DMA-Puffer


    _farsetsel( _dos_ds );

    if( ulBytesInBuffer == 0 )      // Sind ueberhaupt Daten im Puffer ?
    {                               // Nein, dann DMA-Buffer loeschen
        pDMABuf = dmabufadr;

        if( ubGUSUsedBits == 0 )    // 8 Bit ?
            ulTemp = 0x80808080UL;
        else
            ulTemp = 0;

        for( i=ulDMABufMaxSize/4; i>0; i-- )
        {
            _farnspokel( pDMABuf, ulTemp );
            pDMABuf+=4;
        }

    }
    else
    {
        if( ulBytesInBuffer >= ulDMABufMaxSize )    // Sind mehr Daten da, als
        {                                           // der Puffer gross ist ?

            pDMABuf = dmabufadr;                    // Ja, DMA-Puffer vollmachen

            j = nGUSBufferReadIndex;
            for( i=ulDMABufMaxSize/4; i>0; i-- )
            {
                _farnspokel( pDMABuf, *((ULONG*)(pGUSBuffer+j)) );  // 32 bit-copy
                pDMABuf+=4;
                j = (j+4)&(ulGUSSndBufSize-1);
            }

            nGUSBufferReadIndex = (nGUSBufferReadIndex+ulDMABufMaxSize)&(ulGUSSndBufSize-1);

            ulBytesInBuffer -= ulDMABufMaxSize;
        }
        else
        {
            pDMABuf = dmabufadr;          // Nein, nur den vorhanden Teil
                                          // kopieren

            j = nGUSBufferReadIndex;
            for( i=ulBytesInBuffer/4; i>0; i-- )
            {
                _farnspokel( pDMABuf, *((ULONG*)(pGUSBuffer+j)) );
                pDMABuf+=4;
                j = (j+4)&(ulGUSSndBufSize-1);
            }
            nGUSBufferReadIndex = (nGUSBufferReadIndex+ulBytesInBuffer)&(ulGUSSndBufSize-1);


            if( ubGUSUsedBits == 0 )     // 8 Bit ?
                ulTemp = 0x80808080UL;
            else
                ulTemp = 0;


            for( i=(ulDMABufMaxSize-ulBytesInBuffer)/4; i>0; i-- )
            {
                _farnspokel( pDMABuf, ulTemp );
                pDMABuf+=4;
            }


            ulBytesInBuffer = 0;
        }

    }


    if( bLoop == TRUE ) // Byte/Word kopieren, um das Geklicke zu verhindern
    {

        if( ubGUSUsedBits == 0 )    // 8 Bit ?
        {

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRHI );
            WriteGUSw( GUS_DATALO, MSW( ulPosInGUSRAM-ulDMABufMaxSize-1 ) );

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRLO );
            WriteGUSw( GUS_DATALO, LSW( ulPosInGUSRAM-ulDMABufMaxSize-1 ) );

            WriteGUSb( GUS_DRAM, _farnspeekb( dmabufadr+ulDMABufMaxSize-1 )+0x80 );

        }
        else
        {
            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRHI );
            WriteGUSw( GUS_DATALO, MSW( ulPosInGUSRAM-ulDMABufMaxSize-2 ) );

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRLO );
            WriteGUSw( GUS_DATALO, LSW( ulPosInGUSRAM-ulDMABufMaxSize-2 ) );

            WriteGUSb( GUS_DRAM, _farnspeekb( dmabufadr+ulDMABufMaxSize-2 ) );

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRHI );
            WriteGUSw( GUS_DATALO, MSW( ulPosInGUSRAM-ulDMABufMaxSize-1 ) );

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMIOADRLO );
            WriteGUSw( GUS_DATALO, LSW( ulPosInGUSRAM-ulDMABufMaxSize-1 ) );

            WriteGUSb( GUS_DRAM, _farnspeekb( dmabufadr+ulDMABufMaxSize-1 ) );

        }

    }


    // DMA-Controller initialisieren
    DMA_InitTransfer( ubGUSDMA1, DMA_READ, dmabufadr, ulDMABufMaxSize );

    // GUS-Register initialisieren
    WriteGUSb( GUS_COMMAND, GUS_CMD_DMASTARTADR );
    if( ubGUSDMA1>3 )
        WriteGUSw( GUS_DATALO, CONVTO16B(ulPosInGUSRAM>>4) ); // Adresse>>4 !!!!
    else
        WriteGUSw( GUS_DATALO, (UWORD)(ulPosInGUSRAM>>4) ); // Adresse>>4 !!!!

    if( ubGUSUsedBits == 0 )
    {                               // 8 Bit
        if( ubGUSDMA1>3 )
        {
            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
            WriteGUSb( GUS_DATAHI, 0x85 | 0x20 );   // Mit signed->unsigned kopieren

        }
        else
        {
            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
            WriteGUSb( GUS_DATAHI, 0x81 | 0x20 );
        }
    }
    else
    {                       // 16 Bit
        if( ubGUSDMA1>3 )
        {
            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
            WriteGUSb( GUS_DATAHI, 0x05 | 0x20 );   // Ohne signed->unsigned kopieren

        }
        else
        {
            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
            WriteGUSb( GUS_DATAHI, 0x01 | 0x20 );
        }


    }

}



/*******************************

    void GUS_WorkOnVoices( void )

*******************************/


// Hier werden die Interrupts der einzelnen Stimmen bearbeitet

void GUS_WorkOnVoices( void )
{

    UBYTE ubIntArt, ubTemp1, ubTemp2;
    int i, nVoice, nVoiceBit, nWaveIgnore=0, nVolumeIgnore=0;
    int nIntCounts=0;

    // Alle anstehenden Interrupts bearbeiten

    for(;;)
    {


        WriteGUSb( GUS_COMMAND, GUS_CMD_IRQSOURCER );   // IRQ-Art holen
        ubIntArt = ReadGUSb( GUS_DATAHI );

        nVoice = ubIntArt & 0x1f;
        nVoiceBit = ((ULONG)1)<<((ULONG)nVoice);

        // Alle Interrupts abgearbeitet ?
        if( ( ubIntArt & 192) == 192 )      // Negative Logik !!!!
        {
            break;
        }

        nIntCounts++;

        if( nIntCounts>3 )
        {
            #ifdef DEBUG
            fprintf( stderr, "!!!! Breche Int. ab, da mehr als 3 Anfragen !!!\n" );
            #endif

            WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
            WriteGUSb( GUS_DATAHI, 3 );
            GUS_GF1Delay();
            WriteGUSb( GUS_DATAHI, 3 );

            WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
            WriteGUSb( GUS_DATAHI, 3 );
            GUS_GF1Delay();
            WriteGUSb( GUS_DATAHI, 3 );

            return;
        }

        if( bIgnoreGF1IRQ == TRUE )
        {
            #ifdef DEBUG
            fprintf( stderr, "\t\tIgnoriere IRQ !\n" );
            #endif

            WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
            WriteGUSb( GUS_DATAHI, 3 );      // Loop aus, IRQ aus

            WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
            WriteGUSb( GUS_DATAHI, 3 );   // Rollover an, IRQ an
        }

        #ifdef DEBUG
        fprintf( stderr, "\tBearbeite Stimme %d, IntArt: $%X\n", nVoice, ubIntArt );
        #endif

        if( ! (ubIntArt & 128 ) )           // WAVE-Table-IRQ ?
        {
            if( !(nWaveIgnore & nVoiceBit ))
            {
                nWaveIgnore |= nVoiceBit;

                #ifdef DEBUG
                fprintf( stderr,"\t\tBearbeite WAVE-IRQ\n" );
                #endif

                WriteGUSb( GUS_VOICE, nVoice );
                WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLR );
                ubTemp1 = ReadGUSb( GUS_DATAHI );

                WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLR );
                ubTemp2 = ReadGUSb( GUS_DATAHI );


                if( ubTemp2&4 )     // Ist das Rollover an ?
                {                   // Ja

                    // Das Ende neu setzen

                    if( ubGUSUsedBits == 0 )    // 8 Bit ?
                    {
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDHIW );       // Ende setzen
                        WriteGUSw( GUS_DATALO, ADDR_HI( ulDMABufMaxSize*2+32 ) );
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDLOW );
                        WriteGUSw( GUS_DATALO, ADDR_LO( ulDMABufMaxSize*2+32 ) );
                    }
                    else
                    {
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDHIW );       // Ende setzen
                        WriteGUSw( GUS_DATALO, ADDR_HI( CONVTO16B( ulDMABufMaxSize*2+32 ) ) );
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDLOW );
                        WriteGUSw( GUS_DATALO, ADDR_LO( CONVTO16B( ulDMABufMaxSize*2+32 ) ) );
                    }

                    WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
                    WriteGUSb( GUS_DATAHI, 3 );
                    GUS_GF1Delay();
                    WriteGUSb( GUS_DATAHI, 3 );         // Rollover aus

                    WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
                    WriteGUSb( GUS_DATAHI, 32|8|ubGUSUsedBits );
                    GUS_GF1Delay();
                    WriteGUSb( GUS_DATAHI, 32|8|ubGUSUsedBits ); // Loop an

                    GUS_StartDMA( 32, FALSE );

                }
                else
                {

                    // Das Ende neu setzen

                    if( ubGUSUsedBits == 0 )
                    {
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDHIW );       // Ende setzen
                        WriteGUSw( GUS_DATALO, ADDR_HI( ulDMABufMaxSize+32 ) );
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDLOW );
                        WriteGUSw( GUS_DATALO, ADDR_LO( ulDMABufMaxSize+32 ) );
                    }
                    else
                    {
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDHIW );       // Ende setzen
                        WriteGUSw( GUS_DATALO, ADDR_HI( CONVTO16B( ulDMABufMaxSize+32 ) ) );
                        WriteGUSb( GUS_COMMAND, GUS_CMD_ENDLOW );
                        WriteGUSw( GUS_DATALO, ADDR_LO( CONVTO16B( ulDMABufMaxSize+32 ) ) );
                    }

                    WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
                    WriteGUSb( GUS_DATAHI, 3|4 );
                    GUS_GF1Delay();
                    WriteGUSb( GUS_DATAHI, 3|4 );         // Rollover an

                    WriteGUSb( GUS_COMMAND, GUS_CMD_VOICECTRLW );
                    WriteGUSb( GUS_DATAHI, 32|ubGUSUsedBits );
                    GUS_GF1Delay();
                    WriteGUSb( GUS_DATAHI, 32|ubGUSUsedBits ); // Loop aus

                    GUS_StartDMA( ulDMABufMaxSize+32, TRUE );

                }

            }
        }

        if( ! (ubIntArt & 64 ) )        // Volume-IRQ
        {

            if( !(nVolumeIgnore & nVoiceBit ))
            {
                nVolumeIgnore |= nVoiceBit;
                #ifdef DEBUG
                fprintf( stderr, "\t\tBearbeite Volume-IRQ\n" );
                #endif

                WriteGUSb( GUS_VOICE, nVoice );
                WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLR );
                ubTemp2 = ReadGUSb( GUS_DATAHI );

                WriteGUSb(GUS_COMMAND, GUS_CMD_VOICECTRLR );
                ubTemp1 = ReadGUSb( GUS_DATAHI );


                WriteGUSb( GUS_COMMAND, GUS_CMD_VOLUMECTRLW );
                WriteGUSb( GUS_DATAHI, (ubTemp2 & (~32)) | 3 );

            }

        }

    }
}



/*******************************

    void GUS_GF1InterruptHandler( void )

*******************************/


// Hier werden alle Interrupts verarbeitet

void GUS_GF1InterruptHandler( void )
{

    UBYTE ubIntArt, ubTemp;




    if( uwGUSPort == 0 )
        return;

    #ifdef DEBUG
    fprintf( stderr, "- Interrupt aufgetreten. Zeit: %lf\n", ((double)clock())/((double)CLOCKS_PER_SEC) );
    #endif

    for(;;)
    {

        ubIntArt = ReadGUSb( GUS_IRQSTATE );

        #ifdef DEBUG
        fprintf( stderr, "  IRQ-Status (2x6): $%X\n", ubIntArt );
        #endif


        if( ubIntArt == 0 )         // Fertig ?
            break;

        if( ubIntArt & 128 )        // TC-IRQ ?
        {

            WriteGUSb( GUS_COMMAND, GUS_CMD_DRAMDMACTRL );
            if( ReadGUSb( GUS_DATAHI ) & 0x40 )
            {
                #ifdef DEBUG
                fprintf( stderr, "  - Bearbeite DMA-Interrupt\n" );
                #endif

                bDMAInProgress = FALSE;

            }

            WriteGUSb( GUS_COMMAND, GUS_CMD_SAMPLECTRL );
            if( ReadGUSb( GUS_DATAHI ) & 0x40 )
            {
                #ifdef DEBUG
                fprintf( stderr, "  - Bearbeite Sample-Interrupt\n" );
                #endif

            }

        }

        if( ubIntArt & 3 )
        {
            #ifdef DEBUG
            fprintf( stderr, "  - Bearbeite MIDI-IRQ\n" );
            #endif

        }

        if( ubIntArt & 64 )
        {
            #ifdef DEBUG
            fprintf( stderr, "  - Bearbeite VOLUME-IRQ\n" );
            #endif
            GUS_WorkOnVoices();


        }

        if( ubIntArt & 32 )
        {
            #ifdef DEBUG
            fprintf( stderr, "  - Bearbeite WAVETABLE-IRQ\n" );
            #endif
            GUS_WorkOnVoices();

        }


        if( ubIntArt & 4 )          // Timer 1?
        {
            #ifdef DEBUG
            fprintf( stderr, "  - Bearbeite Timer 1-IRQ\n" );
            #endif

            WriteGUSb( GUS_COMMAND, GUS_CMD_TIMERCTRL );
            ubTemp = ReadGUSb( GUS_DATAHI );

            WriteGUSb( GUS_COMMAND, GUS_CMD_TIMERCTRL );
            WriteGUSb( GUS_DATAHI, ubTemp & (~4) );        // Timer 1 aus
        }

        if( ubIntArt & 8 )          // Timer 2?
        {
            #ifdef DEBUG
            fprintf( stderr, "  - Bearbeite Timer 2-IRQ\n" );
            #endif

            WriteGUSb( GUS_COMMAND, GUS_CMD_TIMERCTRL );
            ubTemp = ReadGUSb( GUS_DATAHI );

            WriteGUSb( GUS_COMMAND, GUS_CMD_TIMERCTRL );
            WriteGUSb( GUS_DATAHI, ubTemp & (~8) );        // Timer 2 aus
        }
    }

    #ifdef DEBUG
    fprintf( stderr, "- Interruptende\n\n" );
    #endif

    if( ubGUSGF1Int > 7 )
        outportb( 0xA0, 0x20 );

    outportb( 0x20, 0x20 );

}


// Dummy-Routine zum erkennen des Programmendes

void GUS_DummyFunc( void )
{
    ;
}


