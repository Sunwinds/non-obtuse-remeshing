# Microsoft Developer Studio Generated NMAKE File, Based on libooqpbound.dsp
!IF "$(CFG)" == ""
CFG=libooqpbound - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libooqpbound - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libooqpbound - Win32 Release" && "$(CFG)" != "libooqpbound - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libooqpbound.mak" CFG="libooqpbound - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libooqpbound - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libooqpbound - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libooqpbound - Win32 Release"

OUTDIR=.\../lib
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpbound.lib"


CLEAN :
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\MpsReader.obj"
	-@erase "$(INTDIR)\QpBound.obj"
	-@erase "$(INTDIR)\QpBoundData.obj"
	-@erase "$(INTDIR)\QpBoundLinsys.obj"
	-@erase "$(INTDIR)\QpBoundResiduals.obj"
	-@erase "$(INTDIR)\QpBoundVars.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\libooqpbound.lib"

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
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\DenseLinearAlgebra" /I "..\src\Readers" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpbound.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpbound.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpbound.lib" 
LIB32_OBJS= \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\MpsReader.obj" \
	"$(INTDIR)\QpBound.obj" \
	"$(INTDIR)\QpBoundData.obj" \
	"$(INTDIR)\QpBoundLinsys.obj" \
	"$(INTDIR)\QpBoundResiduals.obj" \
	"$(INTDIR)\QpBoundVars.obj"

"$(OUTDIR)\libooqpbound.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libooqpbound - Win32 Debug"

OUTDIR=.\../lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpbound.lib"


CLEAN :
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\MpsReader.obj"
	-@erase "$(INTDIR)\QpBound.obj"
	-@erase "$(INTDIR)\QpBoundData.obj"
	-@erase "$(INTDIR)\QpBoundLinsys.obj"
	-@erase "$(INTDIR)\QpBoundResiduals.obj"
	-@erase "$(INTDIR)\QpBoundVars.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\libooqpbound.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

F90=df.exe
F90_PROJ=/check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /module:"Debug/" /object:"Debug/" /pdbfile:"../lib/DF60.PDB" 
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
CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\QpBound" /I "..\src\Readers" /I "..\src\DenseLinearAlgebra" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpbound.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpbound.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpbound.lib" 
LIB32_OBJS= \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\MpsReader.obj" \
	"$(INTDIR)\QpBound.obj" \
	"$(INTDIR)\QpBoundData.obj" \
	"$(INTDIR)\QpBoundLinsys.obj" \
	"$(INTDIR)\QpBoundResiduals.obj" \
	"$(INTDIR)\QpBoundVars.obj"

"$(OUTDIR)\libooqpbound.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libooqpbound.dep")
!INCLUDE "libooqpbound.dep"
!ELSE 
!MESSAGE Warning: cannot find "libooqpbound.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libooqpbound - Win32 Release" || "$(CFG)" == "libooqpbound - Win32 Debug"
SOURCE=..\src\Readers\hash.C

"$(INTDIR)\hash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Readers\MpsReader.C

"$(INTDIR)\MpsReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpBound\QpBound.C

"$(INTDIR)\QpBound.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpBound\QpBoundData.C

"$(INTDIR)\QpBoundData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpBound\QpBoundLinsys.C

"$(INTDIR)\QpBoundLinsys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpBound\QpBoundResiduals.C

"$(INTDIR)\QpBoundResiduals.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpBound\QpBoundVars.C

"$(INTDIR)\QpBoundVars.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

