//
// Created by Luis Quesada on 29/4/2026.
//

#include "Simulador.h"
#include "AlgoritmoOrdenamiento.h"
#include "MantenimientoPreventivo.h"
#include "MantenimientoCorrectivo.h"

#include <vector>

using namespace std;

Simulador::Simulador(const string& archivoEntrada, const string& carpetaSalida):archivoEntrada(archivoEntrada),
      escritor(carpetaSalida),numEquipos(0),numIncidencias(0)
{
    for (int i = 0; i < MAX_EQUIPOS; i++) equipos[i] = nullptr;
}

Simulador::~Simulador() {
    for (int i = 0; i < numEquipos; i++) {
        delete equipos[i];
        equipos[i] = nullptr;
    }
}

void Simulador::ejecutar() {
    cargarDatos();
    for (int dia = 1; dia <= DIAS_SIMULACION; dia++)
        simularDia(dia);

    double riesgoFinal = calcularRiesgoGlobal();
    escritor.reporteFinal(DIAS_SIMULACION, equipos, numEquipos, riesgoFinal);
}

void Simulador::cargarDatos() {
    LectorArchivos lector;
    lector.leerArchivo(archivoEntrada,equipos,numEquipos,incidencias, numIncidencias,
        MAX_EQUIPOS, MAX_INCIDENCIAS);

    if (numEquipos == 0)
        throw OperacionContradictoria("El archivo no contiene equipos validos");
    }

void Simulador::simularDia(int dia) {
    degradarEquipos();
    actualizarIncidencias(dia);

    for (int i = 0; i < numEquipos; i++)
        equipos[i]->setPrioridad(equipos[i]->calcularPrioridad());

    vector<Equipo*> ordenados(equipos, equipos + numEquipos);
    AlgoritmoOrdenamiento ordenamiento;
    ordenamiento.ordenar(ordenados);
    for (int i = 0; i < numEquipos; i++) equipos[i] = ordenados[i];

    int numAtendidos = (numEquipos < TECNICOS_DIA) ? numEquipos : TECNICOS_DIA;
    Equipo* atendidos[TECNICOS_DIA];
    for (int i = 0; i < numAtendidos; i++) atendidos[i] = equipos[i];

    aplicarMantenimiento(atendidos, numAtendidos);

    int numPendientes = numEquipos - numAtendidos;
    Equipo** pendientes = equipos + numAtendidos;

    double riesgo = calcularRiesgoGlobal();
    escritor.reporteDiario(dia, atendidos, numAtendidos, pendientes, numPendientes, riesgo);
}

void Simulador::degradarEquipos() {
    for (int i = 0; i < numEquipos; i++)
        equipos[i]->degradar(); // polimórfico — cada tipo degrada distinto
}

void Simulador::actualizarIncidencias(int dia) {
    for (int i = 0; i < numEquipos; i++)
        equipos[i]->setIncidenciasActivas(0);

    for (int i = 0; i < numIncidencias; i++) {
        if (incidencias[i].getDia() <= dia) {
            for (int j = 0; j < numEquipos; j++) {
                if (equipos[j]->getId() == incidencias[i].getIdEquipo()) {
                    equipos[j]->setIncidenciasActivas(
                        equipos[j]->getIncidenciasActivas() + 1);
                    break;
                }
            }
        }
    }
}

void Simulador::aplicarMantenimiento(Equipo** seleccionados, int num) {
    for (int i = 0; i < num; i++) {

        // Downcasting seguro para decidir tipo de mantenimiento
        Mantenimiento* m;
        if (seleccionados[i]->getEstado() > 40)
            m = new MantenimientoPreventivo();

        else
            m = new MantenimientoCorrectivo();

        m->aplicar(seleccionados[i]);
        seleccionados[i]->setTiempoInactivo(0);

        delete m;
    }
}

double Simulador::calcularRiesgoGlobal() const {
    if (numEquipos == 0) return 0.0;
    double suma = 0.0;
    for (int i = 0; i < numEquipos; i++)
        suma += equipos[i]->calcularPrioridad();
    return suma / numEquipos;
}