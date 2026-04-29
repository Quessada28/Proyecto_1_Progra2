//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Equipo.h"

using namespace std;

class EscritorDeReportes {

private:
    string carpeta;
    string nombreArchivoDia(int dia) const;
    string nivelRiesgo(double riesgo) const;

public:
    EscritorDeReportes(const string& carpetaSalida);

    void reporteDiario(int dia, Equipo** atendidos,  int numAtendidos, Equipo** pendientes, int numPendientes,
    double riesgoGlobal);

    void reporteFinal(int diasSimulados, Equipo** equipos, int numEquipos, double riesgoPromedio);

};
