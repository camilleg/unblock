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
** costella_unblock.c: 
**
**   Perform the UnBlock algorithm on a JPEG image.
**
**   Library begun: December 30, 2005.
**   This version: August 25, 2007.
**
**   Written in ANSI C.
*/



/* Include files.
*/

#include "costella_unblock.h"
#include <stdlib.h>
#include <math.h>



/* Debug flag.
*/

/* #define COSTELLA_UNBLOCK_DEBUG_DUMP */



/* Internal function prototypes.
*/

static COSTELLA_FUNCTION( CostellaUnblockComputeVerticalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UD* audYBoundaryU, COSTELLA_UD* audYBoundaryV, 
  COSTELLA_UD* audCbBoundaryU, COSTELLA_UD* audCbBoundaryV, COSTELLA_UD* 
  audCrBoundaryU, COSTELLA_UD* audCrBoundaryV, COSTELLA_UD* audYInternalU, 
  COSTELLA_UD* audYInternalV, COSTELLA_UD* audCbInternalU, COSTELLA_UD* 
  audCbInternalV, COSTELLA_UD* audCrInternalU, COSTELLA_UD* audCrInternalV, 
  COSTELLA_UD* pudTotalLuminance, COSTELLA_UD* pudTotalChrominance, 
  COSTELLA_IMAGE* pi, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )

static COSTELLA_FUNCTION( CostellaUnblockComputeHorizontalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UD* audYBoundaryU, COSTELLA_UD* audYBoundaryV, 
  COSTELLA_UD* audCbBoundaryU, COSTELLA_UD* audCbBoundaryV, COSTELLA_UD* 
  audCrBoundaryU, COSTELLA_UD* audCrBoundaryV, COSTELLA_UD* audYInternalU, 
  COSTELLA_UD* audYInternalV, COSTELLA_UD* audCbInternalU, COSTELLA_UD* 
  audCbInternalV, COSTELLA_UD* audCrInternalU, COSTELLA_UD* audCrInternalV, 
  COSTELLA_UD* pudTotalLuminance, COSTELLA_UD* pudTotalChrominance, 
  COSTELLA_IMAGE* pi, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )

static COSTELLA_FUNCTION( CostellaUnblockCorrectVerticalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UB* aubYAdjustedU, COSTELLA_UB* aubYAdjustedV, 
  COSTELLA_UB* aubCbAdjustedU, COSTELLA_UB* aubCbAdjustedV, COSTELLA_UB* 
  aubCrAdjustedU, COSTELLA_UB* aubCrAdjustedV, COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )

static COSTELLA_FUNCTION( CostellaUnblockCorrectHorizontalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UB* aubYAdjustedU, COSTELLA_UB* aubYAdjustedV, 
  COSTELLA_UB* aubCbAdjustedU, COSTELLA_UB* aubCbAdjustedV, COSTELLA_UB* 
  aubCrAdjustedU, COSTELLA_UB* aubCrAdjustedV, COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )

static COSTELLA_FUNCTION( CostellaUnblockComputeDiscrepancies, ( 
  COSTELLA_SW* aswValues, COSTELLA_SW* pswU, COSTELLA_SW* pswV ) )

static COSTELLA_FUNCTION( CostellaUnblockComputeAdjustments, ( COSTELLA_UD* 
  audReference, COSTELLA_UD* audMeasured, COSTELLA_UD udTotal, COSTELLA_B 
  bConservativePhotographic, COSTELLA_B bConservativeCartoon, COSTELLA_UB* 
  aubAdjusted ) )

static COSTELLA_FUNCTION( CostellaUnblockCorrectDiscrepancies, ( 
  COSTELLA_SW* asw, COSTELLA_SW swU, COSTELLA_SW swV ) )

static COSTELLA_UD costella_unblock_approx_square_root( COSTELLA_UD ud );


#ifdef COSTELLA_UNBLOCK_DEBUG_DUMP

  void CostellaUnblockDebugDump( COSTELLA_UB* aubYAdjustedU, COSTELLA_UB* 
    aubYAdjustedV, COSTELLA_UB* aubCbAdjustedU, COSTELLA_UB* aubCbAdjustedV,
    COSTELLA_UB* aubCrAdjustedU, COSTELLA_UB* aubCrAdjustedV, COSTELLA_UD* 
    audYInternalU, COSTELLA_UD* audYInternalV, COSTELLA_UD* audCbInternalU, 
    COSTELLA_UD* audCbInternalV, COSTELLA_UD* audCrInternalU, COSTELLA_UD* 
    audCrInternalV, COSTELLA_UD* audYBoundaryU, COSTELLA_UD* audYBoundaryV, 
    COSTELLA_UD* audCbBoundaryU, COSTELLA_UD* audCbBoundaryV, COSTELLA_UD* 
    audCrBoundaryU, COSTELLA_UD* audCrBoundaryV, COSTELLA_B bVertical, 
    COSTELLA_B bColor );

#endif

  

/* Internal global variables.
*/

static COSTELLA_B gbInitialized = COSTELLA_FALSE;
static COSTELLA_UB* gaubSquareRoot = 0;
static COSTELLA_UW* gauwMult3 = 0, * gauwMult15Add4 = 0, * gauwMult15 = 0, 
  * gauwMult10 = 0, * gauwMult12Add4 = 0;
static COSTELLA_SW* gaswMultM11Add128 = 0, * gaswMultM31Add128 = 0, 
  * gaswMultM58Add128 = 0, * gaswMultM57Add128 = 0, * gaswMultM22Add128 = 0,
  * gaswMult42Add128 = 0, * gaswMult138Add128 = 0, * gaswMult3Add32 = 0, 
  * gaswMult3Add16 = 0, * gaswMult5Add16 = 0, * gaswMult15Add32 = 0, 
  * gaswMult21Add32 = 0, * gaswMult7Add8 = 0; 



/* Start function bodies.
*/

#include "costella_body.h"



/* costella_unblock_initialize:
**
**   Public interface for initializing this library. 
**
**   Returns 0 if there is an error, or nonzero if there is no error.
*/

