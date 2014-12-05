// Standalone program to convert one .png file into another, processing it pixelwise.
// Link with -lpng -lm.

#include <png.h>
#include <algorithm>
#include <string>
#include <cmath>
#include <cassert>

struct rgbpixel { unsigned char r,g,b; };

using std::string;
string filenameExtension(const string& s) {
  const string::size_type i = s.find_last_of(".");
  if (i == string::npos)
    return "";
  string ext(s.substr(i + 1));
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  return ext;
}

void process(const int w, const int h, rgbpixel **ppRow) {
  // Test pattern.
  for (int y = h/2; y < h; ++y) {
    for (int x = 0; x < w/2; ++x) {
      rgbpixel& rgb = ppRow[y][x];
      rgb.r = x % 128;
      rgb.g /= 2;
    }
  }
}

int main( int argc, char** argv) {
  if (argc != 3) {
    printf("usage: %s in.png out.png\n", argv[0]);
    return 1;
  }
  if (filenameExtension(argv[1]) != "png")
    printf("%s: warning: filename %s doesn't end in png.\n", argv[0], argv[1]);
  if (filenameExtension(argv[2]) != "png")
    printf("%s: warning: filename %s doesn't end in png.\n", argv[0], argv[2]);

  FILE *fp = fopen(argv[1], "rb");
  if (!fp) {
    printf("%s: failed to read file %s.\n", argv[0], argv[1]);
    return 1;
  }
  png_structp pPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop pInfoPNG = png_create_info_struct(pPNG);
  png_init_io(pPNG, fp);
  png_read_png(pPNG, pInfoPNG, PNG_TRANSFORM_STRIP_ALPHA|PNG_TRANSFORM_EXPAND|PNG_TRANSFORM_STRIP_16, NULL);
  // Preallocating a single block (two png_malloc's and one png_set_rows)
  // may be faster and cache-friendlier than how png_get_rows allocates h separate rows,
  // but other things dominate the runtime of this program.
  const int w = png_get_image_width  (pPNG, pInfoPNG);
  const int h = png_get_image_height (pPNG, pInfoPNG);
  rgbpixel **ppRowPNG = (rgbpixel**)png_get_rows(pPNG, pInfoPNG);
  png_destroy_read_struct(&pPNG, NULL, NULL);
  fclose(fp);
  // The pixels, of type rgbpixel, are ppRowPNG[y][x], 0<y<h, 0<x<w.

//printf("Processing.\n");
  process(w, h, ppRowPNG);

  fp = fopen(argv[2], "wb");
  if (!fp) {
    printf("%s: failed to write file %s.\n", argv[0], argv[2]);
    return 1;
  }
  pPNG = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_init_io(pPNG, fp);

  // Without this block, writing is 9x slower than reading + simple processing.
  // The lossless compression can be quite slow.
  png_set_filter(pPNG, 0, PNG_FILTER_VALUE_SUB);
  // PNG_FILTER_NONE is slightly faster than _SUB but ~2x larger;
  // but use NONE when compresion_level is 0.
  png_set_compression_level(pPNG, 1);
  // 0 none (1920x1080 = 20 MB), 1 fast (7 MB), 3-6 decent, 9 full but slow

  png_set_rows(pPNG, pInfoPNG, (png_bytepp)ppRowPNG);
//printf("Writing.\n");
  png_write_png(pPNG, pInfoPNG, PNG_TRANSFORM_IDENTITY, NULL);
  png_destroy_write_struct(&pPNG, &pInfoPNG);
  fclose(fp);
  return 0;
}
