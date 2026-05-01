cmake_minimum_required(VERSION 4.1)
project(Proyecto_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto_1 main.cpp
        SistemaMantenimiento.h
        SistemaMantenimiento.cpp
        Buscador.cpp
        AlgoritmoOrdenamiento.cpp
        Excepciones.h
        LectorArchivos.cpp
        LectorArchivos.h
        EscritorDeReportes.cpp
        EscritorDeReportes.h
        Simulador.cpp
        Simulador.h
        Incidencia.cpp
        Incidencia.h
        Equipo.cpp
        Equipo.h
        EquipoCritico.cpp
        EquipoCritico.h
        EquipoEstandar.cpp
        EquipoEstandar.h
        Mantenimiento.cpp
        Mantenimiento.h
        MantenimientoPreventivo.cpp
        MantenimientoPreventivo.h
        MantenimientoCorrectivo.cpp
        MantenimientoCorrectivo.h
)