COSTELLA_ANSI_FUNCTION( costella_unblock_initialize, int, ( FILE* pfileError
  ) )
{
  if( COSTELLA_CALL( CostellaUnblockInitialize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_unblock_finalize:
**
**   Public interface for finalizing this library. 
**
**   Returns 0 if there is an error, or nonzero if there is no error.
*/

COSTELLA_ANSI_FUNCTION( costella_unblock_finalize, int, ( FILE* pfileError )
  )
{
  if( COSTELLA_CALL( CostellaUnblockFinalize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_unblock:
**
**   Public interface for performing the Unblock algorithm on a 
**   COSTELLA_IMAGE.
**
**   psi{Source,Destination}:  Pointer to the {source,destination} 
**     COSTELLA_SIMAGE.
**
**   Returns 0 if there is an error, or nonzero if there is no error.
*/

COSTELLA_ANSI_FUNCTION( costella_unblock, int, ( COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, int bPhotographic, int bCartoon, int (*pfProgress)(
  void* pvPassback ), void* pvPassback, FILE* pfileError ) )
{
  COSTELLA_WRAP_PROGRESS wp;

  wp.pfProgress = pfProgress;
  wp.pvPassback = pvPassback;

  if( COSTELLA_CALL( CostellaUnblock( piIn, piOut, bPhotographic, bCartoon, 
    CostellaWrapProgress, &wp ) ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  }
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* CostellaUnblockInitialize: 
**
**   Initialize the library. 
*/

COSTELLA_FUNCTION( CostellaUnblockInitialize, ( void ) )
{
  COSTELLA_UW uw;
  COSTELLA_SW sw;


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

  if( COSTELLA_CALL( CostellaImageChrominanceInitialize() ) || 
    COSTELLA_CALL( CostellaImageConvertInitialize() ) ) 
  {
    COSTELLA_ERROR( "Initializing" );
    gbInitialized = COSTELLA_FALSE;
    COSTELLA_RETURN;
  }


  /* Allocate memory for global lookup tables. 
  */

  if( COSTELLA_MALLOC( gaubSquareRoot, 256 ) || COSTELLA_MALLOC( gauwMult3, 
    256 ) || COSTELLA_MALLOC( gauwMult15Add4, 256 ) || COSTELLA_MALLOC( 
    gauwMult15, 256 ) || COSTELLA_MALLOC( gauwMult10, 256 ) || 
    COSTELLA_MALLOC( gauwMult12Add4, 256 ) || COSTELLA_MALLOC_OFFSET( 
    gaswMult3Add32, -255, +255 ) || COSTELLA_MALLOC_OFFSET( gaswMult3Add16, 
    -255, +255 ) || COSTELLA_MALLOC_OFFSET( gaswMult5Add16, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMult15Add32, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMult21Add32, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMult7Add8, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMultM11Add128, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMultM31Add128, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMultM58Add128, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMultM57Add128, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMultM22Add128, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMult42Add128, -255, +255 ) || 
    COSTELLA_MALLOC_OFFSET( gaswMult138Add128, -255, +255 ) ) 
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Allocating" );

    if( COSTELLA_FREE( gaubSquareRoot ) || COSTELLA_FREE( gauwMult3 ) ||
      COSTELLA_FREE( gauwMult15Add4 ) || COSTELLA_FREE( gauwMult15 ) ||
      COSTELLA_FREE( gauwMult10 ) || COSTELLA_FREE( gauwMult12Add4 ) ||
      COSTELLA_FREE_OFFSET( gaswMult3Add32, -255 ) || COSTELLA_FREE_OFFSET( 
      gaswMult3Add16, -255 ) || COSTELLA_FREE_OFFSET( gaswMult5Add16, -255 )
      || COSTELLA_FREE_OFFSET( gaswMult15Add32, -255 ) || 
      COSTELLA_FREE_OFFSET( gaswMult21Add32, -255 ) || COSTELLA_FREE_OFFSET(
      gaswMult7Add8, -255 ) || COSTELLA_FREE_OFFSET( gaswMultM11Add128, -255
      ) || COSTELLA_FREE_OFFSET( gaswMultM31Add128, -255 ) || 
      COSTELLA_FREE_OFFSET( gaswMultM58Add128, -255 ) || 
      COSTELLA_FREE_OFFSET( gaswMultM57Add128, -255 ) ||
      COSTELLA_FREE_OFFSET( gaswMultM22Add128, -255 ) ||
      COSTELLA_FREE_OFFSET( gaswMult42Add128, -255 ) ||
      COSTELLA_FREE_OFFSET( gaswMult138Add128, -255 ) )
    {
      COSTELLA_CLEANUP_FUNDAMENTAL_ERROR( "Freeing" );
    }

    COSTELLA_RETURN;
  }


  /* Initialize the [0,255] square-root table and the [0,255] multiplication
  ** tables. 
  */

  for( uw = 0; uw < 256; uw++ )
  {
    gaubSquareRoot[ uw ] = (COSTELLA_UB) floor( sqrt( (double) uw ) + 0.5 );

    gauwMult3[ uw ] = 3 * uw;
    gauwMult15Add4[ uw ] = 15 * uw + 4;
    gauwMult15[ uw ] = 15 * uw;
    gauwMult10[ uw ] = 10 * uw;
    gauwMult12Add4[ uw ] = 12 * uw + 4;
  }


  /* Initialize the [-255,255] multiplication tables. 
  */

  for( sw = -255; sw < 256; sw++ )
  {
    gaswMult3Add32[ sw ] = 3 * sw + 32;
    gaswMult3Add16[ sw ] = 3 * sw + 16;
    gaswMult5Add16[ sw ] = 5 * sw + 16;
    gaswMult15Add32[ sw ] = 15 * sw + 32;
    gaswMult21Add32[ sw ] = 21 * sw + 32;
    gaswMult7Add8[ sw ] = 7 * sw + 8;
    gaswMultM11Add128[ sw ] = -11 * sw + 128;
    gaswMultM31Add128[ sw ] = -31 * sw + 128;
    gaswMultM58Add128[ sw ] = -58 * sw + 128;
    gaswMultM57Add128[ sw ] = -57 * sw + 128;
    gaswMultM22Add128[ sw ] = -22 * sw + 128;
    gaswMult42Add128[ sw ] = 42 * sw + 128;
    gaswMult138Add128[ sw ] = 138 * sw + 128;
  }
}
COSTELLA_END_FUNCTION



/* CostellaUnblockFinalize: 
**
**   Finalize the library. 
*/

COSTELLA_FUNCTION( CostellaUnblockFinalize, ( void ) )
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


  /* Free the global memory allocated in initialization.
  */

  if( COSTELLA_FREE( gaubSquareRoot ) || COSTELLA_FREE( gauwMult3 ) ||
    COSTELLA_FREE( gauwMult15Add4 ) || COSTELLA_FREE( gauwMult15 ) ||
    COSTELLA_FREE( gauwMult10 ) || COSTELLA_FREE( gauwMult12Add4 ) ||
    COSTELLA_FREE_OFFSET( gaswMult3Add32, -255 ) || COSTELLA_FREE_OFFSET( 
    gaswMult3Add16, -255 ) || COSTELLA_FREE_OFFSET( gaswMult5Add16, -255 ) 
    || COSTELLA_FREE_OFFSET( gaswMult15Add32, -255 ) || 
    COSTELLA_FREE_OFFSET( gaswMult21Add32, -255 ) || COSTELLA_FREE_OFFSET( 
    gaswMult7Add8, -255 ) || COSTELLA_FREE_OFFSET( gaswMultM11Add128, -255 )
    || COSTELLA_FREE_OFFSET( gaswMultM31Add128, -255 ) ||
    COSTELLA_FREE_OFFSET( gaswMultM58Add128, -255 ) || COSTELLA_FREE_OFFSET(
    gaswMultM57Add128, -255 ) || COSTELLA_FREE_OFFSET( gaswMultM22Add128, 
    -255 ) || COSTELLA_FREE_OFFSET( gaswMult42Add128, -255 ) || 
    COSTELLA_FREE_OFFSET( gaswMult138Add128, -255 ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Freeing" );
    COSTELLA_RETURN;
  }
  

  /* Finalize other libraries.
  */

  if( COSTELLA_CALL( CostellaImageChrominanceFinalize() ) || COSTELLA_CALL( 
    CostellaImageConvertFinalize() ) )
  {
    COSTELLA_ERROR( "Finalizing" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* COSTELLA_UNBLOCK_ADJUST_DISCREPANCY: 
**
**   Adjust a discrepancy using the lookup tables. Inline macro.
**
**   lsw:  Discrepancy value to be adjusted.
**
**   laubAdjusted:  Adjustment table to be used.
*/

#define COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( lsw, laubAdjusted ) \
  ( (lsw) < 0 ? -(COSTELLA_SW) (laubAdjusted)[ -(lsw) ] : (COSTELLA_SW) \
  (laubAdjusted)[ (lsw) ] )



/* COSTELLA_UNBLOCK_CORRECT: 
**
**   Correct a discrepancy value and range-limit it. Inline macro.
**
**   lpsw:  Pointer to the discrepancy value to be corrected.
**
**   lswD:  Difference to be added to the discrepancy value.
*/

#define COSTELLA_UNBLOCK_CORRECT( lpsw, lswD ) \
{ \
  COSTELLA_SW lsw; \
  \
  if( lswD ) \
  { \
    lsw = *lpsw + lswD; \
    \
    *lpsw = COSTELLA_IMAGE_LIMIT_RANGE( lsw ); \
  } \
}



/* COSTELLA_UNBLOCK_CLEANUP: 
** 
**   Clean up in case of error.
*/

#define COSTELLA_UNBLOCK_CLEANUP \
{ \
  if( COSTELLA_FREE( aubYAdjustedU ) || COSTELLA_FREE( aubYAdjustedV ) || \
    COSTELLA_FREE( aubCbAdjustedU ) || COSTELLA_FREE( aubCbAdjustedV ) || \
    COSTELLA_FREE( aubCrAdjustedU ) || COSTELLA_FREE( aubCrAdjustedV ) || \
    COSTELLA_FREE( aswBufferY ) || COSTELLA_FREE( aswBufferCb ) || \
    COSTELLA_FREE( aswBufferCr ) || COSTELLA_FREE( audYInternalU ) || \
    COSTELLA_FREE( audYBoundaryU ) || COSTELLA_FREE( audYInternalV ) || \
    COSTELLA_FREE( audYBoundaryV ) || COSTELLA_FREE( audCbInternalU ) || \
    COSTELLA_FREE( audCbBoundaryU ) || COSTELLA_FREE( audCbInternalV ) || \
    COSTELLA_FREE( audCbBoundaryV ) || COSTELLA_FREE( audCrInternalU ) || \
    COSTELLA_FREE( audCrBoundaryU ) || COSTELLA_FREE( audCrInternalV ) || \
    COSTELLA_FREE( audCrBoundaryV ) ) \
  { \
    COSTELLA_CLEANUP_FUNDAMENTAL_ERROR( "Freeing" ); \
  } \
}



/* CostellaUnblock: 
**
**   Perform the UnBlock algorithm. See above description.
**
**   pi{In,Out}:  Pointer to the {in,out}put image. May be the same.
**
**   bOutYCbCr:  If nonzero, we want the output image in YCbCr format; if 
**     zero, we want the output image in RGB format. Only relevant for color
**     images.
**
**   bPhotographic:  If nonzero, we are telling UnBlock that the input image
**     is of a continuous-tone photographic nature. This is used to provide
**     a slightly better statistical analysis. A zero value does not imply
**     anything about the image.
**    
**   bCartoon:  If nonzero, we are telling UnBlock that the input image is 
**     of a cartoon (solid tone) nature. This is used to provide a slightly
**     better statistical analysis. A zero value does not imply anything
**     about the image.
*/

COSTELLA_FUNCTION( CostellaUnblock, ( COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* 
  piOut, COSTELLA_B bPhotographic, COSTELLA_B bCartoon, 
  COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* poPassback ) )
{
  COSTELLA_B bColor, bSmoothlyUpsampleChrominance, bInYCbCr, bOutYCbCr;
  COSTELLA_UB* aubYAdjustedU = 0, * aubYAdjustedV = 0, * aubCbAdjustedU = 0,
    * aubCbAdjustedV = 0, * aubCrAdjustedU = 0, * aubCrAdjustedV = 0;
  COSTELLA_SW* aswBufferY = 0, * aswBufferCb = 0, * aswBufferCr = 0;
  COSTELLA_UD udTotalLuminance, udTotalChrominance;
  COSTELLA_UD* audYInternalU = 0, * audYBoundaryU = 0, * audYInternalV = 0, 
    * audYBoundaryV = 0, * audCrInternalU = 0, * audCrBoundaryU = 0, 
    * audCrInternalV = 0, * audCrBoundaryV = 0, * audCbInternalU = 0, 
    * audCbBoundaryU = 0, * audCbInternalV = 0, * audCbBoundaryV = 0;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }

    if( !piIn )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null input image" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }

    if( !piOut )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null output image" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Extract information.
  */

  bColor = piIn->bColor;
  bInYCbCr = !piIn->bRgb;


  /* Check that the width and height are nonzero.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piIn->udWidth )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Zero width" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }

    if( !piIn->udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Zero height" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Extract flags.
  */
  
  if( bColor )
  {
    bSmoothlyUpsampleChrominance = !piOut->bDownsampledChrominance;
    bInYCbCr = !piIn->bRgb;
    bOutYCbCr = !piOut->bRgb;
  }
  else
  {
    bSmoothlyUpsampleChrominance = COSTELLA_FALSE;
    bInYCbCr = COSTELLA_FALSE;
    bOutYCbCr = COSTELLA_FALSE;
  }


  /* Allocate memory.
  */

  if( COSTELLA_MALLOC( aubYAdjustedU, 256 ) || COSTELLA_MALLOC( 
    aubYAdjustedV, 256 ) || COSTELLA_MALLOC( aubCbAdjustedU, 256 ) || 
    COSTELLA_MALLOC( aubCbAdjustedV, 256 ) || COSTELLA_MALLOC( 
    aubCrAdjustedU, 256 ) || COSTELLA_MALLOC( aubCrAdjustedV, 256 ) || 
    COSTELLA_MALLOC( aswBufferY, 16 ) || COSTELLA_MALLOC( aswBufferCb, 16 
    ) || COSTELLA_MALLOC( aswBufferCr, 16 ) || COSTELLA_MALLOC( 
    audYInternalU, 256 ) || COSTELLA_MALLOC( audYBoundaryU, 256 ) || 
    COSTELLA_MALLOC( audYInternalV, 256 ) || COSTELLA_MALLOC( 
    audYBoundaryV, 256 ) || COSTELLA_MALLOC( audCbInternalU, 256 ) || 
    COSTELLA_MALLOC( audCbBoundaryU, 256 ) || COSTELLA_MALLOC( 
    audCbInternalV, 256 ) || COSTELLA_MALLOC( audCbBoundaryV, 256 ) || 
    COSTELLA_MALLOC( audCrInternalU, 256 ) || COSTELLA_MALLOC( 
    audCrBoundaryU, 256 ) || COSTELLA_MALLOC( audCrInternalV, 256 ) || 
    COSTELLA_MALLOC( audCrBoundaryV, 256 ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Allocating" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* If we have a color image, then we need the input image to be in YCbCr 
  ** format. Check if we have a color input image not in YCbCr format.
  */

  if( bColor && !bInYCbCr )
  {
    /* We have a color image that is not yet in YCbCr format. Convert it 
    ** now.
    */

    if( COSTELLA_CALL( CostellaImageConvertRgbToYcbcr( piIn, piOut, 
      pfProgress, poPassback ) ) )
    {
      COSTELLA_ERROR( "Converting to YCbCr" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }
  }


  /* If we have a color image, we are assuming that its chrominance channels
  ** have been downsampled. Check whether that is true.
  */
  
  if( bColor )
  {
    if( !( ( bInYCbCr ? piIn : piOut )->bDownsampledChrominance ) )
    {
      if( COSTELLA_CALL( CostellaImageChrominanceAverageDownsampleReplicate(
        bInYCbCr ? piIn : piOut, piOut, pfProgress, poPassback ) ) )
      {
        COSTELLA_ERROR( "Downsampling chrominance" );
        COSTELLA_UNBLOCK_CLEANUP;
        COSTELLA_RETURN;
      }
    }
  }


  /* Analyze the vertical discrepancies in the image. Note that a grayscale 
  ** image is still contained in the input image, whereas a color image is 
  ** in the output image.
  */

  if( COSTELLA_CALL( CostellaUnblockComputeVerticalDiscrepancies( 
    aswBufferY, aswBufferCb,aswBufferCr, audYBoundaryU, audYBoundaryV, 
    audCbBoundaryU, audCbBoundaryV, audCrBoundaryU, audCrBoundaryV, 
    audYInternalU, audYInternalV, audCbInternalU, audCbInternalV, 
    audCrInternalU, audCrInternalV, &udTotalLuminance, &udTotalChrominance, 
    bColor ? piOut :piIn, pfProgress, poPassback ) ) )
  {
    COSTELLA_ERROR( "Computing vertical discrepancies" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* Compute the adjustment tables for the vertical disrepancies.
  */

  if( COSTELLA_CALL( CostellaUnblockComputeAdjustments( audYInternalU, 
    audYBoundaryU,udTotalLuminance, bPhotographic, bCartoon, aubYAdjustedU )
    ) || COSTELLA_CALL( CostellaUnblockComputeAdjustments( audYInternalV, 
    audYBoundaryV, udTotalLuminance, bPhotographic, bCartoon, aubYAdjustedV
    ) ) )
  {
    COSTELLA_ERROR( "Computing vertical Y adjustments" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }

  if( bColor )
  {
    if( COSTELLA_CALL( CostellaUnblockComputeAdjustments( audCbInternalU, 
      audCbBoundaryU, udTotalChrominance, bPhotographic, bCartoon, 
      aubCbAdjustedU ) ) || COSTELLA_CALL( 
      CostellaUnblockComputeAdjustments( audCbInternalV, audCbBoundaryV, 
      udTotalChrominance, bPhotographic, bCartoon, aubCbAdjustedV ) ) ) 
    {
      COSTELLA_ERROR( "Computing vertical Cb adjustments" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }

    if( COSTELLA_CALL( CostellaUnblockComputeAdjustments( audCrInternalU, 
      audCrBoundaryU, udTotalChrominance, bPhotographic, bCartoon, 
      aubCrAdjustedU ) ) || COSTELLA_CALL(     
      CostellaUnblockComputeAdjustments( audCrInternalV, audCrBoundaryV,
      udTotalChrominance, bPhotographic, bCartoon, aubCrAdjustedV ) ) )
    {
      COSTELLA_ERROR( "Computing vertical Cr adjustments" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }
  }


  /* Progress callback.
  */

  if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
  {
    COSTELLA_ERROR( "Progress callback" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* Debug dump of frequency distributions and adjustment tables.
  */

  #ifdef COSTELLA_UNBLOCK_DEBUG_DUMP
  {
    CostellaUnblockDebugDump( aubYAdjustedU, aubYAdjustedV, aubCbAdjustedU, 
      aubCbAdjustedV, aubCrAdjustedU, aubCrAdjustedV, audYInternalU, 
      audYInternalV, audCbInternalU, audCbInternalV, audCrInternalU, 
      audCrInternalV, audYBoundaryU, audYBoundaryV, audCbBoundaryU, 
      audCbBoundaryV, audCrBoundaryU, audCrBoundaryV, COSTELLA_TRUE, 
      bColor );
  }
  #endif


  /* Correct the vertical discrepancies. Note that a grayscale image is 
  ** still in the input image, whereas a color image is in the output image.
  */

  if( COSTELLA_CALL( CostellaUnblockCorrectVerticalDiscrepancies( 
    aswBufferY, aswBufferCb, aswBufferCr, aubYAdjustedU, aubYAdjustedV, 
    aubCbAdjustedU, aubCbAdjustedV, aubCrAdjustedU, aubCrAdjustedV, bColor ?
    piOut : piIn, piOut, pfProgress, poPassback ) ) )
  {
    COSTELLA_ERROR( "Correcting vertical discrepancies" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* Compute the horizontal discrepancies. The image is now in the output
  ** image, regardless of type.
  */
  
  if( COSTELLA_CALL( CostellaUnblockComputeHorizontalDiscrepancies( 
    aswBufferY, aswBufferCb, aswBufferCb, audYBoundaryU, audYBoundaryV, 
    audCbBoundaryU, audCbBoundaryV, audCrBoundaryU, audCrBoundaryV, 
    audYInternalU, audYInternalV, audCbInternalU, audCbInternalV, 
    audCrInternalU, audCrInternalV, &udTotalLuminance, &udTotalChrominance, 
    piOut, pfProgress, poPassback ) ) )
  {
    COSTELLA_ERROR( "Computing horizontal discrepancies" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* Compute the adjustment tables for the horizontal disrepancies.
  */

  if( COSTELLA_CALL( CostellaUnblockComputeAdjustments( audYInternalU, 
    audYBoundaryU, udTotalLuminance, bPhotographic, bCartoon, aubYAdjustedU
    ) ) || COSTELLA_CALL( CostellaUnblockComputeAdjustments( audYInternalV, 
    audYBoundaryV, udTotalLuminance, bPhotographic, bCartoon, aubYAdjustedV
    ) ) )
  {
    COSTELLA_ERROR( "Computing Y horizontal adjustments" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }

  if( bColor )
  {
    if( COSTELLA_CALL( CostellaUnblockComputeAdjustments( audCbInternalU, 
      audCbBoundaryU, udTotalChrominance, bPhotographic, bCartoon, 
      aubCbAdjustedU ) ) || COSTELLA_CALL( 
      CostellaUnblockComputeAdjustments( audCbInternalV, audCbBoundaryV,
      udTotalChrominance, bPhotographic, bCartoon, aubCbAdjustedV ) ) )
    {
      COSTELLA_ERROR( "Computing Cb horizontal adjustments" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }

    if( COSTELLA_CALL( CostellaUnblockComputeAdjustments( audCrInternalU, 
      audCrBoundaryU, udTotalChrominance, bPhotographic, bCartoon, 
      aubCrAdjustedU ) ) || COSTELLA_CALL( 
      CostellaUnblockComputeAdjustments( audCrInternalV, audCrBoundaryV,
      udTotalChrominance, bPhotographic, bCartoon, aubCrAdjustedV ) ) )
    {
      COSTELLA_ERROR( "Computing Cr horizontal adjustments" );
      COSTELLA_UNBLOCK_CLEANUP;
      COSTELLA_RETURN;
    }
  }


  /* Progress callback.
  */

  if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
  {
    COSTELLA_ERROR( "Progress callback" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* Debug dump of frequency distributions and adjustment tables.
  */

  #ifdef COSTELLA_UNBLOCK_DEBUG_DUMP
  {
    CostellaUnblockDebugDump( aubYAdjustedU, aubYAdjustedV, aubCbAdjustedU, 
      aubCbAdjustedV, aubCrAdjustedU, aubCrAdjustedV, audYInternalU, 
      audYInternalV, audCbInternalU, audCbInternalV, audCrInternalU, 
      audCrInternalV, audYBoundaryU, audYBoundaryV, audCbBoundaryU, 
      audCbBoundaryV, audCrBoundaryU, audCrBoundaryV, 0, bColor );
  }
  #endif


  /* Correct the horizontal boundary discrepancies. All images are now 
  ** contained in the output image, regardless of type.
  */

  if( COSTELLA_CALL( CostellaUnblockCorrectHorizontalDiscrepancies( 
    aswBufferY, aswBufferCb, aswBufferCr, aubYAdjustedU, aubYAdjustedV, 
    aubCbAdjustedU, aubCbAdjustedV, aubCrAdjustedU, aubCrAdjustedV, piOut, 
    piOut, pfProgress, poPassback ) ) )
  {
    COSTELLA_ERROR( "Correcting horizontal discrepancies" );
    COSTELLA_UNBLOCK_CLEANUP;
    COSTELLA_RETURN;
  }


  /* If we have a color image, then we need to check two things: whether we
  ** want to upsample the chrominance components, and whether we want to 
  ** convert the output to RGB format. Check if we have a color image.
  */
  
  if( bColor )
  {
    /* We have a color image. Check if we want to smoothy upsample the 
    ** chrominance.
    */

    if( bSmoothlyUpsampleChrominance ) 
    {
      /* We want to upsample the chrominance. Do it.
      */

      if( COSTELLA_CALL( CostellaImageChrominanceMagicUpsample( piOut, 
        piOut, pfProgress, poPassback ) ) )
      {
        COSTELLA_ERROR( "Upsampling chrominance" );
        COSTELLA_UNBLOCK_CLEANUP;
        COSTELLA_RETURN;
      }
    }
    else
    {
      /* We don't want to upsample the chrominance. However, in the above we
      ** have only stored the chrominance values in the top-left pixels of
      ** the 2 x 2 blocks. We need to replicate these into the other three
      ** positions in each 2 x 2 block.
      */

      if( COSTELLA_CALL( CostellaImageChrominanceReplicateEq( piOut, 
        pfProgress, poPassback ) ) )
      {
        COSTELLA_ERROR( "Replicating chrominance" );
        COSTELLA_UNBLOCK_CLEANUP;
        COSTELLA_RETURN;
      }
    }


    /* Check if we want our output in RGB space.
    */

    if( !bOutYCbCr )
    {
      /* We want the output to be in RGB space. Convert it.
      */

      if( COSTELLA_CALL( CostellaImageConvertYcbcrToRgb( piOut, piOut, 
        pfProgress, poPassback ) ) ) 
      {
        COSTELLA_ERROR( "Converting to RGB" );
        COSTELLA_UNBLOCK_CLEANUP;
        COSTELLA_RETURN;
      }
    }
  }


  /* Clean up.
  */

  if( COSTELLA_FREE( aubYAdjustedU ) || COSTELLA_FREE( aubYAdjustedV ) || 
    COSTELLA_FREE( aubCbAdjustedU ) || COSTELLA_FREE( aubCbAdjustedV ) || 
    COSTELLA_FREE( aubCrAdjustedU ) || COSTELLA_FREE( aubCrAdjustedV ) || 
    COSTELLA_FREE( aswBufferY ) || COSTELLA_FREE( aswBufferCb ) || 
    COSTELLA_FREE( aswBufferCr ) || COSTELLA_FREE( audYInternalU ) || 
    COSTELLA_FREE( audYBoundaryU ) || COSTELLA_FREE( audYInternalV ) || 
    COSTELLA_FREE( audYBoundaryV ) || COSTELLA_FREE( audCbInternalU ) || 
    COSTELLA_FREE( audCbBoundaryU ) || COSTELLA_FREE( audCbInternalV ) || 
    COSTELLA_FREE( audCbBoundaryV ) || COSTELLA_FREE( audCrInternalU ) || 
    COSTELLA_FREE( audCrBoundaryU ) || COSTELLA_FREE( audCrInternalV ) || 
    COSTELLA_FREE( audCrBoundaryV ) ) 
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Freeing" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaUnblockComputeVerticalDiscrepancies: 
**
**   Internal function that computes the vertical discrepancies in an image.
**
**   aswBuffer{Y,Cb,Cr}:  Buffer for the {Y,Cb,Cr} channel.
**
**   aud{Y,Cb,Cr}{Boundary,Internal}{U,V}:  Array to hold the frequencies 
**     for the {U,V} discrepancies {at block boundaries, internal to each
**     block} for the {Y,Cb,Cr} channel.
**
**   pudTotal{Luminance,Chrominance}:  Pointer to a COSTELLA_UD, into 
**     which will be written the sum of all the {luminance,chrominance} 
**     values of the image.
**
**   pi:  Pointer to the image to be analyzed.
*/

static COSTELLA_FUNCTION( CostellaUnblockComputeVerticalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UD* audYBoundaryU, COSTELLA_UD* audYBoundaryV, 
  COSTELLA_UD* audCbBoundaryU, COSTELLA_UD* audCbBoundaryV, COSTELLA_UD* 
  audCrBoundaryU, COSTELLA_UD* audCrBoundaryV, COSTELLA_UD* audYInternalU, 
  COSTELLA_UD* audYInternalV, COSTELLA_UD* audCbInternalU, COSTELLA_UD* 
  audCbInternalV, COSTELLA_UD* audCrInternalU, COSTELLA_UD* audCrInternalV, 
  COSTELLA_UD* pudTotalLuminance, COSTELLA_UD* pudTotalChrominance, 
  COSTELLA_IMAGE* pi, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
{
  COSTELLA_B bColor;
  COSTELLA_UB ubPosition;
  COSTELLA_SW swYBoundaryU, swYBoundaryV, swYInternalU, swYInternalV, 
    swCbBoundaryU, swCbBoundaryV, swCbInternalU, swCbInternalV, 
    swCrBoundaryU, swCrBoundaryV, swCrInternalU, swCrInternalV;
  COSTELLA_SW* pswBufferY, * pswBufferYOld, * pswBufferCb, * pswBufferCr, 
    * pswBufferCbOld, * pswBufferCrOld;
  COSTELLA_SD sdRowStride, sdDoubleRowStride;
  COSTELLA_UD udWidth, udHeight, udRow, udColumn, udColumnRight, 
    udTotalLuminance, udTotalChrominance;
  COSTELLA_IMAGE_GRAY* pig;
  COSTELLA_IMAGE_COLOR* pic;
  COSTELLA_IMAGE_COLOR_PIXEL icp={0}, icpStart={0};
  COSTELLA_IMAGE_GRAY_PIXEL igp=NULL, igpStart=NULL;


  /* Extract information.
  */

  bColor = pi->bColor;

  udWidth = pi->udWidth;
  udHeight = pi->udHeight;

  sdRowStride = pi->sdRowStride;

  pig = &pi->ig;
  pic = &pi->ic;


  /* Compute double the row stride.
  */

  sdDoubleRowStride = sdRowStride << 1;


  /* Initialize totals.
  */

  udTotalLuminance = 0;
  udTotalChrominance = 0;


  /* Start by initializing the frequency tables.
  */

  COSTELLA_INITIALIZE_ARRAY( audYBoundaryU, 256, COSTELLA_UD );
  COSTELLA_INITIALIZE_ARRAY( audYBoundaryV, 256, COSTELLA_UD );

  COSTELLA_INITIALIZE_ARRAY( audYInternalU, 256, COSTELLA_UD );
  COSTELLA_INITIALIZE_ARRAY( audYInternalV, 256, COSTELLA_UD );

  if( bColor )
  {
    COSTELLA_INITIALIZE_ARRAY( audCbBoundaryU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCbBoundaryV, 256, COSTELLA_UD );

    COSTELLA_INITIALIZE_ARRAY( audCrBoundaryU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCrBoundaryV, 256, COSTELLA_UD );

    COSTELLA_INITIALIZE_ARRAY( audCbInternalU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCbInternalV, 256, COSTELLA_UD );

    COSTELLA_INITIALIZE_ARRAY( audCrInternalU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCrInternalV, 256, COSTELLA_UD );
  }


  /* First we analyze the luminance channel. Start at top-left of image.
  */

  if( bColor )
  {
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpStart, *pic, udWidth, 
      udHeight, sdRowStride );
  }
  else
  {
    COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( igpStart, *pig, udWidth, 
      udHeight, sdRowStride );
  }

    
  /* Walk through the rows of the image.
  */

  for( udRow = 0; udRow < udHeight; udRow++ )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" );
      COSTELLA_RETURN;
    }


    /* Start off at the pixel to the right of the leftmost pixel of this 
    ** row, i.e., at x = 1, where the leftmost pixel is x = 0. 
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpStart, icp );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( igpStart, igp );
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igp );
    }


    /* We start off by filling positions 8 and 9 of the values array from 
    ** the x = 1 and x = 2 columns of this image. These will be shifted back
    ** to the start of the values array in the first step below.
    **
    ** Switch on image type.
    */

    if( bColor )
    {
      for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 2; 
        ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp ) )
      {
        *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( 
          icp );
      }
    }
    else
    {
      for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 2; 
        ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igp ) )
      {
        *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( igp 
          ); 
        }
    }


    /* Walk through the vertical boundaries. We only measure discrepancies 
    ** for a block if it has a right boundary, i.e., at least one pixel to 
    ** its right; this ensures that the total counts for internal and 
    ** boundary discrepancies will be identically equal. We ensure this by
    ** checking if the column that is eight pixels to the right of our block
    ** is within the image.
    */

    for( udColumnRight = 8; udColumnRight < udWidth; udColumnRight += 8 )
    {
      /* Get the first two pixels from what is already in the array. 
      */

      for( ubPosition = 0, pswBufferY = aswBufferY, pswBufferYOld = 
        aswBufferY + 8; ubPosition < 2; ubPosition++ )
      {
        *pswBufferY++ = *pswBufferYOld++;
      }


      /* Extract the next eight pixels from the image. Need to make sure 
      ** that we don't go past the right edge of the image. Switch on image 
      ** type.
      */

      if( bColor )
      {
        for( udColumn = udColumnRight - 5; ubPosition < 10 && udColumn < 
          udWidth; udColumn++, ubPosition++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icp ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( 
            icp );
        }
      }
      else
      {
        for( udColumn = udColumnRight - 5; ubPosition < 10 && udColumn < 
          udWidth; udColumn++, ubPosition++, 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igp ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( igp
            );
        }
      }


      /* Fill any unfilled entries with -1, which the discrepancy function
      ** recognizes as missing entries.
      */

      for( ; ubPosition < 10; ubPosition++ )
      {
        *pswBufferY++ = -1;
      }


      /* Compute discrepancies. 
      */

      if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferY + 
        4, &swYBoundaryU, &swYBoundaryV ) ) || 
        COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferY, 
        &swYInternalU, &swYInternalV ) ) )
      {
        COSTELLA_ERROR( "Computing Y discrepancies" );
        COSTELLA_RETURN;
      }
    

      /* Take absolute values.
      */

      swYBoundaryU = abs( swYBoundaryU );
      swYBoundaryV = abs( swYBoundaryV );

      swYInternalU = abs( swYInternalU );
      swYInternalV = abs( swYInternalV );


      /* Update frequency tables.
      */

      audYBoundaryU[ swYBoundaryU ]++;        
      audYBoundaryV[ swYBoundaryV ]++;        

      audYInternalU[ swYInternalU ]++;        
      audYInternalV[ swYInternalV ]++;        

      udTotalLuminance++;
    }


    /* Walk down to the next row.
    */

    if( bColor ) 
    {
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpStart, sdRowStride );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpStart, sdRowStride );
    }
  }


  /* If we have a color image, we now analyze the discrepancies for the
  ** downsampled chrominance components.
  */

  if( bColor )
  {
    /* Start at the top-left of the image. 
    */

    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpStart, *pic, udWidth, 
      udHeight, sdRowStride );


    /* Walk through those rows that contain chrominance data.
    */

    for( udRow = 0; udRow < udHeight; udRow += 2 )
    {
      /* Progress callback.
      */

      if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
      {
        COSTELLA_ERROR( "Progress callback" );
        COSTELLA_RETURN;
      }


      /* Start off at the pixel to the right of the leftmost downsampled 
      ** pixel of this row, i.e., at x = 2, where the leftmost downsampled
      ** pixel is x = 0.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpStart, icp );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icp );


      /* We start off by filling positions 8 and 9 of the values arrays from
      ** the xd = 1 and xd = 2 downsampled columns of this image, i.e., from
      ** x = 2 and x = 4 of the actual image. These will be shifted back to 
      ** the start of the values arrays in the first step below. Switch on
      ** image type.
      */

      for( ubPosition = 0, pswBufferCb = aswBufferCb + 8, pswBufferCr = 
        aswBufferCr + 8; ubPosition < 2; ubPosition++, 
        COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icp ) )
      {
        *pswBufferCb++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( 
          icp );
        *pswBufferCr++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( 
          icp );
      }


      /* Walk through the vertical boundaries. See above comments re only
      ** using blocks with a right boundary.
      */

      for( udColumnRight = 16; udColumnRight < udWidth; udColumnRight += 16 
        )
      {
        /* Get the first two pixels from what is already in the array. 
        */

        for( ubPosition = 0, pswBufferCb = aswBufferCb, pswBufferCbOld = 
          aswBufferCb + 8, pswBufferCr = aswBufferCr, pswBufferCrOld = 
          aswBufferCr + 8; ubPosition < 2; ubPosition++ )
        {
          *pswBufferCb++ = *pswBufferCbOld++;
          *pswBufferCr++ = *pswBufferCrOld++;
        }


        /* Extract the next eight downsampled pixels from the image. Need to
        ** make sure that we don't go past the right edge of the image. 
        ** Switch on image type.
        */

        for( udColumn = udColumnRight - 10; ubPosition < 10 && udColumn < 
          udWidth; udColumn += 2, ubPosition++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icp ) )
        {
          *pswBufferCb++ = (COSTELLA_SW) 
            COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icp );
          *pswBufferCr++ = (COSTELLA_SW) 
            COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icp );
        }


        /* Fill any unfilled entries with -1, which the discrepancy function
        ** recognizes as missing entries.
        */

        for( ; ubPosition < 10; ubPosition++ )
        {
          *pswBufferCb++ = -1;
          *pswBufferCr++ = -1;
        }


        /* Compute discrepancies.
        */

        if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferCb 
          + 4, &swCbBoundaryU, &swCbBoundaryV ) ) || COSTELLA_CALL(
          CostellaUnblockComputeDiscrepancies( aswBufferCr + 4, 
          &swCrBoundaryU, &swCrBoundaryV ) ) || COSTELLA_CALL( 
          CostellaUnblockComputeDiscrepancies( aswBufferCb, &swCbInternalU, 
          &swCbInternalV ) ) || COSTELLA_CALL( 
          CostellaUnblockComputeDiscrepancies( aswBufferCr, &swCrInternalU, 
          &swCrInternalV ) ) )
        {
          COSTELLA_ERROR( "Computing Cb or Cr discrepancies" );
          COSTELLA_RETURN;
        }


        /* Take absolute values.
        */

        swCbBoundaryU = abs( swCbBoundaryU );
        swCrBoundaryU = abs( swCrBoundaryU );

        swCbBoundaryV = abs( swCbBoundaryV );
        swCrBoundaryV = abs( swCrBoundaryV );

        swCbInternalU = abs( swCbInternalU );
        swCrInternalU = abs( swCrInternalU );

        swCbInternalV = abs( swCbInternalV );
        swCrInternalV = abs( swCrInternalV );


        /* Update totals.
        */

        audCbBoundaryU[ swCbBoundaryU ]++;        
        audCrBoundaryU[ swCrBoundaryU ]++;        

        audCbBoundaryV[ swCbBoundaryV ]++;        
        audCrBoundaryV[ swCrBoundaryV ]++;        

        audCbInternalU[ swCbInternalU ]++;        
        audCrInternalU[ swCrInternalU ]++;        
      
        audCbInternalV[ swCbInternalV ]++;        
        audCrInternalV[ swCrInternalV ]++;        

        udTotalChrominance++;
      }


      /* Walk down to the next downsampled row.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpStart, sdDoubleRowStride 
        );
    }
  }


  /* Store the totals.
  */

  *pudTotalLuminance = udTotalLuminance;
  *pudTotalChrominance = udTotalChrominance;
}
COSTELLA_END_FUNCTION



/* CostellaUnblockComputeHorizontalDiscrepancies: 
**
**   Internal function that computes the horizontal discrepancies in an 
**   image.
**
**   aud{Y,Cb,Cr}{Boundary,Internal}{U,V}:  Array to hold the frequencies 
**     for the {U,V} discrepancies {at block boundaries, internal to each
**     block} for the {Y,Cb,Cr} channel.
**
**   pudTotal{Luminance,Chrominance}:  Pointer to a COSTELLA_UD, into 
**     which will be written the sum of all the {luminance,chrominance} 
**     values of the image.
**
**   pi:  Pointer to the image to be analyzed.
*/

static COSTELLA_FUNCTION( CostellaUnblockComputeHorizontalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UD* audYBoundaryU, COSTELLA_UD* audYBoundaryV, 
  COSTELLA_UD* audCbBoundaryU, COSTELLA_UD* audCbBoundaryV, COSTELLA_UD* 
  audCrBoundaryU, COSTELLA_UD* audCrBoundaryV, COSTELLA_UD* audYInternalU, 
  COSTELLA_UD* audYInternalV, COSTELLA_UD* audCbInternalU, COSTELLA_UD* 
  audCbInternalV, COSTELLA_UD* audCrInternalU, COSTELLA_UD* audCrInternalV, 
  COSTELLA_UD* pudTotalLuminance, COSTELLA_UD* pudTotalChrominance, 
  COSTELLA_IMAGE* pi, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
{
  COSTELLA_B bColor;
  COSTELLA_UB ubPosition;
  COSTELLA_SW swYBoundaryU, swYBoundaryV, swYInternalU, swYInternalV, 
    swCbBoundaryU, swCbBoundaryV, swCbInternalU, swCbInternalV, 
    swCrBoundaryU, swCrBoundaryV, swCrInternalU, swCrInternalV;
  COSTELLA_SW* pswBufferY, * pswBufferYOld, * pswBufferCb, * pswBufferCr, 
    * pswBufferCbOld, * pswBufferCrOld;
  COSTELLA_SD sdRowStride, sdDoubleRowStride;
  COSTELLA_UD udWidth, udHeight, udRow, udColumn, udRowBottom, 
    udTotalLuminance, udTotalChrominance;
  COSTELLA_IMAGE_GRAY* pig;
  COSTELLA_IMAGE_COLOR* pic;
  COSTELLA_IMAGE_COLOR_PIXEL icp={0}, icpStart={0};
  COSTELLA_IMAGE_GRAY_PIXEL igp=NULL, igpStart=NULL;


  /* Extract information.
  */

  bColor = pi->bColor;

  udWidth = pi->udWidth;
  udHeight = pi->udHeight;

  sdRowStride = pi->sdRowStride;

  pig = &pi->ig;
  pic = &pi->ic;


  /* Compute double the row stride.
  */

  sdDoubleRowStride = sdRowStride << 1;


  /* Initialize totals.
  */

  udTotalLuminance = 0;
  udTotalChrominance = 0;


  /* Initialize the frequency tables.
  */

  COSTELLA_INITIALIZE_ARRAY( audYBoundaryU, 256, COSTELLA_UD );
  COSTELLA_INITIALIZE_ARRAY( audYBoundaryV, 256, COSTELLA_UD );

  COSTELLA_INITIALIZE_ARRAY( audYInternalU, 256, COSTELLA_UD );
  COSTELLA_INITIALIZE_ARRAY( audYInternalV, 256, COSTELLA_UD );

  if( bColor )
  {
    COSTELLA_INITIALIZE_ARRAY( audCbBoundaryU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCbBoundaryV, 256, COSTELLA_UD );

    COSTELLA_INITIALIZE_ARRAY( audCrBoundaryU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCrBoundaryV, 256, COSTELLA_UD );

    COSTELLA_INITIALIZE_ARRAY( audCbInternalU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCbInternalV, 256, COSTELLA_UD );

    COSTELLA_INITIALIZE_ARRAY( audCrInternalU, 256, COSTELLA_UD );
    COSTELLA_INITIALIZE_ARRAY( audCrInternalV, 256, COSTELLA_UD );
  }


  /* First we analyze the luminance channel. Start at the top-left of the
  ** image.
  */

  if( bColor )
  {
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpStart, *pic, udWidth, 
      udHeight, sdRowStride );
  }
  else
  {
    COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( igpStart, *pig, udWidth, 
      udHeight, sdRowStride );
  }
  

  /* Walk through the columns of the image.
  */

  for( udColumn = 0; udColumn < udWidth; udColumn++ )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" );
      COSTELLA_RETURN;
    }


    /* Start off at the pixel below the topmost pixel of this column, i.e., 
    ** at y = 1, where the topmost pixel is y = 0.
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpStart, icp );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icp, sdRowStride );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( igpStart, igp );
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igp, sdRowStride );
    }


    /* We start off by filling positions 8 and 9 of the values arrays from 
    ** the y = 1 and y = 2 rows of this image. These will be shifted back to
    ** the start of the values arrays in the first step below. Switch on 
    ** image type.
    */

    if( bColor )
    {
      for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 2; 
        ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icp, sdRowStride
        ) )
      {
        *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( 
          icp );
      }
    }
    else
    {
      for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 2; 
        ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igp, sdRowStride 
        ) )
      {
        *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( igp 
          );
      }
    }


    /* Walk through the horizontal boundaries. See above comments.
    */

    for( udRowBottom = 8; udRowBottom < udHeight; udRowBottom += 8 )
    {
      /* Get the first two pixels from what is already in the array. 
      */

      for( ubPosition = 0, pswBufferY = aswBufferY, pswBufferYOld = 
        aswBufferY + 8; ubPosition < 2; ubPosition++ )
      {
        *pswBufferY++ = *pswBufferYOld++;
      }


      /* Extract the next eight pixels from the image. Need to make sure 
      ** that we don't go past the bottom of the image. Switch on image 
      ** type.
      */

      if( bColor )
      {
        for( udRow = udRowBottom - 5; ubPosition < 10 && udRow < udHeight; 
          udRow++, ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icp, 
          sdRowStride ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( 
            icp );
        }
      }
      else
      {
        for( udRow = udRowBottom - 5; ubPosition < 10 && udRow < udHeight; 
          udRow++, ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igp, 
          sdRowStride ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( igp
            );
        }
      }


      /* Fill any unfilled entries with -1, which the discrepancy function
      ** recognizes as missing entries.
      */

      for( ; ubPosition < 10; ubPosition++ )
      {
        *pswBufferY++ = -1;
      }


      /* Compute discrepancies. 
      */

      if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferY + 
        4, &swYBoundaryU, &swYBoundaryV ) ) || COSTELLA_CALL( 
        CostellaUnblockComputeDiscrepancies( aswBufferY, &swYInternalU, 
        &swYInternalV ) ) )
      {
        COSTELLA_ERROR( "Computing Y discrepancies" );
        COSTELLA_RETURN;
      }


      /* Take absolute values.
      */

      swYBoundaryU = abs( swYBoundaryU );
      swYBoundaryV = abs( swYBoundaryV );

      swYInternalU = abs( swYInternalU );
      swYInternalV = abs( swYInternalV );


      /* Update totals.
      */

      audYBoundaryU[ swYBoundaryU ]++;        
      audYBoundaryV[ swYBoundaryV ]++;        

      audYInternalU[ swYInternalU ]++;        
      audYInternalV[ swYInternalV ]++;        

      udTotalLuminance++;
    }


    /* Walk across to the next column.
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpStart );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpStart );
    }
  }


  /* If we have a color image, analyze the discrepancies for the downsampled
  ** chrominance channels. 
  */
  
  if( bColor )
  {
    /* Start at the top-left of the image. 
    */

    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpStart, *pic, udWidth, 
      udHeight, sdRowStride );

      
    /* Walk through those columns of the image that contain chrominance 
    ** data.
    */

    for( udColumn = 0; udColumn < udWidth; udColumn += 2 )
    {
      /* Progress callback.
      */

      if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
      {
        COSTELLA_ERROR( "Progress callback" );
        COSTELLA_RETURN;
      }


      /* Start off at the downsampled pixel below the topmost pixel of this 
      ** column, i.e., at y = 2, where the topmost downsampled pixel is 
      ** y = 0.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpStart, icp );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icp, sdDoubleRowStride );


      /* We start off by filling positions 8 and 9 of the values arrays from
      ** the yd = 1 and yd = 2 downsampled pixel rows of this image, i.e., 
      ** at y = 2 and y = 4 in the actual image. These will be shifted back 
      ** to the start of the values arrays in the first step below. Switch 
      ** on image type.
      */

      for( ubPosition = 0, pswBufferCb = aswBufferCb + 8, pswBufferCr = 
        aswBufferCr + 8; ubPosition < 2; ubPosition++, 
        COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icp, sdDoubleRowStride ) )
      {
        *pswBufferCb++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( 
          icp );
        *pswBufferCr++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( 
          icp );
      }


      /* Walk through the horizontal boundaries. See above comments.
      */

      for( udRowBottom = 16; udRowBottom < udHeight; udRowBottom += 16 )
      {
        /* Get the first two pixels from what is already in the array. 
        */

        for( ubPosition = 0, pswBufferCb = aswBufferCb, pswBufferCbOld = 
          aswBufferCb + 8, pswBufferCr = aswBufferCr, pswBufferCrOld = 
          aswBufferCr + 8; ubPosition < 2; ubPosition++ )
        {
          *pswBufferCb++ = *pswBufferCbOld++;
          *pswBufferCr++ = *pswBufferCrOld++;
        }


        /* Extract the next eight pixels from the image. Need to make sure 
        ** that we don't go past the bottom edge of the image. Switch on 
        ** image type.
        */

        for( udRow = udRowBottom - 10; ubPosition < 10 && udRow < 
          udHeight; udRow += 2, ubPosition++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icp, sdDoubleRowStride )
          )
        {
          *pswBufferCb++ = (COSTELLA_SW) 
            COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icp );
          *pswBufferCr++ = (COSTELLA_SW) 
            COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icp );
        }


        /* Fill any unfilled entries with -1, which the discrepancy function
        ** recognizes as missing entries.
        */

        for( ; ubPosition < 10; ubPosition++ )
        {
          *pswBufferCb++ = -1;
          *pswBufferCr++ = -1;
        }


        /* Compute discrepancies. 
        */

        if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferCb 
          + 4, &swCbBoundaryU, &swCbBoundaryV ) ) || COSTELLA_CALL( 
          CostellaUnblockComputeDiscrepancies( aswBufferCr + 4, 
          &swCrBoundaryU, &swCrBoundaryV ) ) || COSTELLA_CALL(
          CostellaUnblockComputeDiscrepancies( aswBufferCb, &swCbInternalU, 
          &swCbInternalV ) ) || COSTELLA_CALL(
          CostellaUnblockComputeDiscrepancies( aswBufferCr, &swCrInternalU, 
          &swCrInternalV ) ) )
        {
          COSTELLA_ERROR( "Computing Cb or Cr discrepancies" );
          COSTELLA_RETURN;
        }


        /* Take absolute values.
        */

        swCbBoundaryU = abs( swCbBoundaryU );
        swCrBoundaryU = abs( swCrBoundaryU );

        swCbBoundaryV = abs( swCbBoundaryV );
        swCrBoundaryV = abs( swCrBoundaryV );

        swCbInternalU = abs( swCbInternalU );
        swCrInternalU = abs( swCrInternalU );

        swCbInternalV = abs( swCbInternalV );
        swCrInternalV = abs( swCrInternalV );


        /* Update totals.
        */

        audCbBoundaryU[ swCbBoundaryU ]++;        
        audCrBoundaryU[ swCrBoundaryU ]++;        

        audCbBoundaryV[ swCbBoundaryV ]++;        
        audCrBoundaryV[ swCrBoundaryV ]++;        

        audCbInternalU[ swCbInternalU ]++;        
        audCrInternalU[ swCrInternalU ]++;        

        audCbInternalV[ swCbInternalV ]++;        
        audCrInternalV[ swCrInternalV ]++;        

        udTotalChrominance++;
      }


      /* Walk across to the next downsampled column.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpStart );
    }
  }


  /* Store totals.
  */

  *pudTotalLuminance = udTotalLuminance;
  *pudTotalChrominance = udTotalChrominance;
}
COSTELLA_END_FUNCTION



