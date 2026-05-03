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
    void ordenar(vector<Equipo*>& equipos);

private:
    void quickSort(vector<Equipo*>& v, int low, int high); // Aplicado de forma recursiva

    int partition(vector<Equipo*>& v, int low, int high); // Elige pivotes y reordena elementos alrededor de el

    int medianaDeTres(vector<Equipo*>& v, int low, int high); // Selecciona el indice del pivote usando mediana de tres
};

#endif // ALGORITMO_ORDENAMIENTO_H
