https://github.com/Dalvelac/Tinylisp-2/

# Enunciado del ejercicio

## Descripción General

Se puede ver el codigo original en este repositorio: 
https://github.com/Dalvelac/Caso-final-integrador-Daniel-Alves

Este documento describe la implementación de la funcionalidad para cargar y evaluar scripts en el lenguaje TinyLisp. La funcionalidad permite cargar un archivo de texto, leer las instrucciones del script y evaluar las expresiones usando la lógica de evaluación ya presente en el código. Se han implementado dos funciones principales para la carga de scripts:
```
void load_script(const char* filename, bool show_script = false)
void load_script()
```
Ambas funciones permiten abrir y leer archivos de texto que contienen scripts de TinyLisp, con un manejo adecuado de errores para asegurar la robustez del código.

## Funciones Implementadas

1. void load_script(const char* filename, bool show_script = false)

Esta función toma como parámetro el nombre del archivo que se desea cargar. Si el parámetro show_script es verdadero, muestra el contenido del archivo en la consola. A continuación, se tokeniza cada línea del script y se evalúa usando la función evaluate().

Manejo de Errores: Si el archivo no se puede abrir, se muestra un mensaje de error. Además, se verifica que no ocurran errores durante la lectura del archivo y se manejan excepciones generadas por la función evaluate().

2. void load_script()

Esta función solicita al usuario el nombre del archivo que desea cargar. Luego, llama a la función load_script(const char* filename, bool show_script) para procesar el archivo. Si el usuario no proporciona un nombre de archivo, se muestra un mensaje de error.

Modificaciones en main()

En el bucle principal del programa (main()), se agregó una nueva funcionalidad para permitir al usuario cargar un script escribiendo el comando load <nombre_del_archivo>. El script especificado se carga y evalúa usando las funciones antes mencionadas. Además, se mantiene la funcionalidad original de evaluar expresiones directamente desde la entrada del usuario.

Ejemplo: El usuario puede escribir load script.txt para cargar y evaluar el script contenido en script.txt.

## Manejo de Errores

Para asegurar que el código sea robusto, se han agregado verificaciones para manejar distintos errores posibles:

Archivo no encontrado: Si el archivo no existe o no se puede abrir, se muestra un mensaje de error indicando que no se pudo abrir el archivo.

Errores durante la lectura: Si ocurre un error durante la lectura del archivo, se muestra un mensaje indicando que hubo un problema al leer el archivo.

Errores en la evaluación: Cualquier excepción generada durante la evaluación de una línea del script se captura y se muestra un mensaje de error.

## Ejemplo de Uso

Cargar un script desde la consola:

Ingrese el comando: load nombre_del_archivo.txt.

El contenido del archivo será evaluado línea por línea y se mostrará cualquier resultado o error en la consola.

Ingresar expresiones manualmente:

Puede ingresar expresiones directamente en la consola y ver los resultados inmediatamente.

Para salir del programa, ingrese Salir.

## Consideraciones Adicionales

El código utiliza la función tokenize() para convertir cada línea del script en tokens antes de evaluarlos.

La evaluación de las expresiones se realiza usando evaluate(), que permite realizar operaciones matemáticas y manejar variables y funciones definidas por el usuario.

# Corrección de compañera

https://github.com/victoriavillapad06/3_Caso_Integrador
Compañera: Vicky Villalobos

**Puntuación: 9/10**

Puntos positivos:

## Estructura clara y modularidad:
Usas funciones separadas para diferentes tareas (load_script con y sin argumentos), lo cual mejora la legibilidad y mantenibilidad.
        
## Uso de estándares modernos:
Uso de std::string y istreambuf_iterator para leer el archivo de manera eficiente.
        
## Gestión básica de errores:
Detectas si el archivo no se abre o si el nombre del archivo está vacío.
Capturas excepciones en el main para evitar fallos inesperados.
        
## Estética de consola:
Introduces colores y un cuadro de texto simulado para mejorar la experiencia del usuario.
        
## Extensibilidad:
La funcionalidad es fácilmente adaptable para integrar con un intérprete de Tiny Lisp.



