#pragma once
#include <iostream>
#include <string>
#include <exception>

using namespace std;

// Clase base para los errores del sistema.

class ErrorSistema : public exception {
protected:
    string mensaje;

public:
    ErrorSistema (const string& msj) : mensaje (msj) {}

    const char* what() const noexcept override {
        return mensaje.c_str();
    }
    // El método sirve para hacer un mensaje personalizado.
    //noexcept, garantiza que el método nunca lanzará una excepción diferente.
};

class ArchivoInvalido : public ErrorSistema {
public:
    ArchivoInvalido (const string& ruta) : ErrorSistema
    ("No se enconto el archivo, o es inaccesible a este: " + ruta ) {}
    //Este método hace constar que el archivo no se pudo leer, porque no se encontró.
};

class FormatoInvalido : public ErrorSistema {
public:
    FormatoInvalido (const string& ruta) : ErrorSistema ("Formato invalido en la linea" + to_string(numLinea)
        + ": " + linea) {}
    //Este método marca en donde hay un error de linea, mostrándolo de manera invaluda.
};

class OperacionContradictoria : public ErrorSistema {
public:
    OperacionContradictoria(const string& detalle) : ErrorSistema
    ("La operación es contradictoria: " + detalle){}
    //Funciona si la operación que se quioere realizar, no tiene sentido.
};
