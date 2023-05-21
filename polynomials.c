#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1600
#define HEIGHT 1200
#define MAX_ITER 100
#define EPS 1e-6

typedef struct {
    unsigned char r, g, b;
} Pixel;

struct Roots {
    int n;                  // number of roots
    double complex* roots;  // array of roots
};

double complex eval(double complex* z, struct Roots* p) {
    double complex result = 1.0;
    for (int i = 0; i < p->n; i++) {
        result *= (*z - p->roots[i]);
    }
    return result;
}

double complex derivative(double complex* z, struct Roots* p) {
    double complex result = 0.0;
    for (int i = 0; i < p->n; i++) {
        double complex term = 1.0;
        for (int j = 0; j < p->n; j++) {
            if (i != j) {
                term *= (*z - p->roots[j]);
            }
        }
        result += term;
    }
    return result;
}

double complex newtons_method(double complex x0, struct Roots* p, double eps) {
    double complex x = x0;
    double complex fx, fxprime;
    int iter = 0;
    do {
        fx = eval(&x, p);
        fxprime = derivative(&x, p);
        if (fxprime == 0.0) {
            printf("Error: derivative is zero.\n");
            exit(1);
        }
        x = x - fx / fxprime;
        iter++;
    } while (cabs(fx) > eps && iter < 1000);
    if (iter == 1000) {
        printf("Error: maximum iterations exceeded.\n");
        exit(1);
    }
    return x;
}

int main() {
    complex double z;
    double eps = 1e-6;
    struct Roots* p = malloc(sizeof(struct Roots));
    p->n = 4;

    p->roots = malloc(p->n * sizeof(double complex));
    p->roots[0] = 1 + 1 * I;
    p->roots[1] = 1 - 1 * I;
    p->roots[2] = -1 + 1 * I;
    p->roots[0] = -1 - 1 * I;

    // ---------------------------------------------------------

    // Allocate memory for image data
    int imageSize = WIDTH * HEIGHT;
    Pixel* image = (Pixel*)malloc(imageSize * sizeof(Pixel));

    double x_start = -1;
    double x_end = 1;
    double y_start = -1.5;
    double y_end = (double)HEIGHT / (double)WIDTH * (x_end - x_start) + y_start;
    double x_step = (x_end - x_start) / (double)WIDTH;
    double y_step = (y_end - y_start) / (double)HEIGHT;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int idx = i * WIDTH + j;
            z = x_start + j * x_step + (y_start + i * y_step) * I;
            double complex root = newtons_method(z, p, eps);

            image[idx].r = 0;  // Azul
            image[idx].g = 0;
            image[idx].b = 0;  // Rojo
            for (int k = 0; k < p->n; k++) {
                if (cabs(root - p->roots[k]) < EPS) {
                    if (k == 0) {
                        image[idx].b = 184;
                        image[idx].g = 229;
                        image[idx].r = 206;
                    } else if (k == 1) {
                        image[idx].b = 242;
                        image[idx].g = 184;
                        image[idx].r = 211;
                    } else if (k == 2) {
                        image[idx].b = 206;
                        image[idx].g = 179;
                        image[idx].r = 226;
                    } else {
                        image[idx].b = 149;
                        image[idx].g = 204;
                        image[idx].r = 244;
                    }
                    break;
                }
            }
            // printf("%d ", root_idx);
        }
        // printf("\n");
    }

    // Save image as BMP file
    FILE* outputFile;
    outputFile = fopen("output.bmp", "wb");

    // BMP header
    unsigned char header[54] = {
        0x42, 0x4D, 0x36, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x02,
        0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x13, 0x0B,
        0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    *(int*)&header[18] = WIDTH;
    *(int*)&header[22] = HEIGHT;
    *(int*)&header[34] = imageSize * sizeof(Pixel);

    fwrite(header, sizeof(unsigned char), 54, outputFile);
    fwrite(image, sizeof(Pixel), imageSize, outputFile);

    // Free memory
    free(image);

    free(p->roots);
    free(p);

    return 0;
}
