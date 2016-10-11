/*****************************************************************************
*                                                                            *
* WINAWE32 Sample demo for Sound Blaster Advanced WavEffects                 *
*                                                                            *
* (C) Copyright Creative Technology Ltd. 1992-94. All rights reserved	     *
* worldwide.                                                                 *
*                                                                            *
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY      *
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE	     *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR      *
* PURPOSE.                                                                   *
*                                                                            *
* You have a royalty-free right to use, modify, reproduce and                *
* distribute the Sample Files (and/or any modified version) in               *
* any way you find useful, provided that you agree to                        *
* the Creative's Software Licensing Aggreement and you also agree that       *
* Creative has no warranty obligations or liability for any Sample Files.    *
*                                                                            *
******************************************************************************/
/*****************************************************************************\
*    File name   : WINAWE32.C                                                *
*                                                                            *
*    Programmer  : Cheng Kok Hoong                                           *
*          Creative Technology Ltd, 1994. All rights reserved.               *
*                                                                            *
*    Version     : 1.1.1                                                     *
*
*                                                                            *
*    WINAWE32 is a sample application that uses SB AWE32's AWEMAN.DLL        *
*                                                                            *
******************************************************************************/

#include "windows.h"
#include "mmsystem.h"
#include "winawe32.h"
#include "awe_dll.h"
#include "awe_api.h"
#include "mci.h"
#include "commdlg.h"
#include "string.h"

BOOL InitAWEDLL(void);
BOOL OpenAWEMAN(void);
BOOL CloseAWEMAN(void);
BOOL FetchMIDIFile(HWND);
BOOL FetchUserBankFile(HWND);
void SetReverb(WORD);
void SetChorus(WORD);

BOOL bIsMIDIPlaying	= FALSE;
BOOL bIsUserBankLoaded	= FALSE;

LPFNAWEMANAGER  lpAWEManager    = 0;

HANDLE          hInst;
HANDLE          hAWEMANDLL      = 0;
AWEHANDLE       hAWEHandle      = 0;

WORD		    wMCIDeviceID    = 0;

HWND            hWndDemo        = 0;

char		    szFileName[128];
char		    szBareName[12];
char		    szUserBankName[128];

HMIDIOUT	    hMIDIOut;

#define USER_BANK_NUM	    1

// Portable Macros for splitting WM_COMMAND

#ifdef WIN32
    #define GET_WM_COMMAND_CMD(wp, lp) HIWORD(wp)
    #define GET_WM_COMMAND_ID(wp, lp) LOWORD(wp)
    #define GET_WM_COMMAND_HWND(wp, lp) (HWND) (lp)
#else  // 16-bit version
    #define GET_WM_COMMAND_CMD(wp, lp) HIWORD(lp)
    #define GET_WM_COMMAND_ID(wp, lp) (wp)
    #define GET_WM_COMMAND_HWND(wp, lp) ((HWND) LOWORD(lp))
#endif

                 
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
                     
    
                     
{
    MSG 	msg;

    if (hPrevInstance)
        return FALSE;
    else
        if (!InitApplication(hInstance))
        return (FALSE);

    if (!InitInstance(hInstance, nCmdShow))
	return (FALSE);

    if (!InitAWEDLL() )
	return FALSE;



    if ( !OpenAWEMAN() ) {
	MessageBox(NULL, "Unable to open AWEMAN\0", "Error\0", MB_OK|MB_ICONHAND);
	return FALSE;
    }

    while (GetMessage(&msg, NULL, 0, 0) ) {
	  TranslateMessage(&msg);
	  DispatchMessage(&msg);
    }
    return (msg.wParam);
}


BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASS  wc;

    wc.style = 0;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, (LPCSTR)IDI_ICON1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "WinAWE32Menu";
    wc.lpszClassName = "WinAWE32Class";

    return (RegisterClass(&wc));

}


BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    HWND	    hWnd;
    RECT	    rectWindow;

    hInst = hInstance;

    hWnd = CreateWindow(
	"WinAWE32Class",
	"WinAWE32 Sample Demo",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	NULL,
	NULL,
	hInstance,
	NULL
    );

    if (!hWnd)
	return (FALSE);

    // Resize the Window
    #define WIN_WIDTH	400
    #define WIN_HEIGHT	100

    GetWindowRect(hWnd, &rectWindow);
    MoveWindow(hWnd, rectWindow.left, rectWindow.top, WIN_WIDTH, WIN_HEIGHT, TRUE);
    ShowWindow(hWnd, nCmdShow);
    hWndDemo = hWnd;

    return (TRUE);

}

