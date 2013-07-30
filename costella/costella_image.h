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
** costella_image.h: 
**
**   Header file for costella_image.c.
**
**   This version: November 12, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_IMAGE_H_
#define _COSTELLA_IMAGE_H_



/* Include files.
*/

#include "costella_base.h"



/* Include header file that sets up pixel storage structures and access
** routines for standard (8 bits per channel) images.
*/

#if defined( COSTELLA_IMAGE_SEPARATE_ARRAYS )

  #include "costella_image_separate_arrays.h" 

#elif defined( COSTELLA_IMAGE_RGB_TRIPLE )

  #include "costella_image_rgb_triple.h" 

#elif defined( COSTELLA_IMAGE_WIN32_DIB )

  #include "costella_image_win32_dib.h" 

#else

  #include "costella_image_default.h" 

#endif



/* Structure.
*/

typedef struct
{
  COSTELLA_B bAlpha, bColor, bRgb, bDownsampledChrominance,
    bNonreplicatedDownsampledChrominance;
  COSTELLA_UD udWidth, udHeight;
  COSTELLA_SD sdRowStride, sdAlphaRowStride;
  COSTELLA_IMAGE_ALPHA ia;
  COSTELLA_IMAGE_GRAY ig;
  COSTELLA_IMAGE_COLOR ic;
}
COSTELLA_IMAGE;



/* Public interface.
*/

int costella_image_initialize( FILE* pfileError );
int costella_image_finalize( FILE* pfileError );
  


/* Function prototypes.
*/

COSTELLA_FUNCTION( CostellaImageInitialize, ( void ) )
COSTELLA_FUNCTION( CostellaImageFinalize, ( void ) )
  


/* COSTELLA_IMAGE_LIMIT_RANGE: 
**
**   Limit the range of a signed integer to [0,255].
**
**   sx:  Signed integer to be limited.
*/

#define COSTELLA_IMAGE_LIMIT_RANGE( sx ) \
  ( (sx) < 0 ? 0 : ( (sx) > 255 ? 255 : (sx) ) )



/* Declaration of global variables defined in costella_eimage.c.
*/

#ifndef _COSTELLA_IMAGE_C_

  extern COSTELLA_IMAGE giCostellaImageNull;
  extern COSTELLA_IMAGE_GRAY_PIXEL gigpCostellaImageNull;
  extern COSTELLA_IMAGE_ALPHA_PIXEL giapCostellaImageNull;
  extern COSTELLA_IMAGE_COLOR_PIXEL gicpCostellaImageNull;

#endif



/* File is now included.
*/

#endif



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
