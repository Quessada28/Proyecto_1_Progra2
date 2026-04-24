#pragma once
#ifndef BUSCADOR_H
#define BUSCADOR_H

#include "Equipo.h"
#include <vector>
#include <string>

/**
 * @brief Implementa búsqueda binaria sobre un vector de equipos ordenado por ID.
 *
 * PRECONDICIÓN: el vector debe estar ordenado lexicográficamente por getId()
 * antes de llamar a busquedaBinaria(). Se recomienda ordenarlo con
 * std::sort solo para este propósito (el AlgoritmoOrdenamiento ordena
 * por prioridad, uso diferente).
 *
 * Complejidad: O(log n) por búsqueda.
 */
class Buscador {
public:
    /**
     * @brief Busca un equipo por su ID en tiempo O(log n).
     * @param equipos Vector previamente ordenado por ID (ascendente).
     * @param id      Identificador a buscar.
     * @return Puntero al equipo encontrado, o nullptr si no existe.
     */
    Equipo* busquedaBinaria(std::vector<Equipo*>& equipos, const std::string& id);

    /**
     * @brief Ordena el vector por ID para preparar la búsqueda binaria.
     *        Usa insertion sort (apropiado para colecciones pequeñas o
     *        casi ordenadas, como listas de IDs string).
     */
    void ordenarPorId(std::vector<Equipo*>& equipos);
};

#endif // BUSCADOR_H
