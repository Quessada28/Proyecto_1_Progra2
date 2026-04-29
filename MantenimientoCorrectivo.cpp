//
// Created by Luis Quesada on 29/4/2026.
//

#include "MantenimientoCorrectivo.h"

using namespace std;

string MantenimientoCorrectivo::descripcion() const {
    return "Mantenimiento Correctivo";
}

void MantenimientoCorrectivo::ejecutarProcedimiento(Equipo* equipo) {
    // Reparación de emergencia: doble mejora para recuperar el equipo
    equipo->aplicarMejora();
    equipo->aplicarMejora();
    equipo->setTiempoInactivo(0);
    equipo->setIncidenciasActivas(0);
}

void MantenimientoCorrectivo::registrarResultado(Equipo* equipo) {
    // Intervención mayor — en un sistema real aquí iría al log
}