//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include <string>
#include "Equipo.h"

using namespace std;

// Patrón Template Method:
// aplicar() define el flujo general (no se sobreescribe).
// Cada subclase especializa ejecutarProcedimiento().
class Mantenimiento {
public:
    virtual ~Mantenimiento() {}

    void aplicar(Equipo* equipo); // template method

    virtual string descripcion() const = 0;

protected:
    virtual void prepararHerramientas(Equipo* equipo);
    virtual void ejecutarProcedimiento(Equipo* equipo) = 0;
    virtual void registrarResultado(Equipo* equipo);
};