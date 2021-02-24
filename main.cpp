// Convert a .bmp file (from a high-compression jpg, as from a very cheap camera)
// into another .bmp file with greatly attenuated 8x8-pixel-block jpg artifacts.

extern "C" {
#include "costella_unblock.h"
}
#include "EasyBMP.h"
#include <png.h>
#include <algorithm>
#include <cstdlib>

typedef uint8_t u8;

// Actually YCbCr, not YUV.
// From www.equasys.de/colorconversion.html YCbCr - RGB.
void YUVfromRGB(u8& Y, u8& U, u8& V, const double R, const double G, const double B)
{
  const double y =  0.257 * R + 0.504 * G + 0.098 * B +  16;
  const double u = -0.148 * R - 0.291 * G + 0.439 * B + 128;
  const double v =  0.439 * R - 0.368 * G - 0.071 * B + 128;
  // Clamp to [0, 255].
  Y = COSTELLA_IMAGE_LIMIT_RANGE(y);
  U = COSTELLA_IMAGE_LIMIT_RANGE(u);
  V = COSTELLA_IMAGE_LIMIT_RANGE(v);
}
void RGBfromYUV(u8& R, u8& G, u8& B, double Y, double U, double V)
{
  Y -= 16;
  U -= 128;
  V -= 128;
  const double r = 1.164 * Y             + 1.596 * V;
  const double g = 1.164 * Y - 0.392 * U - 0.813 * V;
  const double b = 1.164 * Y + 2.017 * U;
  // Clamp to [0, 255].
  R = COSTELLA_IMAGE_LIMIT_RANGE(r);
  G = COSTELLA_IMAGE_LIMIT_RANGE(g);
  B = COSTELLA_IMAGE_LIMIT_RANGE(b);
}

std::string filenameExtension(const std::string& s)
{
  const auto i = s.find_last_of(".");
  if (i == std::string::npos)
    return "";
  auto ext(s.substr(i + 1));
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  return ext;
}

int main(int argc, char** argv)
{
  if (argc != 3) {
LUsage:
    printf("usage: %s in.[bmp|png] out.[bmp|png]\n", argv[0]);
    return 1;
  }
  const auto ext1 = filenameExtension(argv[1]);
  const auto ext2 = filenameExtension(argv[2]);
  const bool fBMP = ext1 == "bmp";
  const bool fPNG = ext1 == "png";
  if (!fBMP && !fPNG)
    goto LUsage;
  if (ext2 != ext1)
    printf("%s: warning: filenames %s and %s have different extensions.\nFile %s will get the same format as %s.\n", argv[0], argv[1], argv[2], argv[2], argv[1]);

  FILE *fp;
  unsigned w, h;
  BMP bmp;
  png_bytep *pRows = NULL;

  if (fBMP) {
    bmp.ReadFromFile(argv[1]);
    w = bmp.TellWidth();
    h = bmp.TellHeight();
  } else {
    fp = fopen(argv[1], "rb");
    auto pPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    auto pInfoPNG = png_create_info_struct(pPNG);
    png_init_io(pPNG, fp);
    png_read_info(pPNG, pInfoPNG);
    if (png_get_color_type(pPNG, pInfoPNG) != PNG_COLOR_TYPE_RGB) {
      // Alpha, grayscale, and palette formats wouldn't have come from a JPG.
      printf("%s: png file %s isn't in RGB format.\n", argv[0], argv[1]);
      return 1;
    }
    // Transform 16 bit to 8 bit.
    if (png_get_bit_depth(pPNG, pInfoPNG) == 16) {
      png_set_strip_16(pPNG);
      png_read_update_info(pPNG, pInfoPNG);
    }
    w = png_get_image_width(pPNG, pInfoPNG);
    h = png_get_image_height(pPNG, pInfoPNG);
    pRows = new png_bytep[h];
    const auto cb = png_get_rowbytes(pPNG, pInfoPNG);
    for (unsigned y = 0u; y < h; ++y)
      pRows[y] = new png_byte[cb];
    png_read_image(pPNG, pRows);
    fclose(fp);
    png_destroy_read_struct(&pPNG, &pInfoPNG, NULL);
  }

  // Convert bmp.rgb_data or pRows to YUV color planes.
  const auto cb = w * h;
  u8 bufY[cb];
  u8 bufU[cb];
  u8 bufV[cb];
  unsigned y, x;
  auto i = 0;
  for (y = 0u; y < h; ++y) {
    for (x = 0u; x < w; ++x,++i) {
      double R,G,B;
      if (fBMP) {
	const auto rgb = bmp(x,y);
	R = rgb->Red;
	G = rgb->Green;
	B = rgb->Blue;
      } else {
	const auto rgb = pRows[y] + 3*x;
	R = rgb[0];
	G = rgb[1];
	B = rgb[2];
      }
      YUVfromRGB(bufY[i],bufU[i],bufV[i], R,G,B);
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
  const auto fPhoto = 0; // API docs suggest 1, but that boosts ringing of high-contrast detail (timestamps, windows of buildings).
  // (Internal mucking about, in costella_unblock.c bConservativePhotographic tweaking udCumMeasuredConservative,
  // had either no effect or caused a segfault.)
  auto ok = costella_unblock(&im, &im, fPhoto, 0, NULL, NULL, 0);
  if (!ok)
    printf("%s: costella_unblock() failed.\n", argv[0]);
  costella_unblock_finalize(stdout);

  // Convert bufY, bufU, bufV back into a bmp.
  // (Or, set im.bRgb=1 to avoid this conversion?  That sets bOutYCbCr in CostellaUnblock(),
  // which causes calls to CostellaImageConvertRgbToYcbcr() and CostellaImageConvertYcbcrToRgb().
  // Those call macros like COSTELLA_IMAGE_CONVERT_RGB_TO_YCBCR(),
  // which lookup tables like gasdCostellaImageConvertRCb[] for Red to Cb.)
  i = 0;
  for (y = 0u; y < h; ++y) {
    for (x = 0u; x < w; ++x,++i) {
      u8 R,G,B;
      RGBfromYUV(R,G,B, bufY[i],bufU[i],bufV[i]);
      if (fBMP) {
	auto rgb = bmp(x,y);
	rgb->Red   = R;
	rgb->Green = G;
	rgb->Blue  = B;
      } else {
	auto rgb = pRows[y] + 3*x;
	rgb[0] = R;
	rgb[1] = G;
	rgb[2] = B;
      }
    }
  }
  if (fBMP) {
    bmp.WriteToFile(argv[2]);
  } else {
    fp = fopen(argv[2], "wb");
    auto pPNG = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    auto pInfoPNG = png_create_info_struct(pPNG);
    png_init_io(pPNG, fp);
    png_set_IHDR(pPNG, pInfoPNG, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    // Don't call png_set_tIME, so no tIME chunk is written, no %tEXtdate:create, %tEXtdate:modify, gAMA, cHRM, bKGD.
    // Then the output has no timestamp, so it can be diffed against a known-good test output.
    png_write_info(pPNG, pInfoPNG);
    png_write_image(pPNG, pRows);
    png_write_end(pPNG, NULL);
    fclose(fp);
    png_destroy_write_struct(&pPNG, &pInfoPNG);
  }
  return 0;
}
