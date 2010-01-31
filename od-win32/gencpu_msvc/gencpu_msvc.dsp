# Microsoft Developer Studio Project File - Name="gencpu_msvc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gencpu_msvc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gencpu_msvc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gencpu_msvc.mak" CFG="gencpu_msvc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gencpu_msvc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gencpu_msvc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gencpu_msvc - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\include" /I "..\.." /I "..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winio.lib /nologo /subsystem:console /machine:I386 /out:"gencpu.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Desc=deleting gencpu files
PreLink_Cmds=del ..\..\cputbl.h	del ..\..\cpustbl.c
PostBuild_Desc=generating gencpu files
PostBuild_Cmds=cd ..\..	od-win32\gencpu_msvc\gencpu.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "gencpu_msvc - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\.." /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__STDC__" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winio.lib /nologo /subsystem:console /debug /machine:I386 /out:"gencpu.exe" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Desc=deleting gencpu files
PreLink_Cmds=del ..\..\cputbl.h	del ..\..\cpustbl.c
PostBuild_Desc=generating gencpu files
PostBuild_Cmds=cd ..\..	od-win32\gencpu_msvc\gencpu.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "gencpu_msvc - Win32 Release"
# Name "gencpu_msvc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\cpudefs.c
# End Source File
# Begin Source File

SOURCE=..\..\gencpu.c
# End Source File
# Begin Source File

SOURCE=..\..\missing.c
# End Source File
# Begin Source File

SOURCE=..\..\readcpu.c
# End Source File
# Begin Source File

SOURCE=..\writelog.c
# End Source File
# End Group
# End Target
# End Project
