//
// Created by Luis Quesada on 29/4/2026.
//

#include "LectorArchivos.h"
#include "EquipoCritico.h"
#include "EquipoEstandar.h"

using namespace std;

void LectorArchivos::leerArchivo(const string& ruta,Equipo** equipos, int& numEquipos,
    Incidencia* incidencias, int& numIncidencias, int maxEquipos, int maxIncidencias) {

    ifstream archivo(ruta);
    if (!archivo.is_open()) throw ArchivoInvalido(ruta);

    string linea;
    int numLinea = 0;
    numEquipos     = 0;
    numIncidencias = 0;

    while (getline(archivo, linea)) {
        numLinea++;
        linea = trim(linea);
        if (linea.empty() || linea[0] == '#') continue;

        if (linea.substr(0, 2) == "EQ") {
            if (numEquipos >= maxEquipos)
                throw OperacionContradictoria("Se supero el limite de equipos permitidos");
            equipos[numEquipos++] = parsearEquipo(linea, numLinea);

        } else if (linea.substr(0, 3) == "INC") {
            if (numIncidencias >= maxIncidencias)
                throw OperacionContradictoria("Se supero el limite de incidencias permitidas");
            incidencias[numIncidencias++] = parsearIncidencia(linea, numLinea);

        } else {
            throw FormatoInvalido(linea, numLinea);
        }
    }

    archivo.close();
}

Equipo* LectorArchivos::parsearEquipo(const string& linea, int numLinea) {
    string partes[3];
    int    idx = 0;
    string actual = "";

    for (int i = 0; i <= (int)linea.size(); i++) {
        if (i == (int)linea.size() || linea[i] == ';') {
            if (idx >= 3) throw FormatoInvalido(linea, numLinea);
            partes[idx++] = trim(actual);
            actual = "";
        } else {
            actual += linea[i];
        }
    }

    if (idx != 3) throw FormatoInvalido(linea, numLinea);

    string id = partes[0];
    int criticidad = stoi(extraerValor(partes[1], "criticidad", numLinea));
    int estado = stoi(extraerValor(partes[2], "estado",     numLinea));

    if (criticidad < 0 || criticidad > 10)  throw FormatoInvalido(linea, numLinea);
    if (estado < 0 || estado > 100) throw FormatoInvalido(linea, numLinea);

    // Upcasting: devolvemos Equipo* apuntando a la clase derivada correcta

    if (criticidad >= 7)
        return new EquipoCritico(id, criticidad, estado);
    else
        return new EquipoEstandar(id, criticidad, estado);
}

Incidencia LectorArchivos::parsearIncidencia(const string& linea, int numLinea) {
    string partes[4];
    int idx = 0;
    string actual = "";

    for (int i = 0; i <= (int)linea.size(); i++) {
        if (i == (int)linea.size() || linea[i] == ';') {
            if (idx >= 4) throw FormatoInvalido(linea, numLinea);
            partes[idx++] = trim(actual);
            actual = "";
            }
        else {
            actual += linea[i];
        }
    }

    if (idx != 4) throw FormatoInvalido(linea, numLinea);

    string idEquipo= partes[1];
    string severidad = extraerValor(partes[2], "severidad", numLinea);
    int dia = stoi(extraerValor(partes[3], "dia", numLinea));

    if (severidad != "ALTA" && severidad != "MEDIA" && severidad != "BAJA")
        throw FormatoInvalido(linea, numLinea);

    return Incidencia(idEquipo, severidad, dia);
}

string LectorArchivos::extraerValor(const string& segmento, const string& clave, int numLinea) {
    size_t pos = segmento.find('=');
    if (pos == string::npos) throw FormatoInvalido(segmento, numLinea);

    string k = trim(segmento.substr(0, pos));
    string v = trim(segmento.substr(pos + 1));

    if (k != clave) throw FormatoInvalido(segmento, numLinea);
    if (v.empty())  throw FormatoInvalido(segmento, numLinea);

    return v;
}

string LectorArchivos::trim(const string& s) {
    int ini = 0, fin = (int)s.size() - 1;
    while (ini <= fin && (s[ini] == ' ' || s[ini] == '\t' || s[ini] == '\r')) ini++;
    while (fin >= ini && (s[fin] == ' ' || s[fin] == '\t' || s[fin] == '\r')) fin--;
    return s.substr(ini, fin - ini + 1);
}