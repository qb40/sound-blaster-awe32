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
 Name:          ERRORMSG.H

 Description:   Error message header file for application interfacing with
                AWEMAN.DLL. This header file contains the possible error code
                return by the AWEMAN.DLL. The error code will be process by
                the application that accesses the AWEMAN.DLL via its message
                entry point function. This header must be included in the
                application for de-crypting the error code return by the DLL
                message function.

 Purpose:       Header file meant for any application that wishes to
                programme the APIs provided by AWEMAN.DLL.

 Author:        E.W. Khor
                Copyright 1993-95 Creative Technology Ltd.

 Date:          version 1.0 - 27th Jan 1994

 Revision:
                Version 1.2 - 12th July 1994
                Version 1.40 - 23rd April 1995
                Version 1.44 - 22nd May 1996

=============================================================================
*/
#ifndef  _ERRORMSG_H
#define     _ERRORMSG_H

/*
--------------------------------
      Error Category
--------------------------------
*/
#define BASE_INVALID                    100L
#define BASE_CONTENTION                 200L
#define BASE_EXIT                       800L
#define BASE_UNDEFINE                   900L

/*
--------------------------------
      No Error
--------------------------------
*/
#define AWE_NO_ERR                      0L
#define AWE_ERR_UNDEFINE_ERROR          (BASE_UNDEFINE+1)

/*
--------------------------------
      Error Messages
--------------------------------
*/
#define AWE_ERR_DEVICE_DRV_INVALID      (BASE_INVALID+1)
#define AWE_ERR_USERID_INVALID          (BASE_INVALID+2)
#define AWE_ERR_EFXT_INVALID            (BASE_INVALID+3)
#define AWE_ERR_EFXV_INVALID            (BASE_INVALID+4)
#define AWE_ERR_SBANK_INVALID           (BASE_INVALID+5)
#define AWE_ERR_UBANK_INVALID           (BASE_INVALID+6)
#define AWE_ERR_PATHNAME_INVALID        (BASE_INVALID+7)
#define AWE_ERR_USER_OBJ_INVALID        (BASE_INVALID+8)
#define AWE_ERR_INSTR_INVALID           (BASE_INVALID+9)
#define AWE_ERR_MSG_INVALID             (BASE_INVALID+10)
#define AWE_ERR_ACCESS_NOT_PERMITTED    (BASE_INVALID+11)
#define AWE_ERR_VERSION_INVALID         (BASE_INVALID+12)
#define AWE_ERR_DEVICE_INVALID          (BASE_INVALID+13)
#define AWE_ERR_SUPPORT_INVALID         (BASE_INVALID+14)

#define AWE_ERR_DLL_BUSY                (BASE_CONTENTION+1)
#define AWE_ERR_DEVICE_BUSY             (BASE_CONTENTION+2)
#define AWE_ERR_RESOURCE_INSUFFICIENT   (BASE_CONTENTION+3)
#define AWE_ERR_SYSMEM_INSUFFICIENT     (BASE_CONTENTION+4)
#define AWE_ERR_DRAM_INSUFFICIENT       (BASE_CONTENTION+5)
#define AWE_ERR_EFXT_CHANGE_NOT_ALLOWED (BASE_CONTENTION+6)

#define AWE_ERR_ACTIVATE_BOB            (BASE_EXIT+1)
#define AWE_ERR_ACTIVATE_CONTROLPANEL   (BASE_EXIT+2)
#define AWE_ERR_ACTIVATE_EFX_SETUP      (BASE_EXIT+3)

#define AWE_ERR_OUT_OF_MEM              (0x8003u)   /* AWE_ERR_SYSMEM_INSUFFICIENT */
#define AWE_ERR_INVALID_MESSAGE         (0x8006u)
#define AWE_ERR_INVALID_SAMPLE          (0x8009u)
#define AWE_ERR_OUT_OF_DRAM             (0x800au)
#define AWE_ERR_OUT_OF_GCHANNEL         (0x800bu)
#define AWE_ERR_INVALID_PRESET          (0x800cu)

#endif   /* _ERRORMSG_H */
