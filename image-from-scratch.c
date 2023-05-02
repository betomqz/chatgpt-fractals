#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

int main() {
    // Image dimensions
    int width = 640;
    int height = 480;

    // Allocate memory for image data
    int imageSize = width * height;
    Pixel *image = (Pixel *)malloc(imageSize * sizeof(Pixel));

    // Create image pixel by pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = y * width + x;

            // Create a checkerboard pattern
            if ((x / 32) % 2 == (y / 32) % 2) {
                image[i].r = 255;
                image[i].g = 255;
                image[i].b = 255;
            } else {
                image[i].r = 0;
                image[i].g = 0;
                image[i].b = 0;
            }
        }
    }

    // Save image as BMP file
    FILE *outputFile;
    outputFile = fopen("output.bmp", "wb");

    // BMP header
    unsigned char header[54] = {
        0x42, 0x4D, 0x36, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x02,
        0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x13, 0x0B,
        0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    *(int *)&header[18] = width;
    *(int *)&header[22] = height;
    *(int *)&header[34] = imageSize * sizeof(Pixel);

    fwrite(header, sizeof(unsigned char), 54, outputFile);
    fwrite(image, sizeof(Pixel), imageSize, outputFile);

    // Free memory
    free(image);

    return 0;
}
