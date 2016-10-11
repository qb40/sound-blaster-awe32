/*****************************************************************************
*                                                                            *
* VISAMP : Vienna API sample code for Sound Blaster AWE32 family cards       *
*                                                                            *
* (C) Copyright Creative Technology Ltd. 1992-96. All rights reserved        *
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

#include "windows.h"
#include "mmsystem.h"
#include "visamp.h"
#include "awe_dll.h"
#include "awe_api.h"
#include "commdlg.h"
#include "string.h"

// NOTE : This is important for proper alignment
#pragma pack(2)

#include "sftype.h"
#include "sfenum.h"


///// G l o b a l   V a r i a b l e s  ////////////////////////////////////

HANDLE          hInst;
HANDLE          hAWEMANDLL   = 0;
AWEHANDLE       hAWEHandle   = 0;
HINSTANCE       g_hInstance ;
LPFNAWEMANAGER  lpAWEManager = 0;

char     szWaveFileName[128] ;
DWORD    dwDataSize;
DWORD    dwSampleHandle1 = 0xff,
         dwSampleHandle2 = 0xff;

CViPlayObject   PlayObj1 ,
                PlayObj2 ;

LPVISMPLOBJECT  pSmpl1 ,
                pSmpl2 ;

LRESULT  bLeftPlaying = FALSE , bRightPlaying = FALSE ,
         bSampleLoaded= FALSE , bPresetLoaded = FALSE ,
         bNoteOn      = FALSE , bStereo      = FALSE ;
HMMIO    hmmio ;

//  Handle to memory blocks. For Win3.1x only
#ifndef WIN32
 HANDLE   hSmpl1, hSmpl2, hBuffer ;
#endif

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

// This is the SoundFont2 data structure.
// For detail decription of each field. You're advised to refer to
// the official SoundFont2 Specification which is downloadable
// from Creative's FTP site : ftp.creaf.com
//                          Filename : sf2_00a.zip
//                          Location : /pub/emu
//

typedef struct _SF2STEREOFILE {
		DWORD dwFlags;
        DWORD dwPDTALIST;
        DWORD dwPDTASize;
        DWORD dwPDTA;
            DWORD dwPHDR;
            DWORD dwPHDRSize;
                sf2Preset       phdr[2];
            DWORD dwPBAG;
            DWORD dwPBagSize;
                sf2PresetBag    pbag[2];
            DWORD dwPMOD;
            DWORD dwPMODSize;
                sf2ModList      pmod[1];
            DWORD dwPGEN;
            DWORD dwPGENSize;
                sf2GenList      pgen[1];
            DWORD dwINST;
            DWORD dwINSTSize;
                sf2Inst         inst[2];
            DWORD dwIBAG;
            DWORD dwIBAGSize;
                sf2InstBag      ibag[3];
            DWORD dwIMOD;
            DWORD dwIMODSize;
                sf2InstModList  imod[1];
            DWORD dwIGEN;
            DWORD dwIGENSize;
                sf2InstGenList  igen[11];
            DWORD dwSHDR;
            DWORD dwSHDRSize;
                sf2Sample       shdr[3];
} SF2STEREOFILE , FAR* LPSF2STEREOFILE;


static SF2STEREOFILE SF2StereoFile = {
											0,
/*    DWORD dwPDTALIST;                 */  0x5453494c,
/*    DWORD dwPDTASize;                 */  4 + 9*8 +
                                            sizeof(sf2Preset)*2 +
                                            sizeof(sf2PresetBag)*2 +
                                            sizeof(sf2ModList) +
                                            sizeof(sf2GenList) +
                                            sizeof(sf2Inst)*2 +
                                            sizeof(sf2InstBag)*3 +
                                            sizeof(sf2InstModList) +
                                            sizeof(sf2InstGenList)*11 +
                                            sizeof(sf2Sample)*3,
/*    DWORD dwPDTA;                     */  0x61746470,
/*        DWORD dwPHDR;                 */  0x72646870,
/*        DWORD dwPHDRSize;             */  sizeof(sf2Preset)*2,
/*            sf2Preset       phdr[2]   */  "WaveFx 000",0,0,0,0,0,0,
                                            "EOP",255,255,1,0,0,0,
/*        DWORD dwPBAG;                 */  0x67616270,
/*        DWORD dwPBagSize;             */  sizeof(sf2PresetBag)*2,
/*            sf2PresetBag    pbag[2]   */  0, 0, 1, 0,
/*        DWORD dwPMOD;                 */  0x646f6d70,
/*        DWORD dwPMODSize;             */  sizeof(sf2ModList),
/*            sf2ModList      pmod[1]   */  {0},
/*        DWORD dwPGEN;                 */  0x6e656770,
/*        DWORD dwPGENSize;             */  sizeof(sf2GenList),
/*            sf2GenList      pgen[1]   */  instrument, 0,
/*        DWORD dwINST;                 */  0x74736e69,
/*        DWORD dwINSTSize;             */  sizeof(sf2Inst)*2,
/*            sf2Inst         inst[2]   */  "Instrument", 0,
                                            "EOI", 2,
/*        DWORD dwIBAG;                 */  0x67616269,
/*        DWORD dwIBAGSize;             */  sizeof(sf2InstBag)*3,
/*            sf2InstBag      ibag[3]   */  0, 0, 5, 0, 10, 0,
/*        DWORD dwIMOD;                 */  0x646f6d69,
/*        DWORD dwIMODSize;             */  sizeof(sf2InstModList),
/*            sf2InstModList  imod[1]   */  {0},
/*        DWORD dwIGEN;                 */  0x6e656769,
/*        DWORD dwIGENSize;             */  sizeof(sf2InstGenList)*11,
/*            sf2InstGenList  igen[11]  */  keyRange, 32512,
                                            sustainEnv2, 0,
                                            panEffectsSend, -500,
                                            instVol, 0,
                                            sampleId, 0,
                                            keyRange, 32512,
                                            sustainEnv2, 0,
                                            panEffectsSend, 500,
                                            instVol, 0,
                                            sampleId, 1,
                                            nop, 0,
