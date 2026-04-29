//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include <iostream>
#include <string>

#include "Equipo.h"
#include "Incidencia.h"
#include "LectorArchivos.h"
#include "EscritorDeReportes.h"
#include "Excepciones.h"

using namespace std;

class Simulador {

private:
    static const int MAX_EQUIPOS     = 200;
    static const int MAX_INCIDENCIAS = 400;
    static const int DIAS_SIMULACION = 30;
    static const int TECNICOS_DIA    = 3;

    Equipo* equipos[MAX_EQUIPOS];
    Incidencia incidencias[MAX_INCIDENCIAS];
    int numEquipos;
    int numIncidencias;

    string archivoEntrada;
    EscritorDeReportes escritor;

    void cargarDatos();
    void simularDia(int dia);
    void degradarEquipos();
    void actualizarIncidencias(int dia);
    void aplicarMantenimiento(Equipo** seleccionados, int num);
    double calcularRiesgoGlobal() const;


public:
    Simulador(const string& archivoEntrada, const string& carpetaSalida);
    ~Simulador();
    void ejecutar();

};
