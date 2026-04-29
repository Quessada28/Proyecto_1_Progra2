//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include "Mantenimiento.h"

using namespace std;

class MantenimientoCorrectivo : public Mantenimiento {
public:
    string descripcion() const override;

protected:
    void ejecutarProcedimiento(Equipo* equipo) override;
    void registrarResultado(Equipo* equipo)    override;
};