/* CostellaUnblockCorrectVerticalDiscrepancies: 
**
**   Correct the vertical discrepancies in an image, based on the supplied 
**   adjustment tables. 
**
**   aswBuffer{Y,Cb,Cr}:  Buffer for the {Y,Cb,Cr} channel.
**
**   aub{Y,Cb,Cr}Adjusted{U,V}:  Adjustment tables.
**
**   pi{In,Out}:  Pointer to the {in,out}put image.
*/

static COSTELLA_FUNCTION( CostellaUnblockCorrectVerticalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UB* aubYAdjustedU, COSTELLA_UB* aubYAdjustedV, 
  COSTELLA_UB* aubCbAdjustedU, COSTELLA_UB* aubCbAdjustedV, COSTELLA_UB* 
  aubCrAdjustedU, COSTELLA_UB* aubCrAdjustedV, COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
{
  COSTELLA_B bAlpha, bCopyAlpha, bColor;
  COSTELLA_UB ubPosition, ubY, ubCb, ubCr, ubA;
  COSTELLA_SW swYU, swYV, swCbU, swCbV, swCrU, swCrV;
  COSTELLA_SW* pswBufferY, * pswBufferYOld, * pswBufferCb, * pswBufferCbOld,
    * pswBufferCr, * pswBufferCrOld;
  COSTELLA_UD udWidth, udHeight, udRow, udColumn, udColumnBoundaryRight;
  COSTELLA_SD sdRowStrideIn, sdRowStrideOut, sdDoubleRowStrideIn, 
    sdDoubleRowStrideOut, sdAlphaRowStrideIn, sdAlphaRowStrideOut;
  COSTELLA_IMAGE_ALPHA* piaIn, * piaOut;
  COSTELLA_IMAGE_GRAY* pigIn, * pigOut;
  COSTELLA_IMAGE_COLOR* picIn, * picOut;
  COSTELLA_IMAGE_ALPHA_PIXEL iapInStart=NULL, iapOutStart=NULL, iapIn=NULL, iapOut=NULL;
  COSTELLA_IMAGE_GRAY_PIXEL igpInStart=NULL, igpOutStart=NULL, igpIn=NULL, igpOut=NULL;
  COSTELLA_IMAGE_COLOR_PIXEL icpInStart={0}, icpOutStart={0}, icpIn={0}, icpOut={0};


  /* Extract information.
  */

  bAlpha = piIn->bAlpha;
  bColor = piIn->bColor;

  udWidth = piIn->udWidth;
  udHeight = piIn->udHeight;

  sdRowStrideIn = piIn->sdRowStride;
  sdRowStrideOut = piOut->sdRowStride;

  sdAlphaRowStrideIn = piIn->sdAlphaRowStride;
  sdAlphaRowStrideOut = piOut->sdAlphaRowStride;

  piaIn = &piIn->ia;
  piaOut = &piOut->ia;

  pigIn = &piIn->ig;
  pigOut = &piOut->ig;

  picIn = &piIn->ic;
  picOut = &piOut->ic;


  /* Check that the output image is consistent.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piOut->bAlpha != !bAlpha )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Alpha flag disagrees" );
      COSTELLA_RETURN;
    }

    if( !piOut->bColor != !bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Color flag disagrees" );
      COSTELLA_RETURN;
    }

    if( piOut->udWidth != udWidth )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Width disagrees" );
      COSTELLA_RETURN;
    }

    if( piOut->udHeight != udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Height disagrees" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Determine whether we need to copy across alpha values.
  */

  bCopyAlpha = bAlpha && !COSTELLA_IMAGE_ALPHA_IS_SAME( piaIn, piaOut );


  /* Compute the double row strides.
  */

  sdDoubleRowStrideIn = sdRowStrideIn << 1;
  sdDoubleRowStrideOut = sdRowStrideOut << 1;


  /* Start with the luminance channel. Start at top-left of the input and
  ** output images. 
  */

  if( bColor )
  {
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpInStart, *picIn, udWidth, 
      udHeight, sdRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpOutStart, *picOut, udWidth, 
      udHeight, sdRowStrideOut );
  }
  else
  {
    COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( igpInStart, *pigIn, udWidth, 
      udHeight, sdRowStrideIn );
    COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( igpOutStart, *pigOut, udWidth, 
      udHeight, sdRowStrideOut );
  }
  
  if( bCopyAlpha )     
  {
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapInStart, *piaIn, udWidth, 
      udHeight, sdAlphaRowStrideIn );
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapOutStart, *piaOut, udWidth, 
      udHeight, sdAlphaRowStrideOut );
  }


  /* Walk through the rows of the image.
  */

  for( udRow = 0; udRow < udHeight; udRow++ )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" );
      COSTELLA_RETURN;
    }


    /* If there is an alpha channel, copy it across for the whole row now.
    */

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapInStart, iapIn );
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapOutStart, iapOut );
      
      for( udColumn = 0; udColumn < udWidth; udColumn++, 
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapIn ), 
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOut ) )
      {
        ubA = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubA );
      }
    }


    /* Start off at the leftmost pixel of this row, for both input and 
    ** output images. 
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( igpInStart, igpIn );
      COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( igpOutStart, igpOut );
    }


    /* If the width of the image is less than 9 pixels, then there are no
    ** boundaries. Simply copy the row across.
    */

    if( udWidth < 9 )
    {
      if( bColor )
      {
        for( udColumn = 0; udColumn < udWidth; udColumn++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn ), 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut ) )
        {
          ubY = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
        }
      }
      else
      {
        for( udColumn = 0; udColumn < udWidth; udColumn++, 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpIn ), 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpOut ) )
        {
          ubY = COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( igpIn );
          COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( igpOut, ubY );
        }
      }
    }
    else
    {
      /* There are boundaries. Load the values of the first eight input 
      ** pixels of the row into the right block of the values array. This 
      ** will automatically be shifted to the left block in the first step 
      ** below. Switch on image type.
      */

      if( bColor )
      {
        for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 8; 
          ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( 
            icpIn );
        }
      }
      else
      {
        for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 8; 
          ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpIn ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( 
            igpIn );
        }
      }


      /* Walk through all vertical boundaries. See above comments.
      */

      for( udColumnBoundaryRight = 8; udColumnBoundaryRight < udWidth; 
        udColumnBoundaryRight += 8 )
      {
        /* Shift the right block in the array to the left block. 
        */

        for( ubPosition = 0, pswBufferY = aswBufferY, pswBufferYOld = 
          aswBufferY + 8; ubPosition < 8; ubPosition++ )
        {
          *pswBufferY++ = *pswBufferYOld++;
        }


        /* Extract the next eight pixels from the image. Switch on image 
        ** type.
        */

        if( bColor )
        {
          for( udColumn = udColumnBoundaryRight; ubPosition < 16 && udColumn
            < udWidth; udColumn++, ubPosition++, 
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpIn ) )
          {
            *pswBufferY++ = (COSTELLA_SW) 
              COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn ); 
          }
        }
        else
        {
          for( udColumn = udColumnBoundaryRight; ubPosition < 16 && udColumn
            < udWidth; udColumn++, ubPosition++, 
            COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpIn ) )
          {
            *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( 
              igpIn );
          }
        }


        /* Fill any unfilled entries with -1, which the discrepancy and
        ** correction functions recognize as missing entries.
        */

        for( ; ubPosition < 16; ubPosition++ )
        {
          *pswBufferY++ = -1;
        }


        /* Compute discrepancies at the boundary.
        */

        if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferY +
          5, &swYU, &swYV ) ) )
        {
          COSTELLA_ERROR( "Computing Y discrepancies" );
          COSTELLA_RETURN;
        }


        /* Adjust the discrepancies.
        */

        swYU = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swYU, aubYAdjustedU );
        swYV = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swYV, aubYAdjustedV );


        /* Correct the sixteen values for these adjusted discrepancies.
        */

        if( COSTELLA_CALL( CostellaUnblockCorrectDiscrepancies( aswBufferY, 
          swYU, swYV ) ) )
        {
          COSTELLA_ERROR( "Correcting Y discrepancies" );
          COSTELLA_RETURN;
        }


        /* Write the left block pixels to the output image. Switch on image 
        ** type.
        */

        if( bColor )
        {
          for( ubPosition = 0, pswBufferY = aswBufferY; ubPosition < 8; 
            ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut ) )
          {
            ubY = (COSTELLA_UB) *pswBufferY++;
            COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
          }
        }
        else
        {
          for( ubPosition = 0, pswBufferY = aswBufferY; ubPosition < 8; 
            ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpOut ) )
          {
            ubY = (COSTELLA_UB) *pswBufferY++;
            COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( igpOut, ubY );
          }
        }
      }


      /* Write out any remaining pixels in the row. We subtract 8 from the 
      ** right boundary x value because it was incremented before the above 
      ** loop dropped out. Switch on image type.
      */

      if( bColor )
      {
        for( udColumn = udColumnBoundaryRight - 8; ubPosition < 16 && 
          udColumn < udWidth; udColumn++, ubPosition++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOut ) )
        {
          ubY = (COSTELLA_UB) *pswBufferY++;
          COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
        }
      }
      else
      {
        for( udColumn = udColumnBoundaryRight - 8; ubPosition < 16 && 
          udColumn < udWidth; udColumn++, ubPosition++, 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpOut ) )
        {
          ubY = (COSTELLA_UB) *pswBufferY++;
          COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( igpOut, ubY );
        }
      }
    }


    /* Walk down to the next row.
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpInStart, sdRowStrideIn );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOutStart, sdRowStrideOut );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpInStart, sdRowStrideIn );
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpOutStart, sdRowStrideOut );
    }

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapInStart, sdAlphaRowStrideIn 
        );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOutStart, sdAlphaRowStrideOut
        );
    }
  }


  /* If we have a color image, we now correct the vertical boundaries of
  ** the chrominance channels. 
  */

  if( bColor )
  {
    /* Start at the top-left of the image. 
    */

    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpInStart, *picIn, udWidth, 
      udHeight, sdRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpOutStart, *picOut, udWidth, 
      udHeight, sdRowStrideOut );
    

    /* Walk through those rows of the image that contain chrominance data.
    */

    for( udRow = 0; udRow < udHeight; udRow += 2 )
    {
      /* Progress callback.
      */

      if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
      {
        COSTELLA_ERROR( "Progress callback" );
        COSTELLA_RETURN;
      }


      /* Start off at the leftmost downsampled pixel of the row, for both 
      ** the input and the output.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );


      /* If the width of the image is less than 17 pixels, then there are no
      ** boundaries. Simply copy the row across.
      */

      if( udWidth < 17 )
      {
        for( udColumn = 0; udColumn < udWidth; udColumn += 2, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpIn ), 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpOut ) )
        {
          ubCb = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCr = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );
        }
      }
      else
      {
        /* There are boundaries. Load the values of the first eight 
        ** downsampled pixels of the row into the right blocks of the values
        ** arrays. This will automatically be shifted to the left block in 
        ** the first step below. Switch on image type.
        */

        for( ubPosition = 0, pswBufferCb = aswBufferCb + 8, pswBufferCr = 
          aswBufferCr + 8; ubPosition < 8; ubPosition++,      
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpIn ) )
        {
          *pswBufferCb++ = (COSTELLA_SW) 
            COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          *pswBufferCr++ = (COSTELLA_SW) 
            COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
        }


        /* Walk through all vertical boundaries. See above comments.
        */

        for( udColumnBoundaryRight = 16; udColumnBoundaryRight < udWidth; 
          udColumnBoundaryRight += 16 )
        {
          /* Shift the right block in the array to the left block. 
          */

          for( ubPosition = 0, pswBufferCb = aswBufferCb, pswBufferCbOld = 
            aswBufferCb + 8, pswBufferCr = aswBufferCr, pswBufferCrOld = 
            aswBufferCr + 8; ubPosition < 8; ubPosition++ )
          {
            *pswBufferCb++ = *pswBufferCbOld++;
            *pswBufferCr++ = *pswBufferCrOld++;
          }


          /* Extract the next eight downsampled pixels from the image. Need 
          ** to make sure that we don't go past the right edge of the image. 
          ** Switch on image type.
          */

          for( udColumn = udColumnBoundaryRight; ubPosition < 16 && 
            udColumn < udWidth; udColumn += 2, ubPosition++, 
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpIn ) )
          {
            *pswBufferCb++ = (COSTELLA_SW) 
              COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            *pswBufferCr++ = (COSTELLA_SW) 
              COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }


          /* Fill any unfilled entries with -1, which the discrepancy and
          ** correction functions recognize as missing entries.
          */

          for( ; ubPosition < 16; ubPosition++ )
          {
            *pswBufferCb++ = -1;
            *pswBufferCr++ = -1;
          }


          /* Now compute the boundary discrepancies.
          */

          if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( 
            aswBufferCb + 5, &swCbU, &swCbV ) ) || COSTELLA_CALL(
            CostellaUnblockComputeDiscrepancies( aswBufferCr + 5, &swCrU, 
            &swCrV ) ) )
          {
            COSTELLA_ERROR( "Computing Cb or Cr discrepancies" );
            COSTELLA_RETURN;
          }


          /* Adjust the discrepancies.
          */

          swCbU = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCbU, aubCbAdjustedU
            );
          swCbV = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCbV, aubCbAdjustedV
            );

          swCrU = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCrU, aubCrAdjustedU
            );
          swCrV = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCrV, aubCrAdjustedV
            );


          /* Correct the sixteen values for these adjusted discrepancies.
          */

          if( COSTELLA_CALL( CostellaUnblockCorrectDiscrepancies( 
            aswBufferCb, swCbU, swCbV ) ) || COSTELLA_CALL(
            CostellaUnblockCorrectDiscrepancies( aswBufferCr, swCrU, swCrV )
            ) )
          {
            COSTELLA_ERROR( "Correcting Cb or Cr discrepancies" );
            COSTELLA_RETURN;
          }


          /* Write the left block pixels back to the image. Switch on image 
          ** type.
          */

          for( ubPosition = 0, pswBufferCb = aswBufferCb, pswBufferCr = 
            aswBufferCr; ubPosition < 8; ubPosition++, 
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpOut ) )
          {
            ubCb = (COSTELLA_UB) *pswBufferCb++;
            ubCr = (COSTELLA_UB) *pswBufferCr++;

            COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
            COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );
          }
        }


        /* Write out any remaining pixels in the row. Walk through the 
        ** remaining pixels. We subtract 16 from the right boundary x value 
        ** because it was incremented before the above loop dropped out.
        ** Switch on image type.
        */

        for( udColumn = udColumnBoundaryRight - 16; ubPosition < 16 && 
          udColumn < udWidth; udColumn += 2, ubPosition++,             
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpOut ) )
        {
          ubCb = (COSTELLA_UB) *pswBufferCb++;
          ubCr = (COSTELLA_UB) *pswBufferCr++;

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );
        }
      }
    

      /* Walk down to the next downsampled row.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpInStart, 
        sdDoubleRowStrideIn );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpOutStart, 
        sdDoubleRowStrideOut );
    }
  }
}
COSTELLA_END_FUNCTION



/* CostellaUnblockCorrectHorizontalDiscrepancies: 
**
**   Internal function that corrects the horizontal discrepancies in an 
**   image, based on the supplied adjustment tables. 
**
**   aub{Y,Cb,Cr}Adjusted{U,V}:  Adjustment tables.
**
**   pi{In,Out}:  Pointer to the {in,out}put image.
*/

