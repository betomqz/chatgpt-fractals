# ChatGPT, fractales y C

## 1. Presentación

El lanzamiento de ChatGPT, sin duda, revolucionó la forma en la que pensamos sobre la inteligencia artifical. A todos nos sorprendió la capacidad que tiene para responder a preguntas complejas. A mí, en particular, me dieron ganas de probarlo para escribir programas que, por tiempo, flojera o ignorancia, no he hecho. ¿Realmente servirá? Este artículo es, pues, un ejercicio para escribir la implementación de un programa sobre uno de mis temas favoritos: los fractales de Newton. Estudio Matemáticas Aplicadas e Ing. en Computación y se me hace que este tema es una excelente combinación de ambas carreras. La teoría es asombrosa y las matemáticas bastante simples, pero es difícil apreciar la belleza del tema sin algo que nos permita observar los fractales. Aunque aquí describo brevemente la teoría que nos servirá para escribir el programa, animaría mucho al lector a revisar más bibliografía sobre el tema, como el excelente [video de 3blue1brown](https://youtu.be/-RdOwhmqP5s) o un [artículo]() que escribió mi buen amigo Sebastián de Vega Potts.

Mi intención con este ejercicio es hacer un programa que genere fractales de Newton eficientemente. Aunque ya he implementado diferentes versiones de esto, me gustaría tener la libertad de manipular, uno a uno, los pixeles de una imagen y escribir en ella directamente, sin depender de conversión de gráficas a imágenes; además, para que sea más eficiente, me parece que es un buen ejercicio para implementar concurrencia y lanzar varios hilos que trabajen con un pixel cada uno al mismo tiempo. Por estas dos razones —y para que sea un reto— escogí C como lenguaje de programación. Normalmente eso me daría un poco de miedo y flojera, pero quiero aprovechar la potencia que ofrece ChatGPT y ver realmente qué tan útil puede llegar a ser.

## 2. Una breve introducción a los fractales

### 2.1. El método de Newton

Aunque puede parecer una tarea simple, encontrar los puntos en los que una función es igual a cero puede ser una tarea muy complicada. Uno de los métodos más comunes para resolver este problema es el método de Newton. El método consiste en lo siguiente: una función $f:\mathbb{R} \to \mathbb{R}$ y un punto de partida $x_0$, evaluar la función en el punto de partida —esto es, obtener $f(x_0)$—, calcular la recta tangente a este punto —usamos la derivada, $f'(x_0)$—, encontrar el punto en el que dicha recta interseca al eje horizontal, que llamaremos $x_1$, y repetir el proceso hasta que $f(x_i)$ esté lo suficientemente cerca de cero. Para que quede un poco más claro, podemos ver la siguiente animación de este método:

![](/manim/media/videos/newtons-method/1080p60/NewtonsMethod.gif)

# 3. Manipulación de imágenes con C

Como comenté en la introducción, me gustaría poder manipular una imagen pixel a pixel usando C, pero es algo que nunca he hecho antes. Aquí ChatGPT hace su debut: le pregunté: `how can I manipulate an image pixel by pixel in C?` y me contestó con el siguiente código —no sin antes recordarme que es programación básica—:

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

int main() {
    // Load image
    FILE *imageFile;
    imageFile = fopen("image.bmp", "rb");
    if (imageFile == NULL) {
        printf("Error opening image file.\n");
        exit(1);
    }

    // Read image header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, imageFile);

    // Read image dimensions
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // Allocate memory for image data
    int imageSize = width * height;
    Pixel *image = (Pixel*) malloc(imageSize * sizeof(Pixel));

    // Read image data
    fread(image, sizeof(Pixel), imageSize, imageFile);

    // Manipulate image pixel by pixel
    for (int i = 0; i < imageSize; i++) {
        // Invert each color channel
        image[i].r = 255 - image[i].r;
        image[i].g = 255 - image[i].g;
        image[i].b = 255 - image[i].b;
    }

    // Save modified image
    FILE *outputFile;
    outputFile = fopen("output.bmp", "wb");
    fwrite(header, sizeof(unsigned char), 54, outputFile);
    fwrite(image, sizeof(Pixel), imageSize, outputFile);

    // Free memory
    free(image);

    return 0;
}
```

El código se ve muy bien, pero funciona solamente si ya tenemos una imagen y queremos modificarla. No es eso lo que buscamos en este caso, por lo que le hice la siguiente petición a ChatGPT: `modify the code above so it creates the image from scratch` y aquí la respuesta:

```c
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
    Pixel *image = (Pixel*) malloc(imageSize * sizeof(Pixel));

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
        0x42, 0x4D, 0x36, 0x00, 0x0C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xE0, 0x01,
        0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x13, 0x0B,
        0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00
    };
    *(int*)&header[18] = width;
    *(int*)&header[22] = height;
    *(int*)&header[34] = imageSize * sizeof(Pixel);

    fwrite(header, sizeof(unsigned char), 54, outputFile);
    fwrite(image, sizeof(Pixel), imageSize, outputFile);

    // Free memory
    free(image);

    return 0;
}
```

El código funciona muy bien. La siguiente imagen es el resultado si compilamos y ejecutamos el código:
![](/imgs/fig1.bmp)

# 4. Una librería dinámica en C

Quiero que ChatGPT programe una librería dinámica para trabajar con las funciones de la forma $f: \mathbb{C} \to \mathbb{C}$. Me parece que podemos separar esto porque, para que la implementación que haremos del método de Newton funcione, necesitamos poder evaluar la función, conocer su derivada y también sus raíces. Aunque el método como tal puede trabajar con una familia grande de funciones, aquí nos limitaremos a trabajar con polinomios mónicos —polinomios cuyo coeficiente principal es siempre 1— porque así podemos decidir de antemano las raíces de la función y con eso obtener un polinomio mónico único, así como su derivada, sin tener que rompernos tanto la cabeza; además, esto nos da suficiente espacio para experimentar y jugar con diferentes fractales. Seguro no nos vamos a aburrir, pero por si en algún futuro queremos trabajar con una familia de funciones más amplia, podemos modificar la librería dinámica y ya.

# 5. El Método de Newton en C

# 6. Nuestro primer fractal

# 7. Concurrencia

## 7.1 Concurrencia a la hora de generar el fractal

## 7.2 Concurrencia a la hora de guardar la imagen

# 8. Conclusiones
