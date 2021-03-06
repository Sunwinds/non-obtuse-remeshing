# Microsoft Developer Studio Generated NMAKE File, Based on svm_gondzio.dsp
!IF "$(CFG)" == ""
CFG=svm_gondzio - Win32 Debug
!MESSAGE No configuration specified. Defaulting to svm_gondzio - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "svm_gondzio - Win32 Release" && "$(CFG)" != "svm_gondzio - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "svm_gondzio.mak" CFG="svm_gondzio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "svm_gondzio - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "svm_gondzio - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "svm_gondzio - Win32 Release"

OUTDIR=.\..
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\svm_gondzio.exe"

!ELSE 

ALL : "libooqpgondzio - Win32 Release" "libooqpdense - Win32 Release" "libooqpbase - Win32 Release" "libblas - Win32 Release" "include - Win32 Release" "$(OUTDIR)\svm_gondzio.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"include - Win32 ReleaseCLEAN" "libblas - Win32 ReleaseCLEAN" "libooqpbase - Win32 ReleaseCLEAN" "libooqpdense - Win32 ReleaseCLEAN" "libooqpgondzio - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Svm.obj"
	-@erase "$(INTDIR)\SvmData.obj"
	-@erase "$(INTDIR)\SvmGondzioDriver.obj"
	-@erase "$(INTDIR)\SvmLinsys.obj"
	-@erase "$(INTDIR)\SvmResiduals.obj"
	-@erase "$(INTDIR)\SvmVars.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\svm_gondzio.exe"

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
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "..\src\Abstract" /I "..\src\QpSolvers" /I "..\src\Vector" /I "..\src\DenseLinearAlgebra" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\svm_gondzio.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\svm_gondzio.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\svm_gondzio.pdb" /machine:I386 /out:"$(OUTDIR)\svm_gondzio.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Svm.obj" \
	"$(INTDIR)\SvmData.obj" \
	"$(INTDIR)\SvmGondzioDriver.obj" \
	"$(INTDIR)\SvmLinsys.obj" \
	"$(INTDIR)\SvmResiduals.obj" \
	"$(INTDIR)\SvmVars.obj" \
	"..\lib\vcf2c.lib" \
	"$(OUTDIR)\lib\libblas.lib" \
	"$(OUTDIR)\lib\libooqpbase.lib" \
	"$(OUTDIR)\lib\libooqpdense.lib" \
	"$(OUTDIR)\lib\libooqpgondzio.lib"

"$(OUTDIR)\svm_gondzio.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "svm_gondzio - Win32 Debug"

OUTDIR=.\..
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\svm_gondzio.exe"

!ELSE 

ALL : "libooqpgondzio - Win32 Debug" "libooqpdense - Win32 Debug" "libooqpbase - Win32 Debug" "libblas - Win32 Debug" "include - Win32 Debug" "$(OUTDIR)\svm_gondzio.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"include - Win32 DebugCLEAN" "libblas - Win32 DebugCLEAN" "libooqpbase - Win32 DebugCLEAN" "libooqpdense - Win32 DebugCLEAN" "libooqpgondzio - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Svm.obj"
	-@erase "$(INTDIR)\SvmData.obj"
	-@erase "$(INTDIR)\SvmGondzioDriver.obj"
	-@erase "$(INTDIR)\SvmLinsys.obj"
	-@erase "$(INTDIR)\SvmResiduals.obj"
	-@erase "$(INTDIR)\SvmVars.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\svm_gondzio.exe"
	-@erase "$(OUTDIR)\svm_gondzio.ilk"
	-@erase "$(OUTDIR)\svm_gondzio.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

F90=df.exe
F90_PROJ=/check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /module:"Debug/" /object:"Debug/" /pdbfile:"../DF60.PDB" 
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
CPP_PROJ=/nologo /Gr /MLd /W3 /Gm /GX /ZI /Od /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\DenseLinearAlgebra" /I "..\src\QpSolvers" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\svm_gondzio.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\svm_gondzio.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\svm_gondzio.pdb" /debug /machine:I386 /out:"$(OUTDIR)\svm_gondzio.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Svm.obj" \
	"$(INTDIR)\SvmData.obj" \
	"$(INTDIR)\SvmGondzioDriver.obj" \
	"$(INTDIR)\SvmLinsys.obj" \
	"$(INTDIR)\SvmResiduals.obj" \
	"$(INTDIR)\SvmVars.obj" \
	"..\lib\vcf2c.lib" \
	"$(OUTDIR)\lib\libblas.lib" \
	"$(OUTDIR)\lib\libooqpbase.lib" \
	"$(OUTDIR)\lib\libooqpdense.lib" \
	"$(OUTDIR)\lib\libooqpgondzio.lib"

"$(OUTDIR)\svm_gondzio.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("svm_gondzio.dep")
!INCLUDE "svm_gondzio.dep"
!ELSE 
!MESSAGE Warning: cannot find "svm_gondzio.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "svm_gondzio - Win32 Release" || "$(CFG)" == "svm_gondzio - Win32 Debug"
SOURCE=..\src\Svm\Svm.C

"$(INTDIR)\Svm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Svm\SvmData.C

"$(INTDIR)\SvmData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Svm\SvmGondzioDriver.C

"$(INTDIR)\SvmGondzioDriver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Svm\SvmLinsys.C

"$(INTDIR)\SvmLinsys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Svm\SvmResiduals.C

"$(INTDIR)\SvmResiduals.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Svm\SvmVars.C

"$(INTDIR)\SvmVars.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "svm_gondzio - Win32 Release"

"include - Win32 Release" : 
   cd "."
   NMAKE /f include.mak
   cd "."

"include - Win32 ReleaseCLEAN" : 
   cd "."
   cd "."

!ELSEIF  "$(CFG)" == "svm_gondzio - Win32 Debug"

"include - Win32 Debug" : 
   cd "."
   nmake /f "include.mak"
   cd "."

"include - Win32 DebugCLEAN" : 
   cd "."
   cd "."

!ENDIF 

!IF  "$(CFG)" == "svm_gondzio - Win32 Release"

"libblas - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Release" 
   cd "."

"libblas - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "svm_gondzio - Win32 Debug"

"libblas - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Debug" 
   cd "."

"libblas - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "svm_gondzio - Win32 Release"

"libooqpbase - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Release" 
   cd "."

"libooqpbase - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "svm_gondzio - Win32 Debug"

"libooqpbase - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Debug" 
   cd "."

"libooqpbase - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "svm_gondzio - Win32 Release"

"libooqpdense - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpdense.mak CFG="libooqpdense - Win32 Release" 
   cd "."

"libooqpdense - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpdense.mak CFG="libooqpdense - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "svm_gondzio - Win32 Debug"

"libooqpdense - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpdense.mak CFG="libooqpdense - Win32 Debug" 
   cd "."

"libooqpdense - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpdense.mak CFG="libooqpdense - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "svm_gondzio - Win32 Release"

"libooqpgondzio - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Release" 
   cd "."

"libooqpgondzio - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "svm_gondzio - Win32 Debug"

"libooqpgondzio - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Debug" 
   cd "."

"libooqpgondzio - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

