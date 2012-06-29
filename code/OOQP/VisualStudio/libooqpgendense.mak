# Microsoft Developer Studio Generated NMAKE File, Based on libooqpgendense.dsp
!IF "$(CFG)" == ""
CFG=libooqpgendense - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libooqpgendense - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libooqpgendense - Win32 Release" && "$(CFG)" != "libooqpgendense - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libooqpgendense.mak" CFG="libooqpgendense - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libooqpgendense - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libooqpgendense - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libooqpgendense - Win32 Release"

OUTDIR=.\..\lib
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpgendense.lib"


CLEAN :
	-@erase "$(INTDIR)\cBounds.obj"
	-@erase "$(INTDIR)\cMpsReader.obj"
	-@erase "$(INTDIR)\cQpGen.obj"
	-@erase "$(INTDIR)\cQpGenDense.obj"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\MpsReader.obj"
	-@erase "$(INTDIR)\QpGen.obj"
	-@erase "$(INTDIR)\QpGenData.obj"
	-@erase "$(INTDIR)\QpGenDense.obj"
	-@erase "$(INTDIR)\QpGenDenseLinsys.obj"
	-@erase "$(INTDIR)\QpGenLinsys.obj"
	-@erase "$(INTDIR)\QpGenResiduals.obj"
	-@erase "$(INTDIR)\QpGenVars.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\libooqpgendense.lib"

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
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "..\src\Abstract" /I "..\src\DenseLinearAlgebra" /I "..\src\Vector" /I "..\src\Readers" /I "..\src\QpGen" /I "..\src\QpSolvers" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpgendense.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpgendense.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpgendense.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cBounds.obj" \
	"$(INTDIR)\cMpsReader.obj" \
	"$(INTDIR)\cQpGen.obj" \
	"$(INTDIR)\cQpGenDense.obj" \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\MpsReader.obj" \
	"$(INTDIR)\QpGen.obj" \
	"$(INTDIR)\QpGenData.obj" \
	"$(INTDIR)\QpGenDense.obj" \
	"$(INTDIR)\QpGenDenseLinsys.obj" \
	"$(INTDIR)\QpGenLinsys.obj" \
	"$(INTDIR)\QpGenResiduals.obj" \
	"$(INTDIR)\QpGenVars.obj"

"$(OUTDIR)\libooqpgendense.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libooqpgendense - Win32 Debug"

OUTDIR=.\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpgendense.lib"


CLEAN :
	-@erase "$(INTDIR)\cBounds.obj"
	-@erase "$(INTDIR)\cMpsReader.obj"
	-@erase "$(INTDIR)\cQpGen.obj"
	-@erase "$(INTDIR)\cQpGenDense.obj"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\MpsReader.obj"
	-@erase "$(INTDIR)\QpGen.obj"
	-@erase "$(INTDIR)\QpGenData.obj"
	-@erase "$(INTDIR)\QpGenDense.obj"
	-@erase "$(INTDIR)\QpGenDenseLinsys.obj"
	-@erase "$(INTDIR)\QpGenLinsys.obj"
	-@erase "$(INTDIR)\QpGenResiduals.obj"
	-@erase "$(INTDIR)\QpGenVars.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\libooqpgendense.lib"

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
CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "..\src\Abstract" /I "..\src\DenseLinearAlgebra" /I "..\src\Vector" /I "..\src\Readers" /I "..\src\QpGen" /I "..\src\QpSolvers" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpgendense.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpgendense.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpgendense.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cBounds.obj" \
	"$(INTDIR)\cMpsReader.obj" \
	"$(INTDIR)\cQpGen.obj" \
	"$(INTDIR)\cQpGenDense.obj" \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\MpsReader.obj" \
	"$(INTDIR)\QpGen.obj" \
	"$(INTDIR)\QpGenData.obj" \
	"$(INTDIR)\QpGenDense.obj" \
	"$(INTDIR)\QpGenDenseLinsys.obj" \
	"$(INTDIR)\QpGenLinsys.obj" \
	"$(INTDIR)\QpGenResiduals.obj" \
	"$(INTDIR)\QpGenVars.obj"

"$(OUTDIR)\libooqpgendense.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libooqpgendense.dep")
!INCLUDE "libooqpgendense.dep"
!ELSE 
!MESSAGE Warning: cannot find "libooqpgendense.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libooqpgendense - Win32 Release" || "$(CFG)" == "libooqpgendense - Win32 Debug"
SOURCE=..\src\CInterface\cBounds.c

"$(INTDIR)\cBounds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\CInterface\cMpsReader.C

"$(INTDIR)\cMpsReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\CInterface\cQpGen.C

"$(INTDIR)\cQpGen.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\CInterface\cQpGenDense.C

"$(INTDIR)\cQpGenDense.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Readers\hash.C

"$(INTDIR)\hash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Readers\MpsReader.C

"$(INTDIR)\MpsReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGen.C

"$(INTDIR)\QpGen.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGenData.C

"$(INTDIR)\QpGenData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGenDense.C

"$(INTDIR)\QpGenDense.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGenDenseLinsys.C

"$(INTDIR)\QpGenDenseLinsys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGenLinsys.C

"$(INTDIR)\QpGenLinsys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGenResiduals.C

"$(INTDIR)\QpGenResiduals.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\QpGen\QpGenVars.C

"$(INTDIR)\QpGenVars.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

