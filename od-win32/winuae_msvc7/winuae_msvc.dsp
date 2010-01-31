# Microsoft Developer Studio Project File - Name="winuae_msvc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=winuae_msvc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "winuae_msvc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "winuae_msvc.mak" CFG="winuae_msvc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winuae_msvc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "winuae_msvc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /Op /I "..\..\include" /I "..\.." /I "..\\" /I "..\resources" /I "..\osdep" /I "..\sounddep" /D WINVER=0x0500 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _WIN32_IE=0x0500 /D "UNIX" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 zlibstat.lib delayimp.lib setupapi.lib shlwapi.lib winspool.lib dxerr8.lib d3d9.lib d3dx9.lib opengl32.lib glu32.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib comctl32.lib version.lib vfw32.lib msacm32.lib dsound.lib winio.lib wininet.lib dinput8.lib /nologo /stack:0x280000,0x280000 /subsystem:windows /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"..\..\AllDone\WinUAE.exe" /delayload:setupapi.dll
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "..\..\include" /I "..\.." /I "..\\" /I "..\resources" /I "..\osdep" /I "..\sounddep" /D WINVER=0x0500 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32_IE 0x0500" /D "UNIX" /FD /GZ /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlibstat.lib delayimp.lib setupapi.lib shlwapi.lib winspool.lib dxerr8.lib d3d9.lib d3dx9.lib opengl32.lib glu32.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib comctl32.lib version.lib vfw32.lib msacm32.lib dsound.lib winio.lib wininet.lib dinput8.lib /nologo /stack:0x280000,0x280000 /subsystem:windows /profile /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"..\..\AllDone\DebugUAE.exe" /delayload:setupapi.dll

!ENDIF 

# Begin Target

# Name "winuae_msvc - Win32 Release"
# Name "winuae_msvc - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\resources\35floppy.ico
# End Source File
# Begin Source File

SOURCE=..\resources\amigainfo.ico
# End Source File
# Begin Source File

SOURCE=..\resources\avioutput.ico
# End Source File
# Begin Source File

SOURCE=..\resources\Cddrive.ico
# End Source File
# Begin Source File

SOURCE=..\resources\cpu.ico
# End Source File
# Begin Source File

SOURCE=..\resources\Drive.ico
# End Source File
# Begin Source File

SOURCE=..\resources\H_arrow.cur
# End Source File
# Begin Source File

SOURCE=..\resources\joystick.ico
# End Source File
# Begin Source File

SOURCE=..\resources\misc.ico
# End Source File
# Begin Source File

SOURCE=..\resources\Monitr01.ico
# End Source File
# Begin Source File

SOURCE=..\resources\move_dow.ico
# End Source File
# Begin Source File

SOURCE=..\resources\move_up.ico
# End Source File
# Begin Source File

SOURCE=..\resources\Mycomp.ico
# End Source File
# Begin Source File

SOURCE=..\resources\Volume01.ico
# End Source File
# Begin Source File

SOURCE=..\resources\winuae.ico
# End Source File
# Begin Source File

SOURCE=..\resources\winuae.rc
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\ahidsound.c
# End Source File
# Begin Source File

SOURCE=..\avioutput.c
# End Source File
# Begin Source File

SOURCE=..\blkdev_win32_aspi.c
# End Source File
# Begin Source File

SOURCE=..\blkdev_win32_ioctl.c
# End Source File
# Begin Source File

SOURCE=..\blkdev_win32_spti.c
# End Source File
# Begin Source File

SOURCE=..\bsdsock.c
# End Source File
# Begin Source File

SOURCE=..\caps\caps_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\catweasel.c
# End Source File
# Begin Source File

SOURCE=..\dinput.c
# End Source File
# Begin Source File

SOURCE=..\direct3d.c
# End Source File
# Begin Source File

SOURCE=..\dxwrap.c
# End Source File
# Begin Source File

SOURCE=..\fsdb_win32.c
# End Source File
# Begin Source File

SOURCE=..\hardfile_win32.c
# End Source File
# Begin Source File

SOURCE=..\ioport.c
# End Source File
# Begin Source File

SOURCE=..\keyboard_win32.c
# End Source File
# Begin Source File

SOURCE=..\midi.c
# End Source File
# Begin Source File

SOURCE=..\mman.c
# End Source File
# Begin Source File

SOURCE=..\opengl.c
# End Source File
# Begin Source File

SOURCE=..\parser.c
# End Source File
# Begin Source File

SOURCE=..\picasso96_win.c
# End Source File
# Begin Source File

SOURCE=..\posixemu.c
# End Source File
# Begin Source File

SOURCE=..\scaler.c
# End Source File
# Begin Source File

SOURCE=..\screenshot.c
# End Source File
# Begin Source File

SOURCE=..\serial_win32.c
# End Source File
# Begin Source File

