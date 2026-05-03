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
    Equipo* busquedaBinaria(vector<Equipo*>& equipos, const string& id); //Busca un equipo por su ID

    void ordenarPorId(vector<Equipo*>& equipos); //Ordena el vector por ID para preparar la busqueda binaria
};

#endif // BUSCADOR_H
