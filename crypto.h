#pragma once
#include "bmp.h"

void permute(Bitmap_Image **img);
void encrypt(Bitmap_Image **img);

double* chi_test(const Bitmap_Image *img);

void depermute(Bitmap_Image **img);
void decrypt(Bitmap_Image **img);