static COSTELLA_FUNCTION( CostellaUnblockCorrectHorizontalDiscrepancies, ( 
  COSTELLA_SW* aswBufferY, COSTELLA_SW* aswBufferCb, COSTELLA_SW* 
  aswBufferCr, COSTELLA_UB* aubYAdjustedU, COSTELLA_UB* aubYAdjustedV, 
  COSTELLA_UB* aubCbAdjustedU, COSTELLA_UB* aubCbAdjustedV, COSTELLA_UB* 
  aubCrAdjustedU, COSTELLA_UB* aubCrAdjustedV, COSTELLA_IMAGE* piIn, 
  COSTELLA_IMAGE* piOut, COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* 
  poPassback ) )
{
  COSTELLA_B bAlpha, bCopyAlpha, bColor;
  COSTELLA_UB ubPosition, ubY, ubCb, ubCr, ubA;
  COSTELLA_SW swYU, swYV, swCbU, swCbV, swCrU, swCrV;
  COSTELLA_SW* pswBufferY, * pswBufferYOld, * pswBufferCb, * pswBufferCbOld,
    * pswBufferCr, * pswBufferCrOld;
  COSTELLA_SD sdRowStrideIn, sdRowStrideOut, sdDoubleRowStrideIn, 
    sdDoubleRowStrideOut, sdAlphaRowStrideIn, sdAlphaRowStrideOut;
  COSTELLA_UD udWidth, udHeight, udRow, udColumn, udRowBoundaryBottom;
  COSTELLA_IMAGE_ALPHA* piaIn, * piaOut;
  COSTELLA_IMAGE_GRAY* pigIn, * pigOut;
  COSTELLA_IMAGE_COLOR* picIn, * picOut;
  COSTELLA_IMAGE_ALPHA_PIXEL iapInStart, iapOutStart, iapIn, iapOut;
  COSTELLA_IMAGE_GRAY_PIXEL igpInStart=NULL, igpOutStart=NULL, igpIn=NULL, igpOut=NULL;
  COSTELLA_IMAGE_COLOR_PIXEL icpInStart={0}, icpOutStart={0}, icpIn={0}, icpOut={0};
  

  /* Extract information.
  */

  bAlpha = piIn->bAlpha;
  bColor = piIn->bColor;

  udWidth = piIn->udWidth;
  udHeight = piIn->udHeight;

  sdRowStrideIn = piIn->sdRowStride;
  sdRowStrideOut = piOut->sdRowStride;

  sdAlphaRowStrideIn = piIn->sdAlphaRowStride;
  sdAlphaRowStrideOut = piOut->sdAlphaRowStride;

  piaIn = &piIn->ia;
  piaOut = &piOut->ia;

  pigIn = &piIn->ig;
  pigOut = &piOut->ig;

  picIn = &piIn->ic;
  picOut = &piOut->ic;


  /* Check that the output image is consistent.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !piOut->bAlpha != !bAlpha )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Alpha flag disagrees" );
      COSTELLA_RETURN;
    }

    if( !piOut->bColor != !bColor )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Color flag disagrees" );
      COSTELLA_RETURN;
    }

    if( piOut->udWidth != udWidth )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Width disagrees" );
      COSTELLA_RETURN;
    }

    if( piOut->udHeight != udHeight )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Height disagrees" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Determine whether we need to copy across alpha values.
  */

  bCopyAlpha = bAlpha && !COSTELLA_IMAGE_ALPHA_IS_SAME( piaIn, piaOut );


  /* Compute the double row strides.
  */

  sdDoubleRowStrideIn = sdRowStrideIn << 1;
  sdDoubleRowStrideOut = sdRowStrideOut << 1;


  /* Start with the luminance channel. Start at the top-left of the image. 
  */

  if( bColor )
  {
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpInStart, *picIn, udWidth, 
      udHeight, sdRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpOutStart, *picOut, udWidth,
      udHeight, sdRowStrideOut );
  }
  else
  {
    COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( igpInStart, *pigIn, udWidth, 
      udHeight, sdRowStrideIn );
    COSTELLA_IMAGE_GRAY_PIXEL_SET_TOP_LEFT( igpOutStart, *pigOut, udWidth, 
      udHeight, sdRowStrideOut );
  }

  if( bCopyAlpha )
  {
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapInStart, *piaIn, udWidth, 
      udHeight, sdAlphaRowStrideIn );
    COSTELLA_IMAGE_ALPHA_PIXEL_SET_TOP_LEFT( iapOutStart, *piaOut, udWidth, 
      udHeight, sdAlphaRowStrideOut );
  }

  
  /* Walk through the columns of the image.
  */

  for( udColumn = 0; udColumn < udWidth; udColumn++ )
  {
    /* Progress callback.
    */

    if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
    {
      COSTELLA_ERROR( "Progress callback" );
      COSTELLA_RETURN;
    }


    /* If there is an alpha channel, copy it across for the whole column 
    ** now.
    */

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapInStart, iapIn );
      COSTELLA_IMAGE_ALPHA_PIXEL_ASSIGN( iapOutStart, iapOut );
      
      for( udRow = 0; udRow < udHeight; udRow++, 
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapIn, sdAlphaRowStrideIn ), 
        COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_DOWN( iapOut, sdAlphaRowStrideOut ) 
        )
      {
        ubA = COSTELLA_IMAGE_ALPHA_PIXEL_GET_A( iapIn );
        COSTELLA_IMAGE_ALPHA_PIXEL_SET_A( iapOut, ubA );
      }
    }


    /* Start off at the topmost pixel of this column, for both input and
    ** output.
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( igpInStart, igpIn );
      COSTELLA_IMAGE_GRAY_PIXEL_ASSIGN( igpOutStart, igpOut );
    }

  
    /* If the height of the image is less than 9 pixels, then there are no
    ** boundaries. Simply copy the column across.
    */

    if( udHeight < 9 )
    {
      if( bColor )
      {
        for( udRow = 0; udRow < udHeight; udRow++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn ), 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOut, sdRowStrideOut ) )
        {
          ubY = COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
        }
      }
      else
      {
        for( udRow = 0; udRow < udHeight; udRow++, 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpIn, sdRowStrideIn ), 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpOut, sdRowStrideOut ) )
        {
          ubY = COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( igpIn );
          COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( igpOut, ubY );
        }
      }
    }
    else
    {
      /* There are boundaries. Load the values of the first eight pixels of 
      ** the column into the right block of the values array. This will 
      ** automatically be shifted to the left block in the first step below.
      ** Switch on image type.
      */

      if( bColor )
      {
        for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 8; 
          ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, 
          sdRowStrideIn ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( 
            icpIn );
        }
      }
      else
      {
        for( ubPosition = 0, pswBufferY = aswBufferY + 8; ubPosition < 8; 
          ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpIn, 
          sdRowStrideIn ) )
        {
          *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( 
            igpIn );
        }
      }


      /* Walk through all horizontal boundaries. See above comments.
      */

      for( udRowBoundaryBottom = 8; udRowBoundaryBottom < udHeight; 
        udRowBoundaryBottom += 8 )
      {
        /* Shift the bottom block in the array to the top block. 
        */

        for( ubPosition = 0, pswBufferY = aswBufferY, pswBufferYOld = 
          aswBufferY + 8; ubPosition < 8; ubPosition++ )
        {
          *pswBufferY++ = *pswBufferYOld++;
        }


        /* Extract the next eight pixels from the image. Need to make sure 
        ** that we don't go past the bottom edge of the image. Switch on 
        ** image type.
        */

        if( bColor )
        {
          for( udRow = udRowBoundaryBottom; ubPosition < 16 && udRow < 
            udHeight; udRow++, ubPosition++, 
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpIn, sdRowStrideIn ) )
          {
            *pswBufferY++ = (COSTELLA_SW) 
              COSTELLA_IMAGE_COLOR_PIXEL_GET_R_Y( icpIn );
          }
        }
        else
        {
          for( udRow = udRowBoundaryBottom; ubPosition < 16 && udRow < 
            udHeight; udRow++, ubPosition++, 
            COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpIn, sdRowStrideIn ) )
          {
            *pswBufferY++ = (COSTELLA_SW) COSTELLA_IMAGE_GRAY_PIXEL_GET_Y( 
              igpIn );
          }
        }


        /* Fill any unfilled entries with -1, which the discrepancy and
        ** correction functions recognize as missing entries.
        */

        for( ; ubPosition < 16; ubPosition++ )
        {
          *pswBufferY++ = -1;
        }


        /* Compute the boundary discrepancies.
        */

        if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( aswBufferY +
          5, &swYU, &swYV ) ) )
        {
          COSTELLA_ERROR( "Computing Y discrepancies" );
          COSTELLA_RETURN;
        }


        /* Adjust the discrepancies.
        */

        swYU = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swYU, aubYAdjustedU );
        swYV = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swYV, aubYAdjustedV );


        /* Correct the sixteen values for these adjusted discrepancies.
        */

        if( COSTELLA_CALL( CostellaUnblockCorrectDiscrepancies( aswBufferY, 
          swYU, swYV ) ) )
        {
          COSTELLA_ERROR( "Correcting Y discrepancies" );
          COSTELLA_RETURN;
        }


        /* Write the left block pixels back to the image. Switch on image 
        ** type.
        */

        if( bColor )
        {
          for( ubPosition = 0, pswBufferY = aswBufferY; ubPosition < 8; 
            ubPosition++, COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOut, 
            sdRowStrideOut ) )
          {
            ubY = (COSTELLA_UB) *pswBufferY++;
            COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
          }
        }
        else
        {
          for( ubPosition = 0, pswBufferY = aswBufferY; ubPosition < 8; 
            ubPosition++, COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpOut, 
            sdRowStrideOut ) )
          {
            ubY = (COSTELLA_UB) *pswBufferY++;
            COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( igpOut, ubY );
          }
        }
      }


      /* Write out any remaining pixels in the row. We subtract 8 from the 
      ** right boundary x value because it was incremented before the above 
      ** loop dropped out. Switch on image type.
      */

      if( bColor )
      {
        for( udRow = udRowBoundaryBottom - 8; ubPosition < 16 && udRow < 
          udHeight; udRow++, ubPosition++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN( icpOut, sdRowStrideOut ) )
        {
          ubY = (COSTELLA_UB) *pswBufferY++;
          COSTELLA_IMAGE_COLOR_PIXEL_SET_R_Y( icpOut, ubY );
        }
      }
      else
      {
        for( udRow = udRowBoundaryBottom - 8; ubPosition < 16 && udRow < 
          udHeight; udRow++, ubPosition++, 
          COSTELLA_IMAGE_GRAY_PIXEL_MOVE_DOWN( igpOut, sdRowStrideOut ) )
        {
          ubY = (COSTELLA_UB) *pswBufferY++;
          COSTELLA_IMAGE_GRAY_PIXEL_SET_Y( igpOut, ubY );
        }
      }
    }


    /* Walk across to the next column.
    */

    if( bColor )
    {
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpInStart );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT( icpOutStart );
    }
    else
    {
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpInStart );
      COSTELLA_IMAGE_GRAY_PIXEL_MOVE_RIGHT( igpOutStart );
    }

    if( bCopyAlpha )
    {
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapInStart );
      COSTELLA_IMAGE_ALPHA_PIXEL_MOVE_RIGHT( iapOutStart );
    }
  }


  /* If we have a color image, correct the horizontal boundary discrepancies
  ** for the chrominance channels. 
  */

  if( bColor )
  {
    /* Start at top-left of image. 
    */

    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpInStart, *picIn, udWidth, 
      udHeight, sdRowStrideIn );
    COSTELLA_IMAGE_COLOR_PIXEL_SET_TOP_LEFT( icpOutStart, *picOut, udWidth, 
      udHeight, sdRowStrideOut );


    /* Walk through those columns of the image that contain chrominance 
    ** data.
    */

    for( udColumn = 0; udColumn < udWidth; udColumn += 2 )
    {
      /* Progress callback.
      */

      if( pfProgress && COSTELLA_CALL( pfProgress( poPassback ) ) )
      {
        COSTELLA_ERROR( "Progress callback" );
        COSTELLA_RETURN;
      }


      /* Start off at the topmost downsampled pixel of this column, for both
      ** input and output.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpInStart, icpIn );
      COSTELLA_IMAGE_COLOR_PIXEL_ASSIGN( icpOutStart, icpOut );


      /* If the height of the image is less than 17 pixels, then there are 
      ** no boundaries. Simply copy the column across.
      */

      if( udHeight < 17 )
      {
        for( udRow = 0; udRow < udHeight; udRow += 2, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpIn, 
          sdDoubleRowStrideIn ), COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( 
          icpOut, sdDoubleRowStrideOut ) )
        {
          ubCb = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          ubCr = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );
        }
      }
      else
      {
        /* There are boundaries. Load the values of the first eight 
        ** downsampled pixels of the column into the bottom block of the 
        ** values arrays. This will automatically be shifted to the top 
        ** block in the first step below. Switch on image type.
        */

        for( ubPosition = 0, pswBufferCb = aswBufferCb + 8, pswBufferCr = 
          aswBufferCr + 8; ubPosition < 8; ubPosition++, 
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpIn, 
          sdDoubleRowStrideIn ) )
        {
          *pswBufferCb++ = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
          *pswBufferCr++ = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
        }


        /* Walk through all horizontal boundaries. See above comments.
        */

        for( udRowBoundaryBottom = 16; udRowBoundaryBottom < udHeight; 
          udRowBoundaryBottom += 16 )
        {
          /* Shift the bottom block in the array to the top block. 
          */

          for( ubPosition = 0, pswBufferCb = aswBufferCb, pswBufferCbOld = 
            aswBufferCb + 8, pswBufferCr = aswBufferCr, pswBufferCrOld = 
            aswBufferCr + 8; ubPosition < 8; ubPosition++ )
          {
            *pswBufferCb++ = *pswBufferCbOld++;
            *pswBufferCr++ = *pswBufferCrOld++;
          }


          /* Extract the next eight pixels from the image. Need to make sure
          ** that we don't go past the bottom edge of the image. Switch on 
          ** image type.
          */

          for( udRow = udRowBoundaryBottom; ubPosition < 16 && udRow < 
            udHeight; udRow += 2, ubPosition++, 
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpIn, 
            sdDoubleRowStrideIn ) )
          {
            *pswBufferCb++ = COSTELLA_IMAGE_COLOR_PIXEL_GET_G_CB( icpIn );
            *pswBufferCr++ = COSTELLA_IMAGE_COLOR_PIXEL_GET_B_CR( icpIn );
          }


          /* Fill any unfilled entries with -1, which the discrepancy and
          ** correction functions recognize as missing entries.
          */

          for( ; ubPosition < 16; ubPosition++ )
          {
            *pswBufferCb++ = -1;
            *pswBufferCr++ = -1;
          }


          /* Compute the boundary discrepancies.
          */

          if( COSTELLA_CALL( CostellaUnblockComputeDiscrepancies( 
            aswBufferCb + 5, &swCbU, &swCbV ) ) || COSTELLA_CALL(
            CostellaUnblockComputeDiscrepancies( aswBufferCr + 5, &swCrU, 
            &swCrV ) ) )
          {
            COSTELLA_ERROR( "Computing Cb or Cr discrepancies" );
            COSTELLA_RETURN;
          }


          /* Adjust the discrepancies.
          */

          swCbU = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCbU, aubCbAdjustedU
            );
          swCbV = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCbV, aubCbAdjustedV
            );

          swCrU = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCrU, aubCrAdjustedU
            );
          swCrV = COSTELLA_UNBLOCK_ADJUST_DISCREPANCY( swCrV, aubCrAdjustedV
            );


          /* Correct the sixteen values for these adjusted discrepancies.
          */

          if( COSTELLA_CALL( CostellaUnblockCorrectDiscrepancies( 
            aswBufferCb, swCbU, swCbV ) ) || COSTELLA_CALL(
            CostellaUnblockCorrectDiscrepancies( aswBufferCr, swCrU, swCrV )
            ) )
          {
            COSTELLA_ERROR( "Correcting Cb or Cr discrepancies" );
            COSTELLA_RETURN;
          }


          /* Write the left block pixels back to the image. Switch on image
          ** type.
          */

          for( ubPosition = 0, pswBufferCb = aswBufferCb, pswBufferCr = 
            aswBufferCr; ubPosition < 8; ubPosition++,
            COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpOut, 
            sdDoubleRowStrideOut ) )
          {
            ubCb = (COSTELLA_UB) *pswBufferCb++;
            ubCr = (COSTELLA_UB) *pswBufferCr++;

            COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
            COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );
          }
        }        


        /* Write out any remaining pixels in the row. We subtract 16 from 
        ** the right boundary x value because it was incremented before the 
        ** above loop dropped out. Switch on image type.
        */

        for( udRow = udRowBoundaryBottom - 16; ubPosition < 16 && udRow < 
          udHeight; udRow += 2, ubPosition++,
          COSTELLA_IMAGE_COLOR_PIXEL_MOVE_DOWN_TWO( icpOut, 
          sdDoubleRowStrideOut ) )
        {
          ubCb = (COSTELLA_UB) *pswBufferCb++;
          ubCr = (COSTELLA_UB) *pswBufferCr++;

          COSTELLA_IMAGE_COLOR_PIXEL_SET_G_CB( icpOut, ubCb );
          COSTELLA_IMAGE_COLOR_PIXEL_SET_B_CR( icpOut, ubCr );
        }
      }

    
      /* Walk across to the next downsampled column.
      */

      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpInStart );
      COSTELLA_IMAGE_COLOR_PIXEL_MOVE_RIGHT_TWO( icpOutStart );
    }
  }
}
COSTELLA_END_FUNCTION



