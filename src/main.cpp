#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  bool steep = false;
  if (abs(x0 - x1) < abs(y0 - y1)) {
    swap(x0, y0);
    swap(x1, y1);
    steep = true;
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  for (int x = x0; x <= x1; x++) {
    float t = (x - x0) / (float)(x1 - x0);
    int y = y0 * (1. - t) + y1 * t;
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
  }
}

int main() {
  TGAImage image{100, 100, TGAImage::RGB};
  line(13, 20, 80, 40, image, white);
  line(20, 13, 40, 80, image, red);
  line(80, 40, 13, 20, image, red);
  image.flip_vertically();
  image.write_tga_file("output.tga");
}