/*        DWORD dwSHDR;                 */  0x72646873,
/*        DWORD dwSHDRSize;             */  sizeof(sf2Sample)*3,
/*            sf2Sample       shdr[3]   */  "Left", 0, 0, 0, 0, 44100, 60 , 0, 1, 4,
                                            "Right",0, 0, 0, 0, 44100, 60 , 0, 0, 2,
                                             {"EOS"}
};

// sf2sample.wSampleLink  : the sample header index of the associated right
//                          or left stereo sample respectively.
//                          In this case, "Left" sample is "linked" to the
//                          "Right" sample by setting the value 1
//                                  AND
//                          "Right" sample is "linked" back to the "Left"
//                           sample by setting the value to 0
//
// sf2Sample.wSampleType  : Mono sample = 1 ; right = 2 ; left = 4

// NOTE : For STEREO samples, the PanEffectSend field of sf2InstGenList
//        need to be set. A value of 500 and -500 will pan to extreme
//        right and extreme left respectively.


typedef struct _SF2MONOFILE {
		DWORD dwFlags;
        DWORD dwPDTALIST;
        DWORD dwPDTASize;
        DWORD dwPDTA;
            DWORD dwPHDR;
            DWORD dwPHDRSize;
                sf2Preset       phdr[2];
            DWORD dwPBAG;
            DWORD dwPBagSize;
                sf2PresetBag    pbag[2];
            DWORD dwPMOD;
            DWORD dwPMODSize;
                sf2ModList      pmod[1];
            DWORD dwPGEN;
            DWORD dwPGENSize;
                sf2GenList      pgen[1];
            DWORD dwINST;
            DWORD dwINSTSize;
                sf2Inst         inst[2];
            DWORD dwIBAG;
            DWORD dwIBAGSize;
                sf2InstBag      ibag[2];
            DWORD dwIMOD;
            DWORD dwIMODSize;
                sf2InstModList  imod[1];
            DWORD dwIGEN;
            DWORD dwIGENSize;
                sf2InstGenList  igen[6];
            DWORD dwSHDR;
            DWORD dwSHDRSize;
                sf2Sample       shdr[2];
} SF2MONOFILE , FAR* LPSF2MONOFILE;

static SF2MONOFILE SF2MonoFile = {
											0,
/*    DWORD dwPDTALIST;                 */  0x5453494c,
/*    DWORD dwPDTASize;                 */  4 + 9*8 +
                                            sizeof(sf2Preset)*2 +
                                            sizeof(sf2PresetBag)*2 +
                                            sizeof(sf2ModList) +
                                            sizeof(sf2GenList) +
                                            sizeof(sf2Inst)*2 +
                                            sizeof(sf2InstBag)*2 +
                                            sizeof(sf2InstModList) +
                                            sizeof(sf2InstGenList)*6 +
                                            sizeof(sf2Sample)*2,
/*    DWORD dwPDTA;                     */  0x61746470,
/*        DWORD dwPHDR;                 */  0x72646870,
/*        DWORD dwPHDRSize;             */  sizeof(sf2Preset)*2,
/*            sf2Preset       phdr[2]   */  "WaveFx 000",0,0,0,0,0,0,
                                            "EOP",0,255,1,0,0,0,
/*        DWORD dwPBAG;                 */  0x67616270,
/*        DWORD dwPBagSize;             */  sizeof(sf2PresetBag)*2,
/*            sf2PresetBag    pbag[2]   */  0, 0, 1, 0,
/*        DWORD dwPMOD;                 */  0x646f6d70,
/*        DWORD dwPMODSize;             */  sizeof(sf2ModList),
/*            sf2ModList      pmod[1]   */  {0},
/*        DWORD dwPGEN;                 */  0x6e656770,
/*        DWORD dwPGENSize;             */  sizeof(sf2GenList),
/*            sf2GenList      pgen[1]   */  instrument, 0,
/*        DWORD dwINST;                 */  0x74736e69,
/*        DWORD dwINSTSize;             */  sizeof(sf2Inst)*2,
/*            sf2Inst         inst[2]   */  "Instrument", 0,
                                            "EOI", 2,
/*        DWORD dwIBAG;                 */  0x67616269,
/*        DWORD dwIBAGSize;             */  sizeof(sf2InstBag)*2,
/*            sf2InstBag      ibag[2]   */  0, 0, 2, 0,
/*        DWORD dwIMOD;                 */  0x646f6d69,
/*        DWORD dwIMODSize;             */  sizeof(sf2InstModList),
/*            sf2InstModList  imod[1]   */  {0},
/*        DWORD dwIGEN;                 */  0x6e656769,
/*        DWORD dwIGENSize;             */  sizeof(sf2InstGenList)*6,
/*            sf2InstGenList  igen[6]   */  keyRange, 32512,
                                            sustainEnv2, 0,
                                            panEffectsSend, 0,
                                            instVol, 0,
                                            sampleId, 0,
                                            nop, 0,
/*        DWORD dwSHDR;                 */  0x72646873,
/*        DWORD dwSHDRSize;             */  sizeof(sf2Sample)*2,
/*            sf2Sample       shdr[2]   */  "Left", 0, 30000, 0, 4, 44100, 60 , 0 , 0, 1,
                                             {"EOS"}
};