/* CostellaUnblockComputeDiscrepancies: 
**
**   Compute the discrepancies u and v. 
**
**   aswValues:  Array containing the six intensity values. If any of the 
**     last two values are -1, it indicates missing pixels; alternative 
**     formulas are used. 
**
**   pswU:  Pointer into which the value of u will be stored. Must be valid.
**
**   pswV:  Pointer into which the value of v will be stored. If pswV is 
**     zero, we do not want to compute v.
*/

static COSTELLA_FUNCTION( CostellaUnblockComputeDiscrepancies, ( 
  COSTELLA_SW* aswValues, COSTELLA_SW* pswU, COSTELLA_SW* pswV ) )
{
  COSTELLA_SW sw6, sw7, sw8, sw9, sw10, sw11, swU, swV=0;


  /* Extract pixel values.
  */

  sw6 = *aswValues++;
  sw7 = *aswValues++;
  sw8 = *aswValues++;
  sw9 = *aswValues++;
  sw10 = *aswValues++;
  sw11 = *aswValues;


  /* Check if we have missing pixels.
  */

  if( sw11 >= 0 )
  {
    /* All six pixels present. Use the regular formulas. 
    */

    swU = COSTELLA_SHIFT_RIGHT_FLOOR( (COSTELLA_SW) gauwMult15Add4[ sw9] - 
      (COSTELLA_SW) gauwMult15[ sw8 ] - (COSTELLA_SW) gauwMult10[ sw10 ] + 
      (COSTELLA_SW) gauwMult10[ sw7 ] + (COSTELLA_SW) gauwMult3[ sw11 ] - 
      (COSTELLA_SW) gauwMult3[ sw6 ], 3 );

    if( pswV )
    {
      swV = (COSTELLA_SW) gauwMult3[ sw10 ] + (COSTELLA_SW) gauwMult3[ sw7 ]
        - sw11 - sw6 - ( ( sw9 + sw8 ) << 1 );
    }
  }
  else
  {
    /* The last pixel is missing. Check if the second-last pixel is missing.
    */

    if( sw10 >= 0 )
    {
      /* Have five pixels. Use modified formulas.
      */

      swU = COSTELLA_SHIFT_RIGHT_FLOOR( -(COSTELLA_SW) gauwMult3[ sw6 ] + 
        (COSTELLA_SW) gauwMult10[ sw7 ] - (COSTELLA_SW) gauwMult15[ sw8 ] + 
        (COSTELLA_SW) gauwMult12Add4[ sw9 ] - ( sw10 << 2 ), 3 );

      if( pswV )
      {
        swV = -sw6 + (COSTELLA_SW) gauwMult3[ sw7 ] - ( sw8 << 1 ) - sw9 + 
          sw10;
      }
    }
    else
    {
      /* Must have four pixels. Use modified formulas.
      */

      swU = COSTELLA_SHIFT_RIGHT_FLOOR( -(COSTELLA_SW) gauwMult3[ sw6 ] + 
        (COSTELLA_SW) gauwMult10[ sw7 ] - (COSTELLA_SW) gauwMult15[ sw8 ] + 
        ( sw9 << 3 ) + 4, 3 );

      swV = 0;
    }
  }


  /* Bound u and v to the range [-255,+255].
  */

  if( swU < -255 )
  {
    swU = -255;
  }
  else if( swU > +255 )
  {
    swU = +255;
  }

  if( pswV )
  {
    if( swV < -255 )
    {
      swV = -255;
    }
    else if( swV > +255 )
    {
      swV = +255;
    }
  }


  /* Store u and v.
  */

  *pswU = swU;

  if( pswV )
  {
    *pswV = swV;
  }
}
COSTELLA_END_FUNCTION



