#include "SistemaMantenimiento.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>   // rand
#include <ctime>

// ────────────────────────────────────────────────────────────
// Constructor / Destructor
// ────────────────────────────────────────────────────────────
SistemaMantenimiento::SistemaMantenimiento()
    : diaActual(0),
      equiposAtendidosTotal(0),
      riesgoGlobal(0.0),
      estrategiaPreventiva(new MantenimientoPreventivo()),
      estrategiaCorrectiva(new MantenimientoCorrectivo())
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

SistemaMantenimiento::~SistemaMantenimiento() {
    // El sistema es dueño de las estrategias
    delete estrategiaPreventiva;
    delete estrategiaCorrectiva;
    // Los equipos son gestionados externamente (el caller los crea)
}

// ────────────────────────────────────────────────────────────
// Gestión de equipos
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::agregarEquipo(Equipo* equipo) {
    if (!equipo)
        throw OperacionInconsistente("Se intentó agregar un equipo nulo.");
    equipos.push_back(equipo);
}

Equipo* SistemaMantenimiento::buscarEquipo(const std::string& id) {
    // Copia del vector ordenada por ID para búsqueda binaria
    std::vector<Equipo*> copia = equipos;
    buscador.ordenarPorId(copia);
    return buscador.busquedaBinaria(copia, id);
}

// ────────────────────────────────────────────────────────────
// RF4 — Cálculo de prioridad (fórmula oficial)
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::calcularPrioridades() {
    for (Equipo* e : equipos) {
        double criticidad       = e->getCriticidad();
        double incidencias      = static_cast<double>(e->getCantidadIncidencias());
        double tiempoInactivo   = static_cast<double>(e->getTiempoInactivo());

        // ✅ Fórmula OBLIGATORIA del proyecto (RF4)
        double prioridad = (criticidad    * 0.5)
                         + (incidencias   * 0.3)
                         + (tiempoInactivo * 0.2);

        e->setPrioridad(prioridad);
    }
}

// ────────────────────────────────────────────────────────────
// RF5 — Ordenamiento con QuickSort propio (sin std::sort)
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::ordenarEquipos() {
    ordenador.ordenar(equipos);   // de mayor a menor prioridad
}

// ────────────────────────────────────────────────────────────
// RF6 — Selección de los 3 equipos más urgentes
// ────────────────────────────────────────────────────────────
std::vector<Equipo*> SistemaMantenimiento::seleccionarTop3() {
    std::vector<Equipo*> top3;
    int limite = std::min(3, static_cast<int>(equipos.size()));
    for (int i = 0; i < limite; ++i)
        top3.push_back(equipos[i]);
    return top3;
}

// ────────────────────────────────────────────────────────────
// RF7 — Ejecución de mantenimiento polimórfico (patrón Strategy)
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::ejecutarMantenimiento(std::vector<Equipo*>& seleccionados) {
    for (Equipo* equipo : seleccionados) {
        // Selección dinámica de estrategia según estado del equipo
        Mantenimiento* estrategia = seleccionarEstrategia(equipo);

        // POLIMORFISMO REAL: se invoca aplicar() sin saber la clase concreta
        estrategia->aplicar(*equipo);

        // DOWNCASTING SEGURO (dynamic_cast) para comportamiento exclusivo
        // de MantenimientoCorrectivo (sección 8.2 del enunciado)
        MantenimientoCorrectivo* correctivo =
            dynamic_cast<MantenimientoCorrectivo*>(estrategia);

        if (correctivo != nullptr) {
            // Acceso a método exclusivo de la clase derivada
            std::string informe = correctivo->generarInformeReparacion(*equipo);
            std::cout << informe << "\n";
        }

        equiposAtendidosTotal++;
    }
}

// ────────────────────────────────────────────────────────────
// Helper: elige estrategia según estado del equipo
// Equipos con muchas incidencias o estado < 40% → Correctivo
// El resto → Preventivo
// ────────────────────────────────────────────────────────────
Mantenimiento* SistemaMantenimiento::seleccionarEstrategia(const Equipo* equipo) const {
    bool necesitaCorrectivo = (equipo->getCantidadIncidencias() > 2)
                           || (equipo->getEstado() < 40.0);
    return necesitaCorrectivo ? estrategiaCorrectiva : estrategiaPreventiva;
}

// ────────────────────────────────────────────────────────────
// RF3 — Degradación diaria de todos los equipos
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::degradarEquipos() {
    for (Equipo* e : equipos) {
        e->degradar(2.0);
        e->incrementarTiempoInactivo();  // se resetea solo si recibe mantenimiento
    }
}

// ────────────────────────────────────────────────────────────
// Generación de incidencias aleatorias (simula eventos del día)
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::generarIncidenciasAleatorias() {
    // Cada día, ~20% de los equipos puede recibir una incidencia nueva
    for (Equipo* e : equipos) {
        int roll = std::rand() % 100;
        if (roll < 20) {
            int sevRoll = std::rand() % 3;
            Severidad sev = (sevRoll == 0) ? Severidad::ALTA
                          : (sevRoll == 1) ? Severidad::MEDIA
                          :                  Severidad::BAJA;

            std::string incId = "INC-D" + std::to_string(diaActual)
                              + "-" + e->getId();
            Incidencia* inc = new Incidencia(incId, sev, diaActual, e);
            e->agregarIncidencia(inc);
        }
    }
}

