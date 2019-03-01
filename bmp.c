#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "bmp.h"

Bitmap_Image* read(const char *filename) {
  FILE *f_in = fopen(filename, "rb");
  if (!f_in) {
    printf("File not opened\n");
    perror(fopen);
    exit(1);
  }
  Bitmap_Image *img = (Bitmap_Image*)malloc(sizeof(Bitmap_Image));
  fread(img->header, HEADER_SIZE, sizeof(char), f_in);
  img->width = *(unsigned int*)&img->header[18];
  img->height = *(unsigned int*)&img->header[22];


  int padding;
  if(img->width % 4 != 0)
    padding = 4 - (3 * img->width) % 4;
  else
    padding = 0;

    long int i, j, k = 0;

  img->values = (Pixel*)malloc(sizeof(Pixel)*img->width*img->height);




    img->values_matrix = (Pixel**)malloc(sizeof(Pixel*)*img->height);
    for(i = 0; i < img->height; i++)
            img->values_matrix[i] = (Pixel*)calloc(sizeof(Pixel), img->width);




  for (i = img->height - 1; i >= 0; i--) {
    for (j = 0; j < img->width; j++) {
      char temp_pixel[3];
      fread(temp_pixel, sizeof(char), 3, f_in);
      img->values[i*img->width + j].r = temp_pixel[2];
      img->values[i*img->width + j].g = temp_pixel[1];
      img->values[i*img->width + j].b = temp_pixel[0];
    }
    fseek(f_in, padding, SEEK_CUR);
  }



  for(i = 0; i < img->height; i++)
        for(j = 0; j < img->width; j++)
        {
            img->values_matrix[i][j].r = img->values[k].r;
            img->values_matrix[i][j].g = img->values[k].g;
            img->values_matrix[i][j].b = img->values[k].b;
            k++;
        }

  return img;
}

void write(Bitmap_Image *img, const char *filename) {
  FILE *f_out = fopen(filename, "wb");
  if (!f_out) {
    printf("Output file not open\n");
    exit(2);
  }

  fwrite(img->header, sizeof(char), HEADER_SIZE, f_out);


  int padding;
  if(img->width % 4 != 0)
    padding = 4 - (3 * img->width) % 4;
  else
    padding = 0;

    long int i, j;

  for (i = img->height - 1; i >= 0; i--) {
    for (j = 0; j < img->width; j++) {

      fwrite(&img->values[i*img->width + j].b, sizeof(char), 1, f_out);
      fwrite(&img->values[i*img->width + j].g, sizeof(char), 1, f_out);
      fwrite(&img->values[i*img->width + j].r, sizeof(char), 1, f_out);

    }

    if (i != 0) {
      fseek(f_out, padding, SEEK_CUR);
    }
  }

  if (padding) {
    char* padding_bytes = (char*)calloc(sizeof(char), padding);
    fwrite(padding_bytes, sizeof(char), padding, f_out);
  }
}

void free_image(Bitmap_Image **img) {
  free((*img)->values);
  free((*img)->values_matrix);
  free((*img));
}