/* CostellaUnblockCorrectDiscrepancies: 
**
**   Correct discrepancies across the sixteen pixels in the two blocks.
**
**   asw:  Array of sixteen intensity values covering two complete blocks.
**
**   sw{U,V}:  The value of {u,v} to correct.
*/

static COSTELLA_FUNCTION( CostellaUnblockCorrectDiscrepancies, ( 
  COSTELLA_SW* asw, COSTELLA_SW swU, COSTELLA_SW swV ) )
{
  COSTELLA_SW swD1, swD2, swD3, swD4, swD5, swD6, swD7, swD8, swD9, swD10, 
    swD11, swD12, swD13, swD14, swD15, swD16,
    swD1V, swD2V, swD3V, swD4V, swD5V, swD6V, swD7V, swD8V;
  COSTELLA_SW* psw;


  /* Compute corrections.
  */

  if( swU )
  {
    COSTELLA_SW swD2U = COSTELLA_SHIFT_RIGHT_FLOOR( swU + 32, 6 );
    COSTELLA_SW swD3U = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult3Add32[ swU ], 6 );
    COSTELLA_SW swD4U = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult3Add16[ swU ], 5 );
    COSTELLA_SW swD5U = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult5Add16[ swU ], 5 );
    COSTELLA_SW swD6U = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult15Add32[ swU ], 6 );
    COSTELLA_SW swD7U = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult21Add32[ swU ], 6 );
    COSTELLA_SW swD8U = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult7Add8[ swU ], 4 );

    swD1 = 0;
    swD2 = swD2U;
    swD3 = swD3U;
    swD4 = swD4U;
    swD5 = swD5U;
    swD6 = swD6U;
    swD7 = swD7U;
    swD8 = swD8U;
    swD9 = -swD8U;
    swD10 = -swD7U;
    swD11 = -swD6U;
    swD12 = -swD5U;
    swD13 = -swD4U;
    swD14 = -swD3U;
    swD15 = -swD2U;
    swD16 = 0;
  }
  else
  {
    swD1 = 0;
    swD2 = 0;
    swD3 = 0;
    swD4 = 0;
    swD5 = 0;
    swD6 = 0;
    swD7 = 0;
    swD8 = 0;
    swD9 = 0;
    swD10 = 0;
    swD11 = 0;
    swD12 = 0;
    swD13 = 0;
    swD14 = 0;
    swD15 = 0;
    swD16 = 0;
  }

  if( swV )
  {
    swD1V = COSTELLA_SHIFT_RIGHT_FLOOR( -swV + 128, 8 );
    swD2V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMultM11Add128[ swV ], 8 );
    swD3V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMultM31Add128[ swV ], 8 ); 
    swD4V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMultM58Add128[ swV ], 8 );
    swD5V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMultM57Add128[ swV ], 8 );
    swD6V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMultM22Add128[ swV ], 8 );
    swD7V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult42Add128[ swV ], 8 );
    swD8V = COSTELLA_SHIFT_RIGHT_FLOOR( gaswMult138Add128[ swV ], 8 );

    swD1 += swD1V;
    swD2 += swD2V;
    swD3 += swD3V;
    swD4 += swD4V;
    swD5 += swD5V;
    swD6 += swD6V;
    swD7 += swD7V;
    swD8 += swD8V;
    swD9 += swD8V;
    swD10 += swD7V;
    swD11 += swD6V;
    swD12 += swD5V;
    swD13 += swD4V;
    swD14 += swD3V;
    swD15 += swD2V;
    swD16 += swD1V; 
  }


  /* Perform corrections.
  */

  psw = asw;
  COSTELLA_UNBLOCK_CORRECT( psw, swD1 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD2 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD3 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD4 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD5 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD6 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD7 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD8 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD9 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD10 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD11 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD12 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD13 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD14 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD15 );
  psw++;
  COSTELLA_UNBLOCK_CORRECT( psw, swD16 );
}
COSTELLA_END_FUNCTION



