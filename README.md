# Virtual File System Simulator

Este proyecto consiste en un simulador de sistema de archivos jerárquico desarrollado en C++ para entornos Linux (Ubuntu). La aplicación permite gestionar una estructura de directorios y archivos de forma virtual dentro de la memoria RAM.

## Características Técnicas

La implementación se centra en el uso de estándares modernos de C++ para garantizar la eficiencia y la seguridad de la memoria:

- Gestión de memoria mediante Smart Pointers: Uso de std::shared_ptr para la propiedad de nodos y std::weak_ptr para las referencias a directorios padres, evitando ciclos de referencia y memory leaks.
- Estructura de Datos: Organización jerárquica basada en un árbol de nodos utilizando std::map para la gestión de hijos, lo que optimiza la búsqueda de archivos por nombre.
- Navegación Recursiva: Implementación de algoritmos recursivos para la generación de rutas completas (pathfinding).

## Funcionalidades Implementadas

El simulador incluye comandos básicos de navegación y manipulación de estructura:

- mkdir: Creación de nuevos directorios.
- touch: Generación de archivos vacíos.
- ls: Listado de elementos dentro del directorio actual.
- cd: Cambio de contexto entre directorios (soporta navegación hacia el padre mediante "..").
- pwd: Visualización de la ruta absoluta actual.

## Requisitos y Compilación

Para compilar este proyecto es necesario contar con un compilador que soporte C++17 o superior y la herramienta CMake.

Instalación de dependencias en Ubuntu:
sudo apt install build-essential cmake

Instrucciones de compilación:
1. mkdir cmake-build-debug
2. cd cmake-build-debug
3. cmake ..
4. make
5. ./virtualFiles