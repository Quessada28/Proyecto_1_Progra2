//
// Created by Luis Quesada on 29/4/2026.
//

#include "Incidencia.h"
#include "Equipo.h" // Aquí se necesita la definición completa de Equipo

using namespace std;

Incidencia::Incidencia() : dia(0), equipo(nullptr) {}

Incidencia::Incidencia(const string& idEquipo, const string& severidad, int dia):
idEquipo(idEquipo), severidad(severidad), dia(dia), equipo(nullptr) {}

string Incidencia::getIdEquipo() const { return idEquipo; }
string Incidencia::getSeveridad() const { return severidad; }
int Incidencia::getDia() const { return dia; }
Equipo* Incidencia::getEquipo() const { return equipo; }

void Incidencia::setEquipo(Equipo* e) { equipo = e; }

int Incidencia::getPeso() const {
    if (severidad == "ALTA")  return 3;
    if (severidad == "MEDIA") return 2;
    return 1;
}