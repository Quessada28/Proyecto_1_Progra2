#pragma once
#ifndef SISTEMA_MANTENIMIENTO_H
#define SISTEMA_MANTENIMIENTO_H

#include "Equipo.h"
#include "Mantenimiento.h"
#include "AlgoritmoOrdenamiento.h"
#include "Buscador.h"
#include <vector>
#include <string>

class SistemaMantenimiento {
private:
    vector<Equipo*> equipos;          // colección polimórfica (upcasting)
    AlgoritmoOrdenamiento ordenador;
    Buscador buscador;
    int diaActual;
    int equiposAtendidosTotal;
    double riesgoGlobal;

    // Estrategias de mantenimiento (patrón Strategy)
    Mantenimiento* estrategiaPreventiva;
    Mantenimiento* estrategiaCorrectiva;

    // Auxiliares privados
    Mantenimiento* seleccionarEstrategia(const Equipo* equipo) const;
    double calcularRiesgoGlobal() const;
    void degradarEquipos();
    void generarIncidenciasAleatorias();

public:
    SistemaMantenimiento();
    ~SistemaMantenimiento();

    // Equipos
    void agregarEquipo(Equipo* equipo);
    Equipo* buscarEquipo(const string& id);

    void calcularPrioridades(); // Calcular prioridad para cada equipo

    void ordenarEquipos(); // Ordenamiento mediante Quicksort

    vector<Equipo*> seleccionarTop3(); // Devuelve 3 equipos con mayor prioridad

    void ejecutarMantenimiento(vector<Equipo*>& seleccionados); // Aplicar mantenimiento a los equipos seleccionados por el sistema

    void ejecutarDia(); // Ejecuta dia completo de la simulacion

    // Reportes
    string generarReporteDia(const vector<Equipo*>& atendidos) const;
    string generarReporteResumen() const;
    void imprimirEstadoActual() const;

    // Getters de estado
    int getDiaActual()            const { return diaActual; }
    double getRiesgoGlobal()      const { return riesgoGlobal; }
    int getEquiposAtendidosTotal() const { return equiposAtendidosTotal; }
    int getBacklogPendiente()     const;
    const vector<Equipo*>& getEquipos() const { return equipos; }
};

#endif // SISTEMA_MANTENIMIENTO_H
