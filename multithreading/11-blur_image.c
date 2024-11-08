#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "multithreading.h"

#define MAX_THREADS 16

// Structure for thread arguments
typedef struct {
    img_t *img_blur;
    img_t const *img;
    kernel_t const *kernel;
    size_t start;
    size_t end;
} thread_arg_t;

// Function to apply kernel on a specific image portion (called by each thread)
void *blur_portion(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    size_t x, y, i, j;
    int half_k = targ->kernel->size / 2;

    for (y = targ->start; y < targ->end; y++) {
        for (x = 0; x < targ->img->w; x++) {
            float r = 0, g = 0, b = 0;
            float kernel_sum = 0;

            // Convolution operation
            for (i = 0; i < targ->kernel->size; i++) {
                for (j = 0; j < targ->kernel->size; j++) {
                    int pixel_x = x + i - half_k;
                    int pixel_y = y + j - half_k;

                    // Check bounds
                    if (pixel_x >= 0 && pixel_x < targ->img->w && pixel_y >= 0 && pixel_y < targ->img->h) {
                        pixel_t *pixel = &targ->img->pixels[pixel_y * targ->img->w + pixel_x];
                        float kernel_val = targ->kernel->matrix[i][j];

                        r += pixel->r * kernel_val;
                        g += pixel->g * kernel_val;
                        b += pixel->b * kernel_val;
                        kernel_sum += kernel_val;
                    }
                }
            }

            // Normalize the values
            size_t idx = y * targ->img->w + x;
            targ->img_blur->pixels[idx].r = (unsigned char)(r / kernel_sum);
            targ->img_blur->pixels[idx].g = (unsigned char)(g / kernel_sum);
            targ->img_blur->pixels[idx].b = (unsigned char)(b / kernel_sum);
        }
    }
    return NULL;
}

// Main function for blurring the image using multiple threads
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel) {
    pthread_t threads[MAX_THREADS];
    thread_arg_t args[MAX_THREADS];
    size_t thread_count = MAX_THREADS;
    size_t rows_per_thread = img->h / thread_count;

    for (size_t i = 0; i < thread_count; i++) {
        args[i].img_blur = img_blur;
        args[i].img = img;
        args[i].kernel = kernel;
        args[i].start = i * rows_per_thread;
        args[i].end = (i == thread_count - 1) ? img->h : (i + 1) * rows_per_thread;

        pthread_create(&threads[i], NULL, blur_portion, &args[i]);
    }

    for (size_t i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}
