//
// Created by Luis Quesada on 29/4/2026.
//

#include "Equipo.h"
#include <sstream>

using namespace std;

Equipo::Equipo(const string& id, int criticidad, int estado): id(id), criticidad(criticidad), estado(estado),
incidenciasActivas(0), tiempoInactivo(0), numIncidencias(0), prioridad(0.0)
    {
    for (int i = 0; i < MAX_INC_EQUIPO; i++) incidencias[i] = nullptr;
    }

Equipo::~Equipo() {
    for (int i = 0; i < numIncidencias; ++i) {
        delete incidencias[i];
        incidencias[i] = nullptr;
    }
}

string Equipo::getId() const { return id; }
int Equipo::getCriticidad() const { return criticidad; }
int Equipo::getEstado() const { return estado; }
int Equipo::getIncidenciasActivas() const { return incidenciasActivas; }
int Equipo::getTiempoInactivo() const { return tiempoInactivo; }
double Equipo::getPrioridad() const { return prioridad; }

void Equipo::setEstado(int e) { estado = (e < 0) ? 0 : (e > 100 ? 100 : e); }
void Equipo::setIncidenciasActivas(int n) { incidenciasActivas = n; }
void Equipo::setTiempoInactivo(int t) { tiempoInactivo = t; }
void Equipo::setPrioridad(double p) { prioridad = p; }

void Equipo::agregarIncidencia(Incidencia* inc) {
    if (!inc) return;
    if (numIncidencias < MAX_INC_EQUIPO) {
        inc->setEquipo(this);
        incidencias[numIncidencias++] = inc;
        incidenciasActivas++;
    }
    else {
        delete inc;
    }
}

double Equipo::calcularPrioridad() const {
    return (criticidad * 0.5) + (incidenciasActivas * 0.3) + (tiempoInactivo * 0.2);
}

string Equipo::toString() const {
    ostringstream oss;
    oss << id
        << " [" << tipo() << "]"
        << " prio=" << calcularPrioridad()
        << " criticidad=" << criticidad
        << " estado=" << estado
        << " incidencias=" << incidenciasActivas
        << " inactivo=" << tiempoInactivo;
    return oss.str();
}