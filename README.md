# Multiplicación de Matrices con OpenMP

## Descripción
Este programa implementa la multiplicación de matrices cuadradas utilizando el algoritmo clásico y paralelización con OpenMP. Permite al usuario especificar el tamaño de las matrices y el número de hilos a utilizar para la ejecución paralela.

## Características
- Multiplicación de matrices de dimensión NxN parametrizable
- Paralelización mediante OpenMP con número de hilos configurable
- Medición precisa del tiempo de ejecución en microsegundos
- Visualización de matrices para dimensiones pequeñas (menores a 9x9)
- Manejo eficiente de memoria mediante asignación dinámica

## Requisitos
- Compilador C con soporte para OpenMP (como GCC)
- Biblioteca OpenMP

## Compilación
```bash
gcc -fopenmp multiMatrizOpenMP.c -o multiMatrizOpenMP
```

## Uso
```bash
./multiMatrizOpenMP tamañoMatriz numHilos
```

### Parámetros
- `tamañoMatriz`: Dimensión N para matrices cuadradas de NxN
- `numHilos`: Número de hilos para la paralelización con OpenMP

## Ejemplo
```bash
./multiMatrizOpenMP 1000 4
```
Este comando multiplica dos matrices de 1000x1000 utilizando 4 hilos para la paralelización.

## Implementación Técnica
El programa utiliza la directiva `#pragma omp parallel for schedule(static)` para distribuir las iteraciones del bucle exterior entre múltiples hilos. Cada hilo calcula un subconjunto de filas de la matriz resultante, maximizando así el aprovechamiento de múltiples núcleos de procesamiento.

## Autor
Samuel Emperador, Mayo 2025
