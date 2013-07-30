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
** costella_debug.h: 
**  
**   Header file that sets the COSTELLA_DEBUG flag when the compiler is in
**   debug mode. Modify the entry below to include a different file.
**
**   This version: March 28, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_DEBUG_H_
#define _COSTELLA_DEBUG_H_



/* The default file below sets the COSTELLA_DEBUG flag if the symbol _DEBUG 
** is defined. For a different behavior, comment out the first entry, copy 
** the file costella_debug_default.h, modify it to suit, save it under a new
** name, and include it here.
*/

#include "costella_debug_default.h"
/* #include "costella_debug_my_architecture.h" (for example) */



/* File is now included.
*/

#endif



/* Copyright (c) 2007 John P. Costella.
**
** End of file.
*/
