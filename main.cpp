// Convert a .bmp file (from a high-compression jpg, as from a very cheap camera)
// into another .bmp file with greatly attenuated 8x8-pixel-block jpg artifacts.

extern "C" {
#include "costella_unblock.h"
}
#include "EasyBMP.h"
#include <png.h>
#include <stdlib.h>

struct rgbpixel {unsigned char r,g,b; };

// Actually YCbCr, not YUV.
// From www.equasys.de/colorconversion.html YCbCr - RGB.
// (It might also work to use R,G,B directly, each pretending to be Y of YUV.)
void YUVfromRGB(double& Y, double& U, double& V, const double R, const double G, const double B)
{
  Y =  0.257 * R + 0.504 * G + 0.098 * B +  16;
  U = -0.148 * R - 0.291 * G + 0.439 * B + 128;
  V =  0.439 * R - 0.368 * G - 0.071 * B + 128;
}
void RGBfromYUV(double& R, double& G, double& B, double Y, double U, double V)
{
  Y -= 16;
  U -= 128;
  V -= 128;
  R = 1.164 * Y             + 1.596 * V;
  G = 1.164 * Y - 0.392 * U - 0.813 * V;
  B = 1.164 * Y + 2.017 * U;
}

const bool fPNG = false; // true for PNG, false for BMP

int main( int argc, char** argv)
{
  if (argc != 3) {
    printf("usage: %s in.bmp out.bmp\n", argv[0]);
    return 1;
  }

  int w, h;
  FILE *fp;
  struct rgbpixel **row_pointers;
  png_structp png_ptr;
  png_infop info_ptr;
  BMP bmp;
  if (fPNG) {
    fp = fopen(argv[1], "rb");
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA|PNG_TRANSFORM_EXPAND|PNG_TRANSFORM_STRIP_16, NULL);
    row_pointers = (struct rgbpixel**)png_get_rows(png_ptr, info_ptr);
    w = png_get_image_width(png_ptr, info_ptr);
    h = png_get_image_height(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    fclose(fp);
  } else {
    bmp.ReadFromFile(argv[1]);
    w = bmp.TellWidth();
    h = bmp.TellHeight();
  }

  // Convert bmp.rgb_data or row_pointers to YUV color planes.
  const unsigned cb = w * h;
  unsigned char bufY[cb];
  unsigned char bufU[cb];
  unsigned char bufV[cb];

  int y,x;
  int i=0;
  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x,++i) {
      double R,G,B;
      if (fPNG) {
	const rgbpixel rgb = row_pointers[y][x];
	R = rgb.r;
	G = rgb.g;
	B = rgb.b;
      } else {
	const RGBApixel* rgb = bmp(x,y);
	R = rgb->Red;
	G = rgb->Green;
	B = rgb->Blue;
      }
      double Y,U,V;
      YUVfromRGB(Y,U,V,R,G,B);

#undef testing
#ifdef testing
      double a,b,c;
      RGBfromYUV(a,b,c,Y,U,V);
      if (fabs(a-R) + fabs(b-G) + fabs(c-B) > 0.9 /* in the scale 0.0 to 255.0 */ )
	printf("not inverses.\n");
      // but that's two linear operators, before the necessary clamping.
#endif
      // Clamp to 0..255.
      bufY[i] = COSTELLA_IMAGE_LIMIT_RANGE(Y);
      bufU[i] = COSTELLA_IMAGE_LIMIT_RANGE(U);
      bufV[i] = COSTELLA_IMAGE_LIMIT_RANGE(V);
    }
  }

  COSTELLA_IMAGE im;
  im.bAlpha = 0;
  im.bRgb = 0;
  im.udHeight = h;
  im.udWidth = w; // may fail if not a multiple of 16
  im.sdRowStride = w; // maybe something about bufPitch, width bumped up to multiple of 16.
  im.sdAlphaRowStride = 0;
#if 0
  // Unblock only luma (bufY).
  im.bColor = im.bDownsampledChrominance = im.bNonreplicatedDownsampledChrominance = 0;
  im.ig = bufY /* COSTELLA_IMAGE_GRAY */;
#else
  // Unblock luma, Cb, and Cr.
  im.bColor = im.bDownsampledChrominance = im.bNonreplicatedDownsampledChrominance = 1;
  im.ic.aubRY = bufY;
  im.ic.aubGCb = bufU;
  im.ic.aubBCr = bufV;
#endif

  costella_unblock_initialize(stdout);
  const int fPhoto = 0; // API docs suggest 1, but that boosts ringing of high-contrast detail (timestamps, windows of buildings).
  // (Internal mucking about, in costella_unblock.c bConservativePhotographic tweaking udCumMeasuredConservative,
  // had either no effect or caused a segfault.)
  int ok = costella_unblock(&im, &im, fPhoto, 0, NULL, NULL, 0);
  if (!ok)
    printf("%s: problem during costella_unblock().\n", argv[0]);
  costella_unblock_finalize(stdout);

  // Convert bufY, bufU, bufV back into a bmp.
  // (Or, set im.bRgb=1 to avoid this conversion?  That sets bOutYCbCr in CostellaUnblock(),
  // which causes calls to CostellaImageConvertRgbToYcbcr() and CostellaImageConvertYcbcrToRgb().
  // Those call macros like COSTELLA_IMAGE_CONVERT_RGB_TO_YCBCR(),
  // which lookup tables like gasdCostellaImageConvertRCb[] for Red to Cb.)
  i=0;
  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x,++i) {
      double R,G,B;
      RGBfromYUV(R,G,B, bufY[i], bufU[i], bufV[i]);
      if (fPNG) {
	row_pointers[y][x] = (rgbpixel){
	  COSTELLA_IMAGE_LIMIT_RANGE(R),
	  COSTELLA_IMAGE_LIMIT_RANGE(G),
	  COSTELLA_IMAGE_LIMIT_RANGE(B) };
      } else {
	RGBApixel* rgb = bmp(x,y);
	rgb->Red   = COSTELLA_IMAGE_LIMIT_RANGE(R);
	rgb->Green = COSTELLA_IMAGE_LIMIT_RANGE(G);
	rgb->Blue  = COSTELLA_IMAGE_LIMIT_RANGE(B);
      }
    }
  }
  if (fPNG) {
    fp = fopen(argv[2], "wb");
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, (png_bytepp)row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
  } else {
    bmp.WriteToFile(argv[2]);
  }
  return 0;
}
