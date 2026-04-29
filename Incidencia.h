//
// Created by Luis Quesada on 29/4/2026.
//


#pragma once
#include <iostream>
#include <string>

using namespace std;

class Equipo;
// Forward declaration — Equipo incluye Incidencia.h
// e Incidencia referencia a Equipo: dependencia mutua resuelta así

class Incidencia {
private:
    string idEquipo;
    string severidad;
    int dia;
    Equipo* equipo; // puntero, con forward declaration es suficiente

public:
    Incidencia();
    Incidencia(const string& idEquipo, const string& severidad, int dia);

    string getIdEquipo() const;
    string getSeveridad() const;
    int getDia() const;
    Equipo* getEquipo() const;
    int getPeso() const; // ALTA=3, MEDIA=2, BAJA=1

    void setEquipo(Equipo* e);
};