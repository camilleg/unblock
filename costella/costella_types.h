/* Copyright (c) 2007 John P. Costella.
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
** costella_types.h: 
**  
**   Header file defining standard types for the Costella libraries. 
**   Modify the entry below to include a different file.
**
**   This version: July 9, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_TYPES_H_
#define _COSTELLA_TYPES_H_



/* For plain vanilla ANSI C, include the first file. To optimize for a
** specific architecture, comment out the first entry, copy the file
** costella_types_ansi.h, modify it to suit, save it under a new name, and
** include it here.
*/

#include "costella_types_ansi.h"
/* #include "costella_types_my_architecture.h" (for example) */



/* Definition that provides that COSTELLA_O* is the same as void*.
*/

#define COSTELLA_O void



/* Types that should not be modified for any architecture.
*/

typedef int COSTELLA_B;
typedef char COSTELLA_C;
typedef unsigned int COSTELLA_MC;
typedef int COSTELLA_MI;



/* Flag that states whether your architecture right-shifts in the sign bit.
** Comment out this flag if you use a processor that does not.
*/

#define COSTELLA_SHIFT_RIGHT_IS_SIGNED



/* File is now included.
*/

#endif



/* Copyright (c) 2007 John P. Costella.
**
** End of file.
*/
