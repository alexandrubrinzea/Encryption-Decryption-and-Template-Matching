#pragma once
#define HEADER_SIZE 54
#define TEMPLATE_SIZE 165

typedef struct {
  char r, g, b;
} Pixel, color;

typedef struct {
  unsigned int width, height;
  char header[HEADER_SIZE];
  Pixel *values;

  Pixel **values_matrix;

} Bitmap_Image;

Bitmap_Image* read(const char *filename);
void write(Bitmap_Image *img, const char *filename);

void free_image(Bitmap_Image **img);
