# Microsoft Developer Studio Generated NMAKE File, Based on libooqpbase.dsp
!IF "$(CFG)" == ""
CFG=libooqpbase - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libooqpbase - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libooqpbase - Win32 Release" && "$(CFG)" != "libooqpbase - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libooqpbase.mak" CFG="libooqpbase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libooqpbase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libooqpbase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libooqpbase - Win32 Release"

OUTDIR=.\..\lib
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpbase.lib"


CLEAN :
	-@erase "$(INTDIR)\Data.obj"
	-@erase "$(INTDIR)\drand.obj"
	-@erase "$(INTDIR)\IotrRefCount.obj"
	-@erase "$(INTDIR)\OoqpMonitor.obj"
	-@erase "$(INTDIR)\OoqpVector.obj"
	-@erase "$(INTDIR)\OoqpVersion.obj"
	-@erase "$(INTDIR)\SimpleVector.obj"
	-@erase "$(INTDIR)\Solver.obj"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\Status.obj"
	-@erase "$(INTDIR)\Variables.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VectorUtilities.obj"
	-@erase "$(OUTDIR)\libooqpbase.lib"

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
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "..\src\Abstract" /I "..\src\Utilities" /I "..\src\Vector" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpbase.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpbase.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpbase.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Data.obj" \
	"$(INTDIR)\drand.obj" \
	"$(INTDIR)\IotrRefCount.obj" \
	"$(INTDIR)\OoqpMonitor.obj" \
	"$(INTDIR)\OoqpVector.obj" \
	"$(INTDIR)\OoqpVersion.obj" \
	"$(INTDIR)\SimpleVector.obj" \
	"$(INTDIR)\Solver.obj" \
	"$(INTDIR)\sort.obj" \
	"$(INTDIR)\Status.obj" \
	"$(INTDIR)\Variables.obj" \
	"$(INTDIR)\VectorUtilities.obj"

"$(OUTDIR)\libooqpbase.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libooqpbase - Win32 Debug"

OUTDIR=.\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpbase.lib"


CLEAN :
	-@erase "$(INTDIR)\Data.obj"
	-@erase "$(INTDIR)\drand.obj"
	-@erase "$(INTDIR)\IotrRefCount.obj"
	-@erase "$(INTDIR)\OoqpMonitor.obj"
	-@erase "$(INTDIR)\OoqpVector.obj"
	-@erase "$(INTDIR)\OoqpVersion.obj"
	-@erase "$(INTDIR)\SimpleVector.obj"
	-@erase "$(INTDIR)\Solver.obj"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\Status.obj"
	-@erase "$(INTDIR)\Variables.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VectorUtilities.obj"
	-@erase "$(OUTDIR)\libooqpbase.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

F90=df.exe
F90_PROJ=/check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /module:"Debug/" /object:"Debug/" /pdbfile:"..\lib/DF60.PDB" 
F90_OBJS=.\Debug/

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
CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "..\src\Abstract" /I "..\src\Utilities" /I "..\src\Vector" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpbase.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpbase.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpbase.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Data.obj" \
	"$(INTDIR)\drand.obj" \
	"$(INTDIR)\IotrRefCount.obj" \
	"$(INTDIR)\OoqpMonitor.obj" \
	"$(INTDIR)\OoqpVector.obj" \
	"$(INTDIR)\OoqpVersion.obj" \
	"$(INTDIR)\SimpleVector.obj" \
	"$(INTDIR)\Solver.obj" \
	"$(INTDIR)\sort.obj" \
	"$(INTDIR)\Status.obj" \
	"$(INTDIR)\Variables.obj" \
	"$(INTDIR)\VectorUtilities.obj"

"$(OUTDIR)\libooqpbase.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libooqpbase.dep")
!INCLUDE "libooqpbase.dep"
!ELSE 
!MESSAGE Warning: cannot find "libooqpbase.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libooqpbase - Win32 Release" || "$(CFG)" == "libooqpbase - Win32 Debug"
SOURCE=..\src\Abstract\Data.C

"$(INTDIR)\Data.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utilities\drand.C

"$(INTDIR)\drand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Abstract\IotrRefCount.C

"$(INTDIR)\IotrRefCount.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Abstract\OoqpMonitor.C

"$(INTDIR)\OoqpMonitor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Vector\OoqpVector.C

"$(INTDIR)\OoqpVector.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Abstract\OoqpVersion.C

"$(INTDIR)\OoqpVersion.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Vector\SimpleVector.C

"$(INTDIR)\SimpleVector.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Abstract\Solver.C

"$(INTDIR)\Solver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utilities\sort.C

"$(INTDIR)\sort.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Abstract\Status.C

"$(INTDIR)\Status.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Abstract\Variables.C

"$(INTDIR)\Variables.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Vector\VectorUtilities.C

"$(INTDIR)\VectorUtilities.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

