#pragma once
#ifndef SISTEMA_MANTENIMIENTO_H
#define SISTEMA_MANTENIMIENTO_H

#include "Equipo.h"
#include "Mantenimiento.h"
#include "AlgoritmoOrdenamiento.h"
#include "Buscador.h"
#include "Excepciones.h"
#include <vector>
#include <string>

/**
 * @brief Coordinador central de la simulación de mantenimiento.
 *
 * Responsabilidades (SRP):
 *  - Calcular prioridades usando la fórmula oficial.
 *  - Ordenar equipos con QuickSort propio (via AlgoritmoOrdenamiento).
 *  - Seleccionar los 3 equipos de mayor prioridad.
 *  - Ejecutar mantenimiento polimórfico (Strategy pattern).
 *  - Orquestar el flujo completo de cada día de simulación.
 *
 * Polimorfismo:
 *  - Trabaja con vector<Equipo*> (upcasting).
 *  - Aplica Mantenimiento* de forma polimórfica.
 *  - Usa dynamic_cast para acceder al informe exclusivo de Correctivo.
 */
class SistemaMantenimiento {
private:
    std::vector<Equipo*> equipos;          // colección polimórfica (upcasting)
    AlgoritmoOrdenamiento ordenador;
    Buscador buscador;
    int diaActual;
    int equiposAtendidosTotal;
    double riesgoGlobal;

    // Estrategias de mantenimiento (patrón Strategy)
    Mantenimiento* estrategiaPreventiva;
    Mantenimiento* estrategiaCorrectiva;

    // ── Helpers privados ────────────────────────────────────
    Mantenimiento* seleccionarEstrategia(const Equipo* equipo) const;
    double calcularRiesgoGlobal() const;
    void degradarEquipos();
    void generarIncidenciasAleatorias();

public:
    SistemaMantenimiento();
    ~SistemaMantenimiento();

    // ── Gestión de equipos ──────────────────────────────────
    void agregarEquipo(Equipo* equipo);
    Equipo* buscarEquipo(const std::string& id);

    // ── Pipeline de un día (RF3-RF8) ────────────────────────

    /**
     * @brief RF4: Calcula prioridad de cada equipo con la fórmula oficial:
     *   prioridad = (criticidad * 0.5) + (incidencias * 0.3) + (tiempo_inactivo * 0.2)
     */
    void calcularPrioridades();

    /**
     * @brief RF5: Ordena equipos de mayor a menor prioridad (QuickSort propio).
     */
    void ordenarEquipos();

    /**
     * @brief RF6: Devuelve los 3 equipos con mayor prioridad.
     */
    std::vector<Equipo*> seleccionarTop3();

    /**
     * @brief RF7: Aplica mantenimiento polimórfico a cada equipo seleccionado.
     *             Usa dynamic_cast para generar informe en MantenimientoCorrectivo.
     */
    void ejecutarMantenimiento(std::vector<Equipo*>& seleccionados);

    /**
     * @brief RF2/RF3/RF8: Ejecuta el flujo completo de un día de simulación.
     *        Secuencia: degradar → incidencias → prioridades → ordenar
     *                   → top3 → mantenimiento → actualizar → reportar
     */
    void ejecutarDia();

    // ── Reportes ────────────────────────────────────────────
    std::string generarReporteDia(const std::vector<Equipo*>& atendidos) const;
    std::string generarReporteResumen() const;
    void imprimirEstadoActual() const;

    // ── Getters de estado ───────────────────────────────────
    int getDiaActual()            const { return diaActual; }
    double getRiesgoGlobal()      const { return riesgoGlobal; }
    int getEquiposAtendidosTotal() const { return equiposAtendidosTotal; }
    int getBacklogPendiente()     const;
    const std::vector<Equipo*>& getEquipos() const { return equipos; }
};

#endif // SISTEMA_MANTENIMIENTO_H