// ────────────────────────────────────────────────────────────
// Cálculo del riesgo global del laboratorio
// Promedio ponderado de (criticidad * (1 - estado/100))
// ────────────────────────────────────────────────────────────
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

// ────────────────────────────────────────────────────────────
// RF2 — Flujo completo de un día de simulación
// ────────────────────────────────────────────────────────────
void SistemaMantenimiento::ejecutarDia() {
    diaActual++;

    std::cout << "\n╔══════════════════════════════════════╗\n";
    std::cout <<   "║          DÍA " << std::setw(3) << diaActual
              <<                      " DE SIMULACIÓN          ║\n";
    std::cout <<   "╚══════════════════════════════════════╝\n";

    // PASO 1: Degradación diaria (RF3)
    degradarEquipos();

    // PASO 2: Aparición de nuevas incidencias
    generarIncidenciasAleatorias();

    // PASO 3: Cálculo de prioridades (RF4)
    calcularPrioridades();

    // PASO 4: Reordenamiento (RF5)
    ordenarEquipos();

    // PASO 5: Selección de los 3 más urgentes (RF6)
    std::vector<Equipo*> top3 = seleccionarTop3();

    // PASO 6: Ejecución de mantenimiento (RF7)
    ejecutarMantenimiento(top3);

    // PASO 7: Actualización de riesgo global (RF8)
    riesgoGlobal = calcularRiesgoGlobal();

    // PASO 8: Reporte del día (RF9)
    std::string reporte = generarReporteDia(top3);
    std::cout << reporte;
}

// ────────────────────────────────────────────────────────────
// RF9 — Reporte del día
// ────────────────────────────────────────────────────────────
std::string SistemaMantenimiento::generarReporteDia(
    const std::vector<Equipo*>& atendidos) const
{
    std::ostringstream oss;

    oss << "\n--- Reporte Día " << diaActual << " ---\n";

    // Top prioridad
    oss << "Top prioridad: ";
    for (size_t i = 0; i < atendidos.size(); ++i) {
        oss << atendidos[i]->getId()
            << " (" << std::fixed << std::setprecision(1)
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

    std::string nivelRiesgo;
    if      (riesgoGlobal >= 7.0) nivelRiesgo = "CRÍTICO";
    else if (riesgoGlobal >= 5.0) nivelRiesgo = "ALTO";
    else if (riesgoGlobal >= 3.0) nivelRiesgo = "MEDIO";
    else                          nivelRiesgo = "BAJO";

    oss << "Riesgo global: " << nivelRiesgo
        << " (" << std::fixed << std::setprecision(2) << riesgoGlobal << ")\n";

    // Equipos pendientes (los que NO fueron atendidos hoy)
    oss << "Equipos pendientes (muestra top 5):\n";
    int mostrados = 0;
    for (const Equipo* e : equipos) {
        bool estaEnTop = false;
        for (const Equipo* a : atendidos)
            if (a->getId() == e->getId()) { estaEnTop = true; break; }

        if (!estaEnTop && mostrados < 5) {
            oss << "  " << e->getId()
                << " prio=" << std::fixed << std::setprecision(1) << e->getPrioridad()
                << " inc=" << e->getCantidadIncidencias() << "\n";
            mostrados++;
        }
    }
    oss << "-------------------------------\n";
    return oss.str();
}

// ────────────────────────────────────────────────────────────
// Reporte acumulado de la simulación completa
// ────────────────────────────────────────────────────────────
std::string SistemaMantenimiento::generarReporteResumen() const {
    std::ostringstream oss;
    oss << "\n╔══════════════════════════════════════╗\n";
    oss << "║        RESUMEN FINAL SIMULACIÓN      ║\n";
    oss << "╚══════════════════════════════════════╝\n";
    oss << "Días simulados     : " << diaActual << "\n";
    oss << "Total equipos      : " << equipos.size() << "\n";
    oss << "Atenciones totales : " << equiposAtendidosTotal << "\n";
    oss << "Backlog al final   : " << getBacklogPendiente() << "\n";
    oss << "Riesgo final       : " << std::fixed << std::setprecision(2)
        << riesgoGlobal << "\n";

    oss << "\nEstado final de equipos (top 10 prioridad):\n";
    int n = std::min(10, static_cast<int>(equipos.size()));
    for (int i = 0; i < n; ++i)
        oss << "  " << equipos[i]->toString() << "\n";

    return oss.str();
}

void SistemaMantenimiento::imprimirEstadoActual() const {
    std::cout << "\n=== Estado actual — Día " << diaActual << " ===\n";
    for (const Equipo* e : equipos)
        std::cout << "  " << e->toString() << "\n";
}
