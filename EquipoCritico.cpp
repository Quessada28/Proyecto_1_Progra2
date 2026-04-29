//
// Created by Luis Quesada on 29/4/2026.
//

#include "EquipoCritico.h"

using namespace std;

EquipoCritico::EquipoCritico(const string& id, int criticidad, int estado):Equipo(id, criticidad, estado) {}

void EquipoCritico::degradar() { // Se degrada más rápido por su nivel de uso
    estado -= 3;
    if (estado < 0) estado = 0;
    tiempoInactivo++;
}

void EquipoCritico::aplicarMejora() {
    estado += 20;
    if (estado > 100) estado = 100;
    incidenciasActivas = 0;
}

string EquipoCritico::tipo() const { return "Critico"; }