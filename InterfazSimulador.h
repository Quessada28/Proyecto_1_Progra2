//
// Created by Luis Quesada on 30/4/2026.
//

#pragma once
#include <string>
#include <iostream>
#include "Simulador.h"
#include "Excepciones.h"

using namespace std;

class InterfazSimulador {
private:
    string archivoEntrada;
    string carpetaSalida;
    Simulador* simulador;
    bool simulacionEnCurso;
    bool simulacionCompletada;

    // Métodos privados para el menú
    void mostrarMenuPrincipal();
    void mostrarMenuOpciones();
    int obtenerOpcion();
    bool validarArchivo(const string& ruta);
    void procesarOpcion(int opcion);

public:

    InterfazSimulador(const string& archivoEntrada = "datos.txt",
                      const string& carpetaSalida = "reportes");
    ~InterfazSimulador();

    void ejecutar();
    void iniciarSimulacion();
    void mostrarEstadisticas();
    void cambiarArchivoEntrada(const string& nuevaRuta);
    void cambiarCarpetaSalida(const string& nuevaCarpeta);
    void resetear();
    bool estaSimulacionCompletada() const { return simulacionCompletada; }
};