/////   F u n c t i o n s   P r o t o t y p e   ///////////////////////////

LRESULT FetchWaveFile(HWND) ;
LRESULT LoadWaveSample(HWND) ;
LRESULT StreamMonoSample(PCMWAVEFORMAT* , HWND ) ;
LRESULT StreamStereoSample(PCMWAVEFORMAT* , HWND ) ;
LRESULT InitAWEDLL(void) ;
LRESULT GetWaveHeader(HWND ,LPPCMWAVEFORMAT, MMCKINFO* , MMCKINFO*)  ;


/////  W i n M a i n  /////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance,   HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,   int       nCmdShow)
{
    

    if (!hPrevInstance)
      {
         g_hInstance=hInstance ;
         DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainProc) ;
      }

     return (0) ;

}

///// M a i n P r o c  /////////////////////////////////////////////////////

LRESULT CALLBACK  MainProc(HWND     hWnd,        UINT     message,
                           WPARAM   wParam,      LPARAM   lParam)
{
  WORD Cmd, ID ;
  HWND hWndCtrl ;

  switch (message)
    {
    case WM_INITDIALOG:

      // Dynamically load AWE DLL and get the entry point.
      if (!InitAWEDLL() )
		{
		  EndDialog(hWnd, 0) ;
		  return FALSE;
	    }

	  
      // Vienna API only available from SBAWE32.DRV Version 1.44 above.

      #ifdef WIN32
      if( lpAWEManager(0L, AWE_VIENNA_START, (LPARAM) &hAWEHandle, 0) )
        {
        MessageBox(hWnd, "Error! Another SF2 Editor could be present OR \
			your AWE driver is not the latest!", "Error" , MB_OK | MB_ICONHAND) ;
        EndDialog(hWnd, 0) ;
        return FALSE ;
       }
     else
       return TRUE ;

     #else  // Win3.1

      if(lpAWEManager(0L, AWE_VIENNA_START, (LPARAM) (HANDLE FAR*)&hAWEHandle, 0) )
        {
        MessageBox(hWnd, "Error! Another SF2 Editor could be present OR\
        your AWE driver is not the latest!", "Error" , MB_OK | MB_ICONHAND) ;
		EndDialog(hWnd, 0) ;
        return FALSE ;
        }
      else
        return TRUE ;

    #endif

    case WM_CLOSE:
    case WM_DESTROY:
       if (bLeftPlaying)
          {
            PlayObj1.dwFlags = 0xffffffff ;
            lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE ,
                        (LPARAM) (LPVIPLAYOBJECT) &PlayObj1, 0) ;
           }
       if(bRightPlaying)
            {
             PlayObj2.dwFlags = 0xffffffff ;
             lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE ,
                         (LPARAM) (LPVIPLAYOBJECT) &PlayObj2, 0) ;
            }

       if (bNoteOn)
            lpAWEManager(hAWEHandle, AWE_VIENNA_NOTE_OFF, 0x7f003c, 0) ;

       lpAWEManager(hAWEHandle, AWE_VIENNA_END, 0, 0);
       EndDialog (hWnd,0) ;
       break ;

    case WM_COMMAND:

        Cmd = GET_WM_COMMAND_CMD (wParam, lParam) ;
        ID  = GET_WM_COMMAND_ID  (wParam, lParam) ;
        hWndCtrl =  GET_WM_COMMAND_HWND ( wparam, lParam) ;

        switch (ID)
    		{
            case IDC_LOAD_SAMPLE:
                // Make sure all playing samples are stopped.
                // if not, unwanted noise might occur when other samples
                if(bLeftPlaying)
                     {
                      PlayObj1.dwFlags = 0xffffffff ;
                      lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE ,
                                  (LPARAM) (LPVIPLAYOBJECT) &PlayObj1, 0) ;
                      bLeftPlaying = FALSE ;
                      }
                if(bRightPlaying)
                     {
                      PlayObj2.dwFlags = 0xffffffff ;
                      lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE ,
                                  (LPARAM) (LPVIPLAYOBJECT) &PlayObj2, 0) ;
                      bRightPlaying = FALSE ;
                      }

                // Free any previously loaded samples
                if (bSampleLoaded)
                   {
                   lpAWEManager(hAWEHandle,AWE_VIENNA_FREE_SAMPLE, dwSampleHandle1, 0) ;
                   if (bStereo)
                     lpAWEManager(hAWEHandle,AWE_VIENNA_FREE_SAMPLE, dwSampleHandle2, 0) ;
				   bSampleLoaded = FALSE ;
                   }

                // Free any previously loaded presets
                if (bPresetLoaded)
					{	
					lpAWEManager(hAWEHandle, AWE_VIENNA_FREE_PRESET, 0, 0) ;
					bPresetLoaded = FALSE ;
					}

                LoadWaveSample(hWnd) ;
                SetDlgItemText(hWnd, IDC_PLAY_LEFT_SAMPLE  , "Play Left Sample");
                SetDlgItemText(hWnd, IDC_PLAY_RIGHT_SAMPLE , "Play Right Sample");
            break ;

            ///////////////////////////////////////////////////////////////////

            case IDC_FREE_SAMPLE:
                // Free left sample
                if( lpAWEManager(hAWEHandle, AWE_VIENNA_FREE_SAMPLE, dwSampleHandle1, 0) )
                    MessageBox(hWnd, "Free Sample Error.",
                               "Error" , MB_OK | MB_ICONHAND) ;


                // Free the right sample too for stereo file
                if (bStereo)
                {
                if(lpAWEManager(hAWEHandle, AWE_VIENNA_FREE_SAMPLE, dwSampleHandle2, 0))
                     MessageBox(hWnd, "Free Sample Fail.",
                               "Error" , MB_OK | MB_ICONHAND) ;

                }
                bSampleLoaded = FALSE ;

            break;

            ///////////////////////////////////////////////////////////////////

            case IDC_PLAY_LEFT_SAMPLE:

                if (!bLeftPlaying)
                    {
                    PlayObj1.dwFlags = 0;       // if 1 then plays ROM samples, dwStart and dwEnd must be set
                    PlayObj1.dwSampleHandle = dwSampleHandle1;
                    PlayObj1.dwStart = 0 ;  // set to 0 for RAM sample
                    PlayObj1.dwEnd   = 0 ;  // set to 0 for RAM sample
                    PlayObj1.dwStartLoop = 0;   // if dwStartLoop == dwEndLoop == 0 then no looping
                    PlayObj1.dwEndLoop = 0;

                    if(lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE,  (LPARAM) (LPVIPLAYOBJECT) &PlayObj1,  0))
                        MessageBox(hWnd, "Play sample fail.",
                                    "Error" , MB_OK | MB_ICONHAND) ;
                    else
                       {
                        SetDlgItemText(hWnd, IDC_PLAY_LEFT_SAMPLE , "Stop Left Sample");
                        bLeftPlaying = TRUE ;
                        }
                    }
                else
                   {
                    PlayObj1.dwFlags = 0xffffffff;  // set to this value to stop playing

                    if(lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE, (LPARAM) (LPVIPLAYOBJECT) &PlayObj1, 0))
                        MessageBox(hWnd, "Stop sample fail.",
                                  "Error" , MB_OK | MB_ICONHAND) ;
                    else
                       {
                        SetDlgItemText(hWnd, IDC_PLAY_LEFT_SAMPLE , "Play Left Sample");
                        bLeftPlaying = FALSE ;
                       }
                    }
            break ;

            ///////////////////////////////////////////////////////////////////

            case IDC_PLAY_RIGHT_SAMPLE:
                if (!bRightPlaying)
                    {
                    PlayObj2.dwFlags = 0;   // if 1 then plays ROM samples, dwStart and dwEnd must be set
                    PlayObj2.dwSampleHandle = dwSampleHandle2;
                    PlayObj2.dwStart = 0 ;  // set to 0 for RAM sample
                    PlayObj2.dwEnd   = 0 ;  // set to 0 for RAM sample
                    PlayObj2.dwStartLoop = 0;   // if dwStartLoop == dwEndLoop == 0 then no looping
                    PlayObj2.dwEndLoop = 0;
                    if(lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE,
                            (LPARAM) (LPVIPLAYOBJECT) &PlayObj2, 0))
                        MessageBox(hWnd, "Play sample fail.",
                                  "Error" , MB_OK | MB_ICONHAND) ;
                    else
                        {
                        SetDlgItemText(hWnd, IDC_PLAY_RIGHT_SAMPLE , "Stop Right Sample");
                        bRightPlaying = TRUE ;
                        }
                    }
                else
                    {
                    PlayObj2.dwFlags = 0xffffffff;  // set to this value to stop playing
                    if(lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE,
                        (LPARAM) (LPVIPLAYOBJECT) &PlayObj2, 0) )
                        MessageBox(hWnd, "Stop sample fail.",
                                  "Error" , MB_OK | MB_ICONHAND) ;
                    else
                        {
                        SetDlgItemText(hWnd, IDC_PLAY_RIGHT_SAMPLE , "Play Right Sample");
                        bRightPlaying = FALSE ;
                        }
                    }
            break ;

            ///////////////////////////////////////////////////////////////////

            case IDC_LOAD_PRESET:

                if (bNoteOn)
                   {
                   lpAWEManager(hAWEHandle, AWE_VIENNA_NOTE_OFF, 0x7f003c, 0) ;
                   bNoteOn = FALSE ;
                   }

                // preset has to be built in memory first
                // the structure is the same as it appear in SF2 file
                // the dwFlag of preset struct must be set to 0
                // for RAM sample, set sf2Sample.dwStart = dwSampleHandle (the handle
                // to the loaded sample). So samples have to be loaded first.
                // sf2Sample.dwStartLoop, sf2Sample.dwEndLoop, and sf2Sample.dwEnd are
                // offset from 0.

                if (bStereo == TRUE)
                    {
                    SF2StereoFile.shdr[0].dwStart = dwSampleHandle1 ;
                    SF2StereoFile.shdr[1].dwStart = dwSampleHandle2 ;

                    SF2StereoFile.shdr[0].dwEnd = dwDataSize /2 ; // in terms of word
                    SF2StereoFile.shdr[1].dwEnd = dwDataSize /2 ;

                    SF2StereoFile.shdr[0].dwStartLoop = 0 ;
                    SF2StereoFile.shdr[1].dwStartLoop = 0 ;

                    SF2StereoFile.shdr[0].dwEndLoop   = 0 ;
                    SF2StereoFile.shdr[1].dwEndLoop   = 0 ;

                    SF2StereoFile.shdr[0].wSampleLink = 1 ;
                    SF2StereoFile.shdr[1].wSampleLink = 0 ;

                    SF2StereoFile.shdr[0].wSampleType = 4 ; // left sample
                    SF2StereoFile.shdr[1].wSampleType = 2 ; // right sample
                    if(lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_PRESET, (LPARAM) (LPSF2STEREOFILE) &SF2StereoFile, 0) )
                    MessageBox(hWnd, "Load SF2 preset fail.",
                              "Error" , MB_OK | MB_ICONHAND) ;

                    bPresetLoaded = TRUE ;
                    }
                else
                    {

                    SF2MonoFile.shdr[0].dwStart = dwSampleHandle1 ;
                    SF2MonoFile.shdr[0].dwEnd   = dwDataSize /2 ;

                    SF2MonoFile.shdr[0].dwStartLoop = 0 ;
                    SF2MonoFile.shdr[0].dwEndLoop   = 0 ;

                    SF2MonoFile.shdr[0].wSampleLink = 0 ;

                    SF2MonoFile.shdr[0].wSampleType = 1 ; // MONO sample
                    if(lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_PRESET,
                            (LPARAM) (LPSF2MONOFILE) &SF2MonoFile, 0) )
                      MessageBox(hWnd, "Load SF2 preset fail.",
                             "Error" , MB_OK | MB_ICONHAND) ;

                    bPresetLoaded = TRUE ;
                }

            break;

            ///////////////////////////////////////////////////////////////////

            case IDC_FREE_PRESET:
                if( lpAWEManager(hAWEHandle, AWE_VIENNA_FREE_PRESET, 0, 0) )
                    MessageBox(hWnd, "Free preset fail.",
                              "Error" , MB_OK | MB_ICONHAND) ;
                bPresetLoaded = FALSE ;

            break;


            ///////////////////////////////////////////////////////////////////

            case IDC_NOTE_ON:

                // Do a NOTE ON with value 0x3C ( Middle C / No pitch shift)
                // with velocity 0x7F

                if( lpAWEManager(hAWEHandle, AWE_VIENNA_NOTE_ON, 0x7f003c, 0) )
                    MessageBox(hWnd, "Note On fail.",
                              "Error" , MB_OK | MB_ICONHAND) ;

                bNoteOn = TRUE ;

            break;


            ///////////////////////////////////////////////////////////////////

            case IDC_NOTE_OFF:

                if(lpAWEManager(hAWEHandle, AWE_VIENNA_NOTE_OFF, 0x7f003c, 0) )
                    MessageBox(hWnd, "Note Off fail.",
                              "Error" , MB_OK | MB_ICONHAND) ;

                bNoteOn = FALSE ;

            break;

            ///////////////////////////////////////////////////////////////////

            case IDC_QUIT:
               if (bLeftPlaying)
                  {
                    PlayObj1.dwFlags = 0xffffffff ;
                    lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE ,
                        (LPARAM) (LPVIPLAYOBJECT) &PlayObj1, 0) ;
                   }
               if(bRightPlaying)
                   {
                     PlayObj2.dwFlags = 0xffffffff ;
                     lpAWEManager(hAWEHandle, AWE_VIENNA_PLAY_SAMPLE ,
                         (LPARAM) (LPVIPLAYOBJECT) &PlayObj2, 0) ;
                    }


                lpAWEManager(hAWEHandle, AWE_VIENNA_END, 0, 0);
                PostQuitMessage(0) ;

                break ;

            } // End Switch(message)

        return (TRUE) ;
    }
    return (FALSE);

}

LRESULT LoadWaveSample(HWND hWnd)
{
 MMCKINFO         mmckinfoParent   ,
                  mmckinfoSubchunk ;
 PCMWAVEFORMAT    PCMFormat        ;

 // Get the Wave filename
 if (!FetchWaveFile(hWnd))
   {
    MessageBox(hWnd, "Error Openning File!",
               NULL , MB_OK | MB_ICONEXCLAMATION) ;
    return (FALSE) ;
   }

 // Read the wave header info
 GetWaveHeader(hWnd , (LPPCMWAVEFORMAT) &PCMFormat , &mmckinfoParent, &mmckinfoSubchunk) ;

// Find the "data" chunk
 mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a') ;
 if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, MMIO_FINDCHUNK))
    {
     mmioClose(hmmio,0) ;
     return (FALSE) ;
    }

// Now get the size of data subchunk
   dwDataSize = mmckinfoSubchunk.cksize ;

// Check wave format. Only PCM, 44.1kHz, 16-bit, Mono/Stereo .WAV files
// are supported.
 if ( (PCMFormat.wf.wFormatTag     != WAVE_FORMAT_PCM) ||
       PCMFormat.wf.nSamplesPerSec != 44100            ||
       PCMFormat.wBitsPerSample    != 16     )

    {
     MessageBox(hWnd, "Wave Format not supported!",
                "Error" , MB_OK | MB_ICONEXCLAMATION);
     mmioClose(hmmio, 0) ;
     return (FALSE) ;
     }


 // process stereo/mono files differently
   if( PCMFormat.wf.nChannels == 2 )
       {
       bStereo = TRUE ;  // set the global flag
       EnableWindow ( GetDlgItem(hWnd,IDC_PLAY_RIGHT_SAMPLE) , TRUE) ;
       bSampleLoaded = StreamStereoSample(&PCMFormat , hWnd) ;
       }
   else
       {
       bStereo = FALSE ;
       EnableWindow ( GetDlgItem(hWnd,IDC_PLAY_RIGHT_SAMPLE) , FALSE) ;
       bSampleLoaded = StreamMonoSample( &PCMFormat , hWnd) ;
       }
   mmioClose(hmmio,0) ;

}

LRESULT StreamMonoSample(PCMWAVEFORMAT *pPCMFormat, HWND hWnd)
{

 LONG  wQuotient, wLoop , wStreamDataSize, wRemainder ;
 
 // Allocate  memory of Sample Object Structure
 // NOTE : Stream buffer should be less than 64k!
 //        Here, we arbitrarily choose to use 32k.

#ifdef WIN32
     pSmpl1   = (LPVISMPLOBJECT) malloc (32768) ;
     if (pSmpl1== NULL)
        {
         MessageBox(hWnd, "Could not allocate memory",
                    "Error", MB_ICONHAND | MB_OK) ;
         mmioClose(hmmio,0) ;
         return FALSE ;
        }
#else
    if(hSmpl1 = GlobalAlloc(GMEM_MOVEABLE, 32768) )
         pSmpl1 = (LPVISMPLOBJECT)GlobalLock(hSmpl1) ;
    else
        {
        MessageBox(hWnd, "Could not allocate memory",
                  "Error", MB_ICONHAND | MB_OK) ;
        mmioClose(hmmio,0) ;
        return FALSE ;
        }
#endif      

  
  pSmpl1->dwcbSize         = 32768 ; // size of the stream buffer
  pSmpl1->dwFlags          = 0     ;
  pSmpl1->dwSampleOffset   = 0     ; // must be set to 0 at first call
  pSmpl1->wFormatTag       = pPCMFormat->wf.wFormatTag     ;
  pSmpl1->dwSamplesPerSec  = pPCMFormat->wf.nSamplesPerSec ;
  pSmpl1->wBitsPerSample   = pPCMFormat->wBitsPerSample    ;
  pSmpl1->wChannels        = 1 ;
  pSmpl1->dwSampleSize     = dwDataSize         ;

  wStreamDataSize = 32768-sizeof(CViSmplObject) + 2 ;

  wQuotient  = (WORD) (dwDataSize/wStreamDataSize) ;
  wRemainder = (WORD) (dwDataSize%wStreamDataSize) ;

  // Now start to read the data from file and fill up the stream buffer
  for (wLoop=0 ; wLoop < wQuotient ; wLoop++)
    {
     // Now read the waveform data chunk into the stream buffer
     if (mmioRead(hmmio, (HPSTR) pSmpl1->iSample , wStreamDataSize ) != wStreamDataSize )
         {
          mmioClose(hmmio,0) ;
          MessageBox(hWnd, "Read wavefile data chunk error",
                "Error", MB_ICONHAND | MB_OK) ;
          return(FALSE) ;
          }

      if (lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_SAMPLE, (LPARAM) pSmpl1, 0) )
        {
        #ifdef WIN32
          free(pSmpl1) ;
        #else
          GlobalUnlock(hSmpl1) ;
          GlobalFree(hSmpl1) ;
        #endif
        return (FALSE) ;
        }

       //dwSampleHandle will be returned after the first call to stream sample
       //keep this handle for later use.

        if (wLoop==0)
           dwSampleHandle1 = pSmpl1->dwSampleHandle ;

    }  // End FOR loop

 // Read in the remaining samples
 if (mmioRead(hmmio, (HPSTR) pSmpl1->iSample , wRemainder) != wRemainder)
    {
      mmioClose(hmmio,0) ;
      #ifdef WIN32
          free(pSmpl1) ;
      #else
          GlobalUnlock(hSmpl1) ;
          GlobalFree(hSmpl1) ;
      #endif
      return FALSE ;
     }

 // Now stream the remaining samples
 if ( lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_SAMPLE, (LPARAM) pSmpl1, 0) )
    {
     mmioClose(hmmio,0) ;
     #ifdef WIN32
          free(pSmpl1) ;
     #else
          GlobalUnlock(hSmpl1) ;
          GlobalFree(hSmpl1) ;
     #endif
     return FALSE ;
    }


