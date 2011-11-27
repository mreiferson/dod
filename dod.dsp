# Microsoft Developer Studio Project File - Name="DOD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DOD - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dod.mak" CFG="DOD - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DOD - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DOD - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DOD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\DOD___Wi"
# PROP BASE Intermediate_Dir ".\DOD___Wi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\BuildOutput"
# PROP Intermediate_Dir ".\BuildOutput"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "_WINDOWS" /YX /FD /GM /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib dxguid.lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:".\Build\dod.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\BuildOutput"
# PROP Intermediate_Dir ".\BuildOutput"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GM /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib dxguid.lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /out:".\Build\dod.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DOD - Win32 Release"
# Name "DOD - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\dod.cpp
# End Source File
# Begin Source File

SOURCE=.\dodddraw.cpp
# End Source File
# Begin Source File

SOURCE=.\doddinput.cpp
# End Source File
# Begin Source File

SOURCE=.\doddsound.cpp
# End Source File
# Begin Source File

SOURCE=.\dodengine.cpp
# End Source File
# Begin Source File

SOURCE=.\dodgfx.cpp
# End Source File
# Begin Source File

SOURCE=.\dodinput.cpp
# End Source File
# Begin Source File

SOURCE=.\dodintro.cpp
# End Source File
# Begin Source File

SOURCE=.\dodmemcpy.cpp
# End Source File
# Begin Source File

SOURCE=.\dodmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\dodmidi.cpp
# End Source File
# Begin Source File

SOURCE=.\dodsound.cpp
# End Source File
# Begin Source File

SOURCE=.\dodutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dodworld.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\dod.h
# End Source File
# Begin Source File

SOURCE=.\dodddraw.h
# End Source File
# Begin Source File

SOURCE=.\doddefines.h
# End Source File
# Begin Source File

SOURCE=.\doddinput.h
# End Source File
# Begin Source File

SOURCE=.\doddsound.h
# End Source File
# Begin Source File

SOURCE=.\dodengine.h
# End Source File
# Begin Source File

SOURCE=.\dodgfx.h
# End Source File
# Begin Source File

SOURCE=.\dodinput.h
# End Source File
# Begin Source File

SOURCE=.\dodintro.h
# End Source File
# Begin Source File

SOURCE=.\dodmemcpy.h
# End Source File
# Begin Source File

SOURCE=.\dodmenu.h
# End Source File
# Begin Source File

SOURCE=.\dodmidi.h
# End Source File
# Begin Source File

SOURCE=.\dodsound.h
# End Source File
# Begin Source File

SOURCE=.\dodutil.h
# End Source File
# Begin Source File

SOURCE=.\dodworld.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
