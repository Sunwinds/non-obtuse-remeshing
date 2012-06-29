# Microsoft Developer Studio Generated NMAKE File, Based on qpgen_sparse_gondzio.dsp
!IF "$(CFG)" == ""
CFG=qpgen_sparse_gondzio - Win32 Debug
!MESSAGE No configuration specified. Defaulting to qpgen_sparse_gondzio - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "qpgen_sparse_gondzio - Win32 Release" && "$(CFG)" != "qpgen_sparse_gondzio - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qpgen_sparse_gondzio.mak" CFG="qpgen_sparse_gondzio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qpgen_sparse_gondzio - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "qpgen_sparse_gondzio - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

OUTDIR=.\..
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\qpgen_sparse_gondzio.exe"

!ELSE 

ALL : "libooqpsparse - Win32 Release" "libooqpgondzio - Win32 Release" "libooqpgensparse - Win32 Release" "libooqpbase - Win32 Release" "libMA27 - Win32 Release" "libblas - Win32 Release" "include - Win32 Release" "$(OUTDIR)\qpgen_sparse_gondzio.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"include - Win32 ReleaseCLEAN" "libblas - Win32 ReleaseCLEAN" "libMA27 - Win32 ReleaseCLEAN" "libooqpbase - Win32 ReleaseCLEAN" "libooqpgensparse - Win32 ReleaseCLEAN" "libooqpgondzio - Win32 ReleaseCLEAN" "libooqpsparse - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\QpGenSparseGondzioDriver.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\qpgen_sparse_gondzio.exe"

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
CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\QpSolvers" /I "..\src\Readers" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\qpgen_sparse_gondzio.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\qpgen_sparse_gondzio.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\qpgen_sparse_gondzio.pdb" /machine:I386 /out:"$(OUTDIR)\qpgen_sparse_gondzio.exe" 
LINK32_OBJS= \
	"$(INTDIR)\QpGenSparseGondzioDriver.obj" \
	"..\lib\vcf2c.lib" \
	"$(OUTDIR)\lib\libblas.lib" \
	"$(OUTDIR)\lib\libMA27.lib" \
	"$(OUTDIR)\lib\libooqpbase.lib" \
	"$(OUTDIR)\lib\libooqpgensparse.lib" \
	"$(OUTDIR)\lib\libooqpgondzio.lib" \
	"$(OUTDIR)\lib\libooqpsparse.lib"

"$(OUTDIR)\qpgen_sparse_gondzio.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

OUTDIR=.\..
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\qpgen_sparse_gondzio.exe"

!ELSE 

ALL : "libooqpsparse - Win32 Debug" "libooqpgondzio - Win32 Debug" "libooqpgensparse - Win32 Debug" "libooqpbase - Win32 Debug" "libMA27 - Win32 Debug" "libblas - Win32 Debug" "include - Win32 Debug" "$(OUTDIR)\qpgen_sparse_gondzio.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"include - Win32 DebugCLEAN" "libblas - Win32 DebugCLEAN" "libMA27 - Win32 DebugCLEAN" "libooqpbase - Win32 DebugCLEAN" "libooqpgensparse - Win32 DebugCLEAN" "libooqpgondzio - Win32 DebugCLEAN" "libooqpsparse - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\QpGenSparseGondzioDriver.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\qpgen_sparse_gondzio.exe"
	-@erase "$(OUTDIR)\qpgen_sparse_gondzio.ilk"
	-@erase "$(OUTDIR)\qpgen_sparse_gondzio.pdb"

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
CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "..\src\Abstract" /I "..\src\Vector" /I "..\src\QpSolvers" /I "..\src\Readers" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\qpgen_sparse_gondzio.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c /Tp 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\qpgen_sparse_gondzio.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\qpgen_sparse_gondzio.pdb" /debug /machine:I386 /out:"$(OUTDIR)\qpgen_sparse_gondzio.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\QpGenSparseGondzioDriver.obj" \
	"..\lib\vcf2c.lib" \
	"$(OUTDIR)\lib\libblas.lib" \
	"$(OUTDIR)\lib\libMA27.lib" \
	"$(OUTDIR)\lib\libooqpbase.lib" \
	"$(OUTDIR)\lib\libooqpgensparse.lib" \
	"$(OUTDIR)\lib\libooqpgondzio.lib" \
	"$(OUTDIR)\lib\libooqpsparse.lib"

"$(OUTDIR)\qpgen_sparse_gondzio.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("qpgen_sparse_gondzio.dep")
!INCLUDE "qpgen_sparse_gondzio.dep"
!ELSE 
!MESSAGE Warning: cannot find "qpgen_sparse_gondzio.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release" || "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"
SOURCE=.\..\src\QpGen\QpGenSparseGondzioDriver.C

"$(INTDIR)\QpGenSparseGondzioDriver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"include - Win32 Release" : 
   cd "."
   NMAKE /f include.mak
   cd "."

"include - Win32 ReleaseCLEAN" : 
   cd "."
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"include - Win32 Debug" : 
   cd "."
   nmake /f "include.mak"
   cd "."

"include - Win32 DebugCLEAN" : 
   cd "."
   cd "."

!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"libblas - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Release" 
   cd "."

"libblas - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"libblas - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Debug" 
   cd "."

"libblas - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libblas.mak CFG="libblas - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"libMA27 - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libMA27.mak CFG="libMA27 - Win32 Release" 
   cd "."

"libMA27 - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libMA27.mak CFG="libMA27 - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"libMA27 - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libMA27.mak CFG="libMA27 - Win32 Debug" 
   cd "."

"libMA27 - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libMA27.mak CFG="libMA27 - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"libooqpbase - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Release" 
   cd "."

"libooqpbase - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"libooqpbase - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Debug" 
   cd "."

"libooqpbase - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpbase.mak CFG="libooqpbase - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"libooqpgensparse - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgensparse.mak CFG="libooqpgensparse - Win32 Release" 
   cd "."

"libooqpgensparse - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgensparse.mak CFG="libooqpgensparse - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"libooqpgensparse - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgensparse.mak CFG="libooqpgensparse - Win32 Debug" 
   cd "."

"libooqpgensparse - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgensparse.mak CFG="libooqpgensparse - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"libooqpgondzio - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Release" 
   cd "."

"libooqpgondzio - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"libooqpgondzio - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Debug" 
   cd "."

"libooqpgondzio - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpgondzio.mak CFG="libooqpgondzio - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Release"

"libooqpsparse - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpsparse.mak CFG="libooqpsparse - Win32 Release" 
   cd "."

"libooqpsparse - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpsparse.mak CFG="libooqpsparse - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "qpgen_sparse_gondzio - Win32 Debug"

"libooqpsparse - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpsparse.mak CFG="libooqpsparse - Win32 Debug" 
   cd "."

"libooqpsparse - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\libooqpsparse.mak CFG="libooqpsparse - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

