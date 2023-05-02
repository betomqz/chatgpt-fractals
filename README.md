# ChatGPT, fractales y C

## 1. Presentación

El lanzamiento de ChatGPT, sin duda, revolucionó la forma en la que pensamos sobre la inteligencia artifical. A todos nos sorprendió la capacidad que tiene para responder a preguntas complejas. A mí, en particular, me dieron ganas de probarlo para escribir programas que, por tiempo, flojera o ignorancia, no he hecho. ¿Realmente servirá? Este artículo es, pues, un ejercicio para escribir la implementación de un programa sobre uno de mis temas favoritos: los fractales de Newton. Estudio Matemáticas Aplicadas e Ing. en Computación y se me hace que este tema es una excelente combinación de ambas carreras. La teoría es asombrosa y las matemáticas bastante simples, pero es difícil apreciar la belleza del tema sin algo que nos permita observar los fractales. Aunque aquí describo brevemente la teoría que nos servirá para escribir el programa, animaría mucho al lector a revisar más bibliografía sobre el tema, como el excelente [video de 3blue1brown]() o un [artículo]() que escribió mi buen amigo Sebastián de Vega Potts.

Mi intención con este ejercicio es hacer un programa que genere fractales de Newton eficientemente. Aunque ya he implementado diferentes versiones de esto, me gustaría tener la libertad de manipular, uno a uno, los pixeles de una imagen y escribir en ella directamente, sin depender de conversión de gráficas a imágenes; además, para que sea más eficiente, me parece que es un buen ejercicio para implementar concurrencia y lanzar varios hilos que trabajen con un pixel cada uno al mismo tiempo. Por estas dos razones —y para que sea un reto— escogí C como lenguaje de programación. Normalmente eso me daría un poco de miedo y flojera, pero quiero aprovechar la potencia que ofrece ChatGPT y ver realmente qué tan útil puede llegar a ser.

## 2. Una breve introducción a los fractales

### 2.1. El método de Newton

Aunque puede parecer una tarea simple, encontrar los puntos en los que una función es igual a cero puede ser una tarea muy complicada. Uno de los métodos más comunes para resolver este problema es el método de Newton. El método consiste en lo siguiente: una función $f:\mathbb{R} \to \mathbb{R}$ y un punto de partida $x_0$, evaluar la función en el punto de partida —esto es, obtener $f(x_0)$—, calcular la recta tangente a este punto —usamos la derivada, $f'(x_0)$—, encontrar el punto en el que dicha recta interseca al eje horizontal, que llamaremos $x_1$, y repetir el proceso hasta que $f(x_i)$ esté lo suficientemente cerca de cero. Para que quede un poco más claro, podemos ver la siguiente animación de este método:

![](/manim/media/videos/newtons-method/1080p60/NewtonsMethod.gif)
