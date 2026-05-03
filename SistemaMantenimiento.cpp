#include "SistemaMantenimiento.h"
#include "MantenimientoPreventivo.h"
#include "MantenimientoCorrectivo.h"
#include "Excepciones.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdlib>   // rand
#include <ctime>

SistemaMantenimiento::SistemaMantenimiento()
    : diaActual(0),
      equiposAtendidosTotal(0),
      riesgoGlobal(0.0),
      estrategiaPreventiva(new MantenimientoPreventivo()),
      estrategiaCorrectiva(new MantenimientoCorrectivo())
{
    srand(static_cast<unsigned>(time(nullptr))); // No se aplica downcast
}

SistemaMantenimiento::~SistemaMantenimiento() {
    // El sistema es dueno de las estrategias
    delete estrategiaPreventiva;
    delete estrategiaCorrectiva;
    // Los equipos son gestionados externamente (el caller los crea)
}

void SistemaMantenimiento::agregarEquipo(Equipo* equipo) {
    if (!equipo)
        throw OperacionContradictoria("Se intento agregar un equipo nulo.");
    equipos.push_back(equipo);
}

Equipo* SistemaMantenimiento::buscarEquipo(const std::string& id) {
    // Copia del vector ordenada por ID para busqueda binaria
    vector<Equipo*> copia = equipos;
    buscador.ordenarPorId(copia);
    return buscador.busquedaBinaria(copia, id);
}

// Calculo de prioridad (formula oficial)
void SistemaMantenimiento::calcularPrioridades() {
    for (Equipo* e : equipos) {
        double criticidad     = e->getCriticidad();
        double incidencias    = static_cast<double>(e->getCantidadIncidencias());
        double tiempoInactivo = static_cast<double>(e->getTiempoInactivo());

        double prioridad = (criticidad     * 0.5)
                         + (incidencias    * 0.3)
                         + (tiempoInactivo * 0.2);

        e->setPrioridad(prioridad);
    }
}

// Ordenamiento con QuickSort propio
void SistemaMantenimiento::ordenarEquipos() {
    ordenador.ordenar(equipos);   // de mayor a menor prioridad
}

// Seleccion de los 3 equipos mas urgentes
vector<Equipo*> SistemaMantenimiento::seleccionarTop3() {
    vector<Equipo*> top3;
    int limite = (3 < (int)equipos.size()) ? 3 : (int)equipos.size();
    for (int i = 0; i < limite; ++i)
        top3.push_back(equipos[i]);
    return top3;
}

// Ejecucion de mantenimiento polimorfico (patron Strategy)
void SistemaMantenimiento::ejecutarMantenimiento(vector<Equipo*>& seleccionados) {
    for (Equipo* equipo : seleccionados) {
        // Seleccion dinamica de estrategia segun estado del equipo
        Mantenimiento* estrategia = seleccionarEstrategia(equipo);

        // Se invoca aplicar() sin conocer la clase concreta
        estrategia->aplicar(&*equipo);

        // dynamic_cast para comportamiento exclusivo de MantenimientoCorrectivo
        MantenimientoCorrectivo* correctivo =
            dynamic_cast<MantenimientoCorrectivo*>(estrategia);

        if (correctivo != nullptr) {
            // Acceso a comportamiento exclusivo de la clase derivada
            cout << "  -> " << correctivo->descripcion()
                      << " aplicado a " << equipo->getId() << "\n";
        }

        equiposAtendidosTotal++;
    }
}

// Helper: elige estrategia segun estado del equipo
// Equipos con muchas incidencias o estado < 40% -> Correctivo
// El resto -> Preventivo
Mantenimiento* SistemaMantenimiento::seleccionarEstrategia(const Equipo* equipo) const {
    bool necesitaCorrectivo = (equipo->getCantidadIncidencias() > 2)
                           || (equipo->getEstado() < 40.0);
    return necesitaCorrectivo ? estrategiaCorrectiva : estrategiaPreventiva;
}

// Degradacion diaria de todos los equipos
void SistemaMantenimiento::degradarEquipos() {
    for (Equipo* e : equipos) {
        e->degradar();
    }
}

// Generacion de incidencias aleatorias (simula eventos del dia)
void SistemaMantenimiento::generarIncidenciasAleatorias() {
    // Cada dia, ~20% de los equipos puede recibir una incidencia nueva
    for (Equipo* e : equipos) {
        int roll = std::rand() % 100;
        if (roll < 20) {
            int sevRoll = std::rand() % 3;
            string sev = (sevRoll == 0) ? "ALTA"
                       : (sevRoll == 1) ? "MEDIA"
                       :                  "BAJA";

            string incId = "INC-D" + std::to_string(diaActual)
                         + "-" + e->getId();
            Incidencia* inc = new Incidencia(incId, sev, diaActual);
            e->agregarIncidencia(inc);
        }
    }
}