/* CostellaUnblockComputeAdjustments: 
**
**   Compute the adjustment look-up tables.
**
**   audReference:  Frequency table for the reference discrepancy values.
**
**   audMeasured:  Frequency table for the measured discrepancy values.
**
**   udTotal:  Total frequency.
**
**   bConservativePhotographic:  If nonzero, compute the adjustments 
**     conservatively, assuming a continuous-tone image.
**
**   bConservativeCartoon:  If nonzero, compute the adjustments 
**     conservatively, assuming a solid-tone cartoon image.
**
**   aubAdjusted:  Array into which the adjustment table will be written.
*/

static COSTELLA_FUNCTION( CostellaUnblockComputeAdjustments, ( COSTELLA_UD* 
  audReference, COSTELLA_UD* audMeasured, COSTELLA_UD udTotal, COSTELLA_B 
  bConservativePhotographic, COSTELLA_B bConservativeCartoon, COSTELLA_UB* 
  aubAdjusted ) )
{
  COSTELLA_B bConservative;
  COSTELLA_UB* pubAdjusted;
  COSTELLA_UW uwMeasured, uwReference;
  COSTELLA_UD udCumMeasured, udCumMeasuredConservative=0, udCumReference,
    udHalfTotal, udSubtract, udSigma;
  COSTELLA_UD* pudMeasured, * pudReference;


  /* Compute convenient flag.
  */

  bConservative = bConservativePhotographic || bConservativeCartoon;


  /* Compute half the total.
  */

  udHalfTotal = ( udTotal + 1 ) >> 1;


  /* Walk through the measured discrepancy values, and simultaneously the
  ** reference discrepancy values.
  */

  for( uwMeasured = 0, uwReference = 0, udCumMeasured = 0, udCumReference = 
    0, pudMeasured = audMeasured, pubAdjusted = aubAdjusted, pudReference = 
    audReference; uwMeasured < 256; uwMeasured++, pudMeasured++, 
    pubAdjusted++ ) 
  {
    /* Add the measured frequency into the cumulative measured frequency.
    */

    udCumMeasured += *pudMeasured;


    /* If requested, compute a conservative bound that takes into account 
    ** the limited statistics of both the measured and the reference values.
    */

    if( bConservative )
    {
      if( udCumMeasured > udHalfTotal )
      {
        udSigma = costella_unblock_approx_square_root( udTotal - 
          udCumMeasured );
      }
      else
      {
        udSigma = costella_unblock_approx_square_root( udCumMeasured );
      }

      if( udSigma < 2 )
      {
        udSigma = 2;
      }

      if( bConservativePhotographic )
      {
        udSubtract = udSigma << 3;

        if( udSubtract > udCumMeasured )
        {
          udCumMeasuredConservative = 0;
        }
        else
        {
          udCumMeasuredConservative = udCumMeasured - udSubtract;
        }
      }
      else if( bConservativeCartoon )
      {
        udCumMeasuredConservative = udCumMeasured + ( udSigma << 3 );

        if( udCumMeasuredConservative > udTotal )
        {
          udCumMeasuredConservative = udTotal;
        }
      }
    }
    else
    {
      udCumMeasuredConservative = udCumMeasured;
    }


    /* Build the reference frequency until it is greater than or equal to 
    ** the conservative cumulative measured frequency.
    */

    while( udCumReference < udCumMeasuredConservative ) 
    {
      /* Add the reference frequency into the cumulative reference 
      ** frequency.
      */

      udCumReference += *pudReference;


      /* Increment the internal frequency.
      */

      uwReference++;
      pudReference++;
    }


    /* If the reference cumulative frequency has exceeded the conservative
    ** measured cumulative frequency, move back one reference value.
    */

    if( udCumReference > udCumMeasuredConservative )
    {
      /* Decrement the reference frequency.
      */

      uwReference--;
      pudReference--;


      /* Subtract the reference frequency from the cumulative reference
      ** frequency.
      */

      udCumReference -= *pudReference;
    }


    /* If the measured discrepancy is greater than the reference 
    ** discrepancy, then the adjusted discrepancy is the measured 
    ** discrepancy minus the reference discrepancy. Otherwise, the adjusted 
    ** discrepancy is zero. Note that the reference value is one less than 
    ** the value we are sitting on, unless we are sitting on zero, because 
    ** we are sitting on the value that we are -about- to add in, not the 
    ** value that has actually been added in.
    */

    if( !uwReference )
    {
      *pubAdjusted = (COSTELLA_UB) uwMeasured;
    }
    else if( uwMeasured >= uwReference ) 
    {
      *pubAdjusted = (COSTELLA_UB) ( uwMeasured + 1 - uwReference );
    }
    else
    {
      *pubAdjusted = 0;
    }
  }
}
COSTELLA_END_FUNCTION



