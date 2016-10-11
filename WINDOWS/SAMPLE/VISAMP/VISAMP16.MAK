# Microsoft Visual C++ generated build script - Do not modify

PROJ = VISAMP16
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\ADIP300\WINDOWS\SAMPLE\VISAMP\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = VISAMP.C    
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /G2 /W3 /Zi /AM /Od /D "_DEBUG" /FR /GA /Fd"VISAMP31.PDB"
CFLAGS_R_WEXE = /nologo /W3 /AM /O1 /D "NDEBUG" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /CO /ALIGN:16 /STACK:10240
LFLAGS_R_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /ALIGN:16 /STACK:10240
LIBS_D_WEXE = oldnames libw commdlg shell olecli olesvr mlibcew
LIBS_R_WEXE = oldnames libw commdlg shell olecli olesvr mlibcew
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = VISAMP.DEF
OBJS_EXT = 
LIBS_EXT = E:\MSVC\LIB\MMSYSTEM.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = VISAMP.SBR


MMSYSTEM_DEP = 

VISAMP_DEP = c:\adip300\windows\sample\visamp\visamp.h \
	c:\adip300\windows\sample\visamp\indices.h \
	c:\adip300\windows\sample\visamp\awe_dll.h \
	c:\adip300\windows\sample\visamp\awe_api.h \
	c:\adip300\windows\sample\visamp\errormsg.h \
	c:\adip300\windows\sample\visamp\sftype.h \
	c:\adip300\windows\sample\visamp\sfenum.h


VISAMP_RCDEP = c:\adip300\windows\sample\visamp\visamp.ico


all:	$(PROJ).EXE $(PROJ).BSC

VISAMP.OBJ:	VISAMP.C $(VISAMP_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c VISAMP.C

VISAMP.RES:	VISAMP.RC $(VISAMP_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r VISAMP.RC


$(PROJ).EXE::	VISAMP.RES

$(PROJ).EXE::	VISAMP.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
VISAMP.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
e:\MSVC\LIB\+
E:\MSVC\LIB\MMSYSTEM.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) VISAMP.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	VISAMP.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) VISAMP.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