long APIENTRY  MainWndProc(HWND     hWnd,     UINT     message,
                           WPARAM   wParam,   LPARAM   lParam)
{
    CParamObject cObject;
    CBufferObject cBufferObj;
    DWORD	 dwMaxDRAM, dwAvailableDRAM;
    char	 ach[128];
    HCURSOR	 hCursor;

    cObject.m_SBankIndex    = 0;
    cObject.m_UBankIndex    = 0;
    cObject.m_InstrIndex    = 0;
    cObject.m_TypeIndex     = REVERB_CHORUS;
    cObject.m_SubIndex	    = REVERB;

    switch (message)
    {
	case WM_COMMAND: {
        WORD Cmd = GET_WM_COMMAND_CMD (wParam, lParam) ;
        WORD ID  = GET_WM_COMMAND_ID  (wParam, lParam) ;
        HWND hWndCtrl =  GET_WM_COMMAND_HWND ( wparam, lParam) ;

        switch (ID) {
		case ID_CLEAR_BANK: {
		    if ( bIsUserBankLoaded == TRUE ) {
			if ( lpAWEManager(hAWEHandle, AWE_CLEAR_USER_BANK, USER_BANK_NUM, 0) != AWE_NO_ERR )
			    MessageBox(NULL, "Error clearing user bank\0", "Error\0", MB_OK|MB_ICONHAND);
			else {
			    MessageBox(NULL, "User Bank 1 unloaded\0", "Info\0", MB_OK);
			    bIsUserBankLoaded = FALSE;
			}
		    }
		}
		break;

		case ID_STOP_MIDI:
		    if ( bIsMIDIPlaying == TRUE ) {
			if ( MCIEndPlay(wMCIDeviceID) )
			    MessageBox(NULL, "Error stop playing MCISEQ.\0", "Error.\0", MB_OK|MB_ICONSTOP);
			if ( MCICloseDevice(wMCIDeviceID) )
			    MessageBox(NULL, "Error closing MCISEQ.\0", "Error.\0", MB_OK|MB_ICONSTOP);
			bIsMIDIPlaying = FALSE;
		    }
		    break;

		case ID_LOAD_USERBANK:

            if ( FetchUserBankFile(hWnd) ) {
            // Now load the bank into user bank 1
			cBufferObj.m_Size = 128;    // Size of string buffer
			cBufferObj.m_Flag = OPER_FILE;
			cBufferObj.m_Buffer = (LPSTR)szUserBankName;

			hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

			if ( lpAWEManager(hAWEHandle,
				  AWE_LOAD_USER_BANK,
				  (LPARAM)USER_BANK_NUM,
				  (LPARAM)(LPSTR)&cBufferObj) == AWE_NO_ERR ) {
			    MessageBox(NULL, "User Bank 1 load successful\0",
					 "Info\0",
					 MB_OK);
			    bIsUserBankLoaded = TRUE;
			}
			else {
			    MessageBox(NULL, "User Bank load failed.\0",
					 "Error\0",
					 MB_OK);
			}
			SetCursor(hCursor);
		    }

            break;

		case ID_SETTINGS_TRIGGERME:
		    DialogBox(hInst,	    /* current instance 	 */
			"TriggerMe",        /* resource to use           */
			hWnd,		    /* parent handle		 */
			TriggerMe);	    /* About() instance address  */

		break;

		case ID_FILE_OPEN:
		    if ( FetchMIDIFile(hWnd) ) {
			if ( bIsMIDIPlaying == TRUE ) {
			    MCIEndPlay(wMCIDeviceID);
			    MCICloseDevice(wMCIDeviceID);
			}
			if ( MCIOpenDevice(&wMCIDeviceID, szFileName) ) {
			    MessageBox(NULL, "Error in opening MCI.\0", "Error\0", MB_OK|MB_ICONSTOP);
			    break;
			}
			else {
			    MCIPlayDevice(hWnd, wMCIDeviceID);
			    bIsMIDIPlaying = TRUE;
			    // Update main windows's title to reflect the currently playing MIDI file
			    SetWindowText(hWnd, szBareName);
			}
		    }
		    break;

		case ID_QUIT:
		    PostMessage(hWnd, WM_DESTROY, 0, 0); break;

		case IDM_ABOUT:
		    DialogBox(hInst,	    /* current instance 	 */
			"AboutBox",         /* resource to use           */
			hWnd,		    /* parent handle		 */
			About); 	    /* About() instance address  */

		break;

		case ID_QUERY_SYNTH: {
		    CBufferObject   buffer;
		    char	    voila;
		    enum SBANK	    sBank;

		    buffer.m_Size   = sizeof(voila);
		    buffer.m_Buffer = (LPSTR)&voila;
		    lpAWEManager(hAWEHandle,
				 AWE_GET_SYN_BANK,
				 (LPARAM)(LPBUFFEROBJECT)&buffer,
				 0L);

		    sBank = (enum SBANK)buffer.m_Flag;

		    if ( sBank == GENERAL_MIDI )
			MessageBox(NULL, "Current synthesizer mode is General MIDI\0",
					 "Info\0",
					 MB_OK);
		    if ( sBank == ROLAND_GS )
			MessageBox(NULL, "Current synthesizer mode is GS\0",
					 "Info\0",
					 MB_OK);
		    if ( sBank == MT_32 )
			MessageBox(NULL, "Current synthesizer mode is MT-32\0",
					 "Info\0",
					 MB_OK);
		}
		break;

		case ID_QUERY_DRAM:
		    if ( lpAWEManager(hAWEHandle,
				      AWE_QUERY_DRAM_SIZE,
				      (LPARAM)(LPSTR)&dwMaxDRAM,
				      (LPARAM)(LPSTR)&dwAvailableDRAM) == AWE_NO_ERR ) {

            (void)wsprintf(ach, "Total DRAM = %lu bytes, Available DRAM = %lu bytes\0", dwMaxDRAM*2, dwAvailableDRAM*2);
			MessageBox(NULL, ach, "DRAM Info\0", MB_OK);
		    }
		break;


		case ID_MISC_SYNTHBANK_GM:
		    hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		    if ( lpAWEManager(hAWEHandle, AWE_SELECT_SYN_BANK, GENERAL_MIDI, 0) == AWE_NO_ERR )
				MessageBox(NULL, "Set synthesizer mode to General MIDI successful.\0", "Info\0", MB_OK);
		    else
				MessageBox(NULL, "Set GM failed.\0", "Error\0", MB_OK);
			    SetCursor(hCursor);
		    break;

		case ID_MISC_SYNTHBANK_GS:
		    hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		    if ( lpAWEManager(hAWEHandle, AWE_SELECT_SYN_BANK, ROLAND_GS, 0) == AWE_NO_ERR )
			MessageBox(NULL, "Set synthesizer mode to GS successful.\0", "Info\0", MB_OK);
		    else
			MessageBox(NULL, "Set GS failed.\0", "Error\0", MB_OK);
		    SetCursor(hCursor);
		    break;

		case ID_MISC_SYNTHBANK_MT32:
		    hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		    if ( lpAWEManager(hAWEHandle, AWE_SELECT_SYN_BANK, MT_32, 0) == AWE_NO_ERR )
				MessageBox(NULL, "Set synthesizer mode to MT-32 successful.\0", "Info\0", MB_OK);
		    else
				MessageBox(NULL, "Set MT-32 failed.\0", "Error\0", MB_OK);
			    SetCursor(hCursor);
		    break;

		case ID_SETTINGS_REVERB_ROOM1 :
		case ID_SETTINGS_REVERB_ROOM2 :
		case ID_SETTINGS_REVERB_ROOM3 :
		case ID_SETTINGS_REVERB_HALL1 :
		case ID_SETTINGS_REVERB_HALL2 :
		case ID_SETTINGS_REVERB_PLATE :
		case ID_SETTINGS_REVERB_DELAY :
		case ID_SETTINGS_REVERB_PANNINGDELAY :
        SetReverb(ID)  ;

            break;

		case ID_SETTINGS_CHORUS_CHORUS1 :
		case ID_SETTINGS_CHORUS_CHORUS2 :
		case ID_SETTINGS_CHORUS_CHORUS3 :
		case ID_SETTINGS_CHORUS_CHORUS4 :
		case ID_SETTINGS_CHORUS_FEEBACKCHORUS :
		case ID_SETTINGS_CHORUS_FLANGER :
		case ID_SETTINGS_CHORUS_SHORTDELAY :
		case ID_SETTINGS_CHORUS_SHORTDELAYFB :
            SetChorus(ID) ;
            break;
	    }
	    break;
	}


	case WM_DESTROY:
	    if ( bIsMIDIPlaying == TRUE ) {
		if ( MCIEndPlay(wMCIDeviceID) )
		    MessageBox(NULL, "Error stop playing MCISEQ.\0", "Error.\0", MB_OK|MB_ICONSTOP);
		if ( MCICloseDevice(wMCIDeviceID) )
		    MessageBox(NULL, "Error closing MCISEQ.\0", "Error.\0", MB_OK|MB_ICONSTOP);
	    }

	    if ( bIsUserBankLoaded == TRUE ) {
		if ( lpAWEManager(hAWEHandle, AWE_CLEAR_USER_BANK, USER_BANK_NUM, 0) != AWE_NO_ERR )
		    MessageBox(NULL, "Error clearing user bank\0", "Error\0", MB_OK|MB_ICONHAND);
	    }

	    if ( CloseAWEMAN() == FALSE )
		MessageBox(NULL, "Error closing AWEMAN\0", "Error", MB_OK|MB_ICONSTOP);

	    FreeLibrary(hAWEMANDLL);


	    PostQuitMessage(0);
	    break;

	default:
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}


 LRESULT CALLBACK About(HWND	hDlg,
			     UINT  message,
			     WPARAM	wParam,
			     LPARAM	lParam)
			     
{
    switch (message)
    {
	case WM_INITDIALOG:
	    return (TRUE);

	case WM_COMMAND:
	    if (wParam == IDOK
		|| wParam == IDCANCEL)
	    {
		EndDialog(hDlg, TRUE);
		return (TRUE);
	    }
	    break;
    }
    return (FALSE);
}


LRESULT CALLBACK TriggerMe(HWND	    hDlg,
				           UINT     message,
				           WPARAM	wParam,
				           LPARAM   lParam)
{

    #define BIG_ON	0x007F3C90
    #define BIG_OFF	0x00003C90
    #define PATCH_SET	0x00007FC0

    DWORD   dwErr;
    char    ach[128];
    DWORD   dwPresetNumber;
    static DWORD   dwMIDINumber = 0xF;
    BOOL    fTranslated;

    switch (message)
    {
	case WM_INITDIALOG:
	    // Set to preset 1 on startup, MIDI channel 16
	    SetDlgItemInt(hDlg, IDC_PC, 0x0, FALSE);
	    SetDlgItemInt(hDlg, IDC_MIDI, 0xF, FALSE);
	    return (TRUE);

	case WM_COMMAND:
	    switch ( wParam ) {
		case IDOK:
		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDC_BIGON:
		    dwErr = lpAWEManager(hAWEHandle, AWE_SEND_MIDI, BIG_ON+dwMIDINumber, 0);
		    if ( dwErr != AWE_NO_ERR ) {
			(void)wsprintf(ach, "Error code = %d\0", (WORD)dwErr);
			MessageBox(NULL, ach, "Error\0", MB_OK|MB_ICONSTOP);
		    }
		    return TRUE;

		case IDC_BIGOFF:
		    dwErr = lpAWEManager(hAWEHandle, AWE_SEND_MIDI, BIG_OFF+dwMIDINumber, 0);
		    if ( dwErr != AWE_NO_ERR ) {
			(void)wsprintf(ach, "Error code = %d\0", (WORD)dwErr);
			MessageBox(NULL, ach, "Error\0", MB_OK|MB_ICONSTOP);
		    }
		    return TRUE;

		case IDC_SETPATCH:
		    // Get the patch number
		    dwPresetNumber = (DWORD)GetDlgItemInt(hDlg, IDC_PC, (BOOL FAR*)&fTranslated, FALSE);
		    if ( dwPresetNumber > 0x7F )
			dwPresetNumber = 0x7F;
		    dwPresetNumber = (dwPresetNumber << 8) + dwMIDINumber + 0xC0;

		    dwErr = lpAWEManager(hAWEHandle, AWE_SEND_MIDI, dwPresetNumber, 0);
		    if ( dwErr != AWE_NO_ERR ) {
			(void)wsprintf(ach, "Error code = %d\0", (WORD)dwErr);
			MessageBox(NULL, ach, "Error\0", MB_OK|MB_ICONSTOP);
		    }
		    return TRUE;

		case IDC_SETMIDI:
		    // Get the MIDI channel
		    dwMIDINumber = (DWORD)GetDlgItemInt(hDlg, IDC_MIDI, (BOOL FAR*)&fTranslated, FALSE);
		    if ( dwMIDINumber > 0xF )
			dwMIDINumber = 0xF;
		    return TRUE;
	    }
	    break;
    }
    return (FALSE);
}

BOOL InitAWEDLL(void)
{
    
#ifdef WIN32
     hAWEMANDLL = LoadLibrary("AWEMAN32.DLL");
#else
   	hAWEMANDLL = LoadLibrary("AWEMAN.DLL") ;
#endif 
    	
    lpAWEManager = (LPFNAWEMANAGER)GetProcAddress(hAWEMANDLL, "AWEManager");
	if ( lpAWEManager == NULL)
	    return FALSE;
	else
		return TRUE;
}


BOOL OpenAWEMAN()
{
    if ( lpAWEManager((AWEHANDLE)0, AWE_OPEN, (LPARAM)(LPSTR)&hAWEHandle, 0) == AWE_NO_ERR )
        return TRUE;
    else
        return FALSE;
}


BOOL CloseAWEMAN()
{
    if ( lpAWEManager(hAWEHandle, AWE_CLOSE, 0, 0 ) == AWE_NO_ERR )
        return TRUE;
    else
        return FALSE;
}


BOOL FetchMIDIFile(HWND hWnd)
{

    char	    szMIDIExt[] = "MIDI\0*.MID; *.RMI\0";
    WORD	    wReturnVal = 0;
    DWORD	    dwErrorVal = 0;
    OPENFILENAME    ofnMIDI;
    // char	       szInitFile[128]

    strcpy(szFileName, szBareName);

    // szFileName[0] = 0;

    ofnMIDI.lStructSize 		= sizeof(OPENFILENAME);
    ofnMIDI.hwndOwner			= hWnd;
    ofnMIDI.hInstance			= hInst;
    ofnMIDI.lpstrFilter 		= szMIDIExt;
    ofnMIDI.lpstrCustomFilter		= NULL;
    ofnMIDI.nMaxCustFilter		= 0L;
    ofnMIDI.nFilterIndex		= 1L;
    ofnMIDI.lpstrFile			= (LPSTR)szFileName;
    ofnMIDI.nMaxFile			= 128;
    ofnMIDI.lpstrInitialDir		= NULL;
    ofnMIDI.lpstrTitle			= NULL;
    ofnMIDI.lpstrFileTitle		= (LPSTR)szBareName;
    ofnMIDI.lpstrDefExt 		= NULL;
    ofnMIDI.Flags			= 0;

    wReturnVal = GetOpenFileName((LPOPENFILENAME)&ofnMIDI);
    if ( wReturnVal > 0 )
	 return TRUE;
    else
	return FALSE;

}

BOOL FetchUserBankFile(HWND hWnd)
{

    char	    szSBKExt[] = "SoundFont Bank\0*.SF2 ; *.SBK\0";
    WORD	    wReturnVal = 0;
    DWORD	    dwErrorVal = 0;
    OPENFILENAME    ofnSBK;

    ofnSBK.lStructSize		       = sizeof(OPENFILENAME);
    ofnSBK.hwndOwner		       = hWnd;
    ofnSBK.hInstance		       = hInst;
    ofnSBK.lpstrFilter		       = szSBKExt;
    ofnSBK.lpstrCustomFilter	       = NULL;
    ofnSBK.nMaxCustFilter	       = 0L;
    ofnSBK.nFilterIndex 	       = 1L;
    ofnSBK.lpstrFile		       = (LPSTR)szUserBankName;
    ofnSBK.nMaxFile		       = 128;
    ofnSBK.lpstrInitialDir	       = NULL;
    ofnSBK.lpstrTitle		       = NULL;
    ofnSBK.lpstrFileTitle	       = NULL;
    ofnSBK.lpstrDefExt		       = NULL;
    ofnSBK.Flags		       = 0;

    wReturnVal = GetOpenFileName((LPOPENFILENAME)&ofnSBK);
    if ( wReturnVal > 0 )
	 return TRUE;
    else
	return FALSE;

}

void SetReverb(WORD wParam)
{

    CParamObject	 param;
    enum TYPEINDEX	 m_CurEfxType;
    enum VARIINDEX	 m_CurTypeVari[2];

    param.m_VariIndex[REVERB] = param.m_VariIndex[CHORUS] = 0;

    lpAWEManager(hAWEHandle, AWE_GET_EFX, (LPARAM)(LPSTR)&param, (LPARAM)0L);
    m_CurEfxType = (enum TYPEINDEX)param.m_TypeIndex;

    if ( m_CurEfxType == REVERB_CHORUS ) {
	// Correct !
	m_CurTypeVari[REVERB] = (enum VARIINDEX)param.m_VariIndex[REVERB];
	m_CurTypeVari[CHORUS] = (enum VARIINDEX)param.m_VariIndex[CHORUS];

	// Now issue selection on REVERB
	switch ( wParam ) {
	    case ID_SETTINGS_REVERB_ROOM1 :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)ROOM_1; break;

	    case ID_SETTINGS_REVERB_ROOM2 :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)ROOM_2; break;

	    case ID_SETTINGS_REVERB_ROOM3 :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)ROOM_3; break;

	    case ID_SETTINGS_REVERB_HALL1 :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)HALL_1; break;

	    case ID_SETTINGS_REVERB_HALL2 :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)HALL_2; break;

	    case ID_SETTINGS_REVERB_PLATE :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)PLATE; break;

	    case ID_SETTINGS_REVERB_DELAY :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)DELAY; break;

	    case ID_SETTINGS_REVERB_PANNINGDELAY :
		param.m_VariIndex[REVERB] = (enum VARIINDEX)PANNING_DELAY; break;
	}
	if ( lpAWEManager(hAWEHandle, AWE_SELECT_EFX, (LPARAM)(LPSTR)&param, (LPARAM)0L) != AWE_NO_ERR ) {
	    MessageBox(NULL, "Error in setting effect.\0",
			     "Error\0",
			     MB_OK|MB_ICONSTOP);
	}
	else {
	    MessageBox(NULL, "Effect setting successful.\0",
			     "OK !\0",
			     MB_OK|MB_ICONINFORMATION);
	}
    }
    else {
	MessageBox(NULL, "Current effect type is not Reverb/Chorus.\0",
			 "Error\0",
			 MB_OK|MB_ICONSTOP);
    }
}