/* costella_unblock_approx_square_root: 
**
**   Return the approximate square-root of a COSTELLA_UD.
**
**   ud:  Number to approximately square-root.
*/

static COSTELLA_UD costella_unblock_approx_square_root( COSTELLA_UD ud )
{
  COSTELLA_UB udBitWidth, ubBitsToHalve, ubSquareRoot;
  COSTELLA_UD udTest;

  
  /* Determine the bit-width of the original number.
  */

  for( udBitWidth = 0, udTest = ud; udTest; udBitWidth++, udTest >>= 1 )
  {
  }


  /* If the bit-width is less than 9, just use the table directly.
  */

  if( udBitWidth < 9 )
  {
    return (COSTELLA_UD) gaubSquareRoot[ ud ];
  }


  /* Determine how many bits we will halve. If the bit-width is even, this 
  ** is just eight less than the bit-width. If the bit-width is odd, this is
  ** seven less than the bit-width.
  */

  if( udBitWidth & 0x1 )
  {
    ubBitsToHalve = udBitWidth - 7;
  }
  else
  {
    ubBitsToHalve = udBitWidth - 8;
  }
    

  /* Square-root the top 7 or 8 bits, respectively.
  */

  ubSquareRoot = gaubSquareRoot[ ud >> ubBitsToHalve ];


  /* Shift the result left by half of the number of bits to halve, and 
  ** return the final result.
  */

  return (COSTELLA_UD) ubSquareRoot << ( ubBitsToHalve >> 1 );
}

#ifdef COSTELLA_UNBLOCK_DEBUG_DUMP
  /* CostellaUnblockDebugDump: 
  **
  **   Dumping of frequency tables to text files for debugging purposes.
  **
  **   bVertical:  If nonzero, we are dumping the vertical frequency tables;
  **     if zero, we are dumping the horizontal frequency tables.
  **
  **   bColor:  Color flag.
  */

#include <stdio.h>
  void CostellaUnblockDebugDump( COSTELLA_UB* aubYAdjustedU, COSTELLA_UB* 
    aubYAdjustedV, COSTELLA_UB* aubCbAdjustedU, COSTELLA_UB* aubCbAdjustedV,
    COSTELLA_UB* aubCrAdjustedU, COSTELLA_UB* aubCrAdjustedV, COSTELLA_UD* 
    audYInternalU, COSTELLA_UD* audYInternalV, COSTELLA_UD* audCbInternalU, 
    COSTELLA_UD* audCbInternalV, COSTELLA_UD* audCrInternalU, COSTELLA_UD* 
    audCrInternalV, COSTELLA_UD* audYBoundaryU, COSTELLA_UD* audYBoundaryV, 
    COSTELLA_UD* audCbBoundaryU, COSTELLA_UD* audCbBoundaryV, COSTELLA_UD* 
    audCrBoundaryU, COSTELLA_UD* audCrBoundaryV, COSTELLA_B bVertical, 
    COSTELLA_B bColor )
  {
    FILE* pfile = fopen(bVertical ? "vertical.txt" : "horizontal.txt", "wt");
    if( pfile )
    {
      COSTELLA_UW uw;
      COSTELLA_UD udYUIntCum = 0, udYUBoundCum = 0, udYVIntCum = 0,
	udYVBoundCum = 0, ulCbUIntCum = 0, ulCbUBoundCum = 0, ulCbVIntCum =
	0, ulCbVBoundCum = 0, ulCrUIntCum = 0, ulCrUBoundCum = 0, ulCrVIntCum
	= 0, ulCrVBoundCum = 0;

      if( !bColor )
      {
        /* Grayscale. Write header line.
        */
        fprintf( pfile, "value\tu adj\tv adj\tu int\tu bnd\tu int cum\t"
          "u bnd cum\tv int\tv bnd\tv int cum\tv bnd cum\n" );

        /* Write out the other lines.
        */
        for( uw = 0; uw < 256; uw++ )
        {
          udYUIntCum += audYInternalU[ uw ];
          udYUBoundCum += audYBoundaryU[ uw ];
          udYVIntCum += audYInternalV[ uw ];
          udYVBoundCum += audYBoundaryV[ uw ];

          fprintf(pfile, "%u\t%u\t%u\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t"
            "%lu\n", uw, (unsigned int) aubYAdjustedU[ uw ], (unsigned int)
            aubYAdjustedV[ uw ], audYInternalU[ uw ], audYBoundaryU[ uw ],
            udYUIntCum, udYUBoundCum, audYInternalV[ uw ], audYBoundaryV[ uw
            ], udYVIntCum, udYVBoundCum );
        }
      }
      else
      {
        /* Color. */
        fprintf( pfile, "value\ty u adj\ty v adj\tcb u adj\tcb v adj\t"
          "cbr u adj\tcr v adj\t"
          "y u i\ty u b\ty u i cum\ty u b cum\t"
          "y v i\ty v b\ty v i cum\ty v b cum\t"
          "cb u i\tcb u b\tcb u i cum\tcb u b cum\t"
          "cb v i\tcb v b\tcb v i cum\tcb v b cum\t"
          "cr u i\tcr u b\tcr u i cum\tcr u b cum\t" 
          "cr v i\tcr v b\tcr v i cum\tcr v b cum\n" );

        for( uw = 0; uw < 256; uw++ )
        {
          udYUIntCum += audYInternalU[ uw ];
          udYUBoundCum += audYBoundaryU[ uw ];
          udYVIntCum += audYInternalV[ uw ];
          udYVBoundCum += audYBoundaryV[ uw ];

          ulCbUIntCum += audCbInternalU[ uw ];
          ulCbUBoundCum += audCbBoundaryU[ uw ];
          ulCbVIntCum += audCbInternalV[ uw ];
          ulCbVBoundCum += audCbBoundaryV[ uw ];

          ulCrUIntCum += audCrInternalU[ uw ];
          ulCrUBoundCum += audCrBoundaryU[ uw ];
          ulCrVIntCum += audCrInternalV[ uw ];
          ulCrVBoundCum += audCrBoundaryV[ uw ];
          
          fprintf(pfile, "%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
            "%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu"
            "\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu"
            "\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n", uw, (unsigned int)
            aubYAdjustedU[ uw ], (unsigned int) aubYAdjustedV[ uw ], 
            (unsigned int) aubCrAdjustedU[ uw ], (unsigned int) 
            aubCrAdjustedV[ uw ], (unsigned int) aubCbAdjustedU[ uw ], 
            (unsigned int) aubCbAdjustedV[ uw ], audYInternalU[ uw ], 
            audYBoundaryU[ uw ], udYUIntCum, udYUBoundCum, audYInternalV[ 
            uw ], audYBoundaryV[ uw ], udYVIntCum, udYVBoundCum, 
            audCbInternalU[ uw ], audCbBoundaryU[ uw ], ulCbUIntCum, 
            ulCbUBoundCum, audCbInternalV[ uw ], audCbBoundaryV[ uw ], 
            ulCbVIntCum, ulCbVBoundCum, audCrInternalU[ uw ], 
            audCrBoundaryU[ uw ], ulCrUIntCum, ulCrUBoundCum, 
            audCrInternalV[ uw ], audCrBoundaryV[ uw ], ulCrVIntCum, 
            ulCrVBoundCum );
        }
      }

      fclose( pfile );
    }
  }

#endif

/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
