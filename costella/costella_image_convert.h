/* Copyright (c) 2005-2007 John P. Costella.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software")
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included 
** in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
** OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING
** FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
**
**
** costella_image_convert.h: 
**
**   Header file for costella_image_convert.c.
**
**   This version: September 13, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_IMAGE_CONVERT_H_
#define _COSTELLA_IMAGE_CONVERT_H_



/* Include files.
*/

#include "costella_image.h"
#include "costella_wrap.h"



/* Public interface.
*/

int costella_image_convert_initialize( FILE* pfileError );
int costella_image_convert_finalize( FILE* pfileError );

int costella_image_convert_rgb_to_ycbcr( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, int (*pfProgress)( void* pvPassback ), void* 
  pvPassback, FILE* pfileError );

int costella_image_convert_ycbcr_to_rgb( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, int (*pfProgress)( void* pvPassback ), void* 
  pvPassback, FILE* pfileError );
  


/* Function prototypes.
*/

COSTELLA_FUNCTION( CostellaImageConvertInitialize, ( void ) )
COSTELLA_FUNCTION( CostellaImageConvertFinalize, ( void ) )

COSTELLA_FUNCTION( CostellaImageConvertRgbToYcbcr, ( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
COSTELLA_FUNCTION( CostellaImageConvertYcbcrToRgb, ( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
  


/* COSTELLA_IMAGE_CONVERT_RGB_TO_Y: 
**
**   Convert RGB channel values to a Y channel value only. 
**
**   lub{R,G,B}:  Input {red,green,blue} channel value.
**
**   lpubY:  Pointer to COSTELLA_UB to contain Y output value.
*/

#define COSTELLA_IMAGE_CONVERT_RGB_TO_Y( lubR, lubG, lubB, lpubY ) \
{ \
  *(lpubY) = (COSTELLA_UB) COSTELLA_SHIFT_RIGHT_FLOOR( \
    gasdCostellaImageConvertRY[ (lubR) ] + gasdCostellaImageConvertGY[ \
    (lubG) ] + gasdCostellaImageConvertBY[ (lubB) ], 16 ); \
}



/* COSTELLA_IMAGE_CONVERT_RGB_TO_YCBCR: 
**
**   Convert RGB channel values to YCbCr channel values. 
**
**   lub{R,G,B}:  Input {red,green,blue} channel value.
**
**   lpub{Y,Cb,Cr}:  Pointer to COSTELLA_UB to contain {Y,Cb,Cr} output 
**     value.
*/

#define COSTELLA_IMAGE_CONVERT_RGB_TO_YCBCR( lubR, lubG, lubB, lpubY, \
  lpubCb, lpubCr ) \
{ \
  COSTELLA_IMAGE_CONVERT_RGB_TO_Y( lubR, lubG, lubB, lpubY ); \
  \
  *(lpubCb) = (COSTELLA_UB) COSTELLA_SHIFT_RIGHT_FLOOR( \
    gasdCostellaImageConvertRCb[ (lubR) ] + gasdCostellaImageConvertGCb[ \
    (lubG) ] + gasdCostellaImageConvertBCb[ (lubB) ], 16 ); \
  \
  *(lpubCr) = (COSTELLA_UB) COSTELLA_SHIFT_RIGHT_FLOOR( \
    gasdCostellaImageConvertRCr[ (lubR) ] + gasdCostellaImageConvertGCr[ \
    (lubG) ] + gasdCostellaImageConvertBCr[ (lubB) ], 16 ); \
}



/* COSTELLA_IMAGE_CONVERT_YCBCR_TO_RGB: 
**
**   Convert YCbCr channel values to RGB channel values. 
**
**   lub{Y,Cb,Cr}:  Input {Y,Cb,Cr} channel value.
**
**   lpub{R,G,B}:  Pointer to COSTELLA_UB to contain {R,G,B} output value.
*/

#define COSTELLA_IMAGE_CONVERT_YCBCR_TO_RGB( lubY, lubCb, lubCr, lpubR, \
  lpubG, lpubB ) \
{ \
  COSTELLA_SW lswR, lswG, lswB; \
  \
  lswR = (COSTELLA_SW) (lubY) + gaswCostellaImageConvertCrR[ (lubCr) ];\
  lswG = (COSTELLA_SW) (lubY) + (COSTELLA_SW) COSTELLA_SHIFT_RIGHT_FLOOR( \
    gasdCostellaImageConvertCbG[ (lubCb) ] + gasdCostellaImageConvertCrG[ \
    (lubCr) ], 16 ); \
  lswB = (COSTELLA_SW) (lubY) + gaswCostellaImageConvertCbB[ (lubCb) ]; \
  \
  *(lpubR) = (COSTELLA_UB) COSTELLA_IMAGE_LIMIT_RANGE( lswR ); \
  *(lpubG) = (COSTELLA_UB) COSTELLA_IMAGE_LIMIT_RANGE( lswG ); \
  *(lpubB) = (COSTELLA_UB) COSTELLA_IMAGE_LIMIT_RANGE( lswB ); \
}



/* Declaration of global variables defined in costella_image_convert.c.
*/

#ifndef _COSTELLA_IMAGE_CONVERT_C_

  extern COSTELLA_SW* gaswCostellaImageConvertCrR, 
    * gaswCostellaImageConvertCbB;
  extern COSTELLA_SD* gasdCostellaImageConvertRY, 
    * gasdCostellaImageConvertGY, * gasdCostellaImageConvertBY, 
    * gasdCostellaImageConvertRCb, * gasdCostellaImageConvertGCb, 
    * gasdCostellaImageConvertBCb, * gasdCostellaImageConvertRCr, 
    * gasdCostellaImageConvertGCr, * gasdCostellaImageConvertBCr, 
    * gasdCostellaImageConvertCbG, * gasdCostellaImageConvertCrG;

#endif



/* File is now included.
*/

#endif



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
