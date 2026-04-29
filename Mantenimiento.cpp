//
// Created by Luis Quesada on 29/4/2026.
//

#include "Mantenimiento.h"

using namespace std;

void Mantenimiento::aplicar(Equipo* equipo) {
    prepararHerramientas(equipo);
    ejecutarProcedimiento(equipo);
    registrarResultado(equipo);
}

// Pasos con comportamiento por defecto — subclases pueden sobrescribir
void Mantenimiento::prepararHerramientas(Equipo* equipo) {}
void Mantenimiento::registrarResultado(Equipo* equipo)   {}