SOURCE=..\sounddep\sound.c
# End Source File
# Begin Source File

SOURCE=..\support.c
# End Source File
# Begin Source File

SOURCE=..\win32.c
# End Source File
# Begin Source File

SOURCE=..\win32_scale2x.c
# End Source File
# Begin Source File

SOURCE=..\win32gfx.c
# End Source File
# Begin Source File

SOURCE=..\win32gui.c
# End Source File
# Begin Source File

SOURCE=..\writelog.c
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\akiko.c
# End Source File
# Begin Source File

SOURCE=..\..\ar.c
# End Source File
# Begin Source File

SOURCE=..\..\arcadia.c
# End Source File
# Begin Source File

SOURCE=..\..\audio.c
# End Source File
# Begin Source File

SOURCE=..\..\autoconf.c
# End Source File
# Begin Source File

SOURCE=..\..\blitfunc.c
# End Source File
# Begin Source File

SOURCE=..\..\blittable.c
# End Source File
# Begin Source File

SOURCE=..\..\blitter.c
# End Source File
# Begin Source File

SOURCE=..\..\blkdev.c
# End Source File
# Begin Source File

SOURCE=..\..\bsdsocket.c
# End Source File
# Begin Source File

SOURCE=..\..\cdrom.c
# End Source File
# Begin Source File

SOURCE=..\..\cfgfile.c
# End Source File
# Begin Source File

SOURCE=..\..\cia.c
# End Source File
# Begin Source File

SOURCE=..\..\cpudefs.c
# End Source File
# Begin Source File

SOURCE=..\..\cpuemu_0.c
# End Source File
# Begin Source File

SOURCE=..\..\cpuemu_5.c
# End Source File
# Begin Source File

SOURCE=..\..\cpuemu_6.c
# End Source File
# Begin Source File

SOURCE=..\..\cpustbl.c
# End Source File
# Begin Source File

SOURCE=..\..\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\custom.c
# End Source File
# Begin Source File

SOURCE=..\..\debug.c
# End Source File
# Begin Source File

SOURCE=..\..\disk.c
# End Source File
# Begin Source File

SOURCE=..\..\drawing.c
# End Source File
# Begin Source File

SOURCE=..\..\driveclick.c
# End Source File
# Begin Source File

SOURCE=..\..\enforcer.c
# End Source File
# Begin Source File

SOURCE=..\..\ersatz.c
# End Source File
# Begin Source File

SOURCE=..\..\expansion.c
# End Source File
# Begin Source File

SOURCE=..\..\fdi2raw.c
# End Source File
# Begin Source File

SOURCE=..\..\filesys.c
# End Source File
# Begin Source File

SOURCE=..\..\fpp.c
# End Source File
# Begin Source File

SOURCE=..\..\fsdb.c
# End Source File
# Begin Source File

SOURCE=..\..\fsusage.c
# End Source File
# Begin Source File

SOURCE=..\..\gfxlib.c
# End Source File
# Begin Source File

SOURCE=..\..\gfxutil.c
# End Source File
# Begin Source File

SOURCE=..\..\hardfile.c
# End Source File
# Begin Source File

SOURCE=..\..\identify.c
# End Source File
# Begin Source File

SOURCE=..\..\inputdevice.c
# End Source File
# Begin Source File

SOURCE=..\..\keybuf.c
# End Source File
# Begin Source File

SOURCE=..\..\main.c
# End Source File
# Begin Source File

SOURCE=..\..\memory.c
# End Source File
# Begin Source File

SOURCE=..\..\missing.c
# End Source File
# Begin Source File

SOURCE=..\..\moduleripper.c
# End Source File
# Begin Source File

SOURCE=..\..\native2amiga.c
# End Source File
# Begin Source File

SOURCE=..\..\newcpu.c
# End Source File
# Begin Source File

SOURCE=..\..\readcpu.c
# End Source File
# Begin Source File

SOURCE=..\..\savestate.c
# End Source File
# Begin Source File

SOURCE=..\..\scsiemul.c
# End Source File
# Begin Source File

SOURCE=..\..\uaeexe.c
# End Source File
# Begin Source File

SOURCE=..\..\uaelib.c
# End Source File
# Begin Source File

SOURCE=..\..\unzip.c
# End Source File
# Begin Source File

SOURCE=..\..\zfile.c
# End Source File
# End Group
# Begin Group "jit"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\compemu.c
# End Source File
# Begin Source File

SOURCE=..\..\compemu_fpp.c
# End Source File
# Begin Source File

SOURCE=..\..\compemu_support.c
# End Source File
# Begin Source File

SOURCE=..\..\compstbl.c
# End Source File
# End Group
# Begin Group "dms"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\dms\crc_csum.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\getbits.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\maketbl.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\pfile.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\tables.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\u_deep.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\u_heavy.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\u_init.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\u_medium.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\u_quick.c
# End Source File
# Begin Source File

