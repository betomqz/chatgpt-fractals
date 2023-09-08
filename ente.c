#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 3000
#define HEIGHT 3000
#define MAX_ITER 100

typedef struct {
    unsigned char b, g, r;
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
            // exit(1);
            return (0 + 0 * I);
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
    p->n = 5;

    p->roots = malloc(p->n * sizeof(double complex));
    p->roots[0] = 0.531715 + 1.17575 * I;
    p->roots[1] = 0.531715 - 1.17575 * I;
    p->roots[2] = -1.21218 + 0.543857 * I;
    p->roots[3] = -1.21218 - 0.543857 * I;
    p->roots[4] = 1.36093 + 0.0 * I;
    // ---------------------------------------------------------

    // Allocate memory for image data
    int imageSize = WIDTH * HEIGHT;
    Pixel* image = (Pixel*)malloc(imageSize * sizeof(Pixel));

    // Posicionar el plano.
    double x_start = -1.0;
    double y_start = -1.0;
    double x_end = 1.0;
    double y_end = (x_end - x_start) + y_start;

    double x_step = (x_end - x_start) / (double)WIDTH;
    double y_step = (y_end - y_start) / (double)HEIGHT;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int idx = i * WIDTH + j;
            z = x_start + j * x_step + (y_start + i * y_step) * I;
            double complex root = newtons_method(z, p, eps);

            image[idx].r = 0;
            image[idx].g = 0;
            image[idx].b = 0;

            if (cabs(root - p->roots[0]) < eps) {
                image[idx].r = 0;
                image[idx].g = 0;
                image[idx].b = 0;
            }

            if (cabs(root - p->roots[1]) < eps) {
                image[idx].r = 55;
                image[idx].g = 10;
                image[idx].b = 17;
            }

            if (cabs(root - p->roots[2]) < eps) {
                image[idx].r = 123;
                image[idx].g = 21;
                image[idx].b = 36;
            }

            if (cabs(root - p->roots[3]) < eps) {
                image[idx].r = 81;
                image[idx].g = 13;
                image[idx].b = 24;
            }

            if (cabs(root - p->roots[4]) < eps) {
                image[idx].r = 165;
                image[idx].g = 28;
                image[idx].b = 48;
            }
        }
    }

    // Save image as BMP file
    FILE* outputFile;
    outputFile = fopen("ente2.bmp", "wb");

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