// Calculo del riesgo global del laboratorio
// Promedio ponderado de (criticidad * (1 - estado/100))
double SistemaMantenimiento::calcularRiesgoGlobal() const {
    if (equipos.empty()) return 0.0;
    double sumaRiesgo = 0.0;
    for (const Equipo* e : equipos) {
        sumaRiesgo += e->getCriticidad() * (1.0 - e->getEstado() / 100.0);
    }
    return sumaRiesgo / equipos.size();
}

int SistemaMantenimiento::getBacklogPendiente() const {
    int pendiente = 0;
    for (const Equipo* e : equipos)
        if (e->getCantidadIncidencias() > 0) pendiente++;
    return pendiente;
}

// Flujo completo de un dia de simulacion
void SistemaMantenimiento::ejecutarDia() {
    diaActual++;

    cout << "\n╔══════════════════════════════════════╗\n";
    cout <<   "║          DIA " << setw(3) << diaActual
              <<                      " DE SIMULACION          ║\n";
    cout <<   "╚══════════════════════════════════════╝\n";

    // Degradacion diaria (RF3)
    degradarEquipos();

    // Aparicion de nuevas incidencias
    generarIncidenciasAleatorias();

    // Calculo de prioridades (RF4)
    calcularPrioridades();

    // Reordenamiento (RF5)
    ordenarEquipos();

    // Seleccion de los 3 mas urgentes (RF6)
    vector<Equipo*> top3 = seleccionarTop3();

    // Ejecucion de mantenimiento (RF7)
    ejecutarMantenimiento(top3);

    // Actualizacion de riesgo global (RF8)
    riesgoGlobal = calcularRiesgoGlobal();

    // Reporte del dia (RF9)
    string reporte = generarReporteDia(top3);
    cout << reporte;
}

// Reporte del dia
string SistemaMantenimiento::generarReporteDia(
    const vector<Equipo*>& atendidos) const
{
    ostringstream oss;

    oss << "\n--- Reporte Dia " << diaActual << " ---\n";

    // Top prioridad
    oss << "Top prioridad: ";
    for (size_t i = 0; i < atendidos.size(); ++i) {
        oss << atendidos[i]->getId()
            << " (" << fixed << setprecision(1)
            << atendidos[i]->getPrioridad() << ")";
        if (i + 1 < atendidos.size()) oss << ", ";
    }
    oss << "\n";

    // Equipos asignados
    oss << "Asignados: ";
    for (size_t i = 0; i < atendidos.size(); ++i) {
        oss << atendidos[i]->getId();
        if (i + 1 < atendidos.size()) oss << ", ";
    }
    oss << "\n";

    // Backlog y riesgo
    oss << "Backlog pendiente: " << getBacklogPendiente() << "\n";

    string nivelRiesgo;
    if      (riesgoGlobal >= 7.0) nivelRiesgo = "CRITICO";
    else if (riesgoGlobal >= 5.0) nivelRiesgo = "ALTO";
    else if (riesgoGlobal >= 3.0) nivelRiesgo = "MEDIO";
    else                          nivelRiesgo = "BAJO";

    oss << "Riesgo global: " << nivelRiesgo
        << " (" << fixed << setprecision(2) << riesgoGlobal << ")\n";

    // Equipos pendientes (los que NO fueron atendidos en la iteracion actual)
    oss << "Equipos pendientes (muestra top 5):\n";
    int mostrados = 0;
    for (const Equipo* e : equipos) {
        bool estaEnTop = false;
        for (const Equipo* a : atendidos)
            if (a->getId() == e->getId()) { estaEnTop = true; break; }

        if (!estaEnTop && mostrados < 5) {
            oss << "  " << e->getId()
                << " prio=" << fixed << setprecision(1) << e->getPrioridad()
                << " inc=" << e->getCantidadIncidencias() << "\n";
            mostrados++;
        }
    }
    oss << "-------------------------------\n";
    return oss.str();
}

// Reporte acumulado de la simulacion completa
string SistemaMantenimiento::generarReporteResumen() const {
    ostringstream oss;
    oss << "\n╔══════════════════════════════════════╗\n";
    oss << "║        RESUMEN FINAL SIMULACION      ║\n";
    oss << "╚══════════════════════════════════════╝\n";
    oss << "Dias simulados     : " << diaActual << "\n";
    oss << "Total equipos      : " << equipos.size() << "\n";
    oss << "Atenciones totales : " << equiposAtendidosTotal << "\n";
    oss << "Backlog al final   : " << getBacklogPendiente() << "\n";
    oss << "Riesgo final       : " << fixed << setprecision(2)
        << riesgoGlobal << "\n";

    oss << "\nEstado final de equipos (top 10 prioridad):\n";
    int n = (10 < (int)equipos.size()) ? 10 : (int)equipos.size();
    for (int i = 0; i < n; ++i)
        oss << "  " << equipos[i]->toString() << "\n";

    return oss.str();
}

void SistemaMantenimiento::imprimirEstadoActual() const {
    cout << "\n=== Estado actual - Dia " << diaActual << " ===\n";
    for (const Equipo* e : equipos)
        cout << "  " << e->toString() << "\n";
}