#ifdef WIN32
      free(pSmpl1) ;
#else
      GlobalUnlock(hSmpl1) ;
      GlobalFree(hSmpl1) ;
#endif     

return TRUE ;
}


LRESULT StreamStereoSample(PCMWAVEFORMAT *pPCMFormat , HWND hWnd)
{
  
  LONG   wQuotient, wRemainder, wLoop , wCount , wStreamDataSize ;
  

#ifdef WIN32  
  PWORD   pBuffer, pBufferOrg, pLeft, pRight ;
#else
  LPWORD  pBuffer, pBufferOrg, pLeft, pRight ;
#endif


 dwDataSize /= 2 ; // data split into left and right sample
 wStreamDataSize = 32768-sizeof(CViSmplObject) + 2 ;

// Allocate  memory of 2 Sample Objects Structure + 1 buffer
// Left right samples are treated as 2 seperate MONO samples
// NOTE : Stream buffer should be less than 64k!
//        Here, we arbitrarily choose to use 32k.

#ifdef WIN32  
    pSmpl1  = (LPVISMPLOBJECT) malloc (32768) ;
    pSmpl2  = (LPVISMPLOBJECT) malloc (32768) ;
    pBuffer = malloc(wStreamDataSize*2) ;

    if ( pSmpl1  == NULL ||
         pSmpl2  == NULL ||
         pBuffer == NULL  )
        {
         MessageBox(hWnd, "Could not allocate memory",
                    "Error", MB_ICONHAND | MB_OK) ;
         mmioClose(hmmio,0) ;
         return FALSE       ;
        }

#else  // Win3.1x
   hSmpl1 = GlobalAlloc(GMEM_MOVEABLE, 32768)  ;
   hSmpl2 = GlobalAlloc(GMEM_MOVEABLE, 32768)  ;
   hBuffer = GlobalAlloc(GMEM_MOVEABLE, wStreamDataSize*2) ;

   if ( hSmpl1  !=NULL &&
        hSmpl2  !=NULL &&
        hBuffer !=NULL   )
       {
       pSmpl1  = (LPVISMPLOBJECT) GlobalLock(hSmpl1) ;
       pSmpl2 =  (LPVISMPLOBJECT) GlobalLock(hSmpl2) ;
       pBuffer = (LPWORD)         GlobalLock(hBuffer);
       }
   else
       {
        MessageBox(hWnd, "Could not allocate memory",
                  "Error", MB_ICONHAND | MB_OK) ;
        mmioClose(hmmio,0) ;
        return FALSE ;
       }

#endif

 pSmpl1->dwcbSize       = pSmpl2->dwcbSize       = 32768 ; // size of the stream buffer
 pSmpl1->dwFlags        = pSmpl2->dwFlags        = 0     ;
 pSmpl1->dwSampleOffset = pSmpl2->dwSampleOffset = 0     ; // must set to 0 at first call
 pSmpl1->wFormatTag     = pSmpl2->wFormatTag     = pPCMFormat->wf.wFormatTag     ;
 pSmpl1->dwSamplesPerSec= pSmpl2->dwSamplesPerSec= pPCMFormat->wf.nSamplesPerSec ;
 pSmpl1->wBitsPerSample = pSmpl2->wBitsPerSample = pPCMFormat->wBitsPerSample    ;
 pSmpl1->wChannels      = pSmpl2->wChannels      = 1 ;
 pSmpl1->dwSampleSize   = pSmpl2->dwSampleSize   = dwDataSize  ;                                                


 pBufferOrg = pBuffer ;

 // wQuotient and wRemainder are in terms of Bytes
 // wDataSize and wStreamDataSize are in terms of Bytes
 wQuotient  = (WORD) (dwDataSize/wStreamDataSize) ;
 wRemainder = (WORD) (dwDataSize%wStreamDataSize) ;


 // Now start to read the data from file and fill up the stream buffer

  for (wLoop=0 ; wLoop < wQuotient ; wLoop++)
    {
     pBuffer = pBufferOrg ;

     // First read the waveform data chunk into a buffer
     if (mmioRead(hmmio, (HPSTR) pBuffer , wStreamDataSize*2 ) != wStreamDataSize*2 )
        {
         MessageBox(hWnd, "Read wavefile data chunk error",
                "Error", MB_ICONHAND | MB_OK) ;
         mmioClose(hmmio,0) ;
         return(FALSE) ;
         }  
         
    #ifdef WIN32
        pLeft  = (WORD*) pSmpl1->iSample ;
        pRight = (WORD*) pSmpl2->iSample ;
    #else
        pLeft  = (LPWORD) pSmpl1->iSample ;
        pRight = (LPWORD) pSmpl2->iSample ;
    #endif
     

    // Now split the data into into left and right
    // wStremDataSize /2 to make it in terms of WORDS

    for (wCount=0 ; wCount < wStreamDataSize/2 ; wCount++)
       {
        *pLeft++  =  *pBuffer++ ;
        *pRight++ =  *pBuffer++ ;
       }

    // Stream Left sample
    if( lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_SAMPLE, (LPARAM) pSmpl1, 0) )
       {
        MessageBox(hWnd, "Stream sample fail",
                 "Error", MB_ICONHAND | MB_OK) ;

        #ifdef WIN32
              free(pSmpl1) ;
        #else
              GlobalUnlock(hSmpl1) ;
              GlobalFree(hSmpl1) ;
        #endif
        
        return (FALSE) ;
       }

    //dwSampleHandle will be returned after the first call to stream sample
    //keep this handle for later use.

    if (wLoop==0)
          dwSampleHandle1 = pSmpl1->dwSampleHandle ;

    // Stream Right Sample
      if (lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_SAMPLE, (LPARAM) pSmpl2, 0) )
       {
        MessageBox(hWnd, "Stream sample fail",
                  "Error", MB_ICONHAND | MB_OK) ;

        #ifdef WIN32
              free(pSmpl2) ;
        #else
              GlobalUnlock(hSmpl2) ;
              GlobalFree(hSmpl2) ;
        #endif

        return (FALSE) ;
       }

       //dwSampleHandle will be returned after the first call to stream sample
       //keep this handle for later use.

       if (wLoop==0)
           dwSampleHandle2 = pSmpl2->dwSampleHandle ;

    }  // End FOR loop

  // Now stream the remaining samples, read wRemainder*2 Bytes

  pBuffer=pBufferOrg ;
  
  if(mmioRead(hmmio, (HPSTR) pBuffer , wRemainder*2) != wRemainder*2)
      {
       mmioClose(hmmio,0) ;
       MessageBox(hWnd , "Data reading error",
                  "error", MB_ICONHAND | MB_OK) ;
       return(FALSE) ;
      }

  #ifdef WIN32
      pLeft     =  (PWORD) pSmpl1->iSample ;
      pRight    =  (PWORD) pSmpl2->iSample ;
  #else
      pLeft      =  (LPWORD)pSmpl1->iSample ;
      pRight     =  (LPWORD)pSmpl2->iSample ;
  #endif
  

 // Now split the data into left and right
  wRemainder /=2 ; // in terms of number of WORDS
  for (wLoop=0 ; wLoop < wRemainder ; wLoop++) 
       {
        *pLeft++  =  *pBuffer++ ;
        *pRight++ =  *pBuffer++ ;
       }

 // Stream Left sample
 if(lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_SAMPLE, (LPARAM) pSmpl1, 0) )
    {

    #ifdef WIN32
          free(pSmpl1) ;
    #else
          GlobalUnlock(hSmpl1) ;
          GlobalFree(hSmpl1) ;
    #endif

    return (FALSE) ;
    }

 if (wQuotient == 0)
     dwSampleHandle1 = pSmpl1->dwSampleHandle ;

 // Stream Right Sample
 if ( lpAWEManager(hAWEHandle, AWE_VIENNA_LOAD_SAMPLE, (LPARAM) pSmpl2, 0) )
    {
     #ifdef WIN32
           free(pSmpl2) ;
     #else
       GlobalUnlock(hSmpl2) ;
       GlobalFree(hSmpl2) ;
     #endif
     return (FALSE) ;
    }

 if (wQuotient == 0)
     dwSampleHandle2 = pSmpl2->dwSampleHandle ;

