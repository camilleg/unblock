// Convert a .bmp file (from a high-compression jpg, as from a very cheap camera)
// into another .bmp file with greatly attenuated 8x8-pixel-block jpg artifacts.

extern "C" {
#include "costella_unblock.h"
}
#include "EasyBMP.h"
#include <png.h>
#include <algorithm>
#include <cstdlib>

using u8 = uint8_t;

// Actually YCbCr, not YUV.
// From https://web.archive.org/web/20180423091842/http://www.equasys.de/colorconversion.html YCbCr - RGB.
void YUVfromRGB(u8& Y, u8& U, u8& V, const double R, const double G, const double B)
{
  const auto y =  0.257 * R + 0.504 * G + 0.098 * B +  16.0;
  const auto u = -0.148 * R - 0.291 * G + 0.439 * B + 128.0;
  const auto v =  0.439 * R - 0.368 * G - 0.071 * B + 128.0;
  // Clamp to [0, 255].
  Y = COSTELLA_IMAGE_LIMIT_RANGE(y);
  U = COSTELLA_IMAGE_LIMIT_RANGE(u);
  V = COSTELLA_IMAGE_LIMIT_RANGE(v);
}
void RGBfromYUV(u8& R, u8& G, u8& B, double Y, double U, double V)
{
  Y -=  16.0;
  U -= 128.0;
  V -= 128.0;
  const auto r = 1.164 * Y             + 1.596 * V;
  const auto g = 1.164 * Y - 0.392 * U - 0.813 * V;
  const auto b = 1.164 * Y + 2.017 * U;
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

struct PNG {
  unsigned w = 0;
  unsigned h = 0;
  u8* pixels = nullptr;
  ~PNG() { delete [] pixels; }
};

bool read_png(PNG& dst, const char* path)
{
  dst.w = dst.h = 0; // invalidate
  png_image src;
  std::memset(&src, 0, sizeof(src));
  src.version = PNG_IMAGE_VERSION;
  if (!png_image_begin_read_from_file(&src, path))
    return false;
  src.format = PNG_FORMAT_RGB;

  dst.pixels = new u8[PNG_IMAGE_SIZE(src)];
  if (!png_image_finish_read(&src, nullptr, dst.pixels, 0, nullptr))
    return false;
  dst.w = src.width; // validate
  dst.h = src.height;
  png_image_free(&src);
  return true;
}

bool write_png(const PNG& src, const char* path) {
  png_image dst;
  std::memset(&dst, 0, sizeof(dst));
  dst.version = PNG_IMAGE_VERSION;
  dst.width = src.w;
  dst.height = src.h;
  dst.format = PNG_FORMAT_RGB;
  const bool ok = png_image_write_to_file(&dst, path, 0, src.pixels, 0, nullptr) != 0;
  png_image_free(&dst);
  return ok;
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

  unsigned w, h;
  BMP bmp;
  PNG png;

  if (fBMP) {
    bmp.ReadFromFile(argv[1]);
    w = bmp.TellWidth();
    h = bmp.TellHeight();
  } else {
    (void)read_png(png, argv[1]);
    w = png.w;
    h = png.h;
  }

  // Convert bmp.rgb_data or png to YUV color planes.
  // On the heap, because even a 3 megapixel image overflows the stack.
  const auto cb = w * h;
  auto bufY = new u8[cb];
  auto bufU = new u8[cb];
  auto bufV = new u8[cb];
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
	const auto rgb = png.pixels + 3*(y*w + x);
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
  if (!costella_unblock(&im, &im, fPhoto, 0, NULL, NULL, 0))
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
	auto rgb = png.pixels + 3*(y*w + x);
	rgb[0] = R;
	rgb[1] = G;
	rgb[2] = B;
      }
    }
  }
  if (fBMP) {
    bmp.WriteToFile(argv[2]);
  } else {
    write_png(png, argv[2]);
  }
  delete [] bufY;
  delete [] bufU;
  delete [] bufV;
  return 0;
}
