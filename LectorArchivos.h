//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "Equipo.h"
#include "Incidencia.h"
#include "Excepciones.h"

using namespace std;

class LectorArchivos {
private:
    Equipo* parsearEquipo(const string& linea, int numLinea);
    Incidencia parsearIncidencia(const string& linea, int numLinea);
    string extraerValor(const string& segmento, const string& clave, int numLinea);
    string trim(const string& s);

public:
    void leerArchivo(const string& ruta, Equipo** equipos, int& numEquipos,
    Incidencia* incidencias,  int& numIncidencias, int maxEquipos = 200, int maxIncidencias = 400);

};
