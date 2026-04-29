//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include <string>
#include "Incidencia.h"

using namespace std;

class Equipo {
private:
    static const int MAX_INC_EQUIPO = 50;
    Incidencia* incidencias[MAX_INC_EQUIPO];
    int numIncidencias;
    double prioridad;

protected:
    // Accesibles desde clases derivadas para degradar() y aplicarMejora()
    string id;
    int criticidad;
    int estado;
    int incidenciasActivas;
    int tiempoInactivo;

public:
    Equipo(const string& id, int criticidad, int estado);
    virtual ~Equipo();

    string getId() const;
    int getCriticidad() const;
    int getEstado() const;
    int getIncidenciasActivas() const;
    int getCantidadIncidencias() const { return getIncidenciasActivas(); }
    int getTiempoInactivo() const;
    double getPrioridad() const;

    void setEstado(int e);
    void setIncidenciasActivas(int n);
    void setTiempoInactivo(int t);
    void setPrioridad(double prioridad);

    void   agregarIncidencia(Incidencia* inc);
    double calcularPrioridad() const; // fórmula fija del enunciado
    string toString() const;

    // Métodos polimórficos — cada tipo de equipo los implementa
    virtual void degradar() = 0;
    virtual void aplicarMejora() = 0;
    virtual string tipo() const = 0;
};