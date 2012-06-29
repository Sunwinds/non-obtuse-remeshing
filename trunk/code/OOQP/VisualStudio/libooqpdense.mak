# Microsoft Developer Studio Generated NMAKE File, Based on libooqpdense.dsp
!IF "$(CFG)" == ""
CFG=libooqpdense - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libooqpdense - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libooqpdense - Win32 Release" && "$(CFG)" != "libooqpdense - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libooqpdense.mak" CFG="libooqpdense - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libooqpdense - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libooqpdense - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libooqpdense - Win32 Release"

OUTDIR=.\..\lib
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpdense.lib"


CLEAN :
	-@erase "$(INTDIR)\DenseGenMatrix.obj"
	-@erase "$(INTDIR)\DenseLinearAlgebraPackage.obj"
	-@erase "$(INTDIR)\DenseStorage.obj"
	-@erase "$(INTDIR)\DenseSymMatrix.obj"
	-@erase "$(INTDIR)\DeSymIndefSolver.obj"
	-@erase "$(INTDIR)\DeSymPSDSolver.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\libooqpdense.lib"

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
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "..\src\DenseLinearAlgebra" /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\Utilities" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpdense.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpdense.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpdense.lib" 
LIB32_OBJS= \
	"$(INTDIR)\DenseGenMatrix.obj" \
	"$(INTDIR)\DenseLinearAlgebraPackage.obj" \
	"$(INTDIR)\DenseStorage.obj" \
	"$(INTDIR)\DenseSymMatrix.obj" \
	"$(INTDIR)\DeSymIndefSolver.obj" \
	"$(INTDIR)\DeSymPSDSolver.obj"

"$(OUTDIR)\libooqpdense.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libooqpdense - Win32 Debug"

OUTDIR=.\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libooqpdense.lib"


CLEAN :
	-@erase "$(INTDIR)\DenseGenMatrix.obj"
	-@erase "$(INTDIR)\DenseLinearAlgebraPackage.obj"
	-@erase "$(INTDIR)\DenseStorage.obj"
	-@erase "$(INTDIR)\DenseSymMatrix.obj"
	-@erase "$(INTDIR)\DeSymIndefSolver.obj"
	-@erase "$(INTDIR)\DeSymPSDSolver.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\libooqpdense.lib"

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
CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "..\src\DenseLinearAlgebra" /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\Utilities" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libooqpdense.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libooqpdense.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libooqpdense.lib" 
LIB32_OBJS= \
	"$(INTDIR)\DenseGenMatrix.obj" \
	"$(INTDIR)\DenseLinearAlgebraPackage.obj" \
	"$(INTDIR)\DenseStorage.obj" \
	"$(INTDIR)\DenseSymMatrix.obj" \
	"$(INTDIR)\DeSymIndefSolver.obj" \
	"$(INTDIR)\DeSymPSDSolver.obj"

"$(OUTDIR)\libooqpdense.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libooqpdense.dep")
!INCLUDE "libooqpdense.dep"
!ELSE 
!MESSAGE Warning: cannot find "libooqpdense.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libooqpdense - Win32 Release" || "$(CFG)" == "libooqpdense - Win32 Debug"
SOURCE=..\src\DenseLinearAlgebra\DenseGenMatrix.C

"$(INTDIR)\DenseGenMatrix.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\DenseLinearAlgebra\DenseLinearAlgebraPackage.C

"$(INTDIR)\DenseLinearAlgebraPackage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\DenseLinearAlgebra\DenseStorage.C

"$(INTDIR)\DenseStorage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\DenseLinearAlgebra\DenseSymMatrix.C

"$(INTDIR)\DenseSymMatrix.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\DenseLinearAlgebra\DeSymIndefSolver.C

"$(INTDIR)\DeSymIndefSolver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\DenseLinearAlgebra\DeSymPSDSolver.C

"$(INTDIR)\DeSymPSDSolver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

