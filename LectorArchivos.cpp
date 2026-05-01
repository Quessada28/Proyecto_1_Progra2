//
// Created by Luis Quesada on 29/4/2026.
//

#include "LectorArchivos.h"
#include "EquipoCritico.h"
#include "EquipoEstandar.h"
#include <vector>
#include <iostream>

using namespace std;

void LectorArchivos::leerArchivo(const string& ruta,Equipo** equipos, int& numEquipos,
    Incidencia* incidencias, int& numIncidencias, int maxEquipos, int maxIncidencias) {

    ifstream archivo(ruta);
    if (!archivo.is_open()) throw ArchivoInvalido(ruta);

    string linea;
    int numLinea = 0;
    numEquipos     = 0;
    numIncidencias = 0;

    cout << "[DEBUG] Leyendo archivo: " << ruta << "\n";

    while (getline(archivo, linea)) {
        numLinea++;
        linea = trim(linea);

        // Saltar líneas vacías y comentarios
        if (linea.empty() || linea[0] == '#') continue;

        cout << "[DEBUG] Linea " << numLinea << ": " << linea.substr(0, 50) << "...\n";

        // Detectar equipos: líneas que empiezan con "EQ"
        if (linea.substr(0, 2) == "EQ") {
            if (numEquipos >= maxEquipos)
                throw OperacionContradictoria("Se supero el limite de equipos permitidos");

            cout << "  Parseando equipo...\n";
            equipos[numEquipos++] = parsearEquipo(linea, numLinea);
            cout << "  Equipo cargado. Total: " << numEquipos << "\n";

        }
        // Detectar incidencias: líneas que empiezan con "INC"
        else if (linea.substr(0, 3) == "INC") {
            if (numIncidencias >= maxIncidencias)
                throw OperacionContradictoria("Se supero el limite de incidencias permitidas");

            cout << "   Parseando incidencia...\n";
            incidencias[numIncidencias++] = parsearIncidencia(linea, numLinea);
            cout << "   Incidencia cargada. Total: " << numIncidencias << "\n";

        }
        // Ignorar líneas que empiezan con corchetes (headers de sección)
        else if (linea[0] == '[' && linea[linea.size()-1] == ']') {
            cout << "   Sección: " << linea << "\n";
            continue;
        }
    }

    archivo.close();

    cout << "\n[DEBUG] Lectura completada:\n";
    cout << "  - Equipos leidos: " << numEquipos << "\n";
    cout << "  - Incidencias leidas: " << numIncidencias << "\n\n";

    if (numEquipos == 0) {
        throw OperacionContradictoria("El archivo no contiene equipos validos");
    }
}

Equipo* LectorArchivos::parsearEquipo(const string& linea, int numLinea) {
    // Formato esperado: EQ id ; criticidad=X ; estado=Y

    // Remover el prefijo "EQ"
    string contenido = linea.substr(2);
    contenido = trim(contenido);

    // Dividir por ";"
    vector<string> partes;
    string actual = "";

    for (int i = 0; i <= (int)contenido.size(); i++) {
        if (i == (int)contenido.size() || contenido[i] == ';') {
            partes.push_back(trim(actual));
            actual = "";
        } else {
            actual += contenido[i];
        }
    }

    if (partes.size() < 3) {
        cout << "[ERROR] Formato incorrecto en línea " << numLinea << ": " << linea << "\n";
        throw FormatoInvalido(linea, numLinea);
    }

    string id = partes[0];
    int criticidad = stoi(extraerValor(partes[1], "criticidad", numLinea));
    int estado = stoi(extraerValor(partes[2], "estado", numLinea));

    // Validar rangos
    if (criticidad < 1 || criticidad > 10) {
        cout << "[ERROR] Criticidad fuera de rango en línea " << numLinea << "\n";
        throw FormatoInvalido(linea, numLinea);
    }
    if (estado < 0 || estado > 100) {
        cout << "[ERROR] Estado fuera de rango en línea " << numLinea << "\n";
        throw FormatoInvalido(linea, numLinea);
    }

    // Crear equipo crítico (>=7) o estándar (<7)
    if (criticidad >= 7) {
        cout << "    [CRITICO] ID: " << id << ", Criticidad: " << criticidad << ", Estado: " << estado << "\n";
        return new EquipoCritico(id, criticidad, estado);
    } else {
        cout << "    [ESTANDAR] ID: " << id << ", Criticidad: " << criticidad << ", Estado: " << estado << "\n";
        return new EquipoEstandar(id, criticidad, estado);
    }
}

Incidencia LectorArchivos::parsearIncidencia(const string& linea, int numLinea) {
    // Formato esperado: INC id_equipo ; severidad=X ; dia=Y

    // Remover el prefijo "INC"
    string contenido = linea.substr(3);
    contenido = trim(contenido);

    // Dividir por ";"
    vector <string> partes;
    string actual = "";

    for (int i = 0; i <= (int)contenido.size(); i++) {
        if (i == (int)contenido.size() || contenido[i] == ';') {
            partes.push_back(trim(actual));
            actual = "";
        } else {
            actual += contenido[i];
        }
    }

    if (partes.size() < 3) {
        cout << "[ERROR] Formato incorrecto en línea " << numLinea << ": " << linea << "\n";
        throw FormatoInvalido(linea, numLinea);
    }

    string idEquipo = partes[0];
    string severidad = extraerValor(partes[1], "severidad", numLinea);
    int dia = stoi(extraerValor(partes[2], "dia", numLinea));

    // Validar severidad
    if (severidad != "ALTA" && severidad != "MEDIA" && severidad != "BAJA") {
        cout << "[ERROR] Severidad inválida en línea " << numLinea << ": " << severidad << "\n";
        throw FormatoInvalido(linea, numLinea);
    }

    // Validar día
    if (dia < 1 || dia > 30) {
        cout << "[WARNING] Dia fuera del rango 1-30 en linea " << numLinea << "\n";
    }

    cout << "    ID: " << idEquipo << ", Severidad: " << severidad << ", Dia: " << dia << "\n";

    return Incidencia(idEquipo, severidad, dia);
}

string LectorArchivos::extraerValor(const string& segmento, const string& clave, int numLinea) {
    // Formato: clave=valor

    size_t pos = segmento.find('=');
    if (pos == string::npos) {
        cout << "[ERROR] No se encontró '=' en: " << segmento << "\n";
        throw FormatoInvalido(segmento, numLinea);
    }

    string k = trim(segmento.substr(0, pos));
    string v = trim(segmento.substr(pos + 1));

    if (k != clave) {
        cout << "[ERROR] Clave esperada '" << clave << "', encontrada '" << k << "'\n";
        throw FormatoInvalido(segmento, numLinea);
    }
    if (v.empty()) {
        cout << "[ERROR] Valor vacío para clave '" << clave << "'\n";
        throw FormatoInvalido(segmento, numLinea);
    }

    return v;
}

string LectorArchivos::trim(const string& s) {
    int ini = 0, fin = (int)s.size() - 1;
    while (ini <= fin && (s[ini] == ' ' || s[ini] == '\t' || s[ini] == '\r')) ini++;
    while (fin >= ini && (s[fin] == ' ' || s[fin] == '\t' || s[fin] == '\r')) fin--;
    return s.substr(ini, fin - ini + 1);
}