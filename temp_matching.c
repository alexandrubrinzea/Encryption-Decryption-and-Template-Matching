#include "bmp.h"
#include <math.h>

void temp_matching(Bitmap_Image **source_img, Bitmap_Image **temp, Pixel color) {
    long int  i, j, k_i, k_j, d_i, d_j;
    double corr_S_fI = 0, sigma_temp = 0, sigma_fI = 0, I_grayscale_intensity_average = 0, template_grayscale_intensity_average = 0;

    for(i = 0; i < (*temp)->height; i++)
            for(j = 0; j < (*temp)->width; j++)
                template_grayscale_intensity_average += (*temp)->values_matrix[i][j].r;

    template_grayscale_intensity_average = template_grayscale_intensity_average/TEMPLATE_SIZE;


    for(i = 0; i < (*temp)->height; i++)
            for(j = 0; j < (*temp)->width; j++)
                sigma_temp += ((*temp)->values_matrix[i][j].r - template_grayscale_intensity_average) * ((*temp)->values_matrix[i][j].r - template_grayscale_intensity_average);

    sigma_temp = sigma_temp/(TEMPLATE_SIZE-1);
    sigma_temp = sqrt(sigma_temp);



    for(i = 0; i < (*source_img)->height - 15; i++)
            for(j = 0; j < (*source_img)->width - 11; j++)
            {
                corr_S_fI = 0;
                I_grayscale_intensity_average = 0;
                sigma_fI = 0;

                for(k_i = i; k_i < i+15; k_i++)
                    for(k_j = j; k_j < j+11; k_j++)
                        I_grayscale_intensity_average +=  (*source_img)->values_matrix[k_i][k_j].r;

                I_grayscale_intensity_average = I_grayscale_intensity_average/TEMPLATE_SIZE;


                for(k_i = i; k_i < i+15; k_i++)
                    for(k_j = j; k_j < j+11; k_j++)
                        sigma_fI += ((*source_img)->values_matrix[k_i][k_j].r - I_grayscale_intensity_average) * ((*source_img)->values_matrix[k_i][k_j].r - I_grayscale_intensity_average);

                sigma_fI = sigma_fI/(TEMPLATE_SIZE-1);
                sigma_fI = sqrt(sigma_fI);

                for(k_i = i, d_i = 0; k_i < i+15 && d_i < 15; k_i++, d_i++)
                    for(k_j = j, d_j = 0; k_j < j+11 && d_j < 11; k_j++, d_j++)
                        corr_S_fI += (1/(sigma_temp * sigma_fI))*((*source_img)->values_matrix[k_i][k_j].r - I_grayscale_intensity_average)*((*temp)->values_matrix[d_i][d_j].r - template_grayscale_intensity_average);

                corr_S_fI = corr_S_fI/TEMPLATE_SIZE;

                if (corr_S_fI > 0.33)
                {

                    for(k_j = j; k_j < j+11; k_j++)
                    {
                        (*source_img)->values[i*(*source_img)->width + k_j].r = color.r;
                        (*source_img)->values[i*(*source_img)->width + k_j].g = color.g;
                        (*source_img)->values[i*(*source_img)->width + k_j].b = color.b;
                    }

                    for(k_j = j; k_j < j+11; k_j++)
                    {
                        (*source_img)->values[(i+14)*(*source_img)->width + k_j].r = color.r;
                        (*source_img)->values[(i+14)*(*source_img)->width + k_j].g = color.g;
                        (*source_img)->values[(i+14)*(*source_img)->width + k_j].b = color.b;
                    }

                    for(k_i = i; k_i < i+14; k_i++)
                    {
                        (*source_img)->values[k_i*(*source_img)->width + j].r = color.r;
                        (*source_img)->values[k_i*(*source_img)->width + j].g = color.g;
                        (*source_img)->values[k_i*(*source_img)->width + j].b = color.b;
                    }

                    for(k_i = i; k_i < i+14; k_i++)
                    {
                        (*source_img)->values[k_i*(*source_img)->width + j+10].r = color.r;
                        (*source_img)->values[k_i*(*source_img)->width + j+10].g = color.g;
                        (*source_img)->values[k_i*(*source_img)->width + j+10].b = color.b;
                    }
                }


            }

}

