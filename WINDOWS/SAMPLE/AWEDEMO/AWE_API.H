/*****************************************************************************
*                                                                            *
* (C) Copyright Creative Technology Ltd. 1994-96. All rights reserved        *
* worldwide.                                                                 *
*                                                                            *
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY      *
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE        *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR      *
* PURPOSE.                                                                   *
*                                                                            *
* You have a royalty-free right to use, modify, reproduce and                *
* distribute the Sample Files (and/or any modified version) in               *
* any way you find useful, provided that you agree to                        *
* the Creative's Software Licensing Aggreement and you also agree that       *
* Creative has no warranty obligations or liability for any Sample Files     *
*                                                                            *
******************************************************************************/


/*
=============================================================================
 Name:          AWE_API.H

 Description:   API message function header file for accessing AWEMAN.DLL.
                This header file contains the possible messages to access
                the AWEMAN.DLL. The messages will be process by AWEMAN.DLL
                and respective operations will be performed.

 Purpose:       Header file meant for accessing APIs of AWEMAN.DLL.

 Author:        E.W. Khor
                Copyright 1993-96 Creative Technology Ltd.

 Date:          Version 1.0 - 27th Jan 1994

 Revision:      Version 1.1 - 29th March 1994
                    AWE_SEND_MIDI added.
                    New API for AWE_GET_UBANK_PARENT added.
                    New API for AWE_GET_DRUM_KIT added.
                    New API for AWE_ISHANDLE added.

                Version 1.40 - 25th January 1995
                    API extension for multi-card and new Control Panel
                    supports.

                       AWE_GET_NUM_DEVS,
                       AWE_GET_DEVICE_NODE,
                       AWE_GET_DEVICE_CAPS,
                       AWE_GET_SYNTH_PATH,
                       AWE_GET_UBANK_PATH,
                       AWE_SELECT_EFX_EX


                Version 1.44 - 24/3/96
                    Vienna API added.

=============================================================================
*/

#ifndef _AWE_API_H
#define _AWE_API_H

/*
--------------------------------
      Function Category
--------------------------------
*/
#define BASE_INIT                       100
#define BASE_CONFIG                     200
#define BASE_QUERY                      300
#define BASE_MIDI                       400
#define BASE_VIENNA                     2000       /* v1.44 */

/*
--------------------------------
      Function Messages
--------------------------------
*/
#define AWE_OPEN                        (BASE_INIT+1)
#define AWE_CLOSE                       (BASE_INIT+2)
#define AWE_ISHANDLE                    (BASE_INIT+3)       /* v1.10 */

#define AWE_GET_NUM_DEVS                (BASE_INIT+4)       /* v1.40 */
#define AWE_GET_DEVICE_NODE             (BASE_INIT+5)       /* v1.40 */
#define AWE_GET_DEVICE_CAPS             (BASE_INIT+6)       /* v1.40 */

#define AWE_REFRESH                     (BASE_CONFIG+1)
#define AWE_SELECT_EFX                  (BASE_CONFIG+2)
#define AWE_SELECT_SYN_BANK             (BASE_CONFIG+3)
#define AWE_LOAD_USER_BANK              (BASE_CONFIG+4)
#define AWE_CLEAR_USER_BANK             (BASE_CONFIG+5)
#define AWE_LOAD_USER_INSTR             (BASE_CONFIG+6)
#define AWE_CLEAR_USER_INSTR            (BASE_CONFIG+7)
#define AWE_SELECT_EFX_EX               (BASE_CONFIG+8)     /* v1.40 */
#define AWE_LOAD_WAVEFX                 (BASE_CONFIG+9)     /* v1.44 */
#define AWE_CLEAR_WAVEFX                (BASE_CONFIG+10)    /* v1.44 */

#define AWE_QUERY_EFXT_SUPPORT          (BASE_QUERY+1)
#define AWE_QUERY_EFXV_SUPPORT          (BASE_QUERY+2)
#define AWE_QUERY_SYN_SUPPORT           (BASE_QUERY+3)
#define AWE_GET_EFX                     (BASE_QUERY+4)
#define AWE_GET_SYN_BANK                (BASE_QUERY+5)
#define AWE_GET_USER_BANK               (BASE_QUERY+6)
#define AWE_GET_USER_INSTR              (BASE_QUERY+7)
#define AWE_QUERY_DRAM_SIZE             (BASE_QUERY+8)
#define AWE_GET_VERSION                 (BASE_QUERY+9)
#define AWE_GET_UBANK_PARENT            (BASE_QUERY+10)     /* v1.10 */
#define AWE_GET_DRUM_KIT                (BASE_QUERY+11)     /* v1.10 */
#define AWE_IS_DEVICE_FREE              (BASE_QUERY+12)     /* v1.40 */
#define AWE_GET_UBANK_PATH              (BASE_QUERY+13)     /* v1.40 */
#define AWE_GET_DRIVER_VERSION          (BASE_QUERY+14)     /* v1.40 */
#define AWE_GET_WAVEFX_PATH             (BASE_QUERY+15)     /* v1.44 */

#define AWE_SEND_MIDI                   (BASE_MIDI+1)       /* v1.10 */

#define AWE_VIENNA_START                (BASE_VIENNA+1)     /* v1.44 */
#define AWE_VIENNA_END                  (BASE_VIENNA+2)
#define AWE_VIENNA_LOAD_SAMPLE          (BASE_VIENNA+3)
#define AWE_VIENNA_FREE_SAMPLE          (BASE_VIENNA+4)
#define AWE_VIENNA_PLAY_SAMPLE          (BASE_VIENNA+5)
#define AWE_VIENNA_LOAD_PRESET          (BASE_VIENNA+6)
#define AWE_VIENNA_FREE_PRESET          (BASE_VIENNA+7)
#define AWE_VIENNA_NOTE_ON              (BASE_VIENNA+9)
#define AWE_VIENNA_NOTE_OFF             (BASE_VIENNA+10)
#define AWE_VIENNA_CONTROLLER           (BASE_VIENNA+11)
#define AWE_VIENNA_PITCHBEND            (BASE_VIENNA+12)
#define AWE_VIENNA_CHANNEL_PRESSURE     (BASE_VIENNA+13)
#define AWE_VIENNA_SYSEX                (BASE_VIENNA+14)
#define AWE_VIENNA_GET_DRAM_SIZE        (BASE_VIENNA+16)

#endif      /* _AWE_API_H */
