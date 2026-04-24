#include "Buscador.h"

// ────────────────────────────────────────────────────────────
// Búsqueda binaria sobre vector ordenado por ID
// ────────────────────────────────────────────────────────────
Equipo* Buscador::busquedaBinaria(std::vector<Equipo*>& equipos, const std::string& id) {
    int low  = 0;
    int high = static_cast<int>(equipos.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        const std::string& midId = equipos[mid]->getId();

        if (midId == id) {
            return equipos[mid];          // encontrado
        } else if (midId < id) {
            low = mid + 1;                // buscar en mitad derecha
        } else {
            high = mid - 1;               // buscar en mitad izquierda
        }
    }

    return nullptr;  // no encontrado
}

// ────────────────────────────────────────────────────────────
// Insertion sort por ID (lexicográfico, ascendente)
// Eficiente para listas casi ordenadas de IDs string
// ────────────────────────────────────────────────────────────
void Buscador::ordenarPorId(std::vector<Equipo*>& equipos) {
    int n = static_cast<int>(equipos.size());
    for (int i = 1; i < n; ++i) {
        Equipo* clave = equipos[i];
        int j = i - 1;
        while (j >= 0 && equipos[j]->getId() > clave->getId()) {
            equipos[j + 1] = equipos[j];
            --j;
        }
        equipos[j + 1] = clave;
    }
}
