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
** costella_eimage_convert.c: 
**
**   Conversion of COSTELLA_EIMAGEs between the RGB and YCbCr colorspaces.
**
**   Library begun: December 30, 2005.
**   This version: November 12, 2007.
**
**   Written in ANSI C.
*/



/* Flag to ensure correct declaration of global variables defined below.
*/

#define _COSTELLA_IMAGE_CONVERT_C_



/* Include files.
*/

#include "costella_image_convert.h"



/* Externally visible global variables.
*/

COSTELLA_SW* gaswCostellaImageConvertCrR = 0, 
  * gaswCostellaImageConvertCbB = 0;
COSTELLA_SD* gasdCostellaImageConvertRY = 0, 
  * gasdCostellaImageConvertGY = 0, * gasdCostellaImageConvertBY = 0, 
  * gasdCostellaImageConvertRCb = 0, * gasdCostellaImageConvertGCb = 0, 
  * gasdCostellaImageConvertBCb = 0, * gasdCostellaImageConvertRCr = 0, 
  * gasdCostellaImageConvertGCr = 0, * gasdCostellaImageConvertBCr = 0, 
  * gasdCostellaImageConvertCbG = 0, * gasdCostellaImageConvertCrG = 0;



/* Internal global variables.
*/

static COSTELLA_B gbInitialized = COSTELLA_FALSE;



/* Start function bodies.
*/

#include "costella_body.h"



/* costella_image_convert_initialize:
**
**   Public interface for initializing this library. 
*/