#ifdef WIN32
      free(pSmpl1) ;
      free(pSmpl2) ;
      free(pBufferOrg) ;
#else
      GlobalUnlock(hSmpl1) ;
      GlobalFree(hSmpl1) ;
  
      GlobalUnlock(hSmpl2) ;
      GlobalFree(hSmpl2) ;
  
      GlobalUnlock(hBuffer) ;
      GlobalFree(hBuffer) ;
#endif    

return TRUE ;

}



LRESULT FetchWaveFile(HWND hWnd)
{

    char            szWAVExt[] = "Wave File\0*.WAV\0";
    WORD            wReturnVal = 0;
    DWORD           dwErrorVal = 0;
    OPENFILENAME    ofnWAV;

    ofnWAV.lStructSize             = sizeof(OPENFILENAME);
    ofnWAV.hwndOwner               = hWnd;
    ofnWAV.hInstance               = hInst;
    ofnWAV.lpstrFilter             = szWAVExt;
    ofnWAV.lpstrCustomFilter       = NULL;
    ofnWAV.nMaxCustFilter          = 0L;
    ofnWAV.nFilterIndex            = 1L;
    ofnWAV.lpstrFile               = (LPSTR)szWaveFileName;
    ofnWAV.nMaxFile                = 128;
    ofnWAV.lpstrInitialDir         = NULL;
    ofnWAV.lpstrTitle              = NULL;
    ofnWAV.lpstrFileTitle          = NULL;
    ofnWAV.lpstrDefExt             = NULL;
    ofnWAV.Flags               = 0;

    
	wReturnVal = GetOpenFileName((LPOPENFILENAME)&ofnWAV);
    if ( wReturnVal > 0 )
         return TRUE;
    else
        return FALSE;

}

