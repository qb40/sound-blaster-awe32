# Microsoft Visual C++ generated build script - Do not modify

PROJ = DEMO16
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\ADIP300\WINDOWS\SAMPLE\AWEDEMO\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /W3 /FR /G2 /Zi /D_DEBUG /Od /AM /GA /Fd"TEST31.PDB"
CFLAGS_R_WEXE = /nologo /W3 /FR /O1 /DNDEBUG /AM /GA
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = oldnames libw mlibcew commdlg.lib mmsystem.lib olecli.lib olesvr.lib shell.lib 
LIBS_R_WEXE = oldnames libw mlibcew commdlg.lib olecli.lib olesvr.lib shell.lib 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = WINAWE32.DEF
OBJS_EXT = 
LIBS_EXT = 
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
SBRS = WINAWE32.SBR \
		MCI.SBR


WINAWE32_DEP = c:\adip300\windows\sample\awedemo\winawe32.h \
	c:\adip300\windows\sample\awedemo\resource.h \
	c:\adip300\windows\sample\awedemo\awe_dll.h \
	c:\adip300\windows\sample\awedemo\awe_api.h \
	c:\adip300\windows\sample\awedemo\errormsg.h \
	c:\adip300\windows\sample\awedemo\indices.h \
	c:\adip300\windows\sample\awedemo\mci.h


MCI_DEP = c:\adip300\windows\sample\awedemo\mci.h


WINAWE32_RCDEP = c:\adip300\windows\sample\awedemo\resource.h \
	c:\adip300\windows\sample\awedemo\winawe32.ico


all:	$(PROJ).EXE $(PROJ).BSC

WINAWE32.OBJ:	WINAWE32.C $(WINAWE32_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c WINAWE32.C

MCI.OBJ:	MCI.C $(MCI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MCI.C

WINAWE32.RES:	WINAWE32.RC $(WINAWE32_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r WINAWE32.RC


$(PROJ).EXE::	WINAWE32.RES

$(PROJ).EXE::	WINAWE32.OBJ MCI.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
WINAWE32.OBJ +
MCI.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
e:\MSVC\LIB\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) WINAWE32.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	WINAWE32.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) WINAWE32.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
