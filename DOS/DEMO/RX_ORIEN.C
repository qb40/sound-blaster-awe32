/*
 *  File:	rx_orient.c
 *  Created:	4/17/95
 *
 *  Description:
 *
 *  This file contains sample code to handle receiver orientation
 *  since the basic c3da core does not allow a change in receiver orientation.
 *  There are two routines defined in this example:
 *
 *	setEmitterPosition - a replacement for c3daSetEmitterPosition
 *	setReceiverOrientation - a new routine to handle receiver orientation
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND,  EITHER EXPRESSED OR IMPLIED,  INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1995 Creative Technology Ltd. All Rights Reserved.
 *
 */

#include <stdio.h>
#include <math.h>

#include "ctaweapi.h"

#define	NUMBER_OF_EMITTERS	4
#define	DEG2RAD			0.017453f;

/*
 * example utility structs
 */

typedef struct _Emitter {
    int			x;	/* world x position */
    int			y;	/* world y position */
    int                 z;      /* world z position */
    c3daEmitter		em;	/* handle to c3da core emitter 	*/
				/* need to be "created" using  	*/
				/* c3daCreateEmitter 		*/
} Emitter;

typedef struct _Receiver {
    float		a[9];	/* rotation matrix for yaw, pitch, roll */
				/* A = | a[0]  a[1]  a[2] |	*/
			 	/*     | a[3]  a[4]  a[5] |	*/
				/*     | a[6]  a[7]  a[8] |	*/
    c3daReceiver	rx;	/* handle to c3da core receiver */
				/* need to be "created" using	*/
				/* c3daCreateReceiver		*/
} Receiver;

/* 
 * as an example, allocate some global emitters and one receiver
 */

Receiver	thisRx;
Emitter		Em[NUMBER_OF_EMITTERS];

/*
 * sample "world" setEmitterPosition routine
 */

void
setEmitterPosition( Emitter* pEm, int x, int y, int z )
{

    int		new_x, new_y, new_z;

    /* 
     * save the world coordinates 
     */

    pEm->x = x;
    pEm->y = y;
    pEm->z = z;

    /*
     * rotate this emitter so that it is relative to the fixed receiver
     * in the c3da core
     */

    new_x = thisRx.a[0]*x + thisRx.a[1]*y + thisRx.a[2]*z;
    new_y = thisRx.a[3]*x + thisRx.a[4]*y + thisRx.a[5]*z;
    new_z = thisRx.a[6]*x + thisRx.a[7]*y + thisRx.a[8]*z;

    /*
     * now tell the c3da core about the new position
     */

    c3daSetEmitterPosition ( &(pEm->em), new_x, new_y, new_z );

}

/*
 * sample "world" setReceiverOrientation routine
 *
 * Arguments:
 *	yaw - rotation around the z-axis (-180 to 180 degrees)
 *	pitch - rotation around the y-axis (-180 to 180 degrees)
 *	roll - rotation around the x-axis (-180 to 180 degrees)
 *
 * Notes:
 *	o  order of rotations - roll, pitch, yaw
 *	o  Instead of actually changing the orientation of the receiver,
 *	   we equivalently move all the emitters around the fixed receiver 
 *	   in the c3da core.  The rotation matrix used to move the emitters
 *	   is simply the transpose of the "composite" rotation matrix defined
 *	   by yaw, pitch, roll and the order of rotations.
 */

void
setReceiverOrientation( int yaw, int pitch, int roll )
{

    float	yaw_f;
    float	pitch_f;
    float	roll_f;

    /*
     * convert yaw, pitch, and roll to radians
     */

    yaw_f = DEG2RAD * (float) yaw;
    pitch_f = DEG2RAD * (float) pitch;
    roll_f = DEG2RAD * (float) roll;

    /*
     * setup the inverse rotation matrix to handle yaw, pitch, and roll
     */

    thisRx.a[0] = cos(yaw_f)*cos(pitch_f);
    thisRx.a[1] = sin(yaw_f)*cos(pitch_f);
    thisRx.a[2] = -sin(pitch_f);
    thisRx.a[3] = -sin(yaw_f)*cos(roll_f) + cos(yaw_f)*sin(pitch_f)*sin(roll_f);
    thisRx.a[4] = cos(yaw_f)*cos(roll_f) + sin(yaw_f)*sin(pitch_f)*sin(roll_f);
    thisRx.a[5] = cos(pitch_f)*sin(roll_f);
    thisRx.a[6] = sin(yaw_f)*sin(roll_f) + cos(yaw_f)*sin(pitch_f)*cos(roll_f);
    thisRx.a[7] = -cos(yaw_f)*sin(roll_f) + sin(yaw_f)*sin(pitch_f)*cos(roll_f);
    thisRx.a[8] = cos(pitch_f)*cos(roll_f);

    /*
     * loop over the emitters letting them use the new orientation
     */

    for ( i = 0; i < NUMBER_OF_EMITTERS; i++ )
    	setEmitterPosition( &Em[i], Em[i].x, Em[i].y, Em[i].z );

}


