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
** costella_image_chrominance.c: 
**
**   Library for performing chrominance downsampling, upsampling, and
**   replication, as performed in JPEG, for COSTELLA_IMAGEs.
**
**   Library begun: December 30, 2005.
**   This version: November 12, 2007.
**
**   Written in ANSI C.
*/



/* Include files.
*/

#include "costella_image_chrominance.h"
#include <math.h>



/* Internal global variables.
*/

static COSTELLA_B gbInitialized = COSTELLA_FALSE;
static COSTELLA_SW* gaswCostellaImageChrominanceMult9Add8 = 0, 
  * gaswCostellaImageChrominanceMult3 = 0;



/* Start function bodies.
*/

#include "costella_body.h"



/* costella_image_chrominance_initialize:
**
**   Public interface for initializing this library. 
*/

COSTELLA_ANSI_FUNCTION( costella_image_chrominance_initialize, int, ( FILE* 
  pfileError ) )
{
  if( COSTELLA_CALL( CostellaImageChrominanceInitialize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_chrominance_finalize:
**
**   Public interface for finalizing this library. 
*/

COSTELLA_ANSI_FUNCTION( costella_image_chrominance_finalize, int, ( FILE* 
  pfileError ) )
{
  if( COSTELLA_CALL( CostellaImageChrominanceFinalize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_chrominance_average_downsample_replicate:
**
**   Public interface for downsampling the chrominance of a COSTELLA_IMAGE
**   by simple averaging, replicating the downsampled chrominance data into 
**   each pixel of each 2 x 2 block.
**
**   pi{In,Out}:  Pointer to the {input,output} COSTELLA_IMAGE. May be the
**     same. The input COSTELLA_IMAGE must be in the YCbCr colorspace, and
**     must not have downsampled chrominance. The output COSTELLA_IMAGE will
**     be in the YCbCr colorspace, and will have replicated downsampled 
**     chrominance.
*/

COSTELLA_ANSI_FUNCTION( costella_image_chrominance_downsample, int, ( 
  COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* piOut, int (*pfProgress)( void* 
  pvPassback ), void* pvPassback, FILE* pfileError ) )
{
  COSTELLA_WRAP_PROGRESS wp;

  wp.pfProgress = pfProgress;
  wp.pvPassback = pvPassback;

  if( COSTELLA_CALL( CostellaImageChrominanceAverageDownsampleReplicate( 
    piIn, piOut, CostellaWrapProgress, &wp ) ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  }
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_chrominance_magic_upsample:
**
**   Public interface for upsampling the chrominance of a COSTELLA_IMAGE
**   using the magic kernel.
**
**   pi{In,Out}:  Pointer to the {input,output} COSTELLA_IMAGE. May be the
**     same. The input COSTELLA_IMAGE must be in the YCbCr colorspace, and
**     must have downsampled chrominance (either nonreplicated or 
**     replicated). The output COSTELLA_IMAGE will be in the YCbCr 
**     colorspace, and will not have downsampled chrominance.
*/

COSTELLA_ANSI_FUNCTION( costella_image_chrominance_magic_upsample, int, ( 
  COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* piOut, int (*pfProgress)( void* 
  pvPassback ), void* pvPassback, FILE* pfileError ) )
{
  COSTELLA_WRAP_PROGRESS wp;

  wp.pfProgress = pfProgress;
  wp.pvPassback = pvPassback;

  if( COSTELLA_CALL( CostellaImageChrominanceMagicUpsample( piIn, piOut, 
    CostellaWrapProgress, &wp ) ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  }
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_chrominance_replicate_eq:
**
**   Public interface for replicating the chrominance data in a YCbCr 
**   COSTELLA_IMAGE into the other positions of each 2 x 2 block.
**
**   pi:  Pointer to the COSTELLA_IMAGE. It must be in the YCbCr colorspace,
**     and must have nonreplicated downsampled chrominance. On exit from the
**     function, it will still be in the YCbCr colorspace, and will have
**     replicated downsampled chrominance.
*/

COSTELLA_ANSI_FUNCTION( costella_image_chrominance_replicate_eq, int, ( 
  COSTELLA_IMAGE* pi, int (*pfProgress)( void* pvPassback ), void* 
  pvPassback, FILE* pfileError ) )
{
  COSTELLA_WRAP_PROGRESS wp;

  wp.pfProgress = pfProgress;
  wp.pvPassback = pvPassback;

  if( COSTELLA_CALL( CostellaImageChrominanceReplicateEq( pi, 
    CostellaWrapProgress, &wp ) ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  }
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* CostellaImageChrominanceInitialize: 
**
**   Initialize the library. 
*/

COSTELLA_FUNCTION( CostellaImageChrominanceInitialize, ( void ) )
{
  COSTELLA_SW sw;
  COSTELLA_SW* pswMult3, * pswMult9Add8;


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
    COSTELLA_ERROR( "Initializing" );
    gbInitialized = COSTELLA_FALSE;
    COSTELLA_RETURN;
  }


  /* Allocate arrays for the magic kernel. 
  */

  if( COSTELLA_MALLOC( gaswCostellaImageChrominanceMult9Add8, 256 ) || 
    COSTELLA_MALLOC( gaswCostellaImageChrominanceMult3, 256 ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Allocating" );

    if( COSTELLA_FREE( gaswCostellaImageChrominanceMult9Add8 ) || 
      COSTELLA_FREE( gaswCostellaImageChrominanceMult3 ) )
    {
      COSTELLA_CLEANUP_FUNDAMENTAL_ERROR( "Freeing" );
    }

    COSTELLA_RETURN;
  }


  /* Create the multiplication tables for the magic kernel.
  */

  for( sw = 0, pswMult3 = gaswCostellaImageChrominanceMult3, pswMult9Add8 = 
    gaswCostellaImageChrominanceMult9Add8; sw < 256; sw++, pswMult3++, 
    pswMult9Add8++ )
  {
    *pswMult3 = 3 * sw;
    *pswMult9Add8 = 9 * sw + 8;
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageChrominanceFinalize: 
**
**   Finalize the library. 
*/

COSTELLA_FUNCTION( CostellaImageChrominanceFinalize, ( void ) )
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


  /* Free arrays for the magic kernel.
  */

  if( COSTELLA_FREE( gaswCostellaImageChrominanceMult9Add8 ) || 
    COSTELLA_FREE( gaswCostellaImageChrominanceMult3 ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Freeing" );
    COSTELLA_RETURN;
  }


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



/* CostellaImageChrominanceAverageDownsampleReplicate: 
**
**   Downsample the chrominance of a COSTELLA_IMAGE by simple averaging, and
***  replicate the downsampled chrominance data into each pixel of each 
**   2 x 2 block.
**
**   pi{In,Out}:  Pointer to the {input,output} COSTELLA_IMAGE. May be the
**     same. The input COSTELLA_IMAGE must be in the YCbCr colorspace, and
**     must not have downsampled chrominance. The output COSTELLA_IMAGE will
**     be in the YCbCr colorspace, and will have replicated downsampled 
**     chrominance.
*/

COSTELLA_FUNCTION( CostellaImageChrominanceAverageDownsampleReplicate, ( 
  COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION 
  pfProgress, COSTELLA_O* poPassback ) )
{
  COSTELLA_B bAlpha, bCopyAlpha, bLastRow, bLastColumn;
  COSTELLA_UB ubYA, ubYB, ubYC, ubYD, ubCbA, ubCbB, ubCbC, ubCbD, ubCrA, 
    ubCrB, ubCrC, ubCrD, ubAA = 0, ubAB = 0, ubAC = 0, ubAD = 0, 
    ubCbAverage, ubCrAverage;
  COSTELLA_UD udWidth, udHeight, udColumn, udRow, udWidthMinusOne, 
    udHeightMinusOne;
  COSTELLA_SD sdRowStrideIn, sdAlphaRowStrideIn, sdRowStrideOut, 
    sdAlphaRowStrideOut, sdDoubleRowStrideIn, sdDoubleRowStrideOut;
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


  /* Check that the images are color.
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


  /* Check that the input image does not already have downsampled 
  ** chrominance data.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( piIn->bDownsampledChrominance )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image already downsampled" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Set the output image to be in the YCbCr colorspace.
  */

  piOut->bRgb = COSTELLA_FALSE;


  /* Flag that the output image will have replicated downsampled chrominance
  ** data.
  */

  piOut->bDownsampledChrominance = COSTELLA_TRUE;
  piOut->bNonreplicatedDownsampledChrominance = COSTELLA_FALSE;


  /* Extract data.
  */

  bAlpha = piIn->bAlpha;

  udWidth = piIn->udWidth;
  udHeight = piIn->udHeight;

  sdRowStrideIn = piIn->sdRowStride;
  sdAlphaRowStrideIn = piIn->sdAlphaRowStride;

  sdRowStrideOut = piOut->sdRowStride;
  sdAlphaRowStrideOut = piOut->sdAlphaRowStride;

  piaIn = &piIn->ia;
  picIn = &piIn->ic;

  piaOut = &piOut->ia;
  picOut = &piOut->ic;


  /* Check that the output image details agree.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piOut->bAlpha != !bAlpha )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Alpha flags don't match" );
      COSTELLA_RETURN;
    }

    if( piOut->udWidth != udWidth || piOut->udHeight != udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Dimensions don't match" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Determine whether we need to copy alpha channel data.
  */

  bCopyAlpha = bAlpha && !COSTELLA_IMAGE_ALPHA_IS_SAME( *piaIn, *piaOut );


  /* Compute the double row strides.
  */

  sdDoubleRowStrideIn = sdRowStrideIn << 1;
  sdDoubleRowStrideOut = sdRowStrideOut << 1;


  /* Compute convenient constants.
  */

  udWidthMinusOne = udWidth - 1;
  udHeightMinusOne = udHeight - 1;


  /* Start at the top-left corner.
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

    
  /* Walk through the downsampled rows of the image.
  */

  for( udRow = 0; udRow < udHeight; udRow += 2 )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" )
      COSTELLA_RETURN;
    }


    /* Check if we are in the last row.
    */

    bLastRow = udRow == udHeightMinusOne;


    /* Start at the left of this row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapInStart, iapIn );
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapOutStart, iapOut );
    }


    /* Walk through the downsampled columns of the image.
    */

    for( udColumn = 0; udColumn < udWidth; udColumn += 2 )
    {
      /* Check if we are in the last column.
      */

      bLastColumn = udColumn == udWidthMinusOne;


      /* Extract the top-left pixel's components.
      */

      ubYA = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
      ubCbA = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
      ubCrA = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

      if( bCopyAlpha )
      {
        ubAA = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
      }


      /* Check if we are in the last row.
      */

      if( bLastRow )
      {
        /* In the last row. Check if we are in the last column.
        */

        if( bLastColumn )
        {
          /* We are in the last row and the last column. The average is
          ** simply the top-left pixel, i.e., the only pixel. Store it.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYA, ubCbA, 
            ubCrA );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAA );
          }
        }
        else
        {
          /* We are in the last row, but not the last column. Average the 
          ** chrominance values of the two pixels in this row. Move right.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
          }


          /* Extract the top-right pixel's components.
          */

          ubYD = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          ubCbD = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCrD = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          if( bCopyAlpha )
          {
            ubAD = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
          }

          
          /* Move right for the next block.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
          }


          /* Average the two pixels.
          */

          ubCbAverage = (COSTELLA_UB) ( ( (COSTELLA_UW) ubCbA + 
            (COSTELLA_UW) ubCbD + 1 ) >> 1 );
          ubCrAverage = (COSTELLA_UB) ( ( (COSTELLA_UW) ubCrA + 
            (COSTELLA_UW) ubCrD + 1 ) >> 1 );


          /* Store the average chrominance values, together with the 
          ** original (unchanged) two luminance values, and original 
          ** (unchanged) alpha values, if needed, in the two output pixel 
          ** locations. Start with the top-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYA, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAA );
          }
          

          /* Move right.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
          }


          /* Store the top-right pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYD, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAD );
          }

      
          /* Move right for the next 2 x 2 block.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
          }
        }
      }
      else
      {
        /* Not in the last row. Check if we are in the last column.
        */

        if( bLastColumn )
        {
          /* We are in the last column, but not the last row. Average the 
          ** chrominance values of the two pixels in this column. Move down.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapIn, sdAlphaRowStrideIn 
              );
          }

          /* Extract the bottom-left pixel's components.
          */

          ubYB = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          ubCbB = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCrB = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          if( bCopyAlpha )
          {
            ubAB = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
          }

          
          /* Average the two pixels.
          */

          ubCbAverage = (COSTELLA_UB) ( ( (COSTELLA_UW) ubCbA + 
            (COSTELLA_UW) ubCbB + 1 ) >> 1 );
          ubCrAverage = (COSTELLA_UB) ( ( (COSTELLA_UW) ubCrA + 
            (COSTELLA_UW) ubCrB + 1 ) >> 1 );


          /* Store the average chrominance values, together with the 
          ** original (unchanged) two luminance values, and original 
          ** (unchanged) alpha values, if needed, in the two output pixel 
          ** locations. Start with the top-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYA, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAA );
          }


          /* Move down.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOut, sdRowStrideOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOut, 
              sdAlphaRowStrideOut );
          }


          /* Store the bottom-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYB, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAB );
          }
        }
        else
        {
          /* In neither the last row nor the last column. Move down.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapIn, sdAlphaRowStrideIn 
              );
          }


          /* Extract the bottom-left pixel's components.
          */

          ubYB = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          ubCbB = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCrB = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          if( bCopyAlpha )
          {
            ubAB = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
          }

      
          /* Move right.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
          }


          /* Extract the bottom-right pixel's components.
          */

          ubYC = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          ubCbC = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCrC = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          if( bCopyAlpha )
          {
            ubAC = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
          }

    
          /* Move up.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_UP( icpIn, sdRowStrideIn );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_UP( iapIn, sdAlphaRowStrideIn );
          }


          /* Extract the top-right pixel's components.
          */

          ubYD = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          ubCbD = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCrD = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          if( bCopyAlpha )
          {
            ubAD = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
          }

      
          /* Move right for the next 2 x 2 block.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );
      
          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
          }


          /* Average the four pixels.
          */

          ubCbAverage = (COSTELLA_UB) ( ( (COSTELLA_UW) ubCbA + 
            (COSTELLA_UW) ubCbB + (COSTELLA_UW) ubCbC + (COSTELLA_UW) ubCbD 
            + 2 ) >> 2 );
          ubCrAverage = (COSTELLA_UB) ( ( (COSTELLA_UW) ubCrA + 
            (COSTELLA_UW) ubCrB + (COSTELLA_UW) ubCrC + (COSTELLA_UW) ubCrD 
            + 2 ) >> 2 );

        
          /* Store the average chrominance values, together with the 
          ** original (unchanged) two luminance values, and original 
          ** (unchanged) alpha values, if needed, in the two output pixel 
          ** locations. Start with the top-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYA, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAA );
          }

        
          /* Move down.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOut, sdRowStrideOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOut, 
              sdAlphaRowStrideOut );
          }


          /* Store the bottom-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYB, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAB );
          }


          /* Move right.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
          }


          /* Store the bottom-right pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYC, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAC );
          }

      
          /* Move up.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_UP( icpOut, sdRowStrideOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_UP( iapOut, sdAlphaRowStrideOut 
              );
          }


          /* Store the top-right pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_RGB_YCBCR( icpOut, ubYD, 
            ubCbAverage, ubCrAverage );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubAD );
          }

      
          /* Move right for the next 2 x 2 block.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

          if( bCopyAlpha )
          {
            COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
          }
        }
      }
    }


    /* Walk down to the next row pair.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpInStart, 
      sdDoubleRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpOutStart, 
      sdDoubleRowStrideOut );

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapInStart, sdAlphaRowStrideIn 
        );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapInStart, sdAlphaRowStrideIn 
        );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOutStart, sdAlphaRowStrideOut
        );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOutStart, sdAlphaRowStrideOut
        );
    }
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageChrominanceMagicUpsample: 
**
**   Upsample the chrominance channels of a YCbCr image using the magic 
**   kernel.
**
**   pi{In,Out}:  Pointer to the {input,output} COSTELLA_IMAGE. May be the
**     same. The input COSTELLA_IMAGE must be in the YCbCr colorspace, and
**     must have downsampled chrominance (either nonreplicated or 
**     replicated). The output COSTELLA_IMAGE will be in the YCbCr 
**     colorspace, and will not have downsampled chrominance.
*/

COSTELLA_FUNCTION( CostellaImageChrominanceMagicUpsample, ( COSTELLA_IMAGE* 
  piIn, COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, 
  COSTELLA_O* poPassback ) )
{
  COSTELLA_B bAlpha, bCopyY, bCopyAlpha, bTop, bLeft, bMissingLastColumn;
  COSTELLA_UB ubY, ubCb, ubCr, ubA, ubCbA, ubCbB, ubCbC, ubCbD, ubCrA, 
    ubCrB, ubCrC, ubCrD, ubCbSavedLeft = 0, ubCrSavedLeft = 0; 
  COSTELLA_UB* pubCbBuffer, * pubCrBuffer, * pubCbBufferLeft, 
    * pubCrBufferLeft, * pubCbBufferRight, * pubCrBufferRight;
  COSTELLA_UB* aubCbBuffer = 0, * aubCrBuffer = 0;
  COSTELLA_UD udWidth, udHeight, udBufferWidth, udWidthMinusOne, 
    udHeightMinusOne, udRow, udColumn;
  COSTELLA_SD sdRowStrideIn, sdRowStrideOut, sdAlphaRowStrideIn,
    sdAlphaRowStrideOut;
  COSTELLA_IMAGE_ALPHA* piaIn, * piaOut;
  COSTELLA_IMAGE_COLOR* picIn, * picOut;
  COSTELLA_IMAGE_ALPHA_PIXEL iapIn, iapInStart, iapOut, iapOutStart;
  COSTELLA_IMAGE_COLOR_PIXEL icpIn, icpInTarget, icpInStart, icpOut, 
    icpOutStart;


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

  
  /* Check that the images are color.
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


  /* Set the output image to be in the YCbCr colorspace.
  */

  piOut->bRgb = COSTELLA_FALSE;


  /* Check that the input image has downsampled chrominance data.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piIn->bDownsampledChrominance )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image does not have downsampled "
        "chrominance" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Set the output image to not have downsampled chrominance data.
  */

  piOut->bDownsampledChrominance = COSTELLA_FALSE;


  /* Extract data.
  */

  bAlpha = piIn->bAlpha;

  udWidth = piIn->udWidth;
  udHeight = piIn->udHeight;

  sdRowStrideIn = piIn->sdRowStride;
  sdAlphaRowStrideIn = piIn->sdAlphaRowStride;

  sdRowStrideOut = piOut->sdRowStride;
  sdAlphaRowStrideOut = piOut->sdAlphaRowStride;

  piaIn = &piIn->ia;
  picIn = &piIn->ic;

  piaOut = &piOut->ia;
  picOut = &piOut->ic;


  /* Check that the output image details agree.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piOut->bAlpha != !bAlpha )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Alpha flags don't match" );
      COSTELLA_RETURN;
    }

    if( piOut->udWidth != udWidth || piOut->udHeight != udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Dimensions don't match" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Determine whether we need to copy luminance channel data.
  */

  bCopyY = !COSTELLA_IMAGE_COLOR_IS_SAME( *picIn, *picOut );


  /* Determine whether we need to copy alpha channel data.
  */

  bCopyAlpha = bAlpha && !COSTELLA_IMAGE_ALPHA_IS_SAME( *piaIn, *piaOut );


  /* Compute convenient constants.
  */

  udWidthMinusOne = udWidth - 1;
  udHeightMinusOne = udHeight - 1;


  /* Determine if we are missing the last column.
  */

  bMissingLastColumn = !!( udWidth % 2 );
  
  
  /* Compute buffer width. We need a buffer location for every two pixels
  ** or part thereof. 
  */

  udBufferWidth = ( udWidth + 1 ) >> 1;


  /* Allocate buffer memory.
  */

  if( COSTELLA_MALLOC( aubCbBuffer, udBufferWidth ) || COSTELLA_MALLOC( 
    aubCrBuffer, udBufferWidth ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Allocating" );

    if( COSTELLA_FREE( aubCbBuffer ) || COSTELLA_FREE( aubCrBuffer ) )
    {
      COSTELLA_CLEANUP_FUNDAMENTAL_ERROR( "Freeing" );
    }
    
    COSTELLA_RETURN;
  }


  /* Start at the top-left corner.
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


    /* Start at the left of this row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpInTarget );
    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapInStart, iapIn );
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapOutStart, iapOut );
    }


    /* Walk through the columns of the image.
    */

    for( udColumn = 0, bLeft = COSTELLA_TRUE, pubCbBuffer = aubCbBuffer, 
      pubCrBuffer = aubCrBuffer, pubCbBufferLeft = aubCbBuffer - 1, 
      pubCrBufferLeft = aubCrBuffer - 1, pubCbBufferRight = aubCbBuffer + 1, 
      pubCrBufferRight = aubCrBuffer + 1; udColumn < udWidth; udColumn++, 
      bLeft = !bLeft )
    {
      /* Switch on position in 2 x 2 block.
      */

      if( bTop )
      {
        if( bLeft )
        {
          /* Top-left.
          */

          ubCbA = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpInTarget );
          ubCrA = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpInTarget );

          if( !udRow )
          {
            ubCbB = ubCbA;
            ubCrB = ubCrA;
          }
          else
          {
            ubCbB = *pubCbBuffer;
            ubCrB = *pubCrBuffer;
          }

          if( !udColumn )
          {
            ubCbC = ubCbA;
            ubCrC = ubCrA;
          }
          else
          {
            ubCbC = ubCbSavedLeft;
            ubCrC = ubCrSavedLeft;
          }

          if( !udColumn || !udRow )
          {
            ubCbD = ubCbA;
            ubCrD = ubCrA;
          }
          else
          {
            ubCbD = *pubCbBufferLeft;
            ubCrD = *pubCrBufferLeft;
          }


          /* Store buffer values to our left.
          */

          if( udColumn )
          {
            *pubCbBufferLeft = ubCbSavedLeft;
            *pubCrBufferLeft = ubCrSavedLeft;
          }


          /* Store the current values as the new saved left values.
          */

          ubCbSavedLeft = ubCbA;
          ubCrSavedLeft = ubCrA;
        }
        else
        {
          /* Top-right.
          */

          ubCbA = ubCbSavedLeft;
          ubCrA = ubCrSavedLeft;

          if( !udRow )
          {
            ubCbB = ubCbA;
            ubCrB = ubCrA;
          }
          else
          {
            ubCbB = *pubCbBuffer;
            ubCrB = *pubCrBuffer;
          }

          if( udColumn == udWidthMinusOne )
          {
            ubCbC = ubCbA;
            ubCrC = ubCrA;
          }
          else
          {
            COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInTarget, icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );

            ubCbC = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            ubCrC = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }

          if( udColumn == udWidthMinusOne || !udRow )
          {
            ubCbD = ubCbA;
            ubCrD = ubCrA;
          }
          else
          {
            ubCbD = *pubCbBufferRight;
            ubCrD = *pubCrBufferRight;
          }
        }
      }
      else
      {
        if( bLeft )
        {
          /* Bottom-left.
          */

          ubCbA = *pubCbBuffer;
          ubCrA = *pubCrBuffer;

          if( udRow == udHeightMinusOne )
          {
            ubCbB = ubCbA;
            ubCrB = ubCrA;
          }
          else
          {
            COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInTarget, icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn );
          
            ubCbB = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            ubCrB = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }

          if( !udColumn )
          {
            ubCbC = ubCbA;
            ubCrC = ubCrA;
          }
          else
          {
            ubCbC = *pubCbBufferLeft;
            ubCrC = *pubCrBufferLeft;
          }

          if( !udColumn || udRow == udHeightMinusOne )
          {
            ubCbD = ubCbA;
            ubCrD = ubCrA;
          }
          else
          {
            COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInTarget, icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_LEFT_TWO( icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn );
          
            ubCbD = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            ubCrD = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }
        }
        else
        {
          /* Bottom-right.
          */

          ubCbA = *pubCbBuffer;
          ubCrA = *pubCrBuffer;

          if( udRow == udHeightMinusOne )
          {
            ubCbB = ubCbA;
            ubCrB = ubCrA;
          }
          else
          {
            COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInTarget, icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_LEFT( icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn );
          
            ubCbB = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            ubCrB = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }

          if( udColumn == udWidthMinusOne )
          {
            ubCbC = ubCbA;
            ubCrC = ubCrA;
          }
          else
          {
            ubCbC = *pubCbBufferRight;
            ubCrC = *pubCrBufferRight;
          }

          if( udColumn == udWidthMinusOne || udRow == udHeightMinusOne )
          {
            ubCbD = ubCbA;
            ubCrD = ubCrA;
          }
          else
          {
            COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInTarget, icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn );
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn );
          
            ubCbD = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            ubCrD = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }
        }
      }


      /* Combine the four pixels and divide by 16. 
      */

      ubCb = (COSTELLA_UB) COSTELLA_SHIFT_RIGHT_FLOOR( 
        gaswCostellaImageChrominanceMult9Add8[ ubCbA ] + 
        gaswCostellaImageChrominanceMult3[ ubCbB ] + 
        gaswCostellaImageChrominanceMult3[ ubCbC ] + (COSTELLA_SW) ubCbD, 4 
        );
      ubCr = (COSTELLA_UB) COSTELLA_SHIFT_RIGHT_FLOOR( 
        gaswCostellaImageChrominanceMult9Add8[ ubCrA ] + 
        gaswCostellaImageChrominanceMult3[ ubCrB ] + 
        gaswCostellaImageChrominanceMult3[ ubCrC ] + (COSTELLA_SW) ubCrD, 4 
        );


      /* Store the values.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
      COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );

      if( bCopyY )
      {
        ubY = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpInTarget );
        COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
      }

      if( bCopyAlpha )
      {
        ubA = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubA );
      }


      /* If we in a right column, update the buffer position.
      */
      
      if( !bLeft )
      {
        pubCbBuffer++;
        pubCrBuffer++;

        pubCbBufferLeft++;
        pubCrBufferLeft++;

        pubCbBufferRight++;
        pubCrBufferRight++;
      }


      /* Move right.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpInTarget );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut );

      if( bCopyAlpha )
      {
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut );
      }
    }


    /* Store the last buffer values.
    */

    if( bTop )
    {
      if( bMissingLastColumn )
      {
        pubCbBufferLeft++;
        pubCrBufferLeft++;
      }

      *pubCbBufferLeft = ubCbSavedLeft;
      *pubCrBufferLeft = ubCrSavedLeft;
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


  /* Free buffers.
  */

  if( COSTELLA_FREE( aubCbBuffer ) || COSTELLA_FREE( aubCrBuffer ) )
  {
    COSTELLA_ERROR( "Freeing" )
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageChrominanceReplicateEq: 
**
**   Public interface for replicating the YCbCr data in a COSTELLA_IMAGE 
**   into the other positions of each 2 x 2 block.
**
**   pi:  Pointer to the COSTELLA_IMAGE. It must be in the YCbCr colorspace,
**     and must have nonreplicated downsampled chrominance. On exit from the
**     function, it will still be in the YCbCr colorspace, and will have
**     replicated downsampled chrominance.
*/

COSTELLA_FUNCTION( CostellaImageChrominanceReplicateEq, ( COSTELLA_IMAGE* 
  pi, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* poPassback ) )
{
  COSTELLA_B bLastRow, bLastColumn;
  COSTELLA_UB ubCb, ubCr;
  COSTELLA_UD udWidth, udHeight, udColumn, udRow, udWidthMinusOne, 
    udHeightMinusOne;
  COSTELLA_SD sdRowStride, sdDoubleRowStride;
  COSTELLA_IMAGE_COLOR* pic;
  COSTELLA_IMAGE_COLOR_PIXEL icp, icpStart;


  /* Keep GCC happy.
  */

  icp = icpStart = gicpCostellaImageNull;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !pi )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null image" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that the image is color.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !pi->bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Image not color" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that the image is in the YCbCr colorspace.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( pi->bRgb )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Image is RGB" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that the image has downsampled chrominance data.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !pi->bDownsampledChrominance )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image has downsampled data" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Check that the image has nonreplicated downsampled chrominance data.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !pi->bNonreplicatedDownsampledChrominance )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Input image already replicated" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Set the image to now have replicated chrominance data.
  */

  pi->bNonreplicatedDownsampledChrominance = COSTELLA_FALSE;


  /* Extract data.
  */

  udWidth = pi->udWidth;
  udHeight = pi->udHeight;

  sdRowStride = pi->sdRowStride;

  pic = &pi->ic;


  /* Compute the double row stride.
  */

  sdDoubleRowStride = sdRowStride << 1;


  /* Compute convenient constants.
  */

  udWidthMinusOne = udWidth - 1;
  udHeightMinusOne = udHeight - 1;


  /* Start at the top-left corner.
  */
  
  COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpStart, *pic, udWidth, 
    udHeight, sdRowStride );

    
  /* Walk through the downsampled rows of the image.
  */

  for( udRow = 0; udRow < udHeight; udRow += 2 )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" )
      COSTELLA_RETURN;
    }


    /* Check if we are in the last row.
    */

    bLastRow = udRow == udHeightMinusOne;


    /* Start at the left of this row.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpStart, icp );


    /* Walk through the downsampled columns of the image.
    */

    for( udColumn = 0; udColumn < udWidth; udColumn += 2 )
    {
      /* Check if we are in the last column.
      */

      bLastColumn = udColumn == udWidthMinusOne;


      /* Extract the chrominance components for this block.
      */

      ubCb = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icp );
      ubCr = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icp );


      /* Check if we are in the last row.
      */

      if( bLastRow )
      {
        /* In the last row. Check if we are in the last column.
        */

        if( bLastColumn )
        {
          /* We are in the last row and the last column. Nothing needs to
          ** be done.
          */
        }
        else
        {
          /* We are in the last row, but not the last column. Replicate the
          ** values into the top-right pixel. Move right.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp );


          /* Store the chrominance values in the top-right pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icp, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icp, ubCr );
          
      
          /* Move right for the next 2 x 2 block.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp );
        }
      }
      else
      {
        /* Not in the last row. Check if we are in the last column.
        */

        if( bLastColumn )
        {
          /* We are in the last column, but not the last row. Replicate the
          ** values into the bottom-left pixel. Move down.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icp, sdRowStride );


          /* Store the chrominance values in the bottom-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icp, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icp, ubCr );
        }
        else
        {
          /* In neither the last row nor the last column. Replicate the
          ** chrominance values into the other three pixels in this block.
          ** Move down.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icp, sdRowStride );


          /* Store the bottom-left pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icp, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icp, ubCr );


          /* Move right.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp );


          /* Store the bottom-right pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icp, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icp, ubCr );

      
          /* Move up.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_UP( icp, sdRowStride );


          /* Store the top-right pixel.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icp, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icp, ubCr );

      
          /* Move right for the next 2 x 2 block.
          */

          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp );
        }
      }
    }


    /* Walk down to the next row pair.
    */

    COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpStart, sdDoubleRowStride );
  }
}
COSTELLA_END_FUNCTION



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
