# Microsoft Developer Studio Generated NMAKE File, Based on libblas.dsp
!IF "$(CFG)" == ""
CFG=libblas - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libblas - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libblas - Win32 Release" && "$(CFG)" != "libblas - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libblas.mak" CFG="libblas - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libblas - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libblas - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libblas - Win32 Release"

OUTDIR=.\..\lib
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libblas.lib"


CLEAN :
	-@erase "$(INTDIR)\daxpy.obj"
	-@erase "$(INTDIR)\dcopy.obj"
	-@erase "$(INTDIR)\ddot.obj"
	-@erase "$(INTDIR)\dgemm.obj"
	-@erase "$(INTDIR)\dgemv.obj"
	-@erase "$(INTDIR)\dger.obj"
	-@erase "$(INTDIR)\dpotrf.obj"
	-@erase "$(INTDIR)\dpotrs.obj"
	-@erase "$(INTDIR)\dscal.obj"
	-@erase "$(INTDIR)\dswap.obj"
	-@erase "$(INTDIR)\dsymv.obj"
	-@erase "$(INTDIR)\dsyr.obj"
	-@erase "$(INTDIR)\dsyrk.obj"
	-@erase "$(INTDIR)\dsytrf.obj"
	-@erase "$(INTDIR)\dsytrs.obj"
	-@erase "$(INTDIR)\dtrsm.obj"
	-@erase "$(INTDIR)\idamax.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\libblas.lib"

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
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libblas.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libblas.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libblas.lib" 
LIB32_OBJS= \
	"$(INTDIR)\daxpy.obj" \
	"$(INTDIR)\dcopy.obj" \
	"$(INTDIR)\ddot.obj" \
	"$(INTDIR)\dgemm.obj" \
	"$(INTDIR)\dgemv.obj" \
	"$(INTDIR)\dger.obj" \
	"$(INTDIR)\dpotrf.obj" \
	"$(INTDIR)\dpotrs.obj" \
	"$(INTDIR)\dscal.obj" \
	"$(INTDIR)\dswap.obj" \
	"$(INTDIR)\dsymv.obj" \
	"$(INTDIR)\dsyr.obj" \
	"$(INTDIR)\dsyrk.obj" \
	"$(INTDIR)\dsytrf.obj" \
	"$(INTDIR)\dsytrs.obj" \
	"$(INTDIR)\dtrsm.obj" \
	"$(INTDIR)\idamax.obj"

"$(OUTDIR)\libblas.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libblas - Win32 Debug"

OUTDIR=.\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\lib
# End Custom Macros

ALL : "$(OUTDIR)\libblas.lib"


CLEAN :
	-@erase "$(INTDIR)\daxpy.obj"
	-@erase "$(INTDIR)\dcopy.obj"
	-@erase "$(INTDIR)\ddot.obj"
	-@erase "$(INTDIR)\dgemm.obj"
	-@erase "$(INTDIR)\dgemv.obj"
	-@erase "$(INTDIR)\dger.obj"
	-@erase "$(INTDIR)\dpotrf.obj"
	-@erase "$(INTDIR)\dpotrs.obj"
	-@erase "$(INTDIR)\dscal.obj"
	-@erase "$(INTDIR)\dswap.obj"
	-@erase "$(INTDIR)\dsymv.obj"
	-@erase "$(INTDIR)\dsyr.obj"
	-@erase "$(INTDIR)\dsyrk.obj"
	-@erase "$(INTDIR)\dsytrf.obj"
	-@erase "$(INTDIR)\dsytrs.obj"
	-@erase "$(INTDIR)\dtrsm.obj"
	-@erase "$(INTDIR)\idamax.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\libblas.lib"

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
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libblas.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libblas.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libblas.lib" 
LIB32_OBJS= \
	"$(INTDIR)\daxpy.obj" \
	"$(INTDIR)\dcopy.obj" \
	"$(INTDIR)\ddot.obj" \
	"$(INTDIR)\dgemm.obj" \
	"$(INTDIR)\dgemv.obj" \
	"$(INTDIR)\dger.obj" \
	"$(INTDIR)\dpotrf.obj" \
	"$(INTDIR)\dpotrs.obj" \
	"$(INTDIR)\dscal.obj" \
	"$(INTDIR)\dswap.obj" \
	"$(INTDIR)\dsymv.obj" \
	"$(INTDIR)\dsyr.obj" \
	"$(INTDIR)\dsyrk.obj" \
	"$(INTDIR)\dsytrf.obj" \
	"$(INTDIR)\dsytrs.obj" \
	"$(INTDIR)\dtrsm.obj" \
	"$(INTDIR)\idamax.obj"

"$(OUTDIR)\libblas.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libblas.dep")
!INCLUDE "libblas.dep"
!ELSE 
!MESSAGE Warning: cannot find "libblas.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libblas - Win32 Release" || "$(CFG)" == "libblas - Win32 Debug"
SOURCE=..\extras\blas\daxpy.c

"$(INTDIR)\daxpy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dcopy.c

"$(INTDIR)\dcopy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\ddot.c

"$(INTDIR)\ddot.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dgemm.c

"$(INTDIR)\dgemm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dgemv.c

"$(INTDIR)\dgemv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dger.c

"$(INTDIR)\dger.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utilities\dpotrf.c

"$(INTDIR)\dpotrf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utilities\dpotrs.c

"$(INTDIR)\dpotrs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dscal.c

"$(INTDIR)\dscal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dswap.c

"$(INTDIR)\dswap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dsymv.c

"$(INTDIR)\dsymv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dsyr.c

"$(INTDIR)\dsyr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dsyrk.c

"$(INTDIR)\dsyrk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utilities\dsytrf.c

"$(INTDIR)\dsytrf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utilities\dsytrs.c

"$(INTDIR)\dsytrs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\dtrsm.c

"$(INTDIR)\dtrsm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\extras\blas\idamax.c

"$(INTDIR)\idamax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

