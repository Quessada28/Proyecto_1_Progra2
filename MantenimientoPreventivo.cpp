//
// Created by Luis Quesada on 29/4/2026.
//

#include "MantenimientoPreventivo.h"

using namespace std;

string MantenimientoPreventivo::descripcion() const {
    return "Mantenimiento Preventivo";
}

void MantenimientoPreventivo::ejecutarProcedimiento(Equipo* equipo) {
    // Mantenimiento liviano: aplica la mejora estándar del equipo
    equipo->aplicarMejora();
    equipo->setTiempoInactivo(0);
}