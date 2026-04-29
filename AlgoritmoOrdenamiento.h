#pragma once
#ifndef ALGORITMO_ORDENAMIENTO_H
#define ALGORITMO_ORDENAMIENTO_H

#include "Equipo.h"
#include <vector>

/**
 * @brief Implementa QuickSort manual sobre vector<Equipo*>.
 *
 * Ordena de MAYOR a MENOR prioridad.
 * NO usa std::sort ni ninguna función de ordenamiento de la STL.
 *
 * Justificación del algoritmo:
 *   QuickSort tiene complejidad promedio O(n log n) y O(n²) en el peor
 *   caso (mitigado con pivote mediana-de-tres). Es in-place, lo que
 *   resulta eficiente para vectores de punteros. Apropiado para los
 *   100 equipos del conjunto de prueba y para re-ordenamientos diarios.
 */
class AlgoritmoOrdenamiento {
public:
    /**
     * @brief Punto de entrada público. Ordena el vector completo.
     * @param equipos Vector de punteros a Equipo (se modifica in-place).
     */
    void ordenar(std::vector<Equipo*>& equipos);

private:
    /**
     * @brief Implementación recursiva de QuickSort.
     * @param v     Vector a ordenar.
     * @param low   Índice inferior del subarreglo actual.
     * @param high  Índice superior del subarreglo actual.
     */
    void quickSort(std::vector<Equipo*>& v, int low, int high);

    /**
     * @brief Elige pivote y reordena elementos alrededor de él.
     *        Usa estrategia "mediana de tres" para evitar O(n²) en datos
     *        casi ordenados (situación frecuente en simulaciones diarias).
     * @return Índice final del pivote.
     */
    int partition(std::vector<Equipo*>& v, int low, int high);

    /**
     * @brief Selecciona el índice del pivote usando mediana de tres.
     */
    int medianaDeTres(std::vector<Equipo*>& v, int low, int high);
};

#endif // ALGORITMO_ORDENAMIENTO_H
