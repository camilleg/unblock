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
** costella_image_win32_dib.h: 
**   
**   Set up pixel structure for a Win32 DIB (Device Independent Bitmap), 
**   which uses COLORREF structures to store the pixels.
**
**   This version: May 20, 2007.
**
**   Relies on Win32.
*/


/* Include file only once.
*/

#ifndef _COSTELLA_IMAGE_WIN32_DIB_H_
#define _COSTELLA_IMAGE_WIN32_DIB_H_



/* Include files.
*/

#include <windows.h>



/* Typedefs.
*/

typedef COLORREF* COSTELLA_IMAGE_GRAY;
typedef COLORREF* COSTELLA_IMAGE_ALPHA;
typedef COLORREF* COSTELLA_IMAGE_COLOR;

typedef COLORREF* COSTELLA_IMAGE_GRAY_PIXEL;
typedef COLORREF* COSTELLA_IMAGE_ALPHA_PIXEL;
typedef COLORREF* COSTELLA_IMAGE_COLOR_PIXEL;



/* Macros.
*/

#define COSTELLA_IMAGE_GRAY_IS_SAME( lig1, lig2 ) \
  ( (lig1) == (lig2) )

#define COSTELLA_IMAGE_ALPHA_IS_SAME( lia1, lia2 ) \
  ( (lia1) == (lia2) )

#define COSTELLA_IMAGE_COLOR_IS_SAME( lic1, lic2 ) \
  ( (lic1) == (lic2) )


#define COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( ligpRight, ligpLeft ) \
  ( (ligpLeft) = (ligpRight) )

#define COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( liapRight, liapLeft ) \
  ( (liapLeft) = (liapRight) )

#define COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( licpRight, licpLeft ) \
  ( (licpLeft) = (licpRight) )


#define COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( ligp, lig, udWidth, \
    udHeight, sdRowStride ) \
  ( (ligp) = (lig) )

#define COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( liap, lia, udWidth, \
    udHeight, sdRowStride ) \
  ( (liap) = (lia) )

#define COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( licp, lic, udWidth, \
    udHeight, sdRowStride ) \
  ( (licp) = (lic) )


#define COSTELLA_IMAGE_GRAY_PIXEL_MOVE_LEFT( ligp ) \
  ( (ligp)-- )

#define COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_LEFT( liap ) \
  ( (liap)-- )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_LEFT( licp ) \
  ( (licp)-- )


#define COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( ligp ) \
  ( (ligp)++ )

#define COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( liap ) \
  ( (liap)++ )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( licp ) \
  ( (licp)++ )


#define COSTELLA_IMAGE_GRAY_PIXEL_MOVE_UP( ligp, lsdRowStride ) \
  ( (ligp) -= (lsdRowStride) )

#define COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_UP( liap, lsdRowStride ) \
  ( (liap) -= (lsdRowStride) )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_UP( licp, lsdRowStride ) \
  ( (licp) -= (lsdRowStride) )


#define COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( ligp, lsdRowStride ) \
  ( (ligp) += (lsdRowStride) )

#define COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( liap, lsdRowStride ) \
  ( (liap) += (lsdRowStride) )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( licp, lsdRowStride ) \
  ( (licp) += (lsdRowStride) )


#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_LEFT_TWO( licp ) \
  ( (licp) -= 2 )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( licp ) \
  ( (licp) += 2 )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_UP_TWO( licp, lsdDoubleRowStride ) \
  ( (licp) -= (lsdDoubleRowStride) )

#define COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( licp, lsdDoubleRowStride \
    ) \
  ( (licp) += (lsdDoubleRowStride) )


#define COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( ligp ) \
  ( (BYTE) *(ligp) )

#define COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( liap ) \
  ( (BYTE) ( *(liap) >> 24 ) )

#define COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( licp ) \
  ( (BYTE) ( *(licp) >> 16 ) )

#define COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( licp ) \
  ( (BYTE) ( *(licp) >> 8 ) )

#define COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( licp ) \
  ( (BYTE) *(licp) )


#define COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( ligp, lubY ) \
  ( *(ligp) &= 0xff000000, *(ligp) |= (COLORREF) (BYTE) (lubY) | \
  (COLORREF) (BYTE) (lubY) << 8 | (COLORREF) (BYTE) (lubY) << 16 ) 

#define COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( liap, lubA ) \
  ( *(liap) &= 0x000000ff, *(liap) |= (COLORREF) (BYTE) (lubA) << 24 )

#define COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( licp, lubRY ) \
  ( *(licp) &= 0xff00ffff, *(licp) |= (COLORREF) (BYTE) (lubRY) << 16 )

#define COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( licp, lubGCb ) \
  ( *(licp) &= 0xffff00ff, *(licp) |= (COLORREF) (BYTE) (lubGCb) << 8 )

#define COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( licp, lubBCr ) \
  ( *(licp) &= 0xffffff00, *(licp) |= (COLORREF) (BYTE) (lubBCr) )

#define COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( licp, lubRY, lubGCb, \
    lubBCr ) \
  ( *(licp) &= 0xff000000, *(licp) |= (COLORREF) (BYTE) (lubBCr) | \
  ( (COLORREF) (BYTE) (lubGCb) | (COLORREF) (BYTE) (lubRY) << 8 ) << 8 )



/* File is now included.
*/

#endif



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