LRESULT GetWaveHeader(HWND hWnd , LPPCMWAVEFORMAT lpPCMFormat,
                     MMCKINFO *mmckinfoParent, MMCKINFO *mmckinfoSubchunk )
{

   DWORD            dwFmtSize        ;
   LPPCMWAVEFORMAT  lpPCMfmt ;
#ifndef WIN32   
   HANDLE			hMem ;
#endif   
   
   // Open the .WAV file for reading using MMIO
   if (!(hmmio=mmioOpen(szWaveFileName, NULL , MMIO_READ | MMIO_ALLOCBUF)))
     {
        mmioClose(hmmio, 0) ;
        return (FALSE) ;
     }

   // Locate the "RIFF" chunk with "WAVE" form type
   mmckinfoParent->fccType = mmioFOURCC('W', 'A', 'V', 'E') ;
   if (mmioDescend(hmmio, mmckinfoParent , NULL ,MMIO_FINDRIFF))
       {
          MessageBox(hWnd, "This is not a WAVE file.",
                   NULL, MB_OK | MB_ICONEXCLAMATION);
            mmioClose(hmmio, 0) ;
           return (FALSE) ;
        }

   // Find "fmt" chunk

   mmckinfoSubchunk->ckid = mmioFOURCC('f', 'm', 't', ' ') ;
   if (mmioDescend(hmmio, mmckinfoSubchunk , mmckinfoParent ,MMIO_FINDCHUNK))
        {
         MessageBox(hWnd, "WAVE file is corrupted.",
                       NULL, MB_OK | MB_ICONEXCLAMATION);
           mmioClose(hmmio, 0) ;
          return (FALSE) ;
        }

   // Get the size of "fmt" chunk
   dwFmtSize  = mmckinfoSubchunk->cksize ;
   
#ifdef WIN32   
   lpPCMfmt = malloc(dwFmtSize) ;
#else
   if(hMem = GlobalAlloc (GMEM_MOVEABLE, dwFmtSize) )
   	{
   	
   	 lpPCMfmt=(LPPCMWAVEFORMAT) GlobalLock(hMem) ; 
   	 
   	}
   	else
   	 {
   	  MessageBox (hWnd, "Could not allocate memory",
   	  				"Error", MB_ICONHAND | MB_OK) ;

      mmioClose(hmmio,0) ;
      return FALSE ;

   	  }
#endif

   // Read "fmt" chunk
   if (mmioRead(hmmio, (HPSTR) lpPCMfmt , dwFmtSize)!=(LONG) dwFmtSize)
       {
          mmioClose(hmmio,0) ;
          return(FALSE) ;
       }

   // Ascend out of the "fmt" chunk
   mmioAscend( hmmio , mmckinfoSubchunk , 0) ;

   lpPCMFormat->wf.wFormatTag       = lpPCMfmt->wf.wFormatTag ;
   lpPCMFormat->wf.nChannels        = lpPCMfmt->wf.nChannels;
   lpPCMFormat->wf.nSamplesPerSec   = lpPCMfmt->wf.nSamplesPerSec;
   lpPCMFormat->wf.nAvgBytesPerSec  = lpPCMfmt->wf.nAvgBytesPerSec;
   lpPCMFormat->wf.nBlockAlign      = lpPCMfmt->wf.nBlockAlign;
   lpPCMFormat->wBitsPerSample      = lpPCMfmt->wBitsPerSample;


}


LRESULT InitAWEDLL(void)
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


