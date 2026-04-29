//
// Created by Luis Quesada on 29/4/2026.
//

#include "EquipoEstandar.h"

using namespace std;

EquipoEstandar::EquipoEstandar(const string& id, int criticidad, int estado):Equipo(id, criticidad, estado) {}

void EquipoEstandar::degradar() {
    estado -= 2;
    if (estado < 0) estado = 0;
    tiempoInactivo++;
}

void EquipoEstandar::aplicarMejora() {
    estado += 15;
    if (estado > 100) estado = 100;
    incidenciasActivas = 0;
}

string EquipoEstandar::tipo() const { return "Estandar"; }