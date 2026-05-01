//
// Created by Luis Quesada on 30/4/2026.
//

#include "InterfazSimulador.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

InterfazSimulador::InterfazSimulador(const string& archivoEntrada,
                                     const string& carpetaSalida)
    : archivoEntrada(archivoEntrada),
      carpetaSalida(carpetaSalida),
      simulador(nullptr),
      simulacionEnCurso(false),
      simulacionCompletada(false) {
}

InterfazSimulador::~InterfazSimulador() {
    if (simulador != nullptr) {
        delete simulador;
        simulador = nullptr;
    }
}

bool InterfazSimulador::validarArchivo(const string& ruta) {
    ifstream archivo(ruta);
    return archivo.good();
}

void InterfazSimulador::mostrarMenuPrincipal() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════╗\n";
    cout << "║   SISTEMA DE MANTENIMIENTO DE EQUIPOS - SIMULACION     ║\n";
    cout << "║                   (30 días de prueba)                  ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    cout << "  Archivo de entrada: " << archivoEntrada << "\n";
    cout << "  Carpeta de salida:  " << carpetaSalida << "\n";
    cout << "\n";
}

void InterfazSimulador::mostrarMenuOpciones() {
    cout << "┌────────────────────────────────────────────────────────┐\n";
    cout << "│ MENÚ PRINCIPAL                                         │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  1. Iniciar simulacion (30 días)                       │\n";
    cout << "│  2. Cambiar archivo de entrada                         │\n";
    cout << "│  3. Cambiar carpeta de salida                          │\n";
    cout << "│  4. Ver estadísticas (si hay simulación completada)    │\n";
    cout << "│  5. Resetear simulación                                │\n";
    cout << "│  0. Salir                                              │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
}

int InterfazSimulador::obtenerOpcion() {
    int opcion;
    cout << "  Seleccione opcion: ";
    cin >> opcion;
    cin.ignore(); // Limpiar buffer
    return opcion;
}

void InterfazSimulador::iniciarSimulacion() {
    if (!validarArchivo(archivoEntrada)) {
        throw ArchivoInvalido(archivoEntrada);
    }

    cout << "\n   Iniciando simulacion...\n";
    cout << "  ══════════════════════════════════════════════════════\n";

    if (simulador != nullptr) {
        delete simulador;
    }

    try {
        simulador = new Simulador(archivoEntrada, carpetaSalida);
        simulacionEnCurso = true;

        simulador->ejecutar();

        simulacionEnCurso = false;
        simulacionCompletada = true;

        cout << "\n   Simulación completada exitosamente.\n";
        cout << "   Reportes generados en: " << carpetaSalida << "/\n";
        cout << "  ══════════════════════════════════════════════════════\n\n";

    } catch (const exception& e) {
        simulacionEnCurso = false;
        cout << "\n   Error durante la simulacion: " << e.what() << "\n\n";
        throw;
    }
}

void InterfazSimulador::mostrarEstadisticas() {
    if (!simulacionCompletada) {
        cout << "\n   No hay simulación completada aun.\n";
        cout << "     Por favor, ejecute una simulacion primero.\n\n";
        return;
    }

    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════╗\n";
    cout << "║ ESTADÍSTICAS DE LA SIMULACIÓN                          ║\n";
    cout << "╠════════════════════════════════════════════════════════╣\n";
    cout << "│ Duracion: 30 días                                      │\n";
    cout << "│ Tecnicos disponibles: 3 por dia                        │\n";
    cout << "│ Reportes generados: 31 archivos                        │\n";
    cout << "│  - 30 reportes diarios                                 │\n";
    cout << "│  - 1 reporte final de simulacion                       │\n";
    cout << "╠════════════════════════════════════════════════════════╣\n";
    cout << "│ Ubicacion: " << carpetaSalida << "/\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
}

void InterfazSimulador::cambiarArchivoEntrada(const string& nuevaRuta) {
    if (validarArchivo(nuevaRuta)) {
        archivoEntrada = nuevaRuta;
        simulacionCompletada = false; // Reset porque cambiaron los datos
        cout << "  Archivo de entrada actualizado: " << archivoEntrada << "\n\n";
    } else {
        cout << "  El archivo no existe: " << nuevaRuta << "\n\n";
    }
}

void InterfazSimulador::cambiarCarpetaSalida(const string& nuevaCarpeta) {
    carpetaSalida = nuevaCarpeta;
    cout << "  Carpeta de salida actualizada: " << carpetaSalida << "\n\n";
}

void InterfazSimulador::resetear() {
    if (simulador != nullptr) {
        delete simulador;
        simulador = nullptr;
    }
    simulacionEnCurso = false;
    simulacionCompletada = false;
    cout << "  Simulación reseteada.\n\n";
}

void InterfazSimulador::procesarOpcion(int opcion) {
    switch (opcion) {
        case 1:
            iniciarSimulacion();
            break;
        case 2: {
            string nuevaRuta;
            cout << "  Ingrese nueva ruta: ";
            getline(cin, nuevaRuta);
            cambiarArchivoEntrada(nuevaRuta);
            break;
        }
        case 3: {
            string nuevaCarpeta;
            cout << "  Ingrese nueva carpeta: ";
            getline(cin, nuevaCarpeta);
            cambiarCarpetaSalida(nuevaCarpeta);
            break;
        }
        case 4:
            mostrarEstadisticas();
            break;
        case 5:
            resetear();
            break;
        case 0:
            cout << "\n  ¡Hasta luego!\n\n";
            break;
        default:
            cout << "  Opción inválida. Intente de nuevo.\n\n";
    }
}

void InterfazSimulador::ejecutar() {
    bool salir = false;

    while (!salir) {
        mostrarMenuPrincipal();
        mostrarMenuOpciones();

        int opcion = obtenerOpcion();

        if (opcion == 0) {
            salir = true;
            procesarOpcion(opcion);
        } else {
            try {
                procesarOpcion(opcion);
            } catch (const exception& e) {
                cerr << "  Error: " << e.what() << "\n\n";
            }
        }
    }
}