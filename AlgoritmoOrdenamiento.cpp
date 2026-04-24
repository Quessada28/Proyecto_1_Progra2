#include "AlgoritmoOrdenamiento.h"
#include <algorithm>   // solo para std::swap — NO para ordenar

// ────────────────────────────────────────────────────────────
// Punto de entrada público
// ────────────────────────────────────────────────────────────
void AlgoritmoOrdenamiento::ordenar(std::vector<Equipo*>& equipos) {
    if (equipos.size() <= 1) return;
    quickSort(equipos, 0, static_cast<int>(equipos.size()) - 1);
}

// ────────────────────────────────────────────────────────────
// QuickSort recursivo
// ────────────────────────────────────────────────────────────
void AlgoritmoOrdenamiento::quickSort(std::vector<Equipo*>& v, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(v, low, high);
        quickSort(v, low, pivotIndex - 1);
        quickSort(v, pivotIndex + 1, high);
    }
}

// ────────────────────────────────────────────────────────────
// Partición con mediana de tres
// Ordena de MAYOR a MENOR prioridad (criterio inverso al clásico)
// ────────────────────────────────────────────────────────────
int AlgoritmoOrdenamiento::partition(std::vector<Equipo*>& v, int low, int high) {
    // Coloca el pivote en la posición high
    int pivotIdx = medianaDeTres(v, low, high);
    std::swap(v[pivotIdx], v[high]);

    double pivotPriority = v[high]->getPrioridad();
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        // Orden DESCENDENTE: elementos con prioridad MAYOR van primero
        if (v[j]->getPrioridad() >= pivotPriority) {
            ++i;
            std::swap(v[i], v[j]);
        }
    }

    std::swap(v[i + 1], v[high]);
    return i + 1;
}

// ────────────────────────────────────────────────────────────
// Mediana de tres: compara low, mid, high y retorna el índice
// del valor intermedio para usarlo como pivote
// ────────────────────────────────────────────────────────────
int AlgoritmoOrdenamiento::medianaDeTres(std::vector<Equipo*>& v, int low, int high) {
    int mid = low + (high - low) / 2;

    double pLow  = v[low]->getPrioridad();
    double pMid  = v[mid]->getPrioridad();
    double pHigh = v[high]->getPrioridad();

    // Retorna el índice del valor mediano entre los tres
    if ((pLow <= pMid && pMid <= pHigh) || (pHigh <= pMid && pMid <= pLow))
        return mid;
    if ((pMid <= pLow && pLow <= pHigh) || (pHigh <= pLow && pLow <= pMid))
        return low;
    return high;
}