SOURCE=..\..\dms\u_rle.c
# End Source File
# End Group
# Begin Group "prowizard"

# PROP Default_Filter ""
# Begin Group "rips"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\prowizard\rips\ac1d.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\ambk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\bp.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\bsifc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\bytek30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\bytekil.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\bytekp10.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\crmaddr.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\dblatn.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\defjam32.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\di.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\digibst.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\eureka.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\fc13.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\fc14.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\fc_m.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\fuchs.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\fuzzac.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\gmc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\gnpl.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\gpmo.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\heatseek.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\hqc2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\hrt.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\jamcrack.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\kris.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\ksm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\maxpac12.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\mc30addr.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\mcobj.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\mcrun.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\med.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\mp.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\newtron.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\nfh.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\noiserun.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\npd1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\npd2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\npd3.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p22a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p30a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p40a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p40b.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p41a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p50a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p60a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\p61a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pha.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pm01.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pm10c.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pm18a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pm20.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pm40.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\polka.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\powerp23.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\powerp30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\powerp40.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\powerp4l.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pp10.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pp21.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\pp30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\ppbk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\prun1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\prun2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\ptk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\qc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\relokit.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\skyt.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\soundfx.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\soundtk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\spike.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\starpack.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\startrek.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stc270.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stc299.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stc299b.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stc299d.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stc300.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stc310.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stim.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\stk26.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\superc27.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\syncro.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tdd.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\time17.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tnmc11.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tp1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tp2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tp3.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tpack102.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tpack21.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tpack22.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\tryit101.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\turbosqueezer61.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\unic.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\unic2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\wn.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\xann.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\xm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\rips\zen.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\rips"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\rips"

!ENDIF 

# End Source File
# End Group
# Begin Group "depack"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\prowizard\depack\ac1d.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\ambk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\di.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\eureka.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\prowizard\depack\fc-m.c"

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\fuchs.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\fuzzac.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\gmc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\gnpl.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\gpmo.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\heatseek.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\hrt.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\kris.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\ksm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\mp.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\newtron.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\nfh.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\noiserun.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\np1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\np2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\np3.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p22a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p30a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p40.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p41a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p50a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p60a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\p61a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pha.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pm01.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pm10c.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pm18a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pm20.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pm40.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\polka.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pp10.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pp21.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\pp30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\prun1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\prun2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\qc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\skyt.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\soundfx.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\starpack.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\stim.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\stk26.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\tdd.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\tp1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\tp2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\tp3.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\unic.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\unic2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\wn.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\xann.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\depack\zen.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\depack"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\depack"

!ENDIF 

# End Source File
# End Group
# Begin Group "tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\prowizard\tests\ac1d.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\ambk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\arcd.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\bp.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\bsifc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\bytek13.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\bytek20.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\bytek30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\bytekp10.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\crmaddr.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\crnd.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\dblatn.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\defja32p.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\defjam32.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\di.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\digibst.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\eureka.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\fc13.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\fc14.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\fc_m.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\fuchs.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\fuzzac.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\gmc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\gnpl.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\gpmo.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\heatseek.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\hqc2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\hrt.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\jamcrack.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\kris.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\ksm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\maxpac12.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\mc30addr.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\mcobj.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\mcrun10.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\mcrun12.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\mmd0.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\mp.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\newtron.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\nfh.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\noiserun.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\npd1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\npd2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\npd3.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\p40a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\p41a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\p50a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\p60a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\p61a.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pha.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pm01.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pm10c.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pm20.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pm40.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pmz.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\polka.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\powerp23.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\powerp30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\powerp40.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\powerp4l.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pp10.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pp21.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\pp30.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\ppbk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\prun1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\prun2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\ptk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\qc.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\relokit.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\skyt.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\soundfx.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\soundtk.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\spike.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\starpack.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stc270.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stc299.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stc299b.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stc299d.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stc300.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stc310.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stim.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\stk26.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\superc27.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\syncro.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tdd.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\time17.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tnmc11.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tp1.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tp2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tp3.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tpack102.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tpack21.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tpack22.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\tryit101.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\turbosqueezer61.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\unic.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\unic2.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\wn.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\xann.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\xm.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\prowizard\tests\zen.c

!IF  "$(CFG)" == "winuae_msvc - Win32 Release"

# PROP Intermediate_Dir "Release\tests"

!ELSEIF  "$(CFG)" == "winuae_msvc - Win32 Debug"

# PROP Intermediate_Dir "Debug\tests"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\prowizard\misc\misc.c
# End Source File
# Begin Source File

SOURCE=..\..\prowizard\prowiz.c
# End Source File
# Begin Source File

SOURCE=..\..\prowizard\misc\testbag.c
# End Source File
# End Group
# End Target
# End Project
