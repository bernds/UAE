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
# ADD CPP /nologo /MT /W3 /O2 /I "..\..\include" /I "..\.." /I "..\\" /I "..\resources" /I "..\osdep" /D WINVER=0x0500 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "WIN32_LEAN_AND_MEAN" /D "ZLIB_DLL" /D "OPENGL_SUPPORT" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib sdl.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib comctl32.lib version.lib vfw32.lib msacm32.lib zlib.lib delayimp.lib dsound.lib winio.lib wininet.lib dinput8.lib setupapi.lib capsimg.lib /nologo /stack:0x280000,0x280000 /subsystem:windows /machine:I386 /out:"..\..\..\winuae.exe" /delayload:zlib.dll /DELAYLOAD:winio.dll /DELAYLOAD:capsimg.dll
# SUBTRACT LINK32 /pdb:none

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
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /I "..\..\include" /I "..\.." /I "..\\" /I "..\resources" /I "..\osdep" /D WINVER=0x0500 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32_LEAN_AND_MEAN" /D "ZLIB_DLL" /D "OPENGL_SUPPORT" /FD /GZ /c
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
# ADD LINK32 opengl32.lib glu32.lib sdl.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib comctl32.lib version.lib vfw32.lib msacm32.lib zlib.lib delayimp.lib dsound.lib winio.lib wininet.lib dinput8.lib winio.lib setupapi.lib /nologo /stack:0x280000,0x280000 /subsystem:windows /debug /machine:I386 /out:"..\..\..\winuae.exe" /pdbtype:sept /delayload:zlib.dll
# SUBTRACT LINK32 /pdb:none

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

SOURCE=..\..\compiler.c
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
# End Target
# End Project
