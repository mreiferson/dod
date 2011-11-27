# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=DOD - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to DOD - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DOD - Win32 Release" && "$(CFG)" != "DOD - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dod.mak" CFG="DOD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DOD - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DOD - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "DOD - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "DOD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DOD___Wi"
# PROP BASE Intermediate_Dir "DOD___Wi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\BuildOutput"
# PROP Intermediate_Dir ".\BuildOutput"
# PROP Target_Dir ""
OUTDIR=.\BuildOutput
INTDIR=.\BuildOutput

ALL : ".\Build\dod.exe"

CLEAN : 
	-@erase "$(INTDIR)\dod.obj"
	-@erase "$(INTDIR)\dodddraw.obj"
	-@erase "$(INTDIR)\doddinput.obj"
	-@erase "$(INTDIR)\doddsound.obj"
	-@erase "$(INTDIR)\dodengine.obj"
	-@erase "$(INTDIR)\dodgfx.obj"
	-@erase "$(INTDIR)\dodinput.obj"
	-@erase "$(INTDIR)\dodintro.obj"
	-@erase "$(INTDIR)\dodmemcpy.obj"
	-@erase "$(INTDIR)\dodmenu.obj"
	-@erase "$(INTDIR)\dodmidi.obj"
	-@erase "$(INTDIR)\dodsound.obj"
	-@erase "$(INTDIR)\dodutil.obj"
	-@erase "$(INTDIR)\dodworld.obj"
	-@erase ".\Build\dod.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "_WINDOWS" /YX /GM /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)/dod.pch" /YX /Fo"$(INTDIR)/" /GM /c 
CPP_OBJS=.\.\BuildOutput/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dod.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib dxguid.lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:".\Build/dod.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib\
 dxguid.lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/dod.pdb" /machine:I386 /nodefaultlib\
 /out:".\Build/dod.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dod.obj" \
	"$(INTDIR)\dodddraw.obj" \
	"$(INTDIR)\doddinput.obj" \
	"$(INTDIR)\doddsound.obj" \
	"$(INTDIR)\dodengine.obj" \
	"$(INTDIR)\dodgfx.obj" \
	"$(INTDIR)\dodinput.obj" \
	"$(INTDIR)\dodintro.obj" \
	"$(INTDIR)\dodmemcpy.obj" \
	"$(INTDIR)\dodmenu.obj" \
	"$(INTDIR)\dodmidi.obj" \
	"$(INTDIR)\dodsound.obj" \
	"$(INTDIR)\dodutil.obj" \
	"$(INTDIR)\dodworld.obj"

".\Build\dod.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\BuildOutput"
# PROP Intermediate_Dir ".\BuildOutput"
# PROP Target_Dir ""
OUTDIR=.\BuildOutput
INTDIR=.\BuildOutput

ALL : ".\Build\dod.exe"

CLEAN : 
	-@erase "$(INTDIR)\dod.obj"
	-@erase "$(INTDIR)\dodddraw.obj"
	-@erase "$(INTDIR)\doddinput.obj"
	-@erase "$(INTDIR)\doddsound.obj"
	-@erase "$(INTDIR)\dodengine.obj"
	-@erase "$(INTDIR)\dodgfx.obj"
	-@erase "$(INTDIR)\dodinput.obj"
	-@erase "$(INTDIR)\dodintro.obj"
	-@erase "$(INTDIR)\dodmemcpy.obj"
	-@erase "$(INTDIR)\dodmenu.obj"
	-@erase "$(INTDIR)\dodmidi.obj"
	-@erase "$(INTDIR)\dodsound.obj"
	-@erase "$(INTDIR)\dodutil.obj"
	-@erase "$(INTDIR)\dodworld.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dod.pdb"
	-@erase ".\Build\dod.exe"
	-@erase ".\Build\dod.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /GM /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/dod.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /GM /c 
CPP_OBJS=.\.\BuildOutput/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dod.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib dxguid.lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /out:".\Build/dod.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib\
 dxguid.lib ddraw.lib dsound.lib dinput.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/dod.pdb" /debug /machine:I386 /nodefaultlib\
 /out:".\Build/dod.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dod.obj" \
	"$(INTDIR)\dodddraw.obj" \
	"$(INTDIR)\doddinput.obj" \
	"$(INTDIR)\doddsound.obj" \
	"$(INTDIR)\dodengine.obj" \
	"$(INTDIR)\dodgfx.obj" \
	"$(INTDIR)\dodinput.obj" \
	"$(INTDIR)\dodintro.obj" \
	"$(INTDIR)\dodmemcpy.obj" \
	"$(INTDIR)\dodmenu.obj" \
	"$(INTDIR)\dodmidi.obj" \
	"$(INTDIR)\dodsound.obj" \
	"$(INTDIR)\dodutil.obj" \
	"$(INTDIR)\dodworld.obj"

".\Build\dod.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "DOD - Win32 Release"
# Name "DOD - Win32 Debug"

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dod.cpp
DEP_CPP_DOD_C=\
	".\dod.h"\
	".\dodddraw.h"\
	".\doddinput.h"\
	".\doddsound.h"\
	".\dodengine.h"\
	".\dodgfx.h"\
	".\dodinput.h"\
	".\dodmemcpy.h"\
	".\dodmidi.h"\
	".\dodsound.h"\
	".\dodutil.h"\
	".\dodworld.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dod.obj" : $(SOURCE) $(DEP_CPP_DOD_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodengine.cpp
DEP_CPP_DODEN=\
	".\dod.h"\
	".\dodddraw.h"\
	".\doddinput.h"\
	".\doddsound.h"\
	".\dodengine.h"\
	".\dodgfx.h"\
	".\dodinput.h"\
	".\dodintro.h"\
	".\dodmenu.h"\
	".\dodmidi.h"\
	".\dodsound.h"\
	".\dodworld.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodengine.obj" : $(SOURCE) $(DEP_CPP_DODEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodgfx.cpp
DEP_CPP_DODGF=\
	".\dod.h"\
	".\dodddraw.h"\
	".\dodengine.h"\
	".\dodgfx.h"\
	".\dodinput.h"\
	".\dodmemcpy.h"\
	".\dodsound.h"\
	".\dodworld.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodgfx.obj" : $(SOURCE) $(DEP_CPP_DODGF) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodinput.cpp
DEP_CPP_DODIN=\
	".\doddinput.h"\
	".\dodengine.h"\
	".\dodgfx.h"\
	".\dodinput.h"\
	".\dodworld.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodinput.obj" : $(SOURCE) $(DEP_CPP_DODIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodintro.cpp
DEP_CPP_DODINT=\
	".\dodddraw.h"\
	".\doddinput.h"\
	".\doddsound.h"\
	".\dodgfx.h"\
	".\dodintro.h"\
	".\dodmidi.h"\
	".\dodsound.h"\
	".\dodutil.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodintro.obj" : $(SOURCE) $(DEP_CPP_DODINT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodmenu.cpp
DEP_CPP_DODME=\
	".\dod.h"\
	".\dodddraw.h"\
	".\doddinput.h"\
	".\doddsound.h"\
	".\dodgfx.h"\
	".\dodinput.h"\
	".\dodmenu.h"\
	".\dodmidi.h"\
	".\dodsound.h"\
	".\dodworld.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodmenu.obj" : $(SOURCE) $(DEP_CPP_DODME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodmidi.cpp
DEP_CPP_DODMI=\
	".\dodmidi.h"\
	

"$(INTDIR)\dodmidi.obj" : $(SOURCE) $(DEP_CPP_DODMI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodsound.cpp
DEP_CPP_DODSO=\
	".\doddsound.h"\
	".\dodsound.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodsound.obj" : $(SOURCE) $(DEP_CPP_DODSO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodworld.cpp
DEP_CPP_DODWO=\
	".\dodddraw.h"\
	".\dodgfx.h"\
	".\dodutil.h"\
	".\dodworld.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dodworld.obj" : $(SOURCE) $(DEP_CPP_DODWO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dod.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodengine.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodgfx.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodinput.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodintro.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodmenu.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodmidi.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodsound.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodworld.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodmemcpy.cpp
DEP_CPP_DODMEM=\
	".\dodmemcpy.h"\
	

"$(INTDIR)\dodmemcpy.obj" : $(SOURCE) $(DEP_CPP_DODMEM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\doddinput.cpp
DEP_CPP_DODDI=\
	".\doddinput.h"\
	".\dodutil.h"\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\doddinput.obj" : $(SOURCE) $(DEP_CPP_DODDI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\doddsound.cpp
DEP_CPP_DODDS=\
	".\doddsound.h"\
	

"$(INTDIR)\doddsound.obj" : $(SOURCE) $(DEP_CPP_DODDS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodddraw.cpp
DEP_CPP_DODDD=\
	".\dodddraw.h"\
	

"$(INTDIR)\dodddraw.obj" : $(SOURCE) $(DEP_CPP_DODDD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodmemcpy.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\doddinput.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\doddsound.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodddraw.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodutil.h

!IF  "$(CFG)" == "DOD - Win32 Release"

!ELSEIF  "$(CFG)" == "DOD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dodutil.cpp
DEP_CPP_DODUT=\
	".\dodutil.h"\
	

"$(INTDIR)\dodutil.obj" : $(SOURCE) $(DEP_CPP_DODUT) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