void SetChorus(WORD wParam)
{
    CParamObject	 param;
    enum TYPEINDEX	 m_CurEfxType;
    enum VARIINDEX	 m_CurTypeVari[2];

    param.m_VariIndex[REVERB] = param.m_VariIndex[CHORUS] = 0;

    lpAWEManager(hAWEHandle, AWE_GET_EFX, (LPARAM)(LPSTR)&param, 0L);
    m_CurEfxType = (enum TYPEINDEX)param.m_TypeIndex;

    if ( m_CurEfxType == REVERB_CHORUS ) {
	// Correct !
	m_CurTypeVari[REVERB] = (enum VARIINDEX)param.m_VariIndex[REVERB];
	m_CurTypeVari[CHORUS] = (enum VARIINDEX)param.m_VariIndex[CHORUS];

	// Now issue selection on CHORUS
	switch ( wParam ) {
	    case ID_SETTINGS_CHORUS_CHORUS1 :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)CHORUS_1; break;

	    case ID_SETTINGS_CHORUS_CHORUS2 :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)CHORUS_2; break;

	    case ID_SETTINGS_CHORUS_CHORUS3 :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)CHORUS_3; break;

	    case ID_SETTINGS_CHORUS_CHORUS4 :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)CHORUS_4; break;

	    case ID_SETTINGS_CHORUS_FEEBACKCHORUS :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)FEEDBACK_DELAY; break;

	    case ID_SETTINGS_CHORUS_FLANGER :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)FLANGER; break;

	    case ID_SETTINGS_CHORUS_SHORTDELAY :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)SHORT_DELAY; break;

	    case ID_SETTINGS_CHORUS_SHORTDELAYFB :
		param.m_VariIndex[CHORUS] = (enum VARIINDEX)SHORT_DELAY_FB; break;

	}
	if ( lpAWEManager(hAWEHandle, AWE_SELECT_EFX, (LPARAM)(LPSTR)&param, 0L) != AWE_NO_ERR ) {
	    MessageBox(NULL, "Error in setting effect.\0",
			     "Error\0",
			     MB_OK|MB_ICONSTOP);
	}
	else {
	    MessageBox(NULL, "Effect setting successful.\0",
			     "OK !\0",
			     MB_OK|MB_ICONINFORMATION);
	}
    }
    else {
	MessageBox(NULL, "Current effect type is not Reverb/Chorus.\0",
			 "Error\0",
			 MB_OK|MB_ICONSTOP);
    }

}
