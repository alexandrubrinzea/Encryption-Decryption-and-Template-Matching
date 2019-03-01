#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include "bmp.h"
#include "crypto.h"
#include "xorshift.h"

void permute(Bitmap_Image **img) {
  unsigned int *random_int = xorshift32(123456789, (*img)->width*(*img)->height*2 - 1);
  unsigned int *sigma = (unsigned int*)malloc(sizeof(unsigned int)*(*img)->width*(*img)->height);

  long int index, k;

  for (index = 0; index < (*img)->width*(*img)->height; index++)
    sigma[index] = index;

  for (k = 1, index = (*img)->width*(*img)->height - 1; index >= 0; index--, k++) {
    unsigned int r = random_int[k] % (index + 1);
    unsigned int aux = sigma[index];
    sigma[index] = sigma[r];
    sigma[r] = aux;
  }

  Bitmap_Image* permuted = (Bitmap_Image*)malloc(sizeof(Bitmap_Image));
  permuted->values = (Pixel*)malloc(sizeof(Pixel)*(*img)->width*(*img)->height);
  permuted->width = (*img)->width;
  permuted->height = (*img)->height;

  for (index = 0; index < HEADER_SIZE; index++)
    permuted->header[index] = (*img)->header[index];

  for (index = 0; index < (*img)->width*(*img)->height; index++)
    permuted->values[sigma[index]] = (*img)->values[index];

  free_image(img);
  *img = permuted;

  free(sigma);
  free(random_int);
}

void encrypt(Bitmap_Image **img) {
  unsigned int *random_int = xorshift32(123456789, (*img)->width*(*img)->height*2 - 1);

  unsigned int start_value = 987654321;
  Bitmap_Image *enc = (Bitmap_Image*)malloc(sizeof(Bitmap_Image));
  enc->width = (*img)->width;
  enc->height = (*img)->height;
  enc->values = (Pixel*)malloc(sizeof(Pixel)*enc->width*enc->height);

  int index;

  for (index = 0; index < HEADER_SIZE; index++)
    enc->header[index] = (*img)->header[index];

  enc->values[0].r = (start_value >> 16) ^ (*img)->values[0].r ^ (random_int[enc->width*enc->height] >> 16);
  enc->values[0].g = (start_value >> 8) ^ (*img)->values[0].g ^ (random_int[enc->width*enc->height] >> 8);
  enc->values[0].b = (start_value) ^ (*img)->values[0].b ^ (random_int[enc->width*enc->height]);

  for (index = 1; index < enc->width*enc->height; index++) {
    enc->values[index].r = enc->values[index - 1].r ^ (*img)->values[index].r ^ (random_int[enc->width*enc->height + index] >> 16);
    enc->values[index].g = enc->values[index - 1].g ^ (*img)->values[index].g ^ (random_int[enc->width*enc->height + index] >> 8);
    enc->values[index].b = enc->values[index - 1].b ^ (*img)->values[index].b ^ (random_int[enc->width*enc->height + index]);
  }

  free_image(img);
  *img = enc;
  free(random_int);
}

double* chi_test(const Bitmap_Image *img) {
  double *results = (double*)calloc(sizeof(double), 3);

  long int *freq_r = (long int*)calloc(sizeof(long int),256);
  long int *freq_g = (long int*)calloc(sizeof(long int),256);
  long int *freq_b = (long int*)calloc(sizeof(long int),256);

  long int index;

  for (index = 0; index < img->width*img->height; index++) {
    freq_r[(unsigned char)img->values[index].r]++;
    freq_g[(unsigned char)img->values[index].g]++;
    freq_b[(unsigned char)img->values[index].b]++;
  }
  double f_bared = (double)(img->width*img->height)/256;

  for (index = 0; index < 256; index++) {
    results[0] += (freq_r[index] - f_bared)*(freq_r[index] - f_bared)/f_bared;
    results[1] += (freq_g[index] - f_bared)*(freq_g[index] - f_bared)/f_bared;
    results[2] += (freq_b[index] - f_bared)*(freq_b[index] - f_bared)/f_bared;
  }

  free(freq_r);
  free(freq_b);
  free(freq_g);
  return results;
}

void decrypt(Bitmap_Image **img) {
   unsigned int *random_int = xorshift32(123456789, (*img)->width*(*img)->height*2 - 1);

  unsigned int start_value = 987654321;
  Bitmap_Image *dec = (Bitmap_Image*)malloc(sizeof(Bitmap_Image));
  dec->width = (*img)->width;
  dec->height = (*img)->height;
  dec->values = (Pixel*)malloc(sizeof(Pixel)*dec->width*dec->height);

  int index;

  for (index = 0; index < HEADER_SIZE; index++)
    dec->header[index] = (*img)->header[index];

  dec->values[0].r = (start_value >> 16) ^ (*img)->values[0].r ^ (random_int[dec->width*dec->height] >> 16);
  dec->values[0].g = (start_value >> 8) ^ (*img)->values[0].g ^ (random_int[dec->width*dec->height] >> 8);
  dec->values[0].b = (start_value) ^ (*img)->values[0].b ^ (random_int[dec->width*dec->height]);

  for (index = 1; index < dec->width*dec->height; index++) {
    dec->values[index].r = (*img)->values[index - 1].r ^ (*img)->values[index].r ^ (random_int[dec->width*dec->height + index] >> 16);
    dec->values[index].g = (*img)->values[index - 1].g ^ (*img)->values[index].g ^ (random_int[dec->width*dec->height + index] >> 8);
    dec->values[index].b = (*img)->values[index - 1].b ^ (*img)->values[index].b ^ (random_int[dec->width*dec->height + index]);
  }

  free_image(img);
  *img = dec;
  free(random_int);
}

void depermute(Bitmap_Image **img) {
  unsigned int *random_int = xorshift32(123456789, (*img)->width*(*img)->height*2 - 1);
  unsigned int *sigma = (unsigned int*)malloc(sizeof(unsigned int)*(*img)->width*(*img)->height);

  long int index, k;

  for (index = 0; index < (*img)->width*(*img)->height; index++)
    sigma[index] = index;

  for (k = 1, index = (*img)->width*(*img)->height - 1; index >= 0; index--, k++) {
    unsigned int r = random_int[k] % (index + 1);
    unsigned int aux = sigma[index];
    sigma[index] = sigma[r];
    sigma[r] = aux;
  }

  unsigned int *sigma_inv = (unsigned int*)malloc(sizeof(unsigned int)*(*img)->width*(*img)->height);
  for (index = 0; index < (*img)->width*(*img)->height; index++)
    sigma_inv[sigma[index]] = index;

  Bitmap_Image* permuted = (Bitmap_Image*)malloc(sizeof(Bitmap_Image));
  permuted->values = (Pixel*)malloc(sizeof(Pixel)*(*img)->width*(*img)->height);
  permuted->width = (*img)->width;
  permuted->height = (*img)->height;

  for (index = 0; index < HEADER_SIZE; index++)
    permuted->header[index] = (*img)->header[index];

  for (index = 0; index < (*img)->width*(*img)->height; index++)
    permuted->values[sigma_inv[index]] = (*img)->values[index];

  free_image(img);
  *img = permuted;

  free(sigma);
  free(random_int);
}
