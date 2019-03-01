#include <stdio.h>
#include <stdlib.h>
#include "xorshift.h"
#include "bmp.h"
#include "crypto.h"
#include "temp_matching.h"

int main() {
  Bitmap_Image* img = read("peppers.bmp");
  printf("Width: %u, Height: %u\n", img->width, img->height);
  permute(&img);
  encrypt(&img);
  double *chi_squared = chi_test(img);
  printf("R:%.2f G:%.2f B:%.2f\n", chi_squared[0], chi_squared[1], chi_squared[2]);
  write(img, "encrypted.bmp");

  decrypt(&img);
  depermute(&img);
  write(img, "original.bmp");

    Bitmap_Image* img_I = read("test_grayscale.bmp");
    Bitmap_Image* template_0 = read("cifra0.bmp");
    Bitmap_Image* template_1 = read("cifra1.bmp");
    Bitmap_Image* template_2 = read("cifra2.bmp");
    Bitmap_Image* template_3 = read("cifra3.bmp");
    Bitmap_Image* template_4 = read("cifra4.bmp");
    Bitmap_Image* template_5 = read("cifra5.bmp");
    Bitmap_Image* template_6 = read("cifra6.bmp");
    Bitmap_Image* template_7 = read("cifra7.bmp");
    Bitmap_Image* template_8 = read("cifra8.bmp");
    Bitmap_Image* template_9 = read("cifra9.bmp");

    color red_0, yellow_1, green_2, cyan_3, magenta_4, blue_5, silver_6, blue_7, magenta_8, blue_9;

    red_0.r = -1;           red_0.g = 0;             red_0.b = 0;
    yellow_1.r = -1;        yellow_1.g = -1;         yellow_1.b = 0;
    green_2.r = 0;          green_2.g = -1;          green_2.b = 0;
    cyan_3.r = 0;           cyan_3.g = -1;           cyan_3.b = -1;
    magenta_4.r = -1;       magenta_4.g = 0;         magenta_4.b = -1;
    blue_5.r = 0;           blue_5.g = 0;            blue_5.b = -1;
    silver_6.r = -58;       silver_6.g = -58;        silver_6.b = -58;
    blue_7.r = -1;          blue_7.g = -116;            blue_7.b = 0;
    magenta_8.r = -128;       magenta_8.g = 0;         magenta_8.b = -128;
    blue_9.r = -128;          blue_9.g = 0;            blue_9.b = 0;


   temp_matching(&img_I, &template_0, red_0);
   temp_matching(&img_I, &template_1, yellow_1);
   temp_matching(&img_I, &template_2, green_2);
   temp_matching(&img_I, &template_3, cyan_3);
   temp_matching(&img_I, &template_4, magenta_4);
   temp_matching(&img_I, &template_5, blue_5);
   temp_matching(&img_I, &template_6, silver_6);
   temp_matching(&img_I, &template_7, blue_7);
   temp_matching(&img_I, &template_8, magenta_8);
   temp_matching(&img_I, &template_9, blue_9);

   write(img_I, "imaginecolorata.bmp");

  free(chi_squared);
  free_image(&img);


  free_image(&template_0);
  free_image(&template_1);
  free_image(&template_2);
  free_image(&template_3);
  free_image(&template_4);
  free_image(&template_5);
  free_image(&template_6);
  free_image(&template_7);
  free_image(&template_8);
  free_image(&template_9);

  return 0;
}
