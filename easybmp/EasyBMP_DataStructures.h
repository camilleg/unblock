/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library * 
*                                                *
*  Author: Paul Macklin                          *
*   email: macklin01@users.sourceforge.net       *
* support: http://easybmp.sourceforge.net        *
*                                                *
*          file: EasyBMP_DataStructures.h        *
*    date added: 05-02-2005                      *
* date modified: 12-01-2006                      *
*       version: 1.06                            *
*                                                *
*   License: BSD (revised/modified)              *
* Copyright: 2005-6 by the EasyBMP Project       * 
*                                                *
* description: Defines basic data structures for *
*              the BMP class                     *
*                                                *
*************************************************/

#ifndef _EasyBMP_Custom_Math_Functions_
#define _EasyBMP_Custom_Math_Functions_
inline double Square( double number )
{ return number*number; }

inline int IntSquare( int number )
{ return number*number; }
#endif

int IntPow( int base, int exponent );

#ifndef _EasyBMP_Defined_WINGDI
#define _EasyBMP_Defined_WINGDI
 typedef unsigned char  ebmpBYTE;
 typedef unsigned short ebmpWORD;
 typedef unsigned int  ebmpDWORD;
#endif

#pragma once

inline ebmpWORD FlipWORD( ebmpWORD in )
{ return ( (in >> 8) | (in << 8) ); }

inline ebmpDWORD FlipDWORD( ebmpDWORD in )
{
 return ( ((in&0xFF000000)>>24) | ((in&0x000000FF)<<24) | 
          ((in&0x00FF0000)>>8 ) | ((in&0x0000FF00)<<8 )   );
}

typedef struct RGBApixel {
	ebmpBYTE Blue;
	ebmpBYTE Green;
	ebmpBYTE Red;
	ebmpBYTE Alpha;
} RGBApixel; 

struct BMFH{
 ebmpWORD  bfType;
 ebmpDWORD bfSize;
 ebmpWORD  bfReserved1;
 ebmpWORD  bfReserved2;
 ebmpDWORD bfOffBits; 

 BMFH();
 void display();
 void SwitchEndianess();
};

struct BMIH{
 ebmpDWORD biSize;
 ebmpDWORD biWidth;
 ebmpDWORD biHeight;
 ebmpWORD  biPlanes;
 ebmpWORD  biBitCount;
 ebmpDWORD biCompression;
 ebmpDWORD biSizeImage;
 ebmpDWORD biXPelsPerMeter;
 ebmpDWORD biYPelsPerMeter;
 ebmpDWORD biClrUsed;
 ebmpDWORD biClrImportant;

 BMIH();
 void display();
 void SwitchEndianess();
};

// vim: ts=4 sw=4