COSTELLA_ANSI_FUNCTION( costella_image_convert_initialize, int, ( FILE* 
  pfileError ) )
{
  if( COSTELLA_CALL( CostellaImageConvertInitialize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_convert_finalize:
**
**   Public interface for finalizing this library. 
*/

COSTELLA_ANSI_FUNCTION( costella_image_convert_finalize, int, ( FILE* 
  pfileError ) )
{
  if( COSTELLA_CALL( CostellaImageConvertFinalize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_convert_rgb_to_ycbcr:
**
**   Public interface for converting an RGB COSTELLA_IMAGE to YCbCr.
**
**   pi{In,Out}:  Pointer to the {input,output} COSTELLA_IMAGE.
*/

COSTELLA_ANSI_FUNCTION( costella_image_convert_rgb_to_ycbcr, int, (
  COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* piOut, int (*pfProgress)( void* 
  pvPassback ), void* pvPassback, FILE* pfileError ) )
{
  COSTELLA_WRAP_PROGRESS wp;

  wp.pfProgress = pfProgress;
  wp.pvPassback = pvPassback;

  if( COSTELLA_CALL( CostellaImageConvertRgbToYcbcr( piIn, piOut, 
    CostellaWrapProgress, &wp ) ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  }
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_convert_ycbcr_to_rgb:
**
**   Public interface for converting a YCbCr COSTELLA_IMAGE to RGB.
**
**   pi{In,Out}:  Pointer to the {input,output} COSTELLA_IMAGE.
*/

COSTELLA_ANSI_FUNCTION( costella_image_convert_ycbcr_to_rgb, int, (
  COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* piOut, int (*pfProgress)( void* 
  pvPassback ), void* pvPassback, FILE* pfileError ) )
{
  COSTELLA_WRAP_PROGRESS wp;

  wp.pfProgress = pfProgress;
  wp.pvPassback = pvPassback;

  if( COSTELLA_CALL( CostellaImageConvertYcbcrToRgb( piIn, piOut, 
    CostellaWrapProgress, &wp ) ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  }
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* CostellaImageConvertInitialize: 
**
**   Initialize the library. 
*/

COSTELLA_FUNCTION( CostellaImageConvertInitialize, ( void ) )
{
  COSTELLA_UW uw;


  /* Do nothing if already initialized.
  */

  if( gbInitialized )
  {
    COSTELLA_RETURN;
  }


  /* Set initialized flag.
  */

  gbInitialized = COSTELLA_TRUE;


  /* Initialize other libraries.
  */

  if( COSTELLA_CALL( CostellaImageInitialize() ) || COSTELLA_CALL( 
    CostellaWrapInitialize() ) )
  {
    COSTELLA_ERROR( "Initialization" );
    gbInitialized = COSTELLA_FALSE;
    COSTELLA_RETURN;
  }


  /* Allocate memory for the lookup tables.
  */

  if( COSTELLA_MALLOC( gasdCostellaImageConvertRY, 256 ) || COSTELLA_MALLOC(
    gasdCostellaImageConvertGY, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertBY, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertRCb, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertGCb, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertBCb, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertGCr, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertBCr, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertCbG, 256 ) || COSTELLA_MALLOC( 
    gasdCostellaImageConvertCrG, 256 ) || COSTELLA_MALLOC( 
    gaswCostellaImageConvertCrR, 256 ) || COSTELLA_MALLOC( 
    gaswCostellaImageConvertCbB, 256 ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Allocating" );

    if( COSTELLA_FREE( gasdCostellaImageConvertRY ) || COSTELLA_FREE( 
      gasdCostellaImageConvertGY ) || COSTELLA_FREE( 
      gasdCostellaImageConvertBY ) || COSTELLA_FREE( 
      gasdCostellaImageConvertRCb ) || COSTELLA_FREE( 
      gasdCostellaImageConvertGCb ) || COSTELLA_FREE( 
      gasdCostellaImageConvertBCb ) || COSTELLA_FREE( 
      gasdCostellaImageConvertGCr ) || COSTELLA_FREE( 
      gasdCostellaImageConvertBCr ) || COSTELLA_FREE( 
      gasdCostellaImageConvertCbG ) || COSTELLA_FREE( 
      gasdCostellaImageConvertCrG ) || COSTELLA_FREE( 
      gaswCostellaImageConvertCrR ) || COSTELLA_FREE( 
      gaswCostellaImageConvertCbB ) )
    {
      COSTELLA_CLEANUP_FUNDAMENTAL_ERROR( "Freeing" );
    }

    COSTELLA_RETURN;
  }


  /* Two of the tables are actually equal, so we did not allocate both. 
  ** Simply set the pointer to be equal, for clarity.
  */

  gasdCostellaImageConvertRCr = gasdCostellaImageConvertBCb;


  /* Create the RGB to YCbCr conversion tables. 
  **
  */

  for( uw = 0; uw < 256; uw++ )
  {
    gasdCostellaImageConvertRY[ uw ] = 19595L * (COSTELLA_SD) uw + 32768L;
    gasdCostellaImageConvertGY[ uw ] = 38470L * (COSTELLA_SD) uw;
    gasdCostellaImageConvertBY[ uw ] = 7471L * (COSTELLA_SD) uw;

    gasdCostellaImageConvertRCb[ uw ] = -11058L * (COSTELLA_SD) uw;
    gasdCostellaImageConvertGCb[ uw ] = -21709L * (COSTELLA_SD) uw;
    gasdCostellaImageConvertBCb[ uw ] = 32767L * (COSTELLA_SD) uw + 
      8421376L;

    gasdCostellaImageConvertGCr[ uw ] = -27438L * (COSTELLA_SD) uw;
    gasdCostellaImageConvertBCr[ uw ] = -5329L * (COSTELLA_SD) uw;
  }


  /* Create the YCbCr to RGB conversion tables. 
  */

  for( uw = 0; uw < 256; uw++ )
  {
    gasdCostellaImageConvertCbG[ uw ] = 8910464L - 22554L * (COSTELLA_SD) 
      uw;
    gasdCostellaImageConvertCrG[ uw ] = -46803L * (COSTELLA_SD) uw;
    

    gaswCostellaImageConvertCrR[ uw ] = (COSTELLA_SW) 
      COSTELLA_SHIFT_RIGHT_FLOOR( 91885L * (COSTELLA_SD) uw - 11728512L, 16
      );
    gaswCostellaImageConvertCbB[ uw ] = (COSTELLA_SW) 
      COSTELLA_SHIFT_RIGHT_FLOOR( 116134L * (COSTELLA_SD) uw - 14832384L, 16
      );
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageConvertFinalize: 
**
**   Finalize the library. 
*/

COSTELLA_FUNCTION( CostellaImageConvertFinalize, ( void ) )
{
  /* Do nothing if not initialized.
  */

  if( !gbInitialized )
  {
    COSTELLA_RETURN;
  }


  /* Clear initialized flag.
  */

  gbInitialized = COSTELLA_FALSE;


  /* Free the memory allocated in initialization.
  */

  if( COSTELLA_FREE( gasdCostellaImageConvertRY ) || COSTELLA_FREE( 
    gasdCostellaImageConvertGY ) || COSTELLA_FREE( 
    gasdCostellaImageConvertBY ) || COSTELLA_FREE( 
    gasdCostellaImageConvertRCb ) || COSTELLA_FREE( 
    gasdCostellaImageConvertGCb ) || COSTELLA_FREE( 
    gasdCostellaImageConvertBCb ) || COSTELLA_FREE( 
    gasdCostellaImageConvertGCr ) || COSTELLA_FREE( 
    gasdCostellaImageConvertBCr ) || COSTELLA_FREE( 
    gasdCostellaImageConvertCbG ) || COSTELLA_FREE( 
    gasdCostellaImageConvertCrG ) || COSTELLA_FREE( 
    gaswCostellaImageConvertCrR ) || COSTELLA_FREE( 
    gaswCostellaImageConvertCbB ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Freeing" );
    COSTELLA_RETURN;
  }


  /* Zero pointer that was set to pointer to allocated array that has now
  ** been freed.
  */

  gasdCostellaImageConvertRCr  = 0;


  /* Finalize other libraries.
  */ 

  if( COSTELLA_CALL( CostellaWrapFinalize() ) || COSTELLA_CALL( 
    CostellaImageFinalize() ) )
  {
    COSTELLA_ERROR( "Finalizing" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageConvertRgbToYcbcr: 
**
**   Convert a color image from the RGB colorspace to the YCbCr colorspace. 
**
**   pi{In,Out}:  Pointer to the {input,output} image. Can be the same.
*/

COSTELLA_FUNCTION( CostellaImageConvertRgbToYcbcr, ( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
{
#ifdef COSTELLA_DEBUG
  COSTELLA_B bAlpha;
#endif
  COSTELLA_B bCopyAlpha;
  COSTELLA_UB ubR, ubG, ubB, ubY, ubCb, ubCr, ubA;
  COSTELLA_UD udWidth, udHeight, udColumn, udRow;
  COSTELLA_SD sdRowStrideIn, sdRowStrideOut, sdAlphaRowStrideIn, 
    sdAlphaRowStrideOut;
  COSTELLA_IMAGE_ALPHA* piaIn, * piaOut;
  COSTELLA_IMAGE_COLOR* picIn, * picOut;
  COSTELLA_IMAGE_ALPHA_PIXEL iapIn, iapInStart, iapOut, iapOutStart;
  COSTELLA_IMAGE_COLOR_PIXEL icpIn, icpInStart, icpOut, icpOutStart;


  /* Keep GCC happy.
  */

  iapIn = iapInStart = iapOut = iapOutStart = giapCostellaImageNull;
  icpIn = icpInStart = icpOut = icpOutStart = gicpCostellaImageNull;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !piIn )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null piIn" );
      COSTELLA_RETURN;
    }

    if( !piOut )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null piOut" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that both images are color.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piIn->bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image not color" );
      COSTELLA_RETURN;
    }

    if( !piOut->bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Output image not color" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that the input image is in the RGB colorspace.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piIn->bRgb )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image not RGB" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Set the output image to the YCbCr colorspace.
  */

  piOut->bRgb = COSTELLA_FALSE;


  /* Copy the downsampled chrominance and nonreplicated downsampled
  ** chrominance flags.
  */

  piOut->bDownsampledChrominance = piIn->bDownsampledChrominance;
  piOut->bNonreplicatedDownsampledChrominance = 
    piIn->bNonreplicatedDownsampledChrominance;


  /* Extract data.
  */

#ifdef COSTELLA_DEBUG
  bAlpha = piIn->bAlpha;
#endif

  udWidth = piIn->udWidth;
  udHeight = piIn->udHeight;

  sdRowStrideIn = piIn->sdRowStride;
  sdAlphaRowStrideIn = piIn->sdAlphaRowStride;

  sdRowStrideOut = piOut->sdRowStride;
  sdAlphaRowStrideOut = piOut->sdAlphaRowStride;

  piaIn = &piIn->ia;
  piaOut = &piOut->ia;

  picIn = &piIn->ic;
  picOut = &piOut->ic;


  /* Check that the output image agrees with the input image.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piOut->bAlpha != !bAlpha )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Alpha flags disagree" );
      COSTELLA_RETURN;
    }

    if( piOut->udWidth != udWidth || piOut->udHeight != udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Dimensions do not agree" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check whether we need to copy the alpha channel image data.
  */

  bCopyAlpha = !COSTELLA_IMAGE_ALPHA_IS_SAME( *piaIn, *piaOut );


  /* Start at the top-left corner of images.
  */

  COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpInStart, *picIn, udWidth, 
    udHeight, sdRowStrideIn );
  COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpOutStart, *picOut, udWidth, 
    udHeight, sdRowStrideOut );

  if( bCopyAlpha )
  {
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapInStart, *piaIn, udWidth, 
      udHeight, sdAlphaRowStrideIn );
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapOutStart, *piaOut, udWidth, 
      udHeight, sdAlphaRowStrideOut );
  }


  /* Walk through the rows. 
  */

  for( udRow = 0; udRow < udHeight; udRow++ )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" )
      COSTELLA_RETURN;
    }


    /* Set pixels to the start of this row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );
    
    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapInStart, iapIn );
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapOutStart, iapOut );
    }


    /* Walk through the columns. 
    */

    for( udColumn = 0; udColumn < udWidth; udColumn++ )
    {
      /* Extract the R, G, and B components of the input image.
      */

      ubR = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
      ubG = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
      ubB = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );


      /* Compute Y, Cb, and Cr.
      */

      COSTELLA_IMAGE_CONVERT_RGB_TO_YCBCR( ubR, ubG, ubB, &ubY, &ubCb, 
        &ubCr );

      
      /* Store them.
      */
    
      COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubY, ubCb, ubCr );


      /* Copy the alpha pixel, if we need to.
      */

      if( bCopyAlpha )
      {
        ubA = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubA );
      }


      /* Move to the next column.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

      if( bCopyAlpha )
      {
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
      }
    }


    /* Walk down to the next row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpInStart, sdRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOutStart, sdRowStrideOut );

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapInStart, sdAlphaRowStrideIn 
        );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOutStart, sdAlphaRowStrideOut
        );
    }
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageConvertYcbcrToRgb: 
**
**   Convert a color image from the YCbCr colorspace to the RGB colorspace. 
**
**   pi{In,Out}:  Pointer to the {input,output} image. Can be the same.
*/

COSTELLA_FUNCTION( CostellaImageConvertYcbcrToRgb, ( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
{
#ifdef COSTELLA_DEBUG
  COSTELLA_B bAlpha;
#endif
  COSTELLA_B bCopyAlpha, bDownsampledChrominance, 
    bNonreplicatedDownsampledChrominance, bTop, bLeft;
  COSTELLA_UB ubR, ubG, ubB, ubY, ubCb, ubCr, ubA;
  COSTELLA_UD udWidth, udHeight, udColumn, udRow;
  COSTELLA_SD sdRowStrideIn, sdRowStrideOut, sdAlphaRowStrideIn, 
    sdAlphaRowStrideOut;
  COSTELLA_IMAGE_ALPHA* piaIn, * piaOut;
  COSTELLA_IMAGE_COLOR* picIn, * picOut;
  COSTELLA_IMAGE_ALPHA_PIXEL iapIn, iapInStart, iapOut, iapOutStart;
  COSTELLA_IMAGE_COLOR_PIXEL icpIn, icpInStart, icpOut, icpOutStart;


  /* Keep GCC happy.
  */

  iapIn = iapInStart = iapOut = iapOutStart = giapCostellaImageNull;
  icpIn = icpInStart = icpOut = icpOutStart = gicpCostellaImageNull;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !piIn )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null piIn" );
      COSTELLA_RETURN;
    }
    if( !piOut )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null piOut" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that both images are color.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piIn->bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image not color" );
      COSTELLA_RETURN;
    }

    if( !piOut->bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Output image not color" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that the input image is in the YCbCr colorspace.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( piIn->bRgb )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image not YCbCr" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Set the output image to the RGB colorspace.
  */

  piOut->bRgb = COSTELLA_TRUE;


  /* Extract data.
  */

#ifdef COSTELLA_DEBUG
  bAlpha = piIn->bAlpha;
#endif
  bDownsampledChrominance = piIn->bDownsampledChrominance;
  bNonreplicatedDownsampledChrominance = 
    piIn->bNonreplicatedDownsampledChrominance;

  udWidth = piIn->udWidth;
  udHeight = piIn->udHeight;

  sdRowStrideIn = piIn->sdRowStride;
  sdAlphaRowStrideIn = piIn->sdAlphaRowStride;

  sdRowStrideOut = piOut->sdRowStride;
  sdAlphaRowStrideOut = piOut->sdAlphaRowStride;

  piaIn = &piIn->ia;
  piaOut = &piOut->ia;

  picIn = &piIn->ic;
  picOut = &piOut->ic;


  /* Set downsampled chrominance and nonreplicated downsampled chrominance
  ** flags in the output image.
  */

  piOut->bDownsampledChrominance = bDownsampledChrominance;
  piOut->bNonreplicatedDownsampledChrominance = 
    bNonreplicatedDownsampledChrominance;


  /* Check that the output image agrees with the input image.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piOut->bAlpha != !bAlpha )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Alpha flags disagree" );
      COSTELLA_RETURN;
    }

    if( piOut->udWidth != udWidth || piOut->udHeight != udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Dimensions disagree" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check whether we need to copy the alpha channel image data.
  */

  bCopyAlpha = !COSTELLA_IMAGE_ALPHA_IS_SAME( *piaIn, *piaOut );


  /* Start at the top-left corner of images.
  */

  COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpInStart, *picIn, udWidth, 
    udHeight, sdRowStrideIn );
  COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpOutStart, *picOut, udWidth, 
    udHeight, sdRowStrideOut );

  if( bCopyAlpha )
  {
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapInStart, *piaIn, udWidth, 
      udHeight, sdAlphaRowStrideIn );
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapOutStart, *piaOut, udWidth, 
      udHeight, sdAlphaRowStrideOut );
  }


  /* Walk through the rows. 
  */

  for( udRow = 0, bTop = COSTELLA_TRUE; udRow < udHeight; udRow++, bTop = 
    !bTop )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" )
      COSTELLA_RETURN;
    }


    /* Set pixels to the start of this row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );
    
    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapInStart, iapIn );
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapOutStart, iapOut );
    }


    /* Walk through the columns. 
    */

    for( udColumn = 0, bLeft = COSTELLA_TRUE; udColumn < udWidth; 
      udColumn++, bLeft = !bLeft )
    {
      /* Extract Y.
      */

      ubY = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );


      /* Check if we need to compute chrominance.
      */

      if( !bNonreplicatedDownsampledChrominance || ( bLeft && bTop ) )
      {
        /* We do need to do chrominance. Extract Cb and Cr.
        */

        ubCb = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
        ubCr = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );


        /* Convert to RGB.
        */

        COSTELLA_IMAGE_CONVERT_YCBCR_TO_RGB( ubY, ubCb, ubCr, &ubR, &ubG, 
          &ubB );


        /* Store the R, G, and B components.
        */

        COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubR, ubG, ubB );
      }
      else
      {
        /* We don't need to do chrominance. Set R, G, and B to the Y value.
        */

        COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubY, ubY, ubY );
      }


      /* Copy the alpha pixel, if we need to.
      */

      if( bCopyAlpha )
      {
        ubA = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubA );
      }


      /* Move to the next column.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

      if( bCopyAlpha )
      {
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
      }
    }


    /* Walk down to the next row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpInStart, sdRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOutStart, sdRowStrideOut );

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapInStart, sdAlphaRowStrideIn 
        );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOutStart, sdAlphaRowStrideOut
        );
    }
  }
}
COSTELLA_END_FUNCTION



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
