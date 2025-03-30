#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Zeitmessung
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"


int color_bucket_index(int r, int g, int b) {
    return ((r / 16) << 8) | ((g / 16) << 4) | (b / 16);
}

void compute_color_mask(unsigned char *image, unsigned char *mask, int w, int h, int tolerance) {
    int counts[4096] = {0};

    // Nicht parallelisierbar, wegen counts Zugriff
    for (int i = 0; i < w * h; i++) {
        int r = image[i * 3];
        int g = image[i * 3 + 1];
        int b = image[i * 3 + 2];
        int bucket = color_bucket_index(r, g, b);
        counts[bucket]++;
    }

    // Keine Paraleisierung wegen "nur" 4096 Iterationen und max_count zugriff
    int max_count = 0, dominant = 0;
    for (int i = 0; i < 4096; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            dominant = i;
        }
    }

    int dom_r = ((dominant >> 8) & 0xF) * 16;
    int dom_g = ((dominant >> 4) & 0xF) * 16;
    int dom_b = (dominant & 0xF) * 16;

    #pragma omp parallel for schedule(auto)
    for (int i = 0; i < w * h; i++) {
        int r = image[i * 3];
        int g = image[i * 3 + 1];
        int b = image[i * 3 + 2];
        int dr = abs(r - dom_r);
        int dg = abs(g - dom_g);
        int db = abs(b - dom_b);

        if (dr + dg + db < tolerance) {
            mask[i] = 0;   // Hintergrund
        } else {
            mask[i] = 255; // Vordergrund
        }
    }
}

void apply_grayscale_on_mask(unsigned char *image, unsigned char *mask, unsigned char *output, int w, int h) {
    
    #pragma omp parallel for schedule(auto)
    for (int i = 0; i < w * h; i++) {
        int r = image[i * 3];
        int g = image[i * 3 + 1];
        int b = image[i * 3 + 2];

        if (mask[i] == 255) {
            int gray = (r + g + b) / 3;
            output[i * 3]     = gray;
            output[i * 3 + 1] = gray;
            output[i * 3 + 2] = gray;
        } else {
            output[i * 3]     = r;
            output[i * 3 + 1] = g;
            output[i * 3 + 2] = b;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Verwendung: %s <input.jpg> <output.png> <tolerance>\n", argv[0]);
        return 1;
    }

    int w, h, ch;
    unsigned char *input = stbi_load(argv[1], &w, &h, &ch, 3);
    if (!input) {
        printf("Bild konnte nicht geladen werden.\n");
        return 1;
    }

    int tolerance = atoi(argv[3]);

    unsigned char *mask = calloc(w * h, 1);
    unsigned char *output = malloc(w * h * 3);

       // debugger Zeilen
       #pragma omp parallel
       {
           int thread_num = omp_get_thread_num();
           int num_threads = omp_get_num_threads();
           #pragma omp critical
           {
               // Nummerierung der Threads sollte bei 0 beginnen
           printf("Thread %d von %d Threads aktiv\n", thread_num, num_threads);
           }
       }

    double start = omp_get_wtime();
    
    compute_color_mask(input, mask, w, h, tolerance);
    apply_grayscale_on_mask(input, mask, output, w, h);

    double end = omp_get_wtime();

    // int success = stbi_write_png(argv[2], w, h, 3, output, w * 3);
    int success = stbi_write_png("../webapp/static/output.png", w, h, 3, output, w * 3);
    if (!success) {
        printf("Fehler beim Speichern des Bildes!\n");
    }

    stbi_image_free(input);
    free(mask);
    free(output);

    printf("Gefiltertes Bild geschrieben: %s (Tolerance: %d)\n", argv[2], tolerance);
    printf("Verarbeitungszeit: %.5f Sekunden\n", end - start);
    
    return 0;
}
