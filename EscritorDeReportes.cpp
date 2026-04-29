//
// Created by Luis Quesada on 29/4/2026.
//

#include "EscritorDeReportes.h"

using namespace std;

EscritorDeReportes::EscritorDeReportes(const string& carpetaSalida):carpeta(carpetaSalida) {}

void EscritorDeReportes::reporteDiario(int dia,Equipo** atendidos,  int numAtendidos,
    Equipo** pendientes, int numPendientes,double riesgoGlobal) {

    string  ruta = carpeta + "/" + nombreArchivoDia(dia);
    ofstream f(ruta);
    if (!f.is_open()) return;

    f << "=== DIA " << dia << " ===\n\n";
    f << "Equipos atendidos (" << numAtendidos << "):\n";
    for (int i = 0; i < numAtendidos; i++) {
        f << "  " << atendidos[i]->getId()
          << " [" << atendidos[i]->tipo() << "]"
          << " | prioridad: " << atendidos[i]->calcularPrioridad()
          << " | estado: "    << atendidos[i]->getEstado() << "\n";
    }

    f << "\nEquipos pendientes (" << numPendientes << "):\n";
    for (int i = 0; i < numPendientes; i++) {
        f << "  " << pendientes[i]->getId()
          << " [" << pendientes[i]->tipo() << "]"
          << " | prioridad: " << pendientes[i]->calcularPrioridad()
          << " | estado: "    << pendientes[i]->getEstado() << "\n";
    }

    f << "\nRiesgo global: " << nivelRiesgo(riesgoGlobal)
      << " (" << riesgoGlobal << ")\n";

    f.close();
}

void EscritorDeReportes::reporteFinal(int diasSimulados,Equipo** equipos, int numEquipos, double riesgoPromedio){

    string ruta = carpeta + "/reporte_final.txt";
    ofstream f(ruta);
    if (!f.is_open()) return;

    f << "=== REPORTE FINAL DE SIMULACION ===\n\n";
    f << "Dias simulados: " << diasSimulados << "\n";
    f << "Total de equipos: " << numEquipos    << "\n";
    f << "Riesgo promedio final: " << nivelRiesgo(riesgoPromedio)
      << " (" << riesgoPromedio << ")\n\n";

    f << "Estado final de equipos:\n";
    for (int i = 0; i < numEquipos; i++) {
        f << "  " << equipos[i]->getId()
          << " [" << equipos[i]->tipo() << "]"
          << " | criticidad: " << equipos[i]->getCriticidad()
          << " | estado: " << equipos[i]->getEstado()
          << " | incidencias: " << equipos[i]->getIncidenciasActivas() << "\n";
    }

    f.close();
}

string EscritorDeReportes::nombreArchivoDia(int dia) const {
    string s = to_string(dia);
    if (dia < 10) s = "0" + s;
    return "dia_" + s + ".txt";
}

string EscritorDeReportes::nivelRiesgo(double riesgo) const {
    if (riesgo >= 7.0) return "ALTO";
    if (riesgo >= 4.0) return "MEDIO";
    return "BAJO";
}