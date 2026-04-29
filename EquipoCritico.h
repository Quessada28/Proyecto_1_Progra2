//
// Created by Luis Quesada on 29/4/2026.
//

#pragma once
#include <iostream>
#include "Equipo.h"

using namespace std;

class EquipoCritico : public Equipo {
public:
    EquipoCritico(const string& id, int criticidad, int estado);

    void degradar() override;
    void aplicarMejora() override;
    string tipo() const override;

};