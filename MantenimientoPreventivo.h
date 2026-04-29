//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include "Mantenimiento.h"

using namespace std;

class MantenimientoPreventivo : public Mantenimiento {
protected:
    void ejecutarProcedimiento(Equipo* equipo) override;

public:
    string descripcion() const override;



};