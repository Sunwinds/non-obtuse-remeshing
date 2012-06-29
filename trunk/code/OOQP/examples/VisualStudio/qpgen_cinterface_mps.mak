# Microsoft Developer Studio Generated NMAKE File, Based on qpgen_cinterface_mps.dsp
!IF "$(CFG)" == ""
CFG=qpgen_cinterface_mps - Win32 Debug
!MESSAGE No configuration specified. Defaulting to qpgen_cinterface_mps - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "qpgen_cinterface_mps - Win32 Release" && "$(CFG)" != "qpgen_cinterface_mps - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qpgen_cinterface_mps.mak" CFG="qpgen_cinterface_mps - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qpgen_cinterface_mps - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "qpgen_cinterface_mps - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "qpgen_cinterface_mps - Win32 Release"

OUTDIR=.\..\QpGen\cinterface
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\QpGen\cinterface
# End Custom Macros

ALL : "$(OUTDIR)\mps.exe"


CLEAN :
	-@erase "$(INTDIR)\mps.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\mps.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

F90=df.exe
F90_PROJ=/compile_only /nologo /warn:nofileopt /module:"Release/" /object:"Release/" 
F90_OBJS=.\Release/

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\qpgen_cinterface_mps.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\qpgen_cinterface_mps.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\mps.pdb" /machine:I386 /out:"$(OUTDIR)\mps.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mps.obj" \
	"..\..\lib\vcf2c.lib" \
	"..\..\lib\libMA27.lib" \
	"..\..\lib\libooqpbase.lib" \
	"..\..\lib\libooqpgensparse.lib" \
	"..\..\lib\libooqpgondzio.lib" \
	"..\..\lib\libooqpsparse.lib" \
	"..\..\lib\libblas.lib"

"$(OUTDIR)\mps.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "qpgen_cinterface_mps - Win32 Debug"

OUTDIR=.\..\QpGen\cinterface
INTDIR=.\qpgen_cinterface_mps___Win32_Debug
# Begin Custom Macros
OutDir=.\..\QpGen\cinterface
# End Custom Macros

ALL : "$(OUTDIR)\mps.exe"


CLEAN :
	-@erase "$(INTDIR)\mps.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\mps.exe"
	-@erase "$(OUTDIR)\mps.ilk"
	-@erase "$(OUTDIR)\mps.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

F90=df.exe
F90_PROJ=/check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /module:"qpgen_cinterface_mps___Win32_Debug/" /object:"qpgen_cinterface_mps___Win32_Debug/" /pdbfile:"..\QpGen\cinterface/DF60.PDB" 
F90_OBJS=.\qpgen_cinterface_mps___Win32_Debug/

.SUFFIXES: .fpp

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.fpp{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\qpgen_cinterface_mps.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\qpgen_cinterface_mps.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\mps.pdb" /debug /machine:I386 /out:"$(OUTDIR)\mps.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\mps.obj" \
	"..\..\lib\vcf2c.lib" \
	"..\..\lib\libMA27.lib" \
	"..\..\lib\libooqpbase.lib" \
	"..\..\lib\libooqpgensparse.lib" \
	"..\..\lib\libooqpgondzio.lib" \
	"..\..\lib\libooqpsparse.lib" \
	"..\..\lib\libblas.lib"

"$(OUTDIR)\mps.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("qpgen_cinterface_mps.dep")
!INCLUDE "qpgen_cinterface_mps.dep"
!ELSE 
!MESSAGE Warning: cannot find "qpgen_cinterface_mps.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "qpgen_cinterface_mps - Win32 Release" || "$(CFG)" == "qpgen_cinterface_mps - Win32 Debug"
SOURCE=..\QpGen\cinterface\mps.c

"$(INTDIR)\mps.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

