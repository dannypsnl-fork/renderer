#include "model.h"
#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model *model = NULL;
const int width = 200;
const int height = 200;

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

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
  if (t0.y > t1.y)
    swap(t0, t1);
  if (t0.y > t2.y)
    swap(t0, t2);
  if (t1.y > t2.y)
    swap(t1, t2);
  int total_height = t2.y - t0.y;
  for (int y = t0.y; y <= t1.y; y++) {
    int segment_height = t1.y - t0.y + 1;
    float alpha = (float)(y - t0.y) / total_height;
    float beta =
        (float)(y - t0.y) / segment_height; // be careful with divisions by zero
    Vec2i A = t0 + (t2 - t0) * alpha;
    Vec2i B = t0 + (t1 - t0) * beta;
    if (A.x > B.x)
      swap(A, B);
    for (int j = A.x; j <= B.x; j++) {
      image.set(j, y, color);
    }
  }
  for (int y = t1.y; y <= t2.y; y++) {
    int segment_height = t2.y - t1.y + 1;
    float alpha = (float)(y - t0.y) / total_height;
    float beta =
        (float)(y - t1.y) / segment_height; // be careful with divisions by zero
    Vec2i A = t0 + (t2 - t0) * alpha;
    Vec2i B = t1 + (t2 - t1) * beta;
    if (A.x > B.x)
      swap(A, B);
    for (int j = A.x; j <= B.x; j++) {
      image.set(j, y, color);
    }
  }
}

int main(int argc, char **argv) {
  if (2 == argc) {
    model = new Model(argv[1]);
  } else {
    model = new Model("obj/african_head.obj");
  }

  TGAImage image(width, height, TGAImage::RGB);
  Vec3f light_dir(0, 0, -1);

  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    Vec2i screen_coords[3];
    Vec3f world_coords[3];
    for (int j = 0; j < 3; j++) {
      Vec3f v = model->vert(face[j]);
      screen_coords[j] =
          Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
      world_coords[j] = v;
    }
    Vec3f n = (world_coords[2] - world_coords[0]) ^
              (world_coords[1] - world_coords[0]);
    n.normalize();
    float intensity = n * light_dir;
    if (intensity > 0) {
      triangle(
          screen_coords[0], screen_coords[1], screen_coords[2], image,
          TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
    }
  }

  image.flip_vertically();
  image.write_tga_file("output.tga");
  delete model;
}
