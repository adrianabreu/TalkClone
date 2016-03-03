# Talk - Primera parte

[talk](https://en.wikipedia.org/wiki/Talk_%28software%29) es un programa de mensajería instantánea en modo texto que fue introducido en los sistemas UNIX en 1983. Básicamente permitía que diferentes usuarios en diferentes máquinas pudieran conversar. Este sistema con el tiempo fue sustituido por el [IRC](https://en.wikipedia.org/wiki/Internet_Relay_Chat) —apareció en 1988— y este a su vez dio paso a la [multitud de sistemas de mensajería instantánea](https://en.wikipedia.org/wiki/Comparison_of_instant_messaging_protocols) que existen actualmente.


## Compilación:

   Debido a que se utiliza tanto C++ 11 como hilos, hay que añadir estas directivas al compilar: **-std=c++11 -pthread**
   
   `g++ -std=c++11 -pthread socket.cpp main.cpp`

   Alternativamente si tienes qmake (debe usarse la version de qt5)
   ```
   qmake talk.pro
   make
   ``` 

   
## Prueba básica:
   Este chat esta preparado para probarse en local inicialmente. Para poder hacer las pruebas básicas se han definido dos constantes, **LOCALPORT** y **REMOTEPORT**.

   Se deben compilar dos versiones del programa, con los puertos invertidos. 

   Por ejemplo:
   
      Programa 1: LOCALPORT 3000 REMOTEPORT 4000

      Programa 2: LOCALPORT 4000 REMOTEPORT 3000

   Ahora podemos compilar ambos programas y probar a comunicarlos entre ellos.

## Características:

1. El programa utiliza comunicación mediante SOCKETS.
2. La comunicación es vía UDP.
3. Se mandan estructuras de tipo Message, consistentes por ahora, en cadenas de caracteres.
   
## Modo de uso:
1. Para mandar un mensaje simplemente escribe en la línea el mensaje a enviar y pulsa enter.
2. Si otro usuario conectado manda un mensaje, este aparecerá en la pantalla indicando la ip del usuario que lo dijo.
3. Para salir puedes escribir en cualquier momento `/quit` o presionar `Ctrl + D`.

