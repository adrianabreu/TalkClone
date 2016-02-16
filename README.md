# Talk - Primera parte

[talk](https://en.wikipedia.org/wiki/Talk_%28software%29) es un programa de mensajería instantánea en modo texto que fue introducido en los sistemas UNIX en 1983. Básicamente permitía que diferentes usuarios en diferentes máquinas pudieran conversar. Este sistema con el tiempo fue sustituido por el [IRC](https://en.wikipedia.org/wiki/Internet_Relay_Chat) —apareció en 1988— y este a su vez dio paso a la [multitud de sistemas de mensajería instantánea](https://en.wikipedia.org/wiki/Comparison_of_instant_messaging_protocols) que existen actualmente.

Aunque en tecnología ningún tiempo pasado fue mejor, nosotros vamos a volver temporalmente a 1983 para desarrollar nuestra propia versión de [talk](https://en.wikipedia.org/wiki/Talk_%28software%29).

## Tareas

 1. Acepta la [tarea asignada de GitHub Classroom](https://classroom.github.com/assignment-invitations/9b25d9fc368e2d527099736248cff737). Así obtendrás tu propio repositorio como una copia de este. A este repositorio lo llamaremos `upstream`.
 2. Haz un [fork](https://guides.github.com/activities/forking/) de `upstream`. A nuevo repositorio lo llamaremos `origin`.
 3. [Clona](http://gitref.org/creating/#clone) `origin` en tu ordenador.
 4. Trabaja en tu copia local para desarrollar tu programa de mensajería instantánea, siguiendo los pasos indicados en [el guión](https://docs.google.com/document/d/1y9fXi8-JhAhrwSKZ7_LBuUF0eOW5ehjPE79yLxQM0zQ).
 5. Modifica README.md usando [Markdown](https://guides.github.com/features/mastering-markdown/) para:
   1. Explicar cómo compilar y probar la aplicación, incluyendo los requisitos requeridos para hacerlo.
   3. Comentar las características implementadas.
 5. [Sube](http://gitref.org/remotes/#push) los cambios al repositorio `origin` en GitHub.
 6. Crea un [pull request](https://help.github.com/articles/creating-a-pull-request) de `origin` a `upstream` para entregar la práctica.

## Consideraciones adicionales sobre el desarrollo 
 1. Usa [Git Branching](https://docs.google.com/presentation/d/1EXEiEz1d__aHQvAYcPFOiwzlRMunIsDUlJ4tMMYaFig/edit?usp=sharing) para organizar el desarrollo.
 2. Usa el sistema de [issues](https://guides.github.com/features/issues/) de GitHub para controlar las tareas a realizar.
 3. Opcionalmente, implementa las comunicaciones utilizando sockets tipo SOCK\_STREAM (TCP/IP) en lugar de SOCK\_DGRAM (UDP/IP). Se valorará con puntos adicionales.
