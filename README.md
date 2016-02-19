# Talk - Primera parte

[talk](https://en.wikipedia.org/wiki/Talk_%28software%29) es un programa de mensajería instantánea en modo texto que fue introducido en los sistemas UNIX en 1983. Básicamente permitía que diferentes usuarios en diferentes máquinas pudieran conversar. Este sistema con el tiempo fue sustituido por el [IRC](https://en.wikipedia.org/wiki/Internet_Relay_Chat) —apareció en 1988— y este a su vez dio paso a la [multitud de sistemas de mensajería instantánea](https://en.wikipedia.org/wiki/Comparison_of_instant_messaging_protocols) que existen actualmente.


## Compilación:

   Debido a que se utiliza C++ 11, hay que añadir esta directiva al compilar: **-std=c++11**
   
   	g++ -std=c++11 main.cpp

## Prueba básica:
   Este chat esta preparado para probarse en local inicialmente. Para poder hacer las pruebas básicas se han definido dos constantes, **LOCALPORT** y **REMOTEPORT**.

   Se deben compilar dos versiones del programa, con los puertos invertidos. 

   Por ejemplo:
   
   	Programa 1: LOCALPORT 3000 REMOTEPORT 4000

   	Programa 2: LOCALPORT 4000 REMOTEPORT 3000

   Ahora podemos compilar ambos programas y probar a comunicarlos entre ellos.

## Características:

1. El programa utiliza comunicación mediante SOCKETS de Unix.
2. La comunicación es vía UDP.
3. Se mandan estructuras de tipo Message, consistentes por ahora, en cadenas de caracteres.
	
## Modo de uso:
1. Para mandar un mensaje simplemente escribe en la línea el mensaje a enviar y pulsa enter, recibirás posteriormente el mensaje del otro usuario.
2. Para salir puedes escribir /